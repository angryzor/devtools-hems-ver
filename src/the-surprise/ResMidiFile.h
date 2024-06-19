#pragma once
#include <MidiFile.h>

class ResMidiFile : public hh::fnd::ManagedResource {
public:
   smf::MidiFile midiFile;

   virtual void Load(void* data, size_t size) override;
   virtual void Unload() override;
   virtual void Reload(void* data, size_t size) override;

   MANAGED_RESOURCE_CLASS_DECLARATION(ResMidiFile);
};
