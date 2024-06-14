#pragma once
#include "FireworkSpectacleDesc.h"
#include <MidiFile.h>

class ObjFireworksSpectacle : public hh::game::GameObject {
public:
    smf::MidiFile midiFile{};
    int index{};
    float runTime{};
    bool inFirstSection{ true };

    virtual bool ProcessMessage(hh::fnd::Message& message) override { return false; }
    virtual void Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo) override;
    virtual void AddCallback(hh::game::GameManager* gameManager) override;
    virtual void RemoveCallback(hh::game::GameManager* gameManager) override;

    GAMEOBJECT_CLASS_DECLARATION(ObjFireworksSpectacle)
};
