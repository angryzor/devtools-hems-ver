#include "ObjFireworksSpectacle.h"
#include "SurpriseService.h"

using namespace hh::fnd;
using namespace hh::game;

void* ObjFireworksSpectacleSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjFireworksSpectacleSpawner*>(pInstance);
    self->dummy = 0;
    return self;
}

void ObjFireworksSpectacleSpawner::Finish(void* pInstance) {}
void ObjFireworksSpectacleSpawner::Clean(void* pInstance) {}

const RflClassMember ObjFireworksSpectacleSpawner::rflClassMembers[4] = {
    { "dummy", &SpectacleSignalId::rflClass, nullptr, RflClassMember::Type::TYPE_SINT32, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjFireworksSpectacleSpawner, dummy), nullptr},
};

const RflTypeInfo ObjFireworksSpectacleSpawner::typeInfo = RflTypeInfo{ "ObjFireworksSpectacleSpawner", "ObjFireworksSpectacleSpawner", &ObjFireworksSpectacleSpawner::Construct, &ObjFireworksSpectacleSpawner::Finish, &ObjFireworksSpectacleSpawner::Clean, sizeof(ObjFireworksSpectacleSpawner) };
const RflClass ObjFireworksSpectacleSpawner::rflClass = RflClass{ "ObjFireworksSpectacleSpawner", nullptr, sizeof(ObjFireworksSpectacleSpawner), nullptr, 0, ObjFireworksSpectacleSpawner::rflClassMembers, sizeof(ObjFireworksSpectacleSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

void ObjFireworksSpectacle::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
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

void ObjFireworksSpectacle::AddCallback(GameManager* gameManager) {
    midiResource = ResourceManager::GetInstance()->GetResource<ResMidiFile>("surprising_timings");

    StartSpectacle();
}

void ObjFireworksSpectacle::RemoveCallback(GameManager* gameManager) {
    EndSpectacle();
}

void ObjFireworksSpectacle::StartSpectacle() {
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

const GameObjectClass ObjFireworksSpectacle::gameObjectClass{
    "Firework",
    "Firework",
    sizeof(ObjFireworksSpectacle),
    &ObjFireworksSpectacle::Create,
    0,
    nullptr,
    nullptr,
};

const GameObjectClass* ObjFireworksSpectacle::GetClass() {
    return &gameObjectClass;
}
