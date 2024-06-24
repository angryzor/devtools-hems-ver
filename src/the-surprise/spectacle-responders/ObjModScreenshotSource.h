#pragma once
#include <the-surprise/SpectacleSignalId.h>

struct ObjModScreenshotSourceSpawner {
    SpectacleSignalId signalId;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[1];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

class ObjModScreenshotSource : public hh::game::GameObject {
    SpectacleSignalId signalId;

public:
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    static const hh::fnd::RflClassMember::Value attributes[1];
    GAMEOBJECT_CLASS_DECLARATION(ObjModScreenshotSource)
};
