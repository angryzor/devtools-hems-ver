#pragma once

const short NUM_BANKS = 64;

struct FireworkDesc {
    float explodeTime;
    float travelTime;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[2];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

struct FireworkControlDesc {
    bool enabled;
    unsigned int signals[NUM_BANKS];
    FireworkDesc fireworkDesc;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassEnumMember signalMaskValues[32];
    static const hh::fnd::RflArray<hh::fnd::RflClassEnumMember> signalFlags;
    static const hh::fnd::RflCustomAttribute enabledSignalsAttributeList[1];
    static const hh::fnd::RflCustomAttributes enabledSignalsAttributes;
    static const hh::fnd::RflClassEnum rflClassEnums[1];
    static const hh::fnd::RflClassMember rflClassMembers[3];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

struct FireworkSpectacleDesc {
    FireworkControlDesc triggers[4493];

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[1];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

extern FireworkSpectacleDesc fireworkDescs;

struct ObjFireworkLauncherSpawner {
    short bank;
    short no;
    csl::ut::VariableString effectName;

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[3];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;
};

class ObjFireworkLauncher : public hh::game::GameObject{
public:
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    GAMEOBJECT_CLASS_DECLARATION(ObjFireworkLauncher)
};

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

class ObjHEMSMember : public hh::game::GameObject {
    hh::game::GOCActivator::RangeSpawningConfig activatorConfig{};
    hh::game::ObjectId kodamaId;
public:
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    void Setup(hh::game::ObjectId kodamaId);
    void LookAtHill();

    GAMEOBJECT_CLASS_DECLARATION(ObjHEMSMember)
};

namespace app {
    class ObjKodama {
        GAMEOBJECT_CLASS_DECLARATION(ObjKodama)
    };
}

class SurpriseService : public hh::game::GameService, public hh::game::GameManagerListener {
public:
    enum class Mode {
        COLLECTED_RANDOM,
        COLLECTED,
        ALL_RANDOM,
        ALL,
        DDAY,
    };

    csl::ut::PointerMap<hh::game::GameObject*, ObjHEMSMember*> hemsMembers{ GetAllocator() };
    csl::ut::MoveArray<hh::fnd::Handle<ObjFireworkLauncher>> signalListeners[NUM_BANKS][32];
    std::mt19937 mt;
    Mode mode{};
    uint32_t randomThreshold{};

    void* GetRuntimeTypeInfo() {
        return nullptr;
    }

    void AddSignalListener(short bank, short id, const hh::fnd::Handle<ObjFireworkLauncher>& launcher) {
        signalListeners[bank][id].push_back(launcher);
    }

    void RemoveSignalListener(short bank, short id, const hh::fnd::Handle<ObjFireworkLauncher>& launcher) {
        signalListeners[bank][id].remove(signalListeners[bank][id].find(launcher));
    }

    csl::ut::MoveArray<hh::fnd::Handle<ObjFireworkLauncher>>& GetSignalListeners(short bank, short id) {
        return signalListeners[bank][id];
    }

    virtual bool ProcessMessage(hh::fnd::Message& message) override { return false; }
    virtual void OnAddedToGame();
    virtual void OnRemovedFromGame();
    virtual void GameObjectAddedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject);
    virtual void GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject);
    void AddHEMSMember(hh::game::GameObject* kodama);

    GAMESERVICE_CLASS_DECLARATION(SurpriseService)
};

class ObjFireworksSpectacle : public hh::game::GameObject {
public:
    hh::fnd::ResReflectionT<FireworkSpectacleDesc>* spectacleDesc;
    size_t index{};
    float runTime{};

    virtual bool ProcessMessage(hh::fnd::Message& message) override { return false; }
    virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    GAMEOBJECT_CLASS_DECLARATION(ObjFireworksSpectacle)
};
