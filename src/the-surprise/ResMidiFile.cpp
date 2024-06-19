#include "ResMidiFile.h"

class membuf : public std::basic_streambuf<char> {
public:
   membuf(const uint8_t* p, size_t l) {
       setg((char*)p, (char*)p, (char*)p + l);
   }
};

class memstream : public std::istream {
public:
   memstream(const uint8_t* p, size_t l) :
       std::istream(&_buffer),
       _buffer(p, l) {
       rdbuf(&_buffer);
   }

private:
   membuf _buffer;
};

ResMidiFile::ResMidiFile(csl::fnd::IAllocator* allocator) : hh::fnd::ManagedResource{ allocator } {}

void ResMidiFile::Load(void* data, size_t size)
{
   memstream input{ static_cast<uint8_t*>(data), size };
   midiFile.readSmf(input);
   midiFile.joinTracks();
}

void ResMidiFile::Unload()
{
}

void ResMidiFile::Reload(void* data, size_t size)
{
}

hh::fnd::ManagedResource* ResMidiFile::Create(csl::fnd::IAllocator* allocator) {
    return new (std::align_val_t(alignof(ResMidiFile)), allocator) ResMidiFile{ allocator };
}

const hh::fnd::ResourceTypeInfo ResMidiFile::typeInfo{
    "ResMidiFile",
    "ResMidiFile",
    sizeof(ResMidiFile),
    false,
    &ResMidiFile::Create,
};

const hh::fnd::ResourceTypeInfo* ResMidiFile::GetTypeInfo() {
    return &typeInfo;
}
