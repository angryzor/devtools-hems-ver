#include "TheSurprise.h"
#include <utilities/math/MathUtils.h>
#include "TheSurprise.Timings.h"
#include "TheSurprise.Members.h"

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::eff;

const char* intro = "HEMS members have invaded the island!\n\nCollect at least 50 of them to unlock a reward!";
const char* thankYou = "Thank you Sonic Frontiers, and thank you HEMS.\n\nThis game and this wonderful community helped me through a very difficult period in my life.\n\nHere's to many more amazing modding experiences.";
const char* unlocked = "You have unlocked a new DevTools feature:\n\nCyan Editor";



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
const hh::fnd::RflCustomAttribute FireworkControlDesc::enabledSignalsAttributeList[1]{
    { "DisplayIndex", &signalFlags, nullptr },
};
const hh::fnd::RflCustomAttributes FireworkControlDesc::enabledSignalsAttributes{ enabledSignalsAttributeList, 1 };

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

SurpriseService::SurpriseService(csl::fnd::IAllocator* allocator) : GameService{ allocator }, mt{ std::random_device()() } {}

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

void SurpriseService::AddHEMSMember(hh::game::GameObject* kodama)
{
    auto* hemsMember = ObjHEMSMember::GetClass()->Create<ObjHEMSMember>(GetAllocator());
    auto kodamaId = kodama->GetWorldDataByClass<heur::rfl::ObjKodamaSpawner>()->no;
    WorldPosition pos{};
    hemsMember->Setup(kodama->GetWorldObjectStatus()->objectData->id);
    pGameManager->AddGameObject(hemsMember, kodamaId < sizeof(modderMembers) / sizeof(const char*) ? modderMembers[kodamaId] : normalMembers[mt() % (sizeof(modderMembers) / sizeof(const char*))], false, &pos, kodama);
    //hemsMembers.Insert(kodama, hemsMember);
}

void* ObjFireworkLauncherSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator) {
    auto* self = static_cast<ObjFireworkLauncherSpawner*>(pInstance);
    self->bank = -1;
    self->no = -1;
    new (&self->effectName) csl::ut::VariableString{ pAllocator };
    return self;
}

void ObjFireworkLauncherSpawner::Finish(void* pInstance) {}

void ObjFireworkLauncherSpawner::Clean(void* pInstance) {
    auto* self = static_cast<ObjFireworkLauncherSpawner*>(pInstance);
    self->effectName.~VariableString();
}

const RflClassMember ObjFireworkLauncherSpawner::rflClassMembers[3] = {
    { "bank", nullptr, nullptr, RflClassMember::Type::TYPE_SINT16, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjFireworkLauncherSpawner, bank), nullptr},
    { "no", nullptr, nullptr, RflClassMember::Type::TYPE_SINT16, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjFireworkLauncherSpawner, no), nullptr},
    { "effectName", nullptr, nullptr, RflClassMember::Type::TYPE_STRING, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjFireworkLauncherSpawner, effectName), nullptr },
};

const RflTypeInfo ObjFireworkLauncherSpawner::typeInfo = RflTypeInfo{ "ObjFireworkLauncherSpawner", "ObjFireworkLauncherSpawner", &ObjFireworkLauncherSpawner::Construct, &ObjFireworkLauncherSpawner::Finish, &ObjFireworkLauncherSpawner::Clean, sizeof(ObjFireworkLauncherSpawner) };
const RflClass ObjFireworkLauncherSpawner::rflClass = RflClass{ "ObjFireworkLauncherSpawner", nullptr, sizeof(ObjFireworkLauncherSpawner), nullptr, 0, ObjFireworkLauncherSpawner::rflClassMembers, sizeof(ObjFireworkLauncherSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };

class MsgFireworkTrigger : public app::fnd::AppMessageCustom<MsgFireworkTrigger> {
public:
    FireworkDesc fireworkDesc{};

    MsgFireworkTrigger(const FireworkDesc& fireworkDesc) : app::fnd::AppMessageCustom<MsgFireworkTrigger>{ static_cast<MessageID>(42069) }, fireworkDesc{ fireworkDesc } {

    }
};

class ObjFirework : public GameObject {
    FireworkDesc desc{};
    Handle<ObjFireworkLauncher> launcher{};
    //Handle<hh::path::GOCPath> path{};
    float lifeTime{ 0.0f };

public:
    virtual bool ProcessMessage(Message& message) override { return false; }

    void Setup(ObjFireworkLauncher* launcher, FireworkDesc desc) {
        this->launcher = launcher;
        this->desc = desc;
    }

    virtual void Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
        auto* launcher = GameObjectSystem::GetGameObjectByHandle(this->launcher);
        if (!launcher) {
            Kill();
            return;
        }

        //if (auto* worldData = launcher->GetWorldDataByClass<ObjFireworkLauncherSpawner>()) {
        //    if (lifeTime == 0.0f)
        //        GetComponent<GOCEffect>()->CreateEffect(worldData->trailEffectName, nullptr);

        //    lifeTime += updateInfo.deltaTime;

        //    if (lifeTime > desc.travelTime + 5.0f) {
        //        Kill();
        //        return;
        //    }

        //    auto* gocTransform = GetComponent<GOCTransform>();
        //    gocTransform->SetLocalTranslationAndRotation
        //}
    }

    virtual void AddCallback(GameManager* gameManager) override {
        //if (auto* launcher = GameObjectSystem::GetGameObjectByHandle(this->launcher))
        //if (auto* worldData = launcher->GetWorldDataByClass<ObjFireworkLauncherSpawner>()) {
        //    if (auto* pathSrv = gameManager->GetService<hh::path::PathManager>()) {
        //        path = pathSrv->GetPath(worldData->pathName.c_str());
        //    }

        //    auto* gocEffect = CreateComponent<GOCEffect>();
        //    gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
        //    AddComponent(gocEffect);

        //    //if (auto* pLauncher = GameObjectSystem::GetGameObjectByHandle(launcher))
        //    //if (auto* launcherConfig = pLauncher->GetWorldDataByClass<ObjFireworkLauncherSpawner>())
        //    //    gocEffect->CreateEffect(launcherConfig->effectName.c_str(), nullptr);
        //}
    }

    virtual void RemoveCallback(GameManager* gameManager) override {
    }

    GAMEOBJECT_CLASS_DECLARATION(ObjFirework)
};

const GameObjectClass* ObjFirework::GetClass() {
    return &gameObjectClass;
}

ObjFirework::ObjFirework(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {}

GameObject* ObjFirework::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjFirework{ allocator };
}

const GameObjectClass ObjFirework::gameObjectClass{
    "Firework",
    "Firework",
    sizeof(ObjFirework),
    &ObjFirework::Create,
    0,
    nullptr,
    nullptr,
};

ObjFireworkLauncher::ObjFireworkLauncher(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {}

bool ObjFireworkLauncher::ProcessMessage(Message& message) {
    switch (static_cast<unsigned int>(message.ID)) {
    case 42069: {
        auto* config = GetWorldDataByClass<ObjFireworkLauncherSpawner>();
        auto& msg = static_cast<MsgFireworkTrigger&>(message);

        if (msg.fireworkDesc.travelTime == 0.0f) {
            GetComponent<GOCEffect>()->CreateEffect(config->effectName, nullptr);
        }
        //auto* firework = static_cast<ObjFirework*>(ObjFirework::GetClass()->instantiator(GetAllocator()));
        //firework->Setup(this);

        //WorldPosition position{};
        //gameManager->AddGameObject(firework, nullptr, false, &position, this);
        return true;
    }
    default:
        return false;
    }
}

void ObjFireworkLauncher::AddCallback(GameManager* gameManager) {
    auto* config = GetWorldDataByClass<ObjFireworkLauncherSpawner>();
    if (config->bank < 0 || config->bank >= NUM_BANKS || config->no < 0 || config->no >= 32)
        return;

    gameManager->GetService<SurpriseService>()->AddSignalListener(config->bank, config->no, this);

    auto* gocEffect = CreateComponent<GOCEffect>();
    gocEffect->Setup({ 0, 1, 1.0f, 0, -1, 0, 0 });
    AddComponent(gocEffect);
}

void ObjFireworkLauncher::RemoveCallback(GameManager* gameManager) {
    auto* config = GetWorldDataByClass<ObjFireworkLauncherSpawner>();
    if (config->bank < 0 || config->bank >= NUM_BANKS || config->no < 0 || config->no >= 32)
        return;

    gameManager->GetService<SurpriseService>()->RemoveSignalListener(config->bank, config->no, this);
}

GameObject* ObjFireworkLauncher::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjFireworkLauncher{ allocator };
}

const char* category = "Common";
hh::fnd::RflClassMember::Value launcherAttrs[]{
    { "category", hh::fnd::RflClassMember::Type::TYPE_CSTRING, category },
};

const GameObjectClass ObjFireworkLauncher::gameObjectClass{
    "FireworkLauncher",
    "FireworkLauncher",
    sizeof(ObjFireworkLauncher),
    &ObjFireworkLauncher::Create,
    1,
    launcherAttrs,
    &ObjFireworkLauncherSpawner::rflClass,
};

const GameObjectClass* ObjFireworkLauncher::GetClass() {
    return &gameObjectClass;
}

void ObjFireworksSpectacle::Update(UpdatingPhase phase, const SUpdateInfo& updateInfo) {
    switch (phase) {
    case UpdatingPhase::PRE_ANIM:
        if (!spectacleDesc) {
            Kill();
            break;
        }

        float prevRunTime = runTime;
        runTime += updateInfo.deltaTime;

        auto* sdesc = spectacleDesc->GetData();

        if (auto* surpriseService = gameManager->GetService<SurpriseService>()) {
            while (index < sizeof(sdesc->triggers) / sizeof(FireworkControlDesc) && sdesc->triggers[index].fireworkDesc.explodeTime - sdesc->triggers[index].fireworkDesc.travelTime <= runTime) {
                if (sdesc->triggers[index].enabled) {
                    MsgFireworkTrigger msg{ sdesc->triggers[index].fireworkDesc };
                    for (unsigned short i = 0; i < NUM_BANKS; i++) {
                        // Early exit without iteration and shifting.
                        if (!sdesc->triggers[index].signals[i])
                            continue;

                        for (unsigned short j = 0; j < 32; j++)
                            if (sdesc->triggers[index].signals[i] & (1 << j))
                                for (auto listener : surpriseService->GetSignalListeners(i, j))
                                    SendMessageImmToGameObject(listener, msg);
                    }
                }
                index++;
            }
        }

        if (index >= sizeof(sdesc->triggers) / sizeof(FireworkControlDesc))
            Kill();

        break;
    }
}

void ObjFireworksSpectacle::AddCallback(GameManager* gameManager) {
    spectacleDesc = ResourceManager::GetInstance()->GetResource<ResReflectionT<FireworkSpectacleDesc>>("surprising_timings");
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

bool ObjHEMSMember::ProcessMessage(hh::fnd::Message& message)
{
    switch (message.ID) {
    case MessageID::UPDATE_ACTIVE_OBJECT_IN_EDITOR:
    case MessageID::PARAM_CHANGED_IN_EDITOR: {
        float scale = GetWorldDataByClass<ObjHEMSMemberSpawner>()->scale;
        GetComponent<hh::gfx::GOCVisualTransformed>()->SetLocalScale({ scale, scale, scale });
        LookAtHill();
        return true;
    }
    default:
        return false;
    }
}

static Eigen::Vector3f attractor{ 1575.0f, 0.0f, 1443.0f };

void ObjHEMSMember::AddCallback(hh::game::GameManager* gameManager)
{
    auto* worldData = GetWorldDataByClass<ObjHEMSMemberSpawner>();
    float scale = worldData ? worldData->scale : 1.0f;
    auto* model = CreateComponent<hh::gfx::GOCVisualModel>();
    hh::gfx::GOCVisualModelDescription mdesc{};

    //HFrame* frame = new (std::align_val_t(16), GetAllocator()) HFrame{ GetAllocator() };
    //frame->SetLocalTranslation({ 0.0f, 0.0f, -0.1f });
    //mdesc.frame = frame;

    mdesc.model = ResourceManager::GetInstance()->GetResource<hh::gfx::ResModel>("pou");
    model->Setup(mdesc);
    model->SetLocalScale({ scale, scale, scale });
    AddComponent(model);

    LookAtHill();

    if (auto* objectWorld = gameManager->GetService<ObjectWorld>()) {
        auto& chunk = objectWorld->GetWorldChunks()[0];

        if (auto* kodama = chunk->GetGameObjectByObjectId(kodamaId)) {
            kodama->Shutdown();
            kodama->Kill();
        }

        if (auto* kodamaObjectData = chunk->GetWorldObjectStatusByObjectId(kodamaId).objectData)
        if (auto* kodamaRangeSpawningData = kodamaObjectData->GetComponentDataByType("RangeSpawning")) {
            auto* activator = CreateComponent<GOCActivator>();
            activator->Setup({ *static_cast<GOCActivator::RangeSpawningConfig*>(kodamaRangeSpawningData->data), false });
            AddComponent(activator);
        }
    }
}

void ObjHEMSMember::RemoveCallback(hh::game::GameManager* gameManager)
{
    if (auto* objectWorld = gameManager->GetService<ObjectWorld>()) {
        auto& chunk = objectWorld->GetWorldChunks()[0];

        chunk->Restart(chunk->GetObjectIndexById(kodamaId), false);
    }
}

void ObjHEMSMember::Setup(ObjectId kodamaId)
{
    this->kodamaId = kodamaId;
}

void ObjHEMSMember::LookAtHill()
{
    auto* model = GetComponent<hh::gfx::GOCVisualModel>();
    auto worldMat = TransformToAffine3f(model->frame2->fullTransform);
    auto worldPos = worldMat * Eigen::Vector3f{ 0.0f, 0.0f, 0.0f };
    model->SetLocalRotation({ Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f{ 0.0f, 0.0f, 1.0f }, worldMat.inverse() * Eigen::Vector3f{ 1575.0f, worldPos.y(), 1443.0f }) });
}

ObjHEMSMember::ObjHEMSMember(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(8);
}

GameObject* ObjHEMSMember::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjHEMSMember{ allocator };
}

hh::fnd::RflClassMember::Value hemsMemberAttrs[]{
    { "category", hh::fnd::RflClassMember::Type::TYPE_CSTRING, category },
};

const GameObjectClass ObjHEMSMember::gameObjectClass{
    "HEMSMember",
    "HEMSMember",
    sizeof(ObjHEMSMember),
    &ObjHEMSMember::Create,
    1,
    hemsMemberAttrs,
    &ObjHEMSMemberSpawner::rflClass,
};

const GameObjectClass* ObjHEMSMember::GetClass() {
    return &gameObjectClass;
}

void* ObjHEMSMemberSpawner::Construct(void* pInstance, csl::fnd::IAllocator* pAllocator)
{
    auto* self = static_cast<ObjHEMSMemberSpawner*>(pInstance);
    self->scale = 1.0f;
    return self;
}

void ObjHEMSMemberSpawner::Finish(void* pInstance)
{
}

void ObjHEMSMemberSpawner::Clean(void* pInstance)
{
}

const RflClassMember ObjHEMSMemberSpawner::rflClassMembers[1] = {
    { "scale", nullptr, nullptr, RflClassMember::Type::TYPE_FLOAT, RflClassMember::Type::TYPE_VOID, 0, 0, offsetof(ObjHEMSMemberSpawner, scale), nullptr},
};

const RflTypeInfo ObjHEMSMemberSpawner::typeInfo = RflTypeInfo{ "ObjHEMSMemberSpawner", "ObjHEMSMemberSpawner", &ObjHEMSMemberSpawner::Construct, &ObjHEMSMemberSpawner::Finish, &ObjHEMSMemberSpawner::Clean, sizeof(ObjHEMSMemberSpawner) };
const RflClass ObjHEMSMemberSpawner::rflClass = RflClass{ "ObjHEMSMemberSpawner", nullptr, sizeof(ObjHEMSMemberSpawner), nullptr, 0, ObjHEMSMemberSpawner::rflClassMembers, sizeof(ObjHEMSMemberSpawner::rflClassMembers) / sizeof(RflClassMember), nullptr };
