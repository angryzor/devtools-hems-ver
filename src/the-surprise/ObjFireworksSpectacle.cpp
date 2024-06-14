#include "ObjFireworksSpectacle.h"
#include "SurpriseService.h"

using namespace hh::fnd;
using namespace hh::game;

void ObjFireworksSpectacle::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
    switch (phase) {
    case UpdatingPhase::PRE_ANIM:
        if (auto* soundDirector = gameManager->GetService<app::snd::SoundDirector>())
        if (auto* surpriseService = gameManager->GetService<SurpriseService>()) {
            float playTime = soundDirector->GetBgmSoundHandle(0).GetPlayTime();

            if (inFirstSection && playTime > 279.7350993377483443f) {
                soundDirector->PlayBgm({ "bgm_lastboss", 2, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0x10001, 0 });
                inFirstSection = false;
            }

            while (index < midiFile.getEventCount(0)) {
                auto& midiEvent = midiFile.getEvent(0, index);
                
                if (static_cast<float>(midiEvent.tick) > playTime * 151.0f * static_cast<float>(midiFile.getTicksPerQuarterNote()) / 120.0f)
                    break;

                if (midiEvent.track > NUM_BANKS) {
                    index++;
                    continue;
                }

                if (midiEvent.isNoteOn()) {
                    SpectacleSignalId signalId{ static_cast<short>(midiEvent.track), static_cast<short>(midiEvent.getKeyNumber() - 60) };

                    for (auto listener : surpriseService->GetSignalListeners(signalId)) {
                        MsgSpectacleMidiNoteOn msg{ signalId };
                        SendMessageImmToGameObject(listener, msg);
                    }
                }
                else if (midiEvent.isNoteOff()) {
                    SpectacleSignalId signalId{ static_cast<short>(midiEvent.track), static_cast<short>(midiEvent.getKeyNumber() - 60) };

                    for (auto listener : surpriseService->GetSignalListeners(signalId)) {
                        MsgSpectacleMidiNoteOff msg{ signalId };
                        SendMessageImmToGameObject(listener, msg);
                    }
                }

                index++;
            }
        }

        if (index >= midiFile.getEventCount(0))
            Kill();

        break;
    }
}

void ObjFireworksSpectacle::AddCallback(GameManager* gameManager) {
    inFirstSection = true;

    //spectacleDesc = ResourceManager::GetInstance()->GetResource<ResReflectionT<FireworkSpectacleDesc>>("surprising_timings");
    midiFile.readSmf(".\\Mods\\devtools\\surprising_timings.mid");
    midiFile.joinTracks();

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
