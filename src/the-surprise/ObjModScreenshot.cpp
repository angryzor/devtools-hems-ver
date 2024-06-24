#include "ObjModScreenshot.h"
#include "SurpriseService.h"

using namespace hh::fnd;
using namespace hh::game;

void ObjModScreenshot::AddCallback(hh::game::GameManager* gameManager)
{
    hh::gfx::GOCVisualModelDescription mdesc{};
    auto* resModel = ResourceManager::GetInstance()->GetResource<hh::gfx::ResModel>("test");
    mdesc.flags.m_dummy |= 0x2000000;
    mdesc.model = resModel;

    auto* model = CreateComponent<hh::gfx::GOCVisualModel>();
    model->Setup(mdesc);
    model->SetLocalScale({ 15.0f, 15.0f, 15.0f });
    AddComponent(model);

    auto* surpSrv = gameManager->GetService<SurpriseService>();

    char screenshotName[100];
    snprintf(screenshotName, sizeof(screenshotName), "mod_screenshot_%d", surpSrv->mt() % 1392);

    if (auto* textureResource = ResourceManager::GetInstance()->GetResource<hh::gfnd::ResTexture>(screenshotName))
    if (auto* meshResource = resModel->GetMeshResource())
    if (auto* material = meshResource->GetMaterialNameID(0))
    if (auto paramsIdx = model->implementation.modelInstance->GetParameterValueObjectContainerByName(material))
    if (auto* params = model->implementation.modelInstance->GetParameterValueObjectContainer(paramsIdx))
    if (auto* texture = textureResource->GetTexture()) {
        auto* dirty = *(hh::needle::CNameIDObject***)(*(size_t*)params + 0x18);
        params->SetTextureByName(dirty[0], texture);
        params->SetTextureByName(dirty[1], texture);
        params->SetTextureByName(dirty[2], texture);
    }

    travelTime = static_cast<float>(surpSrv->mt() % 20) + 35.0f;

    app::game::GOCMotorConstant::SetupInfo gmcsi{};
    gmcsi.timeType = app::game::GOCMotor::TimeType::LOCAL;
    gmcsi.direction = { 0.0f, 1.0f, 0.0f };
    gmcsi.distance = 2000.0f;
    gmcsi.travelTime = travelTime;
    gmcsi.returnTime = 0.0f;
    gmcsi.travelWaitTime = 0.0f;
    gmcsi.returnWaitTime = 0.0f;

    auto* motor = CreateComponent<app::game::GOCMotorConstant>();
    motor->Setup(gmcsi);
    AddComponent(motor);
}

void ObjModScreenshot::RemoveCallback(hh::game::GameManager* gameManager)
{
    
}

void ObjModScreenshot::Update(hh::fnd::UpdatingPhase phase, const hh::fnd::SUpdateInfo& updateInfo)
{
    lifeTime += updateInfo.deltaTime;
    if (lifeTime >= travelTime)
        Kill();
}

ObjModScreenshot::ObjModScreenshot(csl::fnd::IAllocator* allocator) : GameObject{ allocator } {
    SetLayer(8);
    SetUpdateFlag(UpdatingPhase::PRE_ANIM, true);
}

GameObject* ObjModScreenshot::Create(csl::fnd::IAllocator* allocator) {
    return new (allocator) ObjModScreenshot{ allocator };
}

RflClassMember::Value modScreenshotAttrs[]{
    { "category", RflClassMember::Type::TYPE_CSTRING, "HEMS" },
};

const GameObjectClass ObjModScreenshot::gameObjectClass{
    "ModScreenshot",
    "ModScreenshot",
    sizeof(ObjModScreenshot),
    &ObjModScreenshot::Create,
    1,
    modScreenshotAttrs,
    nullptr,
};

const GameObjectClass* ObjModScreenshot::GetClass() {
    return &gameObjectClass;
}
