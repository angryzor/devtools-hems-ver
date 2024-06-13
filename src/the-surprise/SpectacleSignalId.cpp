#include "SpectacleSignalId.h"
#include "FireworkSpectacleDesc.h"

using namespace hh::fnd;

void* SpectacleSignalId::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<SpectacleSignalId*>(pInstance);
    self->bank = -1;
    self->id = -1;
    return self;
}

void SpectacleSignalId::Finish(void* pInstance) {}

void SpectacleSignalId::Clean(void* pInstance) {}

const RflClassMember SpectacleSignalId::rflClassMembers[2] = {
    { "bank", nullptr, nullptr, RflClassMember::Type::TYPE_SINT16, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(SpectacleSignalId, bank), nullptr},
    { "id", nullptr, nullptr, RflClassMember::Type::TYPE_SINT16, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(SpectacleSignalId, id), nullptr},
};

const RflTypeInfo SpectacleSignalId::typeInfo = RflTypeInfo{ "SpectacleSignalId", "SpectacleSignalId", &SpectacleSignalId::Construct, &SpectacleSignalId::Finish, &SpectacleSignalId::Clean, sizeof(SpectacleSignalId) };
const RflClass SpectacleSignalId::rflClass = RflClass{ "SpectacleSignalId", nullptr, sizeof(SpectacleSignalId), nullptr, 0, SpectacleSignalId::rflClassMembers, sizeof(SpectacleSignalId::rflClassMembers) / sizeof(RflClassMember), nullptr };

bool SpectacleSignalId::IsValid() const {
    return bank >= 0 && bank < NUM_BANKS && id >= 0 && id < 32;
}

bool SpectacleSignalId::operator==(SpectacleSignalId other) const
{
    return bank == other.bank && id == other.id;
}

bool SpectacleSignalId::operator!=(SpectacleSignalId other) const
{
    return !operator==(other);
}
