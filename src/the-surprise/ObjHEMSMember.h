#pragma once

namespace app {
    class ObjKodama {
        GAMEOBJECT_CLASS_DECLARATION(ObjKodama)
    };
}

struct ObjHEMSMemberSpawner {
    float scale;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[1];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

class ObjHEMSMember : public hh::game::GameObject, public app::game::GOCEventListener {
    hh::game::ObjectId kodamaId{};
    bool placeholder{};
    bool found{};
public:
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    void Setup(hh::game::ObjectId kodamaId, bool placeholder);
    void LookAtHill();

    GAMEOBJECT_CLASS_DECLARATION(ObjHEMSMember)
};
