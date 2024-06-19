#pragma once
#include "SpectacleSignalId.h"
#include "MidiMessages.h"
#include "ObjHEMSMember.h"
#include <MidiFile.h>

using namespace hh::fnd;
using namespace hh::game;

constexpr hh::fnd::MessageID HEMS_MEMBER_FOUND{ static_cast<hh::fnd::MessageID>(42200) };

class MsgHEMSMemberFound : public app::fnd::AppMessageCustom<MsgHEMSMemberFound> {
public:
    int kodamaNo;

    MsgHEMSMemberFound(int kodamaNo) : app::fnd::AppMessageCustom<MsgHEMSMemberFound>{ HEMS_MEMBER_FOUND }, kodamaNo{ kodamaNo } {

    }
};

class SurpriseService : public hh::game::GameService, public hh::game::GameManagerListener {
public:
    enum class Mode {
        COLLECTED_RANDOM,
        COLLECTED,
        ALL_RANDOM,
        ALL,
        DDAY,
    };

    csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>> signalListeners[NUM_BANKS][32];
    csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>> controlListeners[128];
    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader{};
    std::mt19937 mt;
    Mode mode{ Mode::COLLECTED_RANDOM };
    uint32_t randomThreshold{};
    const char* modderMembersByKodamaNo[500]{ nullptr };
    bool isHEMSMemberFound[1000]{ false };

    virtual void* GetRuntimeTypeInfo() override;
    virtual bool ProcessMessage(hh::fnd::Message& message) override;
    virtual void OnAddedToGame() override;
    virtual void OnRemovedFromGame() override;
    virtual void GameObjectAddedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;
    virtual void GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;

    void AddHEMSMember(hh::game::GameObject* kodama, bool placeholder);
    void AddSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher);
    void RemoveSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher);
    void AddControlListener(short controlId, const hh::fnd::Handle<hh::game::GameObject>& launcher);
    void RemoveControlListener(short controlId, const hh::fnd::Handle<hh::game::GameObject>& launcher);
    csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>& GetSignalListeners(SpectacleSignalId signalId);
    csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>& GetControlListeners(short controlId);

    GAMESERVICE_CLASS_DECLARATION(SurpriseService)
};
