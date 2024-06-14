#pragma once
#include "SpectacleSignalId.h"

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

constexpr hh::fnd::MessageID SPECTACLE_MIDI_NOTE_ON{ static_cast<hh::fnd::MessageID>(42069) };
constexpr hh::fnd::MessageID SPECTACLE_MIDI_NOTE_OFF{ static_cast<hh::fnd::MessageID>(42070) };

class MsgSpectacleMidiNoteOn : public app::fnd::AppMessageCustom<MsgSpectacleMidiNoteOn> {
public:
    SpectacleSignalId signalId{};

    MsgSpectacleMidiNoteOn(SpectacleSignalId signalId) : app::fnd::AppMessageCustom<MsgSpectacleMidiNoteOn>{ SPECTACLE_MIDI_NOTE_ON }, signalId{ signalId } {

    }
};

class MsgSpectacleMidiNoteOff : public app::fnd::AppMessageCustom<MsgSpectacleMidiNoteOff> {
public:
    SpectacleSignalId signalId{};

    MsgSpectacleMidiNoteOff(SpectacleSignalId signalId) : app::fnd::AppMessageCustom<MsgSpectacleMidiNoteOff>{ SPECTACLE_MIDI_NOTE_OFF }, signalId{ signalId } {

    }
};
