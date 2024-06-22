#include "ObjSpectacleObjectStatusController.h"
#include <the-surprise/SurpriseService.h>

using namespace hh::fnd;
using namespace hh::game;

void* ObjSpectacleObjectStatusControllerSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjSpectacleObjectStatusControllerSpawner*>(pInstance);
    self->signalId = SpectacleSignalId{};
    self->unused = SpectacleSignalId{};
    new (&self->targets) csl::ut::MoveArray<ObjectId>{ pAllocator };
    return self;
}

void ObjSpectacleObjectStatusControllerSpawner::Finish(void* pInstance) {}

void ObjSpectacleObjectStatusControllerSpawner::Clean(void* pInstance) {
    auto* self = static_cast<ObjSpectacleObjectStatusControllerSpawner*>(pInstance);
    self->targets.~MoveArray<ObjectId>();
}

const RflClassMember ObjSpectacleObjectStatusControllerSpawner::rflClassMembers[3] = {
    { "signalId", &SpectacleSignalId::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleObjectStatusControllerSpawner, signalId), nullptr},
    { "unused", &SpectacleSignalId::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleObjectStatusControllerSpawner, unused), nullptr},
    { "targets", nullptr, nullptr, RflClassMember::Type::TYPE_ARRAY, RflClassMember::Type::TYPE_OBJECT_ID, 0, 0, offsetof(ObjSpectacleObjectStatusControllerSpawner, targets), nullptr },
};

const RflTypeInfo ObjSpectacleObjectStatusControllerSpawner::typeInfo = RflTypeInfo{ "ObjSpectacleObjectStatusControllerSpawner", "ObjSpectacleObjectStatusControllerSpawner", &ObjSpectacleObjectStatusControllerSpawner::Construct, &ObjSpectacleObjectStatusControllerSpawner::Finish, &ObjSpectacleObjectStatusControllerSpawner::Clean, sizeof(ObjSpectacleObjectStatusControllerSpawner) };
const RflClass ObjSpectacleObjectStatusControllerSpawner::rflClass = RflClass{ "ObjSpectacleObjectStatusControllerSpawner", nullptr, sizeof(ObjSpectacleObjectStatusControllerSpawner), nullptr, 0, ObjSpectacleObjectStatusControllerSpawner::rflClassMembers, sizeof(ObjSpectacleObjectStatusControllerSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

bool ObjSpectacleObjectStatusController::ProcessMessage(Message& message)
{
    switch (message.ID) {
    case MessageID::PARAM_CHANGED_IN_EDITOR: {
        auto* worldData = GetWorldDataByClass<ObjSpectacleObjectStatusControllerSpawner>();

        if (signalId.IsValid())
            gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);
        signalId = worldData->signalId;
        if (signalId.IsValid())
            gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);

        return true;
    }
    case MessageID::GET_DEBUG_COMMENT_IN_EDITOR: {
        auto& msg = static_cast<hh::dbg::MsgGetDebugCommentInEditor&>(message);
        snprintf(msg.comment, sizeof(msg.comment), "On/Off: %d:%d", signalId.channel, signalId.id);
        return true;
    }
    case SPECTACLE_MIDI_NOTE_ON:
        StartTargets();
        return true;
    case SPECTACLE_MIDI_NOTE_OFF:
        StopTargets();
        return true;
    default:
        return GameObject::ProcessMessage(message);
    }
}

void ObjSpectacleObjectStatusController::AddCallback(GameManager* gameManager)
{
    if (!statusFlags.test(StatusFlags::EDITOR))
        StopTargets();

    auto* config = GetWorldDataByClass<ObjSpectacleObjectStatusControllerSpawner>();
    signalId = config->signalId;

    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);
}

void ObjSpectacleObjectStatusController::RemoveCallback(GameManager* gameManager)
{
    auto* config = GetWorldDataByClass<ObjSpectacleObjectStatusControllerSpawner>();

    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);

    signalId = {};
}

void ObjSpectacleObjectStatusController::StartTargets() const
{
    auto* objWorld = gameManager->GetService<ObjectWorld>();
    auto* worldData = GetWorldDataByClass<ObjSpectacleObjectStatusControllerSpawner>();

    for (auto& target : worldData->targets) {
        auto& chunk = objWorld->GetWorldChunks()[0];
        auto idx = chunk->GetObjectIndexById(target);
        chunk->Restart(idx, false);
    }
}

void ObjSpectacleObjectStatusController::StopTargets() const
{
    auto* objWorld = gameManager->GetService<ObjectWorld>();
    auto* worldData = GetWorldDataByClass<ObjSpectacleObjectStatusControllerSpawner>();

    for (auto& target : worldData->targets) {
        auto& chunk = objWorld->GetWorldChunks()[0];
        auto idx = chunk->GetObjectIndexById(target);
        chunk->Shutdown(idx);

        if (auto* gameObj = chunk->GetGameObjectByObjectId(target))
            gameObj->Kill();
    }
}

ObjSpectacleObjectStatusController::ObjSpectacleObjectStatusController(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(0);
}

GameObject* ObjSpectacleObjectStatusController::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjSpectacleObjectStatusController{ allocator };
}

//const char* ObjSpectacleObjectStatusController::category = "HEMS";
const hh::fnd::RflClassMember::Value ObjSpectacleObjectStatusController::attributes[1]{
    { "category", hh::fnd::RflClassMember::Type::TYPE_CSTRING, "HEMS" },
    //{ "spawntype", hh::fnd::RflClassMember::Type::TYPE_CSTRING, "ETERNAL" },
};

const GameObjectClass ObjSpectacleObjectStatusController::gameObjectClass{
    "SpectacleObjectStatusController",
    "SpectacleObjectStatusController",
    sizeof(ObjSpectacleObjectStatusController),
    &ObjSpectacleObjectStatusController::Create,
    sizeof(ObjSpectacleObjectStatusController::attributes) / sizeof(hh::fnd::RflClassMember::Value),
    ObjSpectacleObjectStatusController::attributes,
    &ObjSpectacleObjectStatusControllerSpawner::rflClass,
};

const GameObjectClass* ObjSpectacleObjectStatusController::GetClass() {
    return &gameObjectClass;
}
