#pragma once
#include "MidiMessages.h"
#include "ResMidiFile.h"

struct ObjFireworksSpectacleSpawner {
    int dummy{};
private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[1];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

class ObjFireworksSpectacle : public hh::game::GameObject {
public:
    hh::fnd::Reference<ResMidiFile> midiResource;
    int index{};
    float runTime{};
    bool inFirstSection{ true };
    bool started{ false };

    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;
    void StartSpectacle();
    void EndSpectacle();

    static const hh::fnd::RflClassMember::Value attributes[1];

    GAMEOBJECT_CLASS_DECLARATION(ObjFireworksSpectacle)
};
