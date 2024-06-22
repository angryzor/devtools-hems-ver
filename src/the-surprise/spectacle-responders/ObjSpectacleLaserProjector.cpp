#include "ObjSpectacleLaserProjector.h"
#include <the-surprise/SurpriseService.h>
#include <utilities/math/MathUtils.h>

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::eff;

void* ObjSpectacleLaserProjectorSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjSpectacleLaserProjectorSpawner*>(pInstance);
    self->signalId = SpectacleSignalId{};
    self->rotationControlId = -1;
    self->angleControlId = -1;
    self->initialRotationSpeed = 0.0f;
    self->maxRotationSpeed = 0.5f;
    self->initialAngle = 0.785398f;
    self->maxAngle = 1.5708f;
    new (&self->effectName) csl::ut::VariableString{ pAllocator };
    self->laserCount = 8;
    self->anisotropicScale = { 1.0f, 1.0f, 1.0f };
    self->isotropicScale = 1.0f;
    self->minAngle = 0.0f;
    return self;
}

void ObjSpectacleLaserProjectorSpawner::Finish(void* pInstance) {}

void ObjSpectacleLaserProjectorSpawner::Clean(void* pInstance) {
    auto* self = static_cast<ObjSpectacleLaserProjectorSpawner*>(pInstance);
    self->effectName.~VariableString();
}

const RflClassMember ObjSpectacleLaserProjectorSpawner::rflClassMembers[12] = {
    { "signalId", &SpectacleSignalId::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, signalId), nullptr},
    { "rotationControlId", nullptr, nullptr, RflClassMember::Type::TYPE_SINT16, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, rotationControlId), nullptr},
    { "angleControlId", nullptr, nullptr, RflClassMember::Type::TYPE_SINT16, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, angleControlId), nullptr},
    { "initialRotationSpeed", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, initialRotationSpeed), nullptr},
    { "maxRotationSpeed", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, maxRotationSpeed), nullptr},
    { "initialAngle", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, initialAngle), nullptr},
    { "maxAngle", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, maxAngle), nullptr},
    { "effectName", nullptr, nullptr, RflClassMember::Type::TYPE_STRING, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, effectName), nullptr },
    { "laserCount", nullptr, nullptr, RflClassMember::Type::TYPE_UINT8, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, laserCount), nullptr },
    { "anisotropicScale", nullptr, nullptr, RflClassMember::Type::TYPE_VECTOR3, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, anisotropicScale), nullptr },
    { "isotropicScale", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, isotropicScale), nullptr },
    { "minAngle", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjSpectacleLaserProjectorSpawner, minAngle), nullptr},
};

const RflTypeInfo ObjSpectacleLaserProjectorSpawner::typeInfo = RflTypeInfo{ "ObjSpectacleLaserProjectorSpawner", "ObjSpectacleLaserProjectorSpawner", &ObjSpectacleLaserProjectorSpawner::Construct, &ObjSpectacleLaserProjectorSpawner::Finish, &ObjSpectacleLaserProjectorSpawner::Clean, sizeof(ObjSpectacleLaserProjectorSpawner) };
const RflClass ObjSpectacleLaserProjectorSpawner::rflClass = RflClass{ "ObjSpectacleLaserProjectorSpawner", nullptr, sizeof(ObjSpectacleLaserProjectorSpawner), nullptr, 0, ObjSpectacleLaserProjectorSpawner::rflClassMembers, sizeof(ObjSpectacleLaserProjectorSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

ObjSpectacleLaserProjector::ObjSpectacleLaserProjector(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(0);
    SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

bool ObjSpectacleLaserProjector::ProcessMessage(Message& message) {
    switch (message.ID) {
    case MessageID::PARAM_CHANGED_IN_EDITOR: {
        auto* worldData = GetWorldDataByClass<ObjSpectacleLaserProjectorSpawner>();

        if (signalId.IsValid())
            gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);
        signalId = worldData->signalId;
        if (signalId.IsValid())
            gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);

        if (rotationControlId >= 0 && rotationControlId < 128)
            gameManager->GetService<SurpriseService>()->RemoveControlListener(rotationControlId, this);
        rotationControlId = worldData->rotationControlId;
        if (rotationControlId >= 0 && rotationControlId < 128)
            gameManager->GetService<SurpriseService>()->AddControlListener(rotationControlId, this);

        if (angleControlId >= 0 && angleControlId < 128)
            gameManager->GetService<SurpriseService>()->RemoveControlListener(angleControlId, this);
        angleControlId = worldData->angleControlId;
        if (angleControlId >= 0 && angleControlId < 128)
            gameManager->GetService<SurpriseService>()->AddControlListener(angleControlId, this);

        GetComponent<GOCEffect>()->StopEffectAll();
        SetupLaserFrames();

        return true;
    }
    case MessageID::GET_DEBUG_COMMENT_IN_EDITOR: {
        auto& msg = static_cast<hh::dbg::MsgGetDebugCommentInEditor&>(message);
        snprintf(msg.comment, sizeof(msg.comment), "On/Off: %d:%d, Rotation: %d, Angle: %d", signalId.channel, signalId.id, rotationControlId, angleControlId);
        return true;
    }
    case SPECTACLE_MIDI_NOTE_ON: {
        auto* worldData = GetWorldDataByClass<ObjSpectacleLaserProjectorSpawner>();
        auto* gocEffect = GetComponent<GOCEffect>();

        for (auto& laserFrame : laserFrames) {
            EffectTransFrameCreateInfo eci{ worldData->effectName };
            eci.transInfo.frame = laserFrame;
            eci.transInfo.scale = true;
            gocEffect->CreateEffectEx(eci, nullptr);
        }

        return true;
    }
    case SPECTACLE_MIDI_NOTE_OFF: {
        GetComponent<GOCEffect>()->StopEffectAll();
        return true;
    }
    case SPECTACLE_MIDI_CONTROL_CHANGE: {
        auto* worldData = GetWorldDataByClass<ObjSpectacleLaserProjectorSpawner>();
        auto& msg = static_cast<MsgSpectacleMidiControlChange&>(message);

        if (msg.controlId == rotationControlId) {
            rotationSpeed = worldData->maxRotationSpeed * static_cast<float>(static_cast<short>(msg.value) - 64) / 63.0f;
        }
        if (msg.controlId == angleControlId) {
            angle = worldData->minAngle + (worldData->maxAngle - worldData->minAngle) * static_cast<float>(msg.value) / 127.0f;
        }

        return true;
    }
    default:
        return GameObject::ProcessMessage(message);
    }
}

void ObjSpectacleLaserProjector::AddCallback(GameManager* gameManager) {
    auto* worldData = GetWorldDataByClass<ObjSpectacleLaserProjectorSpawner>();

    rotationSpeed = worldData->initialRotationSpeed;
    angle = worldData->initialAngle;

    rotationFrame = new (std::align_val_t(16), GetAllocator()) HFrame{ GetAllocator() };
    GetComponent<GOCTransform>()->GetFrame().AddChild(rotationFrame);

    SetupLaserFrames();

    auto* gocEffect = CreateComponent<GOCEffect>();
    gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
    AddComponent(gocEffect);

    signalId = worldData->signalId;
    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->AddSignalListener(signalId, this);

    rotationControlId = worldData->rotationControlId;
    if (rotationControlId >= 0 && rotationControlId < 128)
        gameManager->GetService<SurpriseService>()->AddControlListener(rotationControlId, this);

    angleControlId = worldData->angleControlId;
    if (angleControlId >= 0 && angleControlId < 128)
        gameManager->GetService<SurpriseService>()->AddControlListener(angleControlId, this);
}

void ObjSpectacleLaserProjector::RemoveCallback(GameManager* gameManager) {
    if (angleControlId >= 0 && angleControlId < 128)
        gameManager->GetService<SurpriseService>()->RemoveControlListener(angleControlId, this);
    angleControlId = -1;

    if (rotationControlId >= 0 && rotationControlId < 128)
        gameManager->GetService<SurpriseService>()->RemoveControlListener(rotationControlId, this);
    rotationControlId = -1;

    if (signalId.IsValid())
        gameManager->GetService<SurpriseService>()->RemoveSignalListener(signalId, this);
    signalId = {};

    laserFrames.clear();
    rotationFrame = nullptr;
}

void ObjSpectacleLaserProjector::Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo)
{
    auto* worldData = GetWorldDataByClass<ObjSpectacleLaserProjectorSpawner>();

    rotation += updateInfo.deltaTime * rotationSpeed;
    rotationFrame->SetLocalRotation({ Eigen::AngleAxisf{ rotation, Eigen::Vector3f::UnitY() } });

    size_t laserCount = laserFrames.size();
    for (uint8_t i = 0; i < laserCount; i++) {
        laserFrames[i]->SetLocalRotation({Eigen::AngleAxisf{i * 6.28319f / static_cast<float>(laserCount), Eigen::Vector3f::UnitY()} *Eigen::AngleAxisf{angle, Eigen::Vector3f::UnitZ()}});
    }
}

void ObjSpectacleLaserProjector::SetupLaserFrames()
{
    auto* worldData = GetWorldDataByClass<ObjSpectacleLaserProjectorSpawner>();

    laserFrames.clear();

    for (uint8_t i = 0; i < worldData->laserCount; i++) {
        auto* laserFrame = new (std::align_val_t(16), GetAllocator()) HFrame{ GetAllocator() };

        csl::math::Transform transform{};
        transform.scale = { worldData->anisotropicScale * worldData->isotropicScale };
        laserFrame->SetLocalTransform(transform);
        rotationFrame->AddChild(laserFrame);

        laserFrames.push_back(laserFrame);
    }
}

GameObject* ObjSpectacleLaserProjector::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjSpectacleLaserProjector{ allocator };
}

const hh::fnd::RflClassMember::Value ObjSpectacleLaserProjector::attributes[1]{
    { "category", hh::fnd::RflClassMember::Type::TYPE_CSTRING, "Tutorial/HEMS" },
    //{ "spawntype", hh::fnd::RflClassMember::Type::TYPE_CSTRING, "ETERNAL" },
};

const GameObjectClass ObjSpectacleLaserProjector::gameObjectClass{
    "SpectacleLaserProjector",
    "SpectacleLaserProjector",
    sizeof(ObjSpectacleLaserProjector),
    &ObjSpectacleLaserProjector::Create,
    sizeof(ObjSpectacleLaserProjector::attributes) / sizeof(hh::fnd::RflClassMember::Value),
    ObjSpectacleLaserProjector::attributes,
    &ObjSpectacleLaserProjectorSpawner::rflClass,
};

const GameObjectClass* ObjSpectacleLaserProjector::GetClass() {
    return &gameObjectClass;
}
