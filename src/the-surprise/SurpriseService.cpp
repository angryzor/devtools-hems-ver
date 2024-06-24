#include "SurpriseService.h"
#include "ObjHEMSMember.h"
#include "Members.h"
#include <chrono>
#include <the-surprise/ResMidiFile.h>
#include <the-surprise/ObjFireworksSpectacle.h>
#include <the-surprise/ObjHEMSMember.h>
#include <the-surprise/spectacle-responders/ObjSpectacleEffectEmitter.h>
#include <the-surprise/spectacle-responders/ObjSpectacleObjectStatusController.h>
#include <the-surprise/spectacle-responders/ObjSpectacleLaserProjector.h>

using namespace hh::fnd;
using namespace hh::game;

SurpriseService::SurpriseService(csl::fnd::IAllocator* allocator) : GameService{ allocator }, mt{ std::random_device()() } {
    for (short i = 0; i < NUM_BANKS; i++) {
        for (short j = 0; j < 32; j++) {
            new (&signalListeners[i][j]) csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>(GetAllocator());
        }
    }
    for (short j = 0; j < 128; j++) {
        new (&controlListeners[j]) csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>(GetAllocator());
    }
    for (size_t i = 0; i < sizeof(modderMembers) / sizeof(const char*); i++) {
        assert(i < sizeof(kodamaNos) / sizeof(int));
        assert(i < sizeof(modderMembers) / sizeof(const char*));
        assert(kodamaNos[i] < sizeof(modderMembersByKodamaNo) / sizeof(const char*));
        modderMembersByKodamaNo[kodamaNos[i]] = modderMembers[i];
    }
}

const GameServiceClass* SurpriseService::GetClass() {
    return &gameServiceClass;
}

GameService* SurpriseService::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) SurpriseService{ allocator };
}

const GameServiceClass SurpriseService::gameServiceClass{
    "DevToolsService",
    &SurpriseService::Create,
    0,
};

void* SurpriseService::GetRuntimeTypeInfo() {
    return nullptr;
}

bool SurpriseService::ProcessMessage(Message& message) {
    switch (message.ID) {
    case MessageID::NOTIFY_LEVEL_STATUS: {
        auto& msg = static_cast<app::game::MsgNotifyLevelStatus&>(message);
        switch (msg.status) {
        case 0:
            if (auto* objectWorld = pGameManager->GetService<ObjectWorld>())
            if (auto* levelInfo = pGameManager->GetService<app::level::LevelInfo>())
            if (!strcmp(levelInfo->stageData->name.c_str(), "w1r03")) {
                resourceLoader->LoadPackfile("mods/angryzor_devtools/devtools_extra", 0);

                if (mode == Mode::DDAY) {
                    resourceLoader->LoadPackfile("stage/w2_common", 0);
                    auto* resource = ResourceManager::GetInstance()->GetResource<ResObjectWorld>("surprising_objects");
                    auto* layer = ObjectWorldChunkLayer::Create(hh::fnd::MemoryRouter::GetModuleAllocator(), resource);
                    objectWorld->GetWorldChunks()[0]->AddLayer(layer);
                    objectWorld->GetWorldChunks()[0]->SetLayerEnabled("surprising_objects", true);

                    resource = ResourceManager::GetInstance()->GetResource<ResObjectWorld>("surprising_pous");
                    layer = ObjectWorldChunkLayer::Create(hh::fnd::MemoryRouter::GetModuleAllocator(), resource);
                    objectWorld->GetWorldChunks()[0]->AddLayer(layer);
                    objectWorld->GetWorldChunks()[0]->SetLayerEnabled("surprising_pous", true);
                }
            }
            break;
        case 1:
            if (auto* overlayService = pGameManager->GetService<app::ui::UIOverlayService>()) {
                auto* request = app::ui::RequestOverlayWindow::Create(GetAllocator());
                request->header.copyFrom("devtools_birthday_header");
                request->text.copyFrom("devtools_birthday_main");
                request->unk2 = 0;
                request->unk101 = 1;
                overlayService->QueueBeginRequest(request);
            }
            break;
        }
        return true;
    }
    case HEMS_MEMBER_FOUND: {
        auto& msg = static_cast<MsgHEMSMemberFound&>(message);
        assert(msg.kodamaNo >= 0 && msg.kodamaNo < sizeof(isHEMSMemberFound) / sizeof(bool));
        isHEMSMemberFound[msg.kodamaNo] = true;

        if (auto* overlayService = pGameManager->GetService<app::ui::UIOverlayService>()) {
            auto* request = app::ui::RequestOverlayCaption::Create(GetAllocator());
            request->Setup("devtools_birthday_member_found", nullptr, 2.0f);
            request->unk206 = 3;

            if (auto* sender = static_cast<hh::game::GameObject*>(hh::game::GameObjectSystem::GetGameObjectByHandle(msg.Sender))) {
                auto* memberName = request->textVariables.Add(&pAllocator);
                memberName->SetUntranslatedValue("devtools_birthday_member_name", sender->name.c_str());
            }

            overlayService->QueueBeginRequest(request);

            if (GetFoundMemberCount() >= 0) {
                auto* request = app::ui::RequestOverlayCaption::Create(GetAllocator());
                request->Setup("devtools_birthday_enough_members", nullptr, 2.0f);
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

void SurpriseService::OnAddedToGame() {
    resourceLoader = new (GetAllocator()) ResourceLoader{ GetAllocator() };

    pGameManager->AddListener(this);
}
void SurpriseService::OnRemovedFromGame() {
    pGameManager->RemoveListener(this);
}

void SurpriseService::GameObjectAddedCallback(GameManager* gameManager, GameObject* gameObject) {
    if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>()) {
        if (strcmp(levelInfo->stageData->name.c_str(), "w1r03"))
            return;

        if (gameObject->objectClass == app::ObjKodama::GetClass()) {
            auto* worldData = gameObject->GetWorldDataByClass<heur::rfl::ObjKodamaSpawner>();

            if (worldData->no < 0 || worldData->no >= sizeof(isHEMSMemberFound) / sizeof(bool) || isHEMSMemberFound[worldData->no])
                return;

            switch (mode) {
            case Mode::COLLECTED_RANDOM:
                AddHEMSMember(gameObject, !gameManager->IsWorldFlagSet(worldData->no, 64) || (mt() % 100) >= randomThreshold);
                break;
            case Mode::COLLECTED:
                AddHEMSMember(gameObject, !gameManager->IsWorldFlagSet(worldData->no, 64));
                break;
            case Mode::ALL_RANDOM:
                AddHEMSMember(gameObject, !gameManager->IsWorldFlagSet(worldData->no, 64) && (mt() % 100) >= randomThreshold);
                break;
            case Mode::ALL:
            case Mode::DDAY:
                AddHEMSMember(gameObject, false);
                break;
            }
        }
    }
}

void SurpriseService::GameObjectRemovedCallback(GameManager* gameManager, GameObject* gameObject) {
}

void SurpriseService::AddSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher) {
    assert(signalId.IsValid());
    signalListeners[signalId.channel][signalId.id].push_back(launcher);
}

void SurpriseService::RemoveSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher) {
    assert(signalId.IsValid());
    signalListeners[signalId.channel][signalId.id].remove(signalListeners[signalId.channel][signalId.id].find(launcher));
}

csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>& SurpriseService::GetSignalListeners(SpectacleSignalId signalId) {
    assert(signalId.IsValid());
    return signalListeners[signalId.channel][signalId.id];
}

void SurpriseService::AddControlListener(short controlId, const hh::fnd::Handle<hh::game::GameObject>& launcher) {
    assert(controlId >= 0 && controlId < sizeof(controlListeners) / sizeof(csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>));
    controlListeners[controlId].push_back(launcher);
}

void SurpriseService::RemoveControlListener(short controlId, const hh::fnd::Handle<hh::game::GameObject>& launcher) {
    assert(controlId >= 0 && controlId < sizeof(controlListeners) / sizeof(csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>));
    controlListeners[controlId].remove(controlListeners[controlId].find(launcher));
}

csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>& SurpriseService::GetControlListeners(short controlId) {
    assert(controlId >= 0 && controlId < sizeof(controlListeners) / sizeof(csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>));
    return controlListeners[controlId];
}

size_t SurpriseService::GetFoundMemberCount() const
{
    size_t res{};

    for (size_t i = 0; i < sizeof(isHEMSMemberFound) / sizeof(bool); i++) {
        res += isHEMSMemberFound[i] ? 1 : 0;
    }

    return res;
}

void SurpriseService::AddHEMSMember(GameObject* kodama, bool placeholder)
{
    auto* hemsMember = GameObject::Create<ObjHEMSMember>(GetAllocator());
    auto kodamaNo = kodama->GetWorldDataByClass<heur::rfl::ObjKodamaSpawner>()->no;
    WorldPosition pos{};
    hemsMember->Setup(kodama->GetWorldObjectStatus()->objectData->id, placeholder);
    pGameManager->AddGameObject(hemsMember, placeholder ? kodama->name.c_str() : modderMembersByKodamaNo[kodamaNo] != nullptr ? modderMembersByKodamaNo[kodamaNo] : normalMembers[mt() % (sizeof(normalMembers) / sizeof(const char*))], placeholder, &pos, kodama);
}
