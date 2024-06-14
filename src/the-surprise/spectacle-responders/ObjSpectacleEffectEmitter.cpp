#include "ObjSpectacleEffectEmitter.h"
#include <the-surprise/SurpriseService.h>

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::eff;

void* ObjSpectacleEffectEmitterSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjSpectacleEffectEmitterSpawner*>(pInstance);
    self->signalId = SpectacleSignalId{};
    new (&self->effectName) csl::ut::VariableString{ pAllocator };
    self->anisotropicScale = { 1.0f, 1.0f, 1.0f };
    self->isotropicScale = 1.0f;
    return self;
}

void ObjSpectacleEffectEmitterSpawner::Finish(void* pInstance) {}

void ObjSpectacleEffectEmitterSpawner::Clean(void* pInstance) {
    auto* self = static_cast<ObjSpectacleEffectEmitterSpawner*>(pInstance);
    self->effectName.~VariableString();
}

const RflClassMember ObjSpectacleEffectEmitterSpawner::rflClassMembers[4] = {
    { "signalId", &SpectacleSignalId::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleEffectEmitterSpawner, signalId), nullptr},
    { "effectName", nullptr, nullptr, RflClassMember::Type::TYPE_STRING, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleEffectEmitterSpawner, effectName), nullptr },
    { "anisotropicScale", nullptr, nullptr, RflClassMember::Type::TYPE_VECTOR3, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleEffectEmitterSpawner, anisotropicScale), nullptr },
    { "isotropicScale", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleEffectEmitterSpawner, isotropicScale), nullptr },
};

const RflTypeInfo ObjSpectacleEffectEmitterSpawner::typeInfo = RflTypeInfo{ "ObjSpectacleEffectEmitterSpawner", "ObjSpectacleEffectEmitterSpawner", &ObjSpectacleEffectEmitterSpawner::Construct, &ObjSpectacleEffectEmitterSpawner::Finish, &ObjSpectacleEffectEmitterSpawner::Clean, sizeof(ObjSpectacleEffectEmitterSpawner) };
const RflClass ObjSpectacleEffectEmitterSpawner::rflClass = RflClass{ "ObjSpectacleEffectEmitterSpawner", nullptr, sizeof(ObjSpectacleEffectEmitterSpawner), nullptr, 0, ObjSpectacleEffectEmitterSpawner::rflClassMembers, sizeof(ObjSpectacleEffectEmitterSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

// class ObjFirework : public GameObject {
//     FireworkDesc desc{};
//     Handle<ObjSpectacleEffectEmitter> launcher{};
//     //Handle<hh::path::GOCPath> path{};
//     float lifeTime{ 0.0f };

// public:
//     virtual bool ProcessMessage(Message& message) override { return false; }

//     void Setup(ObjSpectacleEffectEmitter* launcher, FireworkDesc desc) {
//         this->launcher = launcher;
//         this->desc = desc;
//     }

//     virtual void Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
//         auto* launcher = GameObjectSystem::GetGameObjectByHandle(this->launcher);
//         if (!launcher) {
//             Kill();
//             return;
//         }

//         //if (auto* worldData = launcher->GetWorldDataByClass<ObjSpectacleEffectEmitterSpawner>()) {
//         //    if (lifeTime == 0.0f)
//         //        GetComponent<GOCEffect>()->CreateEffect(worldData->trailEffectName, nullptr);

//         //    lifeTime += updateInfo.deltaTime;

//         //    if (lifeTime > desc.travelTime + 5.0f) {
//         //        Kill();
//         //        return;
//         //    }

//         //    auto* gocTransform = GetComponent<GOCTransform>();
//         //    gocTransform->SetLocalTranslationAndRotation
//         //}
//     }

//     virtual void AddCallback(GameManager* gameManager) override {
//         //if (auto* launcher = GameObjectSystem::GetGameObjectByHandle(this->launcher))
//         //if (auto* worldData = launcher->GetWorldDataByClass<ObjSpectacleEffectEmitterSpawner>()) {
//         //    if (auto* pathSrv = gameManager->GetService<hh::path::PathManager>()) {
//         //        path = pathSrv->GetPath(worldData->pathName.c_str());
//         //    }

//         //    auto* gocEffect = CreateComponent<GOCEffect>();
//         //    gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
//         //    AddComponent(gocEffect);

//         //    //if (auto* pLauncher = GameObjectSystem::GetGameObjectByHandle(launcher))
//         //    //if (auto* launcherConfig = pLauncher->GetWorldDataByClass<ObjSpectacleEffectEmitterSpawner>())
//         //    //    gocEffect->CreateEffect(launcherConfig->effectName.c_str(), nullptr);
//         //}
//     }

//     virtual void RemoveCallback(GameManager* gameManager) override {
//     }

//     GAMEOBJECT_CLASS_DECLARATION(ObjFirework)
// };

// const GameObjectClass* ObjFirework::GetClass() {
//     return &gameObjectClass;
// }

// ObjFirework::ObjFirework(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {}

// GameObject* ObjFirework::Create(csl::fnd::IAllocator* allocator) {
//     return new (allocator) ObjFirework{ allocator };
// }

// const GameObjectClass ObjFirework::gameObjectClass{
//     "Firework",
//     "Firework",
//     sizeof(ObjFirework),
//     &ObjFirework::Create,
//     0,
//     nullptr,
//     nullptr,
// };

ObjSpectacleEffectEmitter::ObjSpectacleEffectEmitter(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(0);
}

bool ObjSpectacleEffectEmitter::ProcessMessage(Message& message) {
    switch (message.ID) {
    case MessageID::PARAM_CHANGED_IN_EDITOR: {
        auto* worldData = GetWorldDataByClass<ObjSpectacleEffectEmitterSpawner>();

        if (signalId.IsValid())
            gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);
        signalId = worldData->signalId;
        if (signalId.IsValid())
            gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);

        return true;
    }
    case MessageID::GET_DEBUG_COMMENT_IN_EDITOR: {
        auto& msg = static_cast<hh::dbg::MsgGetDebugCommentInEditor&>(message);
        snprintf(msg.comment, sizeof(msg.comment), "Emit: %d:%d", signalId.channel, signalId.id);
        return true;
    }
    case SPECTACLE_MIDI_NOTE_ON:
        GetComponent<GOCEffect>()->CreateEffect(GetWorldDataByClass<ObjSpectacleEffectEmitterSpawner>()->effectName, nullptr);
        return true;
    default:
        return GameObject::ProcessMessage(message);
    }
}

void ObjSpectacleEffectEmitter::AddCallback(GameManager* gameManager) {
    auto* config = GetWorldDataByClass<ObjSpectacleEffectEmitterSpawner>();

    auto* gocTransform = GetComponent<GOCTransform>();
    auto transform = gocTransform->GetTransform();
    transform.scale = { config->anisotropicScale * config->isotropicScale };
    gocTransform->SetLocalTransform(transform);

    auto* gocEffect = CreateComponent<GOCEffect>();
    gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
    AddComponent(gocEffect);

    signalId = config->signalId;
    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);
}

void ObjSpectacleEffectEmitter::RemoveCallback(GameManager* gameManager) {
    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);
    signalId = {};
}

GameObject* ObjSpectacleEffectEmitter::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjSpectacleEffectEmitter{ allocator };
}

const hh::fnd::RflClassMember::Value ObjSpectacleEffectEmitter::attributes[2]{
    { "category", hh::fnd::RflClassMember::Type::TYPE_CSTRING, "HEMS" },
    { "spawntype", hh::fnd::RflClassMember::Type::TYPE_CSTRING, "ETERNAL" },
};

const GameObjectClass ObjSpectacleEffectEmitter::gameObjectClass{
    "SpectacleEffectEmitter",
    "SpectacleEffectEmitter",
    sizeof(ObjSpectacleEffectEmitter),
    &ObjSpectacleEffectEmitter::Create,
    sizeof(ObjSpectacleEffectEmitter::attributes) / sizeof(hh::fnd::RflClassMember::Value),
    ObjSpectacleEffectEmitter::attributes,
    &ObjSpectacleEffectEmitterSpawner::rflClass,
};

const GameObjectClass* ObjSpectacleEffectEmitter::GetClass() {
    return &gameObjectClass;
}
