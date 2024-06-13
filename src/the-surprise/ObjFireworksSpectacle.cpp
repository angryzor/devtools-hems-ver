#include "ObjFireworksSpectacle.h"
#include "SurpriseService.h"

using namespace hh::fnd;
using namespace hh::game;

void ObjFireworksSpectacle::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
    switch (phase) {
    case UpdatingPhase::PRE_ANIM:
        if (!spectacleDesc) {
            Kill();
            break;
        }

        float prevRunTime = runTime;
        runTime += updateInfo.deltaTime;

        auto* sdesc = spectacleDesc->GetData();

        if (auto* surpriseService = gameManager->GetService<SurpriseService>()) {
            while (index < sizeof(sdesc->triggers) / sizeof(FireworkControlDesc) && sdesc->triggers[index].fireworkDesc.explodeTime - sdesc->triggers[index].fireworkDesc.travelTime <= runTime) {
                auto& trigger = sdesc->triggers[index];

                if (trigger.enabled) {
                    for (short i = 0; i < NUM_BANKS; i++) {
                        // Early exit without iteration and shifting.
                        if (!trigger.signals[i])
                            continue;

                        for (short j = 0; j < 32; j++) {
                            if (trigger.signals[i] & (1 << j)) {
                                SpectacleSignalId signalId{ i, j };

                                for (auto listener : surpriseService->GetSignalListeners(signalId)) {
                                    MsgSpectacleSignalFired msg{ signalId, trigger.fireworkDesc };
                                    SendMessageImmToGameObject(listener, msg);
                                }
                            }
                        }
                    }
                }
                index++;
            }
        }

        if (index >= sizeof(sdesc->triggers) / sizeof(FireworkControlDesc))
            Kill();

        break;
    }
}

void ObjFireworksSpectacle::AddCallback(GameManager* gameManager) {
    spectacleDesc = ResourceManager::GetInstance()->GetResource<ResReflectionT<FireworkSpectacleDesc>>("surprising_timings");
    auto* soundDirector = gameManager->GetService<app::snd::SoundDirector>();
    soundDirector->PlayBgm({ "bgm_lastboss", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
}

void ObjFireworksSpectacle::RemoveCallback(GameManager* gameManager) {
    auto* soundDirector = gameManager->GetService<app::snd::SoundDirector>();
    soundDirector->PlayBgm({ "", 0, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
}

ObjFireworksSpectacle::ObjFireworksSpectacle(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(5);
    SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

GameObject* ObjFireworksSpectacle::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjFireworksSpectacle{ allocator };
}

const GameObjectClass ObjFireworksSpectacle::gameObjectClass{
    "Firework",
    "Firework",
    sizeof(ObjFireworksSpectacle),
    &ObjFireworksSpectacle::Create,
    0,
    nullptr,
    nullptr,
};

const GameObjectClass* ObjFireworksSpectacle::GetClass() {
    return &gameObjectClass;
}
