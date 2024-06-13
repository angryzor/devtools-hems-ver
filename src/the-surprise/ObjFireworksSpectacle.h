#pragma once
#include "FireworkSpectacleDesc.h"

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
