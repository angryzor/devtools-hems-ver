#pragma once
#include <the-surprise/SpectacleSignalId.h>

struct ObjSpectacleLaserProjectorSpawner {
    SpectacleSignalId signalId;
    short rotationControlId;
    short angleControlId;
    float initialRotationSpeed;
    float maxRotationSpeed;
    float initialAngle;
    float maxAngle;
    csl::ut::VariableString effectName;
    uint8_t laserCount;
    csl::math::Vector3 anisotropicScale;
    float isotropicScale;
    float minAngle;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[12];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

class ObjSpectacleLaserProjector : public hh::game::GameObject {
    SpectacleSignalId signalId{};
    short rotationControlId{ -1 };
    short angleControlId{ -1 };
    float rotationSpeed{};
    float rotation{};
    float angle{};
    hh::fnd::Reference<hh::fnd::HFrame> rotationFrame{};
    csl::ut::MoveArray<hh::fnd::Reference<hh::fnd::HFrame>> laserFrames{ GetAllocator() };

public:
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;
    virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;

    void SetupLaserFrames();

    static const char* category;
    static const hh::fnd::RflClassMember::Value attributes[1];
    GAMEOBJECT_CLASS_DECLARATION(ObjSpectacleLaserProjector)
};
