#include "GameServiceInspector.h"
#include <imgui_internal.h>
#include <ui/Desktop.h>

#ifdef DEVTOOLS_TARGET_SDK_wars
#include "game-service-inspectors/FxParamManager.h"
#include "game-service-inspectors/ObjectWorld.h"
#include "game-service-inspectors/StageInfo.h"
#include "game-service-inspectors/CameraManager.h"

typedef std::tuple<
	app::gfx::FxParamManager,
	hh::game::ObjectWorld,
	app::level::StageInfo,
	hh::game::CameraManager
> InspectableServices;
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "game-service-inspectors/FxParamManager.h"
#include "game-service-inspectors/ObjectWorld.h"
#include "game-service-inspectors/StageInfo.h"
#include "game-service-inspectors/LevelInfo.h"
#include "game-service-inspectors/CameraManager.h"
#include "game-service-inspectors/CameraService.h"
#include "game-service-inspectors/GameModeResourceManager.h"
#include "game-service-inspectors/TerrainManager.h"
#include "game-service-inspectors/SoundDirector.h"

#include <the-surprise/SurpriseService.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>

const char* surpriseModeNames[] = { "COLLECTED_RANDOM", "COLLECTED", "ALL_RANDOM", "ALL", "DDAY" };
void RenderGameServiceInspector(SurpriseService& service) {
	ComboEnum("Mode", service.mode, surpriseModeNames);
	Editor("Random threshold", service.randomThreshold);
	Editor("Signal listeners", service.signalListeners);
}

typedef std::tuple<
	app::gfx::FxParamManager,
	hh::game::ObjectWorld,
	app::level::StageInfo,
	app::level::LevelInfo,
	hh::game::CameraManager,
	app::camera::CameraService,
	app::game::GameModeResourceManager,
	app::trr::TerrainManager,
	app::snd::SoundDirector,
	SurpriseService
> InspectableServices;

#endif


template<typename T>
bool RenderServiceInspector(hh::game::GameService& service) {
	bool result{ service.pStaticClass == T::GetClass() };
	
	if (result) {
		ImGui::PushID(&service);
		RenderGameServiceInspector(static_cast<T&>(service));
		ImGui::PopID();
	}

	return result;
}

template<typename = InspectableServices>
class ServiceIterator;
template<typename... T>
class ServiceIterator<std::tuple<T...>> {
public:
	static void Render(hh::game::GameService& service) {
		(RenderServiceInspector<T>(service) || ...) || (ImGui::Text("Inspector for this service not yet implemented"), true);
	}
};

using namespace hh::game;

GameServiceInspector::GameServiceInspector(csl::fnd::IAllocator* allocator) : StandaloneWindow{ allocator } {
	SetTitle("GameService inspector");
}

void GameServiceInspector::RenderContents() {
	auto* gameManager = GameManager::GetInstance();

	for (auto* service : gameManager->GetServices()) {
		ImGui::PushID(service);
		if (ImGui::CollapsingHeader(service->pStaticClass->pName))
			ServiceIterator<>::Render(*service);
		ImGui::PopID();
	}
}
