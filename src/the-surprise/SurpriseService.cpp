#include "SurpriseService.h"
#include "ObjHEMSMember.h"
#include "Members.h"

using namespace hh::fnd;
using namespace hh::game;

SurpriseService::SurpriseService(csl::fnd::IAllocator* allocator) : GameService{ allocator }, mt{ std::random_device()() } {
    for (short i = 0; i < NUM_BANKS; i++) {
        for (short j = 0; j < 32; j++) {
            new (&signalListeners[i][j]) csl::ut::MoveArray<SpectacleSignalId>(GetAllocator());
        }
    }
}

const GameServiceClass* SurpriseService::GetClass() {
    return &gameServiceClass;
}

GameService* SurpriseService::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) SurpriseService{ allocator };
}

const GameServiceClass SurpriseService::gameServiceClass{
    "SurpriseService",
    &SurpriseService::Create,
    0,
};

void* SurpriseService::GetRuntimeTypeInfo() {
    return nullptr;
}

bool SurpriseService::ProcessMessage(Message& message) {
    return false;
}

void SurpriseService::OnAddedToGame() {
    pGameManager->AddListener(this);
}
void SurpriseService::OnRemovedFromGame() {
    pGameManager->RemoveListener(this);
}

void SurpriseService::GameObjectAddedCallback(GameManager* gameManager, GameObject* gameObject) {
    if (gameObject->objectClass == app::ObjKodama::GetClass()) {
        auto* worldData = gameObject->GetWorldDataByClass<heur::rfl::ObjKodamaSpawner>();
        switch (mode) {
        case Mode::COLLECTED_RANDOM:
            if (gameManager->IsWorldFlagSet(worldData->no, 64) && (mt() % 100) < randomThreshold)
                AddHEMSMember(gameObject);
            break;
        case Mode::COLLECTED:
            if (gameManager->IsWorldFlagSet(worldData->no, 64))
                AddHEMSMember(gameObject);
            break;
        case Mode::ALL_RANDOM:
            if ((mt() % 100) < randomThreshold)
                AddHEMSMember(gameObject);
            break;
        case Mode::ALL:
        case Mode::DDAY:
            AddHEMSMember(gameObject);
            break;
        }
    }
}

void SurpriseService::GameObjectRemovedCallback(GameManager* gameManager, GameObject* gameObject) {
    //if (gameObject->objectClass == app::ObjKodama::GetClass())
    //    if (auto* hemsMember = hemsMembers.GetValueOrFallback(gameObject, nullptr))
    //        hemsMember->Kill();
}

void SurpriseService::AddSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher) {
    signalListeners[signalId.channel][signalId.id].push_back(launcher);
}

void SurpriseService::RemoveSignalListener(SpectacleSignalId signalId, const hh::fnd::Handle<hh::game::GameObject>& launcher) {
    signalListeners[signalId.channel][signalId.id].remove(signalListeners[signalId.channel][signalId.id].find(launcher));
}

csl::ut::MoveArray<hh::fnd::Handle<hh::game::GameObject>>& SurpriseService::GetSignalListeners(SpectacleSignalId signalId) {
    return signalListeners[signalId.channel][signalId.id];
}

void SurpriseService::AddHEMSMember(GameObject* kodama)
{
    auto* hemsMember = ObjHEMSMember::GetClass()->Create<ObjHEMSMember>(GetAllocator());
    auto kodamaId = kodama->GetWorldDataByClass<heur::rfl::ObjKodamaSpawner>()->no;
    WorldPosition pos{};
    hemsMember->Setup(kodama->GetWorldObjectStatus()->objectData->id);
    pGameManager->AddGameObject(hemsMember, kodamaId < sizeof(modderMembers) / sizeof(const char*) ? modderMembers[kodamaId] : normalMembers[mt() % (sizeof(modderMembers) / sizeof(const char*))], false, &pos, kodama);
    //hemsMembers.Insert(kodama, hemsMember);
}

//class membuf : public std::basic_streambuf<char> {
//public:
//    membuf(const uint8_t* p, size_t l) {
//        setg((char*)p, (char*)p, (char*)p + l);
//    }
//};
//
//class memstream : public std::istream {
//public:
//    memstream(const uint8_t* p, size_t l) :
//        std::istream(&_buffer),
//        _buffer(p, l) {
//        rdbuf(&_buffer);
//    }
//
//private:
//    membuf _buffer;
//};
//
//void ResMidiFile::Load(void* data, size_t size)
//{
//    memstream input{ static_cast<uint8_t*>(data), size };
//    midiFile.readSmf(input);
//    midiFile.doTimeAnalysis();
//    midiFile.joinTracks();
//}
//
//void ResMidiFile::Unload()
//{
//}
//
//void ResMidiFile::Reload(void* data, size_t size)
//{
//}
