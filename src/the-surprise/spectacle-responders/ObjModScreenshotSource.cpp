#include "ObjModScreenshotSource.h"
#include <the-surprise/ObjModScreenshot.h>
#include <the-surprise/SurpriseService.h>
#include <utilities/math/MathUtils.h>

using namespace hh::fnd;
using namespace hh::game;

void* ObjModScreenshotSourceSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjModScreenshotSourceSpawner*>(pInstance);
    self->signalId = SpectacleSignalId{};
    return self;
}

void ObjModScreenshotSourceSpawner::Finish(void* pInstance) {}

void ObjModScreenshotSourceSpawner::Clean(void* pInstance) {}

const RflClassMember ObjModScreenshotSourceSpawner::rflClassMembers[1] = {
    { "signalId", &SpectacleSignalId::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjModScreenshotSourceSpawner, signalId), nullptr},
};

const RflTypeInfo ObjModScreenshotSourceSpawner::typeInfo = RflTypeInfo{ "ObjModScreenshotSourceSpawner", "ObjModScreenshotSourceSpawner", &ObjModScreenshotSourceSpawner::Construct, &ObjModScreenshotSourceSpawner::Finish, &ObjModScreenshotSourceSpawner::Clean, sizeof(ObjModScreenshotSourceSpawner) };
const RflClass ObjModScreenshotSourceSpawner::rflClass = RflClass{ "ObjModScreenshotSourceSpawner", nullptr, sizeof(ObjModScreenshotSourceSpawner), nullptr, 0, ObjModScreenshotSourceSpawner::rflClassMembers, sizeof(ObjModScreenshotSourceSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

ObjModScreenshotSource::ObjModScreenshotSource(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(0);
}

bool ObjModScreenshotSource::ProcessMessage(Message& message) {
    switch (message.ID) {
    case MessageID::PARAM_CHANGED_IN_EDITOR: {
        auto* worldData = GetWorldDataByClass<ObjModScreenshotSourceSpawner>();

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
    case SPECTACLE_MIDI_NOTE_ON: {
        auto* ss = GameObject::Create<ObjModScreenshot>(GetAllocator());
        auto* gocTf = GetComponent<GOCTransform>();
        auto transform = gocTf->GetFrame().fullTransform;

        auto* surpSrv = gameManager->GetService<SurpriseService>();
        Eigen::Vector3f pos = transform.position + Eigen::Vector3f{ static_cast<float>(surpSrv->mt() % 50), 0.0f, static_cast<float>(surpSrv->mt() % 50) };

        WorldPosition wpos{};
        wpos.m_Position = pos;
        wpos.m_Rotation = { Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f{ 0.0f, 0.0f, -1.0f }, Eigen::Vector3f{ 924.0f, pos.y(), 757.0f } - pos) };

        gameManager->AddGameObject(ss, "ModScreenshot", false, &wpos, nullptr);
        return true;
    }
    default:
        return GameObject::ProcessMessage(message);
    }
}

void ObjModScreenshotSource::AddCallback(GameManager* gameManager) {
    auto* config = GetWorldDataByClass<ObjModScreenshotSourceSpawner>();

    signalId = config->signalId;
    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);
}

void ObjModScreenshotSource::RemoveCallback(GameManager* gameManager) {
    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);
    signalId = {};
}

GameObject* ObjModScreenshotSource::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjModScreenshotSource{ allocator };
}

const RflClassMember::Value ObjModScreenshotSource::attributes[]{
    { "category", RflClassMember::Type::TYPE_CSTRING, "HEMS" },
};

const GameObjectClass ObjModScreenshotSource::gameObjectClass{
    "ModScreenshotSource",
    "ModScreenshotSource",
    sizeof(ObjModScreenshotSource),
    &ObjModScreenshotSource::Create,
    1,
    ObjModScreenshotSource::attributes,
    &ObjModScreenshotSourceSpawner::rflClass,
};

const GameObjectClass* ObjModScreenshotSource::GetClass() {
    return &gameObjectClass;
}
