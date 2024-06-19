#pragma once
#include "SpectacleSignalId.h"

constexpr hh::fnd::MessageID SPECTACLE_MIDI_NOTE_ON{ static_cast<hh::fnd::MessageID>(42069) };
constexpr hh::fnd::MessageID SPECTACLE_MIDI_NOTE_OFF{ static_cast<hh::fnd::MessageID>(42070) };
constexpr hh::fnd::MessageID SPECTACLE_MIDI_CONTROL_CHANGE{ static_cast<hh::fnd::MessageID>(42071) };

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

class MsgSpectacleMidiControlChange : public app::fnd::AppMessageCustom<MsgSpectacleMidiControlChange> {
public:
    short controlId{};
    uint8_t value{};

    MsgSpectacleMidiControlChange(short controlId, uint8_t value) : app::fnd::AppMessageCustom<MsgSpectacleMidiControlChange>{ SPECTACLE_MIDI_CONTROL_CHANGE }, controlId{ controlId }, value{ value } {

    }
};
