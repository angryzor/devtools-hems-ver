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

constexpr hh::fnd::MessageID SPECTACLE_SIGNAL_FIRED{ static_cast<hh::fnd::MessageID>(42069) };

class MsgSpectacleSignalFired : public app::fnd::AppMessageCustom<MsgSpectacleSignalFired> {
public:
    SpectacleSignalId signalId{};
    FireworkDesc fireworkDesc{};

    MsgSpectacleSignalFired(SpectacleSignalId signalId, const FireworkDesc& fireworkDesc) : app::fnd::AppMessageCustom<MsgSpectacleSignalFired>{ SPECTACLE_SIGNAL_FIRED }, signalId{ signalId }, fireworkDesc{ fireworkDesc } {

    }
};
