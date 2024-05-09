#include "NeedleFxSceneDataTesterV2.h"

using namespace hh::fnd;

NeedleFxSceneDataTesterV2::NeedleFxSceneDataTesterV2(csl::fnd::IAllocator* allocator) : StandaloneWindow(allocator)
{
	SetTitle("NeedleFxSceneData testing tool V2");

	auto* gameManager = hh::game::GameManager::GetInstance();

	//if (auto* fxColMgr = gameManager->GetService<app::gfx::FxColManager>()) {
	//	fxColManager = fxColMgr;
	//	gameManager->UnregisterService(fxColMgr);
	//}

	//if (auto* fxParamMgr = gameManager->GetService<app::gfx::FxParamManager>()) {
	//	fxParamManager = fxParamMgr;
	//	gameManager->UnregisterService(fxParamMgr);
	//}
}

NeedleFxSceneDataTesterV2::~NeedleFxSceneDataTesterV2() {
	auto* gameManager = hh::game::GameManager::GetInstance();

	//if (fxParamManager != nullptr)
	//	gameManager->RegisterService(fxParamManager);
	//if (fxColManager != nullptr)
	//	gameManager->RegisterService(fxColManager);
}

void NeedleFxSceneDataTesterV2::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(600, 350), ImGuiCond_Always);
}

constexpr size_t rootJobId = 0x31744634ui64;
constexpr size_t timeJobId = 0x56235675ui64;
constexpr size_t weatherJobId = 0x76894421ui64;

void NeedleFxSceneDataTesterV2::RenderContents()
{
	auto* fxParamManager = hh::game::GameManager::GetInstance()->GetService<app::gfx::FxParamManager>();

	if (!fxParamManager) {
		ImGui::Text("FxParamManager service not active!");
		return;
	}

	if (ImGui::BeginChild("DropRegion", ImVec2(600, 250))) {
		if (resource == nullptr) {
			ImGui::Text("Drag a NeedleFxSceneData RFL resource into this window to test it out.");
			ImGui::Text("Do not drag non-NeedleFxSceneData RFL resources into this window!");
		}
		else {
			ImGui::Text("Linked to resource \"%s\" (ResReflection @ 0x%zx)", resource->GetName(), resource);

			if (ImGui::Checkbox("Set Scene Config", &setConfig)) {
				if (setConfig) {
					fxParamManager->AddNeedleFxSceneConfigInterpolationJob(rootJobId, &resource->reflectionData->config, 20000, 0.0f);
				}
				else {
					fxParamManager->RemoveNeedleFxSceneConfigInterpolationJob(rootJobId, 0.0f);
				}
			}
			if (ImGui::Checkbox("Set NeedleFX Parameter", &setParam)) {
				if (setParam) {
					fxParamManager->AddNeedleFxParameterInterpolationJob(rootJobId, &resource->reflectionData->items[itemId], 20000, 0.0f);
				}
				else {
					fxParamManager->RemoveNeedleFxParameterInterpolationJob(rootJobId, 0.0f);
				}
			}

			if (setParam) {
				ImGui::Indent();
				if (ImGui::SliderInt("Item ID", &itemId, 0, 15))
					fxParamManager->UpdateNeedleFxParameterInterpolationJob(rootJobId, &resource->reflectionData->items[itemId]);
			}
			if(true) {
				if (ImGui::Checkbox("Overlay time parameters", &setTimeParam)) {
					if (setTimeParam) {
						app::rfl::FxAtmosphereParameter atmosphereParam;
						RESOLVE_STATIC_VARIABLE(app::rfl::FxAtmosphereParameter::typeInfo).m_fpConstruct(&atmosphereParam, GetAllocator());
						atmosphereParam.enable = resource->reflectionData->items[itemId].atmosphere.enable;
						atmosphereParam.sunParam = resource->reflectionData->items[itemId].atmosphere.sunParam;
						atmosphereParam.moonParam = resource->reflectionData->items[itemId].atmosphere.moonParam;
						atmosphereParam.commonSkyParam = resource->reflectionData->timeItems[timeItemId].atmosphere.commonSkyParam;
						atmosphereParam.brunetonSkyParam = resource->reflectionData->timeItems[timeItemId].atmosphere.brunetonSkyParam;
						atmosphereParam.sebastienSkyParam = resource->reflectionData->timeItems[timeItemId].atmosphere.sebastienSkyParam;
						atmosphereParam.cloudParam = resource->reflectionData->timeItems[timeItemId].atmosphere.cloudParam;
						atmosphereParam.crepuscularRayParam = resource->reflectionData->timeItems[timeItemId].atmosphere.crepuscularRayParam;
						atmosphereParam.heightFogParam = resource->reflectionData->timeItems[timeItemId].atmosphere.heightFogParam;

						fxParamManager->mutex.Lock();
						fxParamManager->paramInterpolators.bloomInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].bloom, 0xFFFFFFFE, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.cameraControlInterpolator->AddJob(timeJobId, timeUseEventCameraControl ? &resource->reflectionData->timeItems[timeItemId].cameraControlEvent : &resource->reflectionData->timeItems[timeItemId].cameraControl, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.tonemapInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].tonemap, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.colorContrastInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].colorContrast, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.lightscatteringInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].lightscattering, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.effectInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].effect, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.ssGodrayInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].ssGodray, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.godrayInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].godray, 0xFFFFFFFC, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.heatHazeInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].heatHaze, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.weatherInterpolator->AddJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].weather, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->paramInterpolators.atmosphereInterpolator->AddJob(timeJobId, &atmosphereParam, 0xFFFFFFFF, 20001, 0.0f, -1);
						fxParamManager->mutex.Unlock();
					}
					else {
						fxParamManager->mutex.Lock();
						fxParamManager->paramInterpolators.bloomInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.cameraControlInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.tonemapInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.colorContrastInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.lightscatteringInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.effectInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.ssGodrayInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.godrayInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.heatHazeInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.weatherInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->paramInterpolators.atmosphereInterpolator->ReverseJob(timeJobId, 0.0f);
						fxParamManager->mutex.Unlock();
					}
				}

				if (setTimeParam) {
					ImGui::Indent();
					ImGui::SliderInt("Time item ID", &timeItemId, 0, 23);
					ImGui::Checkbox("Use event camera control setting", &timeUseEventCameraControl);

					app::rfl::FxAtmosphereParameter atmosphereParam;
					RESOLVE_STATIC_VARIABLE(app::rfl::FxAtmosphereParameter::typeInfo).m_fpConstruct(&atmosphereParam, GetAllocator());
					atmosphereParam.enable = resource->reflectionData->items[itemId].atmosphere.enable;
					atmosphereParam.sunParam = resource->reflectionData->items[itemId].atmosphere.sunParam;
					atmosphereParam.moonParam = resource->reflectionData->items[itemId].atmosphere.moonParam;
					atmosphereParam.commonSkyParam = resource->reflectionData->timeItems[timeItemId].atmosphere.commonSkyParam;
					atmosphereParam.brunetonSkyParam = resource->reflectionData->timeItems[timeItemId].atmosphere.brunetonSkyParam;
					atmosphereParam.sebastienSkyParam = resource->reflectionData->timeItems[timeItemId].atmosphere.sebastienSkyParam;
					atmosphereParam.cloudParam = resource->reflectionData->timeItems[timeItemId].atmosphere.cloudParam;
					atmosphereParam.crepuscularRayParam = resource->reflectionData->timeItems[timeItemId].atmosphere.crepuscularRayParam;
					atmosphereParam.heightFogParam = resource->reflectionData->timeItems[timeItemId].atmosphere.heightFogParam;

					fxParamManager->mutex.Lock();
					fxParamManager->paramInterpolators.bloomInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].bloom);
					fxParamManager->paramInterpolators.cameraControlInterpolator->UpdateJob(timeJobId, timeUseEventCameraControl ? &resource->reflectionData->timeItems[timeItemId].cameraControlEvent : &resource->reflectionData->timeItems[timeItemId].cameraControl);
					fxParamManager->paramInterpolators.tonemapInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].tonemap);
					fxParamManager->paramInterpolators.colorContrastInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].colorContrast);
					fxParamManager->paramInterpolators.lightscatteringInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].lightscattering);
					fxParamManager->paramInterpolators.effectInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].effect);
					fxParamManager->paramInterpolators.ssGodrayInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].ssGodray);
					fxParamManager->paramInterpolators.godrayInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].godray);
					fxParamManager->paramInterpolators.heatHazeInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].heatHaze);
					fxParamManager->paramInterpolators.weatherInterpolator->UpdateJob(timeJobId, &resource->reflectionData->timeItems[timeItemId].weather);
					fxParamManager->paramInterpolators.atmosphereInterpolator->UpdateJob(timeJobId, &atmosphereParam);
					fxParamManager->mutex.Unlock();

					ImGui::Unindent();
				}

				if (ImGui::Checkbox("Overlay weather parameters", &setWeatherParam)) {
					if (setWeatherParam) {
						app::rfl::FxAtmosphereParameter atmosphereParam;
						RESOLVE_STATIC_VARIABLE(app::rfl::FxAtmosphereParameter::typeInfo).m_fpConstruct(&atmosphereParam, GetAllocator());
						atmosphereParam.enable = resource->reflectionData->items[itemId].atmosphere.enable;
						atmosphereParam.sunParam = resource->reflectionData->items[itemId].atmosphere.sunParam;
						atmosphereParam.moonParam = resource->reflectionData->items[itemId].atmosphere.moonParam;
						atmosphereParam.commonSkyParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.commonSkyParam;
						atmosphereParam.brunetonSkyParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.brunetonSkyParam;
						atmosphereParam.sebastienSkyParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.sebastienSkyParam;
						atmosphereParam.cloudParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.cloudParam;
						atmosphereParam.crepuscularRayParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.crepuscularRayParam;
						atmosphereParam.heightFogParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.heightFogParam;

						fxParamManager->mutex.Lock();
						fxParamManager->paramInterpolators.bloomInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].bloom, 0xFFFFFFFE, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.cameraControlInterpolator->AddJob(weatherJobId, weatherUseEventCameraControl ? &resource->reflectionData->weatherItems[weatherItemId].cameraControlEvent : &resource->reflectionData->weatherItems[weatherItemId].cameraControl, 0x10, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.tonemapInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].tonemap, 0x3, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.colorContrastInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].colorContrast, 0xFFFFFFFF, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.lightscatteringInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].lightscattering, 0xFFFFFFFF, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.effectInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].effect, 0x4, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.ssGodrayInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].ssGodray, 0xFFFFFFFF, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.godrayInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].godray, 0xFFFFFFFC, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.heatHazeInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].heatHaze, 0xFFFFFFFF, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.weatherInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].weather, 0xE, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.windInterpolator->AddJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].wind, 0xFFFFFFFF, 20002, 0.0f, -1);
						fxParamManager->paramInterpolators.atmosphereInterpolator->AddJob(weatherJobId, &atmosphereParam, 0xFF28, 20002, 0.0f, -1);
						fxParamManager->mutex.Unlock();
					}
					else {
						fxParamManager->mutex.Lock();
						fxParamManager->paramInterpolators.bloomInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.cameraControlInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.tonemapInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.colorContrastInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.lightscatteringInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.effectInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.ssGodrayInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.godrayInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.heatHazeInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.weatherInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.windInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->paramInterpolators.atmosphereInterpolator->ReverseJob(weatherJobId, 0.0f);
						fxParamManager->mutex.Unlock();
					}
				}

				if (setWeatherParam) {
					ImGui::Indent();
					ImGui::SliderInt("Weather item ID", &weatherItemId, 0, 6);
					ImGui::Checkbox("Use event camera control setting", &weatherUseEventCameraControl);

					app::rfl::FxAtmosphereParameter atmosphereParam;
					RESOLVE_STATIC_VARIABLE(app::rfl::FxAtmosphereParameter::typeInfo).m_fpConstruct(&atmosphereParam, GetAllocator());
					atmosphereParam.enable = resource->reflectionData->items[itemId].atmosphere.enable;
					atmosphereParam.sunParam = resource->reflectionData->items[itemId].atmosphere.sunParam;
					atmosphereParam.moonParam = resource->reflectionData->items[itemId].atmosphere.moonParam;
					atmosphereParam.commonSkyParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.commonSkyParam;
					atmosphereParam.brunetonSkyParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.brunetonSkyParam;
					atmosphereParam.sebastienSkyParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.sebastienSkyParam;
					atmosphereParam.cloudParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.cloudParam;
					atmosphereParam.crepuscularRayParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.crepuscularRayParam;
					atmosphereParam.heightFogParam = resource->reflectionData->weatherItems[weatherItemId].atmosphere.heightFogParam;

					fxParamManager->mutex.Lock();
					fxParamManager->paramInterpolators.bloomInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].bloom);
					fxParamManager->paramInterpolators.cameraControlInterpolator->UpdateJob(weatherJobId, weatherUseEventCameraControl ? &resource->reflectionData->weatherItems[weatherItemId].cameraControlEvent : &resource->reflectionData->weatherItems[weatherItemId].cameraControl);
					fxParamManager->paramInterpolators.tonemapInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].tonemap);
					fxParamManager->paramInterpolators.colorContrastInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].colorContrast);
					fxParamManager->paramInterpolators.lightscatteringInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].lightscattering);
					fxParamManager->paramInterpolators.effectInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].effect);
					fxParamManager->paramInterpolators.ssGodrayInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].ssGodray);
					fxParamManager->paramInterpolators.godrayInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].godray);
					fxParamManager->paramInterpolators.heatHazeInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].heatHaze);
					fxParamManager->paramInterpolators.weatherInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].weather);
					fxParamManager->paramInterpolators.windInterpolator->UpdateJob(weatherJobId, &resource->reflectionData->weatherItems[weatherItemId].wind);
					fxParamManager->paramInterpolators.atmosphereInterpolator->UpdateJob(weatherJobId, &atmosphereParam);
					fxParamManager->mutex.Unlock();

					ImGui::Unindent();
				}
				ImGui::Unindent();
			}

			//auto* renderingEngine = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance())->GetNeedleResourceDevice();
			//if (setConfig)
			//	renderingEngine->SetSceneConfig(&resource->reflectionData->config);
			//if (setParam)
			//	renderingEngine->SetFXParameter(&resource->reflectionData->items[itemId], 0);
		}
	}
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget()) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResReflection<void>::GetTypeInfo())
				resource = static_cast<ResReflection<app::rfl::NeedleFxSceneData>*>(res);
		}
		ImGui::EndDragDropTarget();
	}
}
