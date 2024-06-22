#include "TheSurprise.h"

#include <the-surprise/ResMidiFile.h>
#include <the-surprise/ObjFireworksSpectacle.h>
#include <the-surprise/ObjHEMSMember.h>
#include <the-surprise/spectacle-responders/ObjSpectacleEffectEmitter.h>
#include <the-surprise/spectacle-responders/ObjSpectacleObjectStatusController.h>
#include <the-surprise/spectacle-responders/ObjSpectacleLaserProjector.h>
#include <the-surprise/SurpriseService.h>

using namespace hh::fnd;

const char* intro = "HEMS members have invaded the island!\n\nCollect at least 50 of them to unlock a reward!";

void InitTheSurprise(ResourceLoader& resourceLoader)
{
	BuiltinTypeRegistry::GetTypeInfoRegistry()->Register(&ObjSpectacleEffectEmitterSpawner::typeInfo);
	BuiltinTypeRegistry::GetClassNameRegistry()->Register(&ObjSpectacleEffectEmitterSpawner::rflClass);
	BuiltinTypeRegistry::GetTypeInfoRegistry()->Register(&ObjSpectacleObjectStatusControllerSpawner::typeInfo);
	BuiltinTypeRegistry::GetClassNameRegistry()->Register(&ObjSpectacleObjectStatusControllerSpawner::rflClass);
	BuiltinTypeRegistry::GetTypeInfoRegistry()->Register(&ObjSpectacleLaserProjectorSpawner::typeInfo);
	BuiltinTypeRegistry::GetClassNameRegistry()->Register(&ObjSpectacleLaserProjectorSpawner::rflClass);
	BuiltinTypeRegistry::GetTypeInfoRegistry()->Register(&ObjHEMSMemberSpawner::typeInfo);
	BuiltinTypeRegistry::GetClassNameRegistry()->Register(&ObjHEMSMemberSpawner::rflClass);

	const hh::game::GameObjectClass* goClass;
	goClass = ObjSpectacleEffectEmitter::GetClass();
	GameObjectSystem::GetInstance()->gameObjectRegistry->AddObject(&goClass);
	goClass = ObjSpectacleObjectStatusController::GetClass();
	GameObjectSystem::GetInstance()->gameObjectRegistry->AddObject(&goClass);
	goClass = ObjSpectacleLaserProjector::GetClass();
	GameObjectSystem::GetInstance()->gameObjectRegistry->AddObject(&goClass);
	goClass = ObjHEMSMember::GetClass();
	GameObjectSystem::GetInstance()->gameObjectRegistry->AddObject(&goClass);

	ResourceLoader::Unk1 locale{};

	resourceLoader.LoadResource(InplaceTempUri{ "sound/the_end01_sound/bgm_lastboss" }, hh::snd::ResAtomCueSheet::GetTypeInfo(), 0, 1, locale);
	resourceLoader.LoadResource(InplaceTempUri{ "mods/angryzor_devtools/surprising_timings.mid" }, ResMidiFile::GetTypeInfo(), 0, 1, locale);
	resourceLoader.LoadResource(InplaceTempUri{ "mods/angryzor_devtools/surprising_objects.gedit" }, hh::game::ResObjectWorld::GetTypeInfo(), 0, 1, locale);
	resourceLoader.LoadResource(InplaceTempUri{ "mods/angryzor_devtools/surprising_pous.gedit" }, hh::game::ResObjectWorld::GetTypeInfo(), 0, 1, locale);
	resourceLoader.LoadResource(InplaceTempUri{ "mods/angryzor_devtools/surprising_text" }, hh::text::ResText::GetTypeInfo(), 0, 1, locale);

	auto* gameManager = hh::game::GameManager::GetInstance();
	auto* s = gameManager->CreateService<SurpriseService>(hh::fnd::MemoryRouter::GetModuleAllocator());
	gameManager->RegisterService(s);
}
