#pragma once
#include "SpectacleSignalId.h"
#include "FireworkSpectacleDesc.h"
#include "ObjHEMSMember.h"

using namespace hh::fnd;
using namespace hh::game;

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
    csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>> signalListeners[NUM_BANKS][32];
    std::mt19937 mt;
    Mode mode{};
    uint32_t randomThreshold{};

    virtual void* GetRuntimeTypeInfo() override;
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void OnAddedToGame() override;
    virtual void OnRemovedFromGame() override;
    virtual void GameObjectAddedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;
    virtual void GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;

    void AddHEMSMember(hh::game::GameObject* kodama);
    void AddSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher);
    void RemoveSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher);
    csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>& GetSignalListeners(SpectacleSignalId signalId);

    GAMESERVICE_CLASS_DECLARATION(SurpriseService)
};
