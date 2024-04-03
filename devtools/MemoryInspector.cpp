#include "Pch.h"
#include "MemoryInspector.h"

using namespace csl::fnd;
using namespace hh::fnd;

HeapInspector::HeapInspector(IAllocator* allocator, const char* name, TlsfHeapBase* target) : ReferencedObject{ allocator, true }, name{ name }, target { target }
{
}

void HeapInspector::Render()
{
	allocationsHistory[nextFrame] = static_cast<float>(target->liveAllocations);
	maxAllocations = std::max(maxAllocations, target->liveAllocations);

	ImGui::SeparatorText(name);
	ImGui::PlotLines("Live allocations", allocationsHistory, 512, nextFrame, nullptr, 0, static_cast<float>(maxAllocations), ImVec2{ 0.0f, 50.0f });
	ImGui::Text("Live allocations: %d", target->liveAllocations);
	ImGui::Text("Total allocations: %d", target->totalAllocations);
	ImGui::Text("Block count: %d", target->blockCount);

	nextFrame = (nextFrame + 1) % 512;
}

MemoryInspector::MemoryInspector(IAllocator* allocator) : StandaloneWindow{ allocator }
{
	auto* mr = hh::fnd::MemoryRouter::GetInstance();
	auto* ahm = app::fnd::AppHeapManager::GetInstance();

	moduleInspector = new (allocator) HeapInspector{ allocator, "Module", mr->moduleAllocator->heap };
	debugInspector = new (allocator) HeapInspector{ allocator, "Debug", mr->debugAllocator->heap };

	criHedgehogInspector = new (allocator) HeapInspector { allocator, "Hedgehog", &(*rangerssdk::GetAddress(&hh::cri::CriSystem::instance))->hedgehogAllocator->heap };

	residentInspector = new (allocator) HeapInspector{ allocator, "Resident", &ahm->residentAllocator.heap };
	cyloopInspector = new (allocator) HeapInspector{ allocator, "Cyloop", &ahm->cyloopAllocator.heap };
	heightfieldInspector = new (allocator) HeapInspector{ allocator, "Heightfield", &ahm->heightfieldAllocator.heap };
	needleInspector = new (allocator) HeapInspector{ allocator, "Needle", &ahm->needleAllocator->heap };

	SetTitle("Memory Inspector");
}

void MemoryInspector::RenderContents()
{
	if (ImGui::CollapsingHeader("Base heaps")) {
		moduleInspector->Render();
		debugInspector->Render();
	}

	if (ImGui::CollapsingHeader("CriSystem heaps")) {
		criHedgehogInspector->Render();
	}

	if (ImGui::CollapsingHeader("AppHeapManager heaps")) {
		residentInspector->Render();
		cyloopInspector->Render();
		heightfieldInspector->Render();
		needleInspector->Render();
	}
}
