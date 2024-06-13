#include "FireworkSpectacleDesc.h"

using namespace hh::fnd;

void* FireworkDesc::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<FireworkDesc*>(pInstance);
    self->explodeTime = 0.0f;
    self->travelTime = 0.0f;
    return self;
}

void FireworkDesc::Finish(void* pInstance) {}

void FireworkDesc::Clean(void* pInstance) {
}

const RflClassMember FireworkDesc::rflClassMembers[2] = {
    { "explodeTime", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(FireworkDesc, explodeTime), nullptr},
    { "travelTime", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(FireworkDesc, travelTime), nullptr },
};

const RflTypeInfo FireworkDesc::typeInfo = RflTypeInfo{ "FireworkDesc", "FireworkDesc", &FireworkDesc::Construct, &FireworkDesc::Finish, &FireworkDesc::Clean, sizeof(FireworkDesc) };
const RflClass FireworkDesc::rflClass = RflClass{ "FireworkDesc", nullptr, sizeof(FireworkDesc), nullptr, 0, FireworkDesc::rflClassMembers, sizeof(FireworkDesc::rflClassMembers) / sizeof(RflClassMember), nullptr };

void* FireworkControlDesc::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<FireworkControlDesc*>(pInstance);
    self->enabled = true;
    for (size_t i = 0; i < NUM_BANKS; i++)
        self->signals[i] = 0;
    FireworkDesc::typeInfo.ConstructObject(&self->fireworkDesc, pAllocator);
    return self;
}

void FireworkControlDesc::Finish(void* pInstance) {
    auto* self = static_cast<FireworkControlDesc*>(pInstance);
    FireworkDesc::typeInfo.FinishLoadedObject(&self->fireworkDesc);
}

void FireworkControlDesc::Clean(void* pInstance) {
    auto* self = static_cast<FireworkControlDesc*>(pInstance);
    FireworkDesc::typeInfo.CleanupLoadedObject(&self->fireworkDesc);
}

const RflClassEnumMember FireworkControlDesc::signalMaskValues[32]{
    { 0, "Signal 0", "Signal 0", 0 },
    { 1, "Signal 1", "Signal 1", 0 },
    { 2, "Signal 2", "Signal 2", 0 },
    { 3, "Signal 3", "Signal 3", 0 },
    { 4, "Signal 4", "Signal 4", 0 },
    { 5, "Signal 5", "Signal 5", 0 },
    { 6, "Signal 6", "Signal 6", 0 },
    { 7, "Signal 7", "Signal 7", 0 },
    { 8, "Signal 8", "Signal 8", 0 },
    { 9, "Signal 9", "Signal 9", 0 },
    { 10, "Signal 10", "Signal 10", 0 },
    { 11, "Signal 11", "Signal 11", 0 },
    { 12, "Signal 12", "Signal 12", 0 },
    { 13, "Signal 13", "Signal 13", 0 },
    { 14, "Signal 14", "Signal 14", 0 },
    { 15, "Signal 15", "Signal 15", 0 },
    { 16, "Signal 16", "Signal 16", 0 },
    { 17, "Signal 17", "Signal 17", 0 },
    { 18, "Signal 18", "Signal 18", 0 },
    { 19, "Signal 19", "Signal 19", 0 },
    { 20, "Signal 20", "Signal 20", 0 },
    { 21, "Signal 21", "Signal 21", 0 },
    { 22, "Signal 22", "Signal 22", 0 },
    { 23, "Signal 23", "Signal 23", 0 },
    { 24, "Signal 24", "Signal 24", 0 },
    { 25, "Signal 25", "Signal 25", 0 },
    { 26, "Signal 26", "Signal 26", 0 },
    { 27, "Signal 27", "Signal 27", 0 },
    { 28, "Signal 28", "Signal 28", 0 },
    { 29, "Signal 29", "Signal 29", 0 },
    { 30, "Signal 30", "Signal 30", 0 },
    { 31, "Signal 31", "Signal 31", 0 },
};
const RflArray<RflClassEnumMember> FireworkControlDesc::signalFlags{ signalMaskValues, 32 };
const RflCustomAttribute FireworkControlDesc::enabledSignalsAttributeList[1]{
    { "DisplayIndex", &signalFlags, nullptr },
};
const RflCustomAttributes FireworkControlDesc::enabledSignalsAttributes{ enabledSignalsAttributeList, 1 };

const RflClassMember FireworkControlDesc::rflClassMembers[3]{
    { "enabled", nullptr, nullptr, RflClassMember::Type::TYPE_BOOL, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(FireworkControlDesc, enabled), nullptr},
    { "signals", nullptr, nullptr, RflClassMember::Type::TYPE_FLAGS, RflClassMember::Type::TYPE_UINT32, NUM_BANKS, 0, offsetof(FireworkControlDesc, signals), &enabledSignalsAttributes },
    { "fireworkDesc", &FireworkDesc::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(FireworkControlDesc, fireworkDesc), nullptr },
};

const RflTypeInfo FireworkControlDesc::typeInfo = RflTypeInfo{ "FireworkControlDesc", "FireworkControlDesc", &FireworkControlDesc::Construct, &FireworkControlDesc::Finish, &FireworkControlDesc::Clean, sizeof(FireworkControlDesc) };
const RflClass FireworkControlDesc::rflClass = RflClass{ "FireworkControlDesc", nullptr, sizeof(FireworkControlDesc), nullptr, 0, FireworkControlDesc::rflClassMembers, sizeof(FireworkControlDesc::rflClassMembers) / sizeof(RflClassMember), nullptr };

void* FireworkSpectacleDesc::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<FireworkSpectacleDesc*>(pInstance);
    for (size_t i = 0; i < sizeof(self->triggers) / sizeof(FireworkControlDesc); i++)
        FireworkControlDesc::typeInfo.ConstructObject(&self->triggers[i], pAllocator);
    return self;
}

void FireworkSpectacleDesc::Finish(void* pInstance) {
    auto* self = static_cast<FireworkSpectacleDesc*>(pInstance);
    for (size_t i = 0; i < sizeof(self->triggers) / sizeof(FireworkControlDesc); i++)
        FireworkControlDesc::typeInfo.FinishLoadedObject(&self->triggers[i]);
}

void FireworkSpectacleDesc::Clean(void* pInstance) {
    auto* self = static_cast<FireworkSpectacleDesc*>(pInstance);
    for (size_t i = 0; i < sizeof(self->triggers) / sizeof(FireworkControlDesc); i++)
        FireworkControlDesc::typeInfo.CleanupLoadedObject(&self->triggers[i]);
}

const RflClassMember FireworkSpectacleDesc::rflClassMembers[1] = {
    { "triggers", &FireworkControlDesc::rflClass, nullptr, RflClassMember::Type::TYPE_STRUCT, RflClassMember::Type::TYPE_VOID, sizeof(decltype(FireworkSpectacleDesc::triggers)) / sizeof(FireworkControlDesc), 0, offsetof(FireworkSpectacleDesc, triggers), nullptr},
};

const RflTypeInfo FireworkSpectacleDesc::typeInfo = RflTypeInfo{ "FireworkSpectacleDesc", "FireworkSpectacleDesc", &FireworkSpectacleDesc::Construct, &FireworkSpectacleDesc::Finish, &FireworkSpectacleDesc::Clean, sizeof(FireworkSpectacleDesc) };
const RflClass FireworkSpectacleDesc::rflClass = RflClass{ "FireworkSpectacleDesc", nullptr, sizeof(FireworkSpectacleDesc), nullptr, 0, FireworkSpectacleDesc::rflClassMembers, sizeof(FireworkSpectacleDesc::rflClassMembers) / sizeof(RflClassMember), nullptr };
