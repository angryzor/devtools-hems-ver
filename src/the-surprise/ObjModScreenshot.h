#pragma once

// struct ObjModScreenshotSpawner {
//     csl::ut::VariableString screenshotName;

// private:
//     static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
//     static void Finish(void* pInstance);
//     static void Clean(void* pInstance);
// public:
//     static const hh::fnd::RflClassMember rflClassMembers[1];
//     static const hh::fnd::RflTypeInfo typeInfo;
//     static const hh::fnd::RflClass rflClass;
// };

class ObjModScreenshot : public hh::game::GameObject {
    float travelTime{};
    float lifeTime{};
public:
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;
    virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;

    GAMEOBJECT_CLASS_DECLARATION(ObjModScreenshot)
};
