#include "ObjFireworksSpectacle.h"
#include "SurpriseService.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::eff;

void* ObjFireworksSpectacleSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjFireworksSpectacleSpawner*>(pInstance);
    self->dummy = 0;
    return self;
}

void ObjFireworksSpectacleSpawner::Finish(void* pInstance) {}
void ObjFireworksSpectacleSpawner::Clean(void* pInstance) {}

const RflClassMember ObjFireworksSpectacleSpawner::rflClassMembers[1] = {
    { "dummy", nullptr, nullptr, RflClassMember::Type::TYPE_SINT32, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjFireworksSpectacleSpawner, dummy), nullptr},
};

const RflTypeInfo ObjFireworksSpectacleSpawner::typeInfo = RflTypeInfo{ "ObjFireworksSpectacleSpawner", "ObjFireworksSpectacleSpawner", &ObjFireworksSpectacleSpawner::Construct, &ObjFireworksSpectacleSpawner::Finish, &ObjFireworksSpectacleSpawner::Clean, sizeof(ObjFireworksSpectacleSpawner) };
const RflClass ObjFireworksSpectacleSpawner::rflClass = RflClass{ "ObjFireworksSpectacleSpawner", nullptr, sizeof(ObjFireworksSpectacleSpawner), nullptr, 0, ObjFireworksSpectacleSpawner::rflClassMembers, sizeof(ObjFireworksSpectacleSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

void ObjFireworksSpectacle::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
    if (!started)
        return;

    switch (phase) {
    case UpdatingPhase::PRE_ANIM:
        if (auto* soundDirector = gameManager->GetService<app::snd::SoundDirector>())
        if (auto* surpriseService = gameManager->GetService<SurpriseService>()) {
            float playTime = soundDirector->GetBgmSoundHandle(0).GetPlayTime();

            if (inFirstSection && playTime > 278.7350993377483443f) {
                soundDirector->PlayBgm({ "bgm_lastboss", 2, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
                inFirstSection = false;
            }

            while (index < midiResource->midiFile.getEventCount(0)) {
                auto& midiEvent = midiResource->midiFile.getEvent(0, index);

                if (static_cast<float>(midiEvent.tick) > playTime * 151.0f * static_cast<float>(midiResource->midiFile.getTicksPerQuarterNote()) / 120.0f)
                    break;

                if (midiEvent.track > NUM_BANKS) {
                    index++;
                    continue;
                }

                if (midiEvent.isNoteOn()) {
                    SpectacleSignalId signalId{ static_cast<short>(midiEvent.track), static_cast<short>(midiEvent.getKeyNumber() - 60) };

                    for (auto listener : surpriseService->GetSignalListeners(signalId)) {
                        MsgSpectacleMidiNoteOn msg{ signalId };
                        SendMessageImmToGameObject(listener, msg);
                    }
                }
                else if (midiEvent.isNoteOff()) {
                    SpectacleSignalId signalId{ static_cast<short>(midiEvent.track), static_cast<short>(midiEvent.getKeyNumber() - 60) };

                    for (auto listener : surpriseService->GetSignalListeners(signalId)) {
                        MsgSpectacleMidiNoteOff msg{ signalId };
                        SendMessageImmToGameObject(listener, msg);
                    }
                }
                else if (midiEvent.isController()) {
                    short controlId = midiEvent.getControllerNumber();

                    for (auto listener : surpriseService->GetControlListeners(controlId)) {
                        MsgSpectacleMidiControlChange msg{ controlId, static_cast<uint8_t>(midiEvent.getControllerValue()) };
                        SendMessageImmToGameObject(listener, msg);
                    }
                }

                index++;
            }
        }

        if (index >= midiResource->midiFile.getEventCount(0))
            Kill();

        break;
    }
}

bool ObjFireworksSpectacle::ProcessMessage(Message& message)
{
    switch (message.ID) {
    case MessageID::TRIGGER_ENTER: {
        auto* surpServ = gameManager->GetService<SurpriseService>();
        size_t numMembers{};

        if (surpServ->GetFoundMemberCount() >= 0) {
            GetComponent<hh::physics::GOCSphereCollider>()->SetEnabled(false);
            auto* eff = GetComponent<hh::eff::GOCEffect>();
            eff->StopEffectAll();
            eff->CreateEffect("ef_ob_guidecircle_out01", nullptr);
            StartSpectacle();
        }
        else {
            if (auto* overlayService = gameManager->GetService<app::ui::UIOverlayService>()) {
                auto* request = app::ui::RequestOverlayCaption::Create(GetAllocator());
                request->Setup("devtools_birthday_not_enough_members", nullptr, 2.0f);
                request->unk206 = 3;
                overlayService->QueueBeginRequest(request);
            }
        }

        return true;
    }
    default:
        return false;
    }
}

void ObjFireworksSpectacle::AddCallback(GameManager* gameManager) {
    midiResource = ResourceManager::GetInstance()->GetResource<ResMidiFile>("surprising_timings");

    auto* gocEffect = CreateComponent<GOCEffect>();
    gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
    AddComponent(gocEffect);

    auto* startCollider = CreateComponent<hh::physics::GOCSphereCollider>();
    hh::physics::GOCSphereCollider::SetupInfo colliderSetupInfo{};
    colliderSetupInfo.radius = 1.0f;
    colliderSetupInfo.unk3 |= 1;
    colliderSetupInfo.filterCategory = 25;
    colliderSetupInfo.unk4 = 0x8000;
    colliderSetupInfo.SetPosition({ 0.0f, 0.0f, 0.0f });
    startCollider->Setup(colliderSetupInfo);
    AddComponent(startCollider);

    gocEffect->CreateEffect("ef_ob_guidecircle_loop01", nullptr);
}

void ObjFireworksSpectacle::RemoveCallback(GameManager* gameManager) {
    if (started)
        EndSpectacle();
}

void ObjFireworksSpectacle::StartSpectacle() {
    started = true;
    inFirstSection = true;

    if (auto* resMgr = hh::fnd::ResourceManager::GetInstance()) {
        // Prefer hearing music over ObjKodamaFollower yapping.
        if (auto* islandParamResource = resMgr->GetResource<hh::fnd::ResReflectionT<heur::rfl::IslandParameter>>("island_param"))
        if (auto* islandParam = islandParamResource->GetData())
            islandParam->kodama.normal.follow.maxFollowNum = 0;
        
        // Don't dim music volume when watching.
        if (auto* soundCommonParamResource = resMgr->GetResource<hh::fnd::ResReflectionT<heur::rfl::SoundCommonParameter>>("sound_common_param"))
        if (auto* soundCommonParam = soundCommonParamResource->GetData())
            soundCommonParam->bgmVolumeInfo.playerStop.value = 1.0f;

        // End please shut up...
        if (auto* scriptParamResource = resMgr->GetResource<hh::fnd::ResReflectionT<heur::rfl::ScriptParameter>>("script"))
        if (auto* scriptParam = scriptParamResource->GetData())
            scriptParam->intervalMinuntes = 10000;
    }

    auto* soundDirector = gameManager->GetService<app::snd::SoundDirector>();
    soundDirector->PlayBgm({ "bgm_lastboss", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
}

void ObjFireworksSpectacle::EndSpectacle() {
    auto* soundDirector = gameManager->GetService<app::snd::SoundDirector>();
    soundDirector->PlayBgm({ "", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
}

ObjFireworksSpectacle::ObjFireworksSpectacle(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(0);
    SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

GameObject* ObjFireworksSpectacle::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjFireworksSpectacle{ allocator };
}

const RflClassMember::Value ObjFireworksSpectacle::attributes[]{
    { "category", RflClassMember::Type::TYPE_CSTRING, "HEMS" },
};

const GameObjectClass ObjFireworksSpectacle::gameObjectClass{
    "FireworksSpectacle",
    "FireworksSpectacle",
    sizeof(ObjFireworksSpectacle),
    &ObjFireworksSpectacle::Create,
    1,
    ObjFireworksSpectacle::attributes,
    &ObjFireworksSpectacleSpawner::rflClass,
};

const GameObjectClass* ObjFireworksSpectacle::GetClass() {
    return &gameObjectClass;
}
