#include "ObjHEMSMember.h"
#include <utilities/math/MathUtils.h>
#include "SurpriseService.h"

using namespace hh::fnd;
using namespace hh::game;

bool ObjHEMSMember::ProcessMessage(Message& message)
{
    switch (message.ID) {
    case MessageID::UPDATE_ACTIVE_OBJECT_IN_EDITOR:
    case MessageID::PARAM_CHANGED_IN_EDITOR: {
        float scale = GetWorldDataByClass<ObjHEMSMemberSpawner>()->scale;
        GetComponent<hh::gfx::GOCVisualTransformed>()->SetLocalScale({ scale, scale, scale });
        LookAtHill();
        return true;
    }
    case MessageID::GET_DEBUG_COMMENT_IN_EDITOR: {
        auto& msg = static_cast<hh::dbg::MsgGetDebugCommentInEditor&>(message);
        snprintf(msg.comment, sizeof(msg.comment), "%s", name.c_str());
        return true;
    }
    case MessageID::TRIGGER_ENTER: {
        if (kodamaId == ObjectId{})
            return true;

        auto* gocEffect = GetComponent<hh::eff::GOCEffect>();
        gocEffect->CreateEffect("ef_ko_vanish01_gold", nullptr);

        if (auto* objectWorld = gameManager->GetService<ObjectWorld>()) {
            auto& chunk = objectWorld->GetWorldChunks()[0];
            if (auto* kodamaObjectData = chunk->GetWorldObjectStatusByObjectId(kodamaId).objectData) {
                auto kodamaNo = static_cast<heur::rfl::ObjKodamaSpawner*>(kodamaObjectData->spawnerData)->no;

                if (kodamaNo < 0 || kodamaNo >= 999)
                    return true;

                MsgHEMSMemberFound msg{ kodamaNo };
                msg.Sender = this;
                gameManager->SendMessageImmToService(msg);
            }
        }

        found = true;
        Kill();
        return true;
    }
    default:
        return GameObject::ProcessMessage(message);
    }
}

void ObjHEMSMember::AddCallback(hh::game::GameManager* gameManager)
{
    if (!placeholder) {
        auto* worldData = GetWorldDataByClass<ObjHEMSMemberSpawner>();
        float scale = worldData ? worldData->scale : 1.0f;

        hh::gfx::GOCVisualModelDescription mdesc{};
        mdesc.model = ResourceManager::GetInstance()->GetResource<hh::gfx::ResModel>("pou");

        auto* model = CreateComponent<hh::gfx::GOCVisualModel>();
        model->Setup(mdesc);
        model->SetLocalScale({ scale, scale, scale });
        AddComponent(model);

        LookAtHill();
    }

    if (kodamaId != ObjectId{}) {
        if (auto* objectWorld = gameManager->GetService<ObjectWorld>()) {
            auto& chunk = objectWorld->GetWorldChunks()[0];

            if (!placeholder) {
                if (auto* kodama = chunk->GetGameObjectByObjectId(kodamaId)) {
                    kodama->Shutdown();
                    kodama->Kill();
                }
            }

            if (auto* kodamaObjectData = chunk->GetWorldObjectStatusByObjectId(kodamaId).objectData)
            if (auto* kodamaRangeSpawningData = kodamaObjectData->GetComponentDataByType("RangeSpawning")) {
                auto* activator = CreateComponent<GOCActivator>();
                activator->Setup({ *static_cast<GOCActivator::RangeSpawningConfig*>(kodamaRangeSpawningData->data), false });
                AddComponent(activator);
            }

            if (auto* surpriseService = gameManager->GetService<SurpriseService>())
            if (surpriseService->mode == SurpriseService::Mode::DDAY) {
                auto* collectCollider = CreateComponent<hh::physics::GOCSphereCollider>();
                hh::physics::GOCSphereCollider::SetupInfo colliderSetupInfo{};
                colliderSetupInfo.radius = 1.5f;
                colliderSetupInfo.unk3 |= 1;
                colliderSetupInfo.filterCategory = 25;
                colliderSetupInfo.unk4 = 0x8000;
                colliderSetupInfo.SetPosition({ 0.0f, 0.25f, 0.0f });
                collectCollider->Setup(colliderSetupInfo);
                AddComponent(collectCollider);

                auto* gocEffect = CreateComponent<hh::eff::GOCEffect>();
                gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
                AddComponent(gocEffect);
            }
        }
    }
}

void ObjHEMSMember::RemoveCallback(hh::game::GameManager* gameManager)
{
    if (kodamaId != ObjectId{} && !found)
    if (auto* objectWorld = gameManager->GetService<ObjectWorld>()) {
        auto& chunk = objectWorld->GetWorldChunks()[0];

        chunk->Restart(chunk->GetObjectIndexById(kodamaId), false);
    }
}

void ObjHEMSMember::Setup(ObjectId kodamaId, bool placeholder)
{
    this->kodamaId = kodamaId;
    this->placeholder = placeholder;

    if (placeholder)
        SetLayer(4);
}

void ObjHEMSMember::LookAtHill()
{
    auto* model = GetComponent<hh::gfx::GOCVisualModel>();
    auto worldMat = TransformToAffine3f(model->frame2->fullTransform);
    auto worldPos = worldMat * Eigen::Vector3f{ 0.0f, 0.0f, 0.0f };
    model->SetLocalRotation({ Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f{ 0.0f, 0.0f, 1.0f }, worldMat.inverse() * Eigen::Vector3f{ 1575.0f, worldPos.y(), 1443.0f }) });
}

ObjHEMSMember::ObjHEMSMember(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(8);
}

GameObject* ObjHEMSMember::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjHEMSMember{ allocator };
}

RflClassMember::Value hemsMemberAttrs[]{
    { "category", RflClassMember::Type::TYPE_CSTRING, "HEMS" },
};

const GameObjectClass ObjHEMSMember::gameObjectClass{
    "HEMSMember",
    "HEMSMember",
    sizeof(ObjHEMSMember),
    &ObjHEMSMember::Create,
    1,
    hemsMemberAttrs,
    &ObjHEMSMemberSpawner::rflClass,
};

const GameObjectClass* ObjHEMSMember::GetClass() {
    return &gameObjectClass;
}

void* ObjHEMSMemberSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator)
{
    auto* self = static_cast<ObjHEMSMemberSpawner*>(pInstance);
    self->scale = 1.0f;
    return self;
}

void ObjHEMSMemberSpawner::Finish(void* pInstance)
{
}

void ObjHEMSMemberSpawner::Clean(void* pInstance)
{
}

const RflClassMember ObjHEMSMemberSpawner::rflClassMembers[1] = {
    { "scale", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjHEMSMemberSpawner, scale), nullptr},
};

const RflTypeInfo ObjHEMSMemberSpawner::typeInfo = RflTypeInfo{ "ObjHEMSMemberSpawner", "ObjHEMSMemberSpawner", &ObjHEMSMemberSpawner::Construct, &ObjHEMSMemberSpawner::Finish, &ObjHEMSMemberSpawner::Clean, sizeof(ObjHEMSMemberSpawner) };
const RflClass ObjHEMSMemberSpawner::rflClass = RflClass{ "ObjHEMSMemberSpawner", nullptr, sizeof(ObjHEMSMemberSpawner), nullptr, 0, ObjHEMSMemberSpawner::rflClassMembers, sizeof(ObjHEMSMemberSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };
