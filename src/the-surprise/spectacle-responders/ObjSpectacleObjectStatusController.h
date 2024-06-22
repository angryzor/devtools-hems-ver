#pragma once
#include <the-surprise/SpectacleSignalId.h>

struct ObjSpectacleObjectStatusControllerSpawner {
    SpectacleSignalId signalId;
    SpectacleSignalId unused;
    csl::ut::MoveArray<hh::game::ObjectId> targets;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[3];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

class ObjSpectacleObjectStatusController : public hh::game::GameObject {
    SpectacleSignalId signalId{};
public:
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    void StartTargets() const;
    void StopTargets() const;

    static const char* category;
    static const hh::fnd::RflClassMember::Value attributes[1];
    GAMEOBJECT_CLASS_DECLARATION(ObjSpectacleObjectStatusController)
};
