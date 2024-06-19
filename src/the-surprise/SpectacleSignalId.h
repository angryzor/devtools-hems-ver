#pragma once

const short NUM_BANKS = 16;

struct SpectacleSignalId {
    short channel{ -1 };
    short id{ -1 };

private:
    static void* Construct(void* pInstance, csl::fnd::IAllocator* pAllocator);
    static void Finish(void* pInstance);
    static void Clean(void* pInstance);
public:
    static const hh::fnd::RflClassMember rflClassMembers[2];
    static const hh::fnd::RflTypeInfo typeInfo;
    static const hh::fnd::RflClass rflClass;

    bool IsValid() const;
    bool operator==(SpectacleSignalId other) const;
    bool operator!=(SpectacleSignalId other) const;
};
