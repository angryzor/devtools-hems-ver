void RenderGameServiceInspector(app::game::GameModeResourceManager& service)
{
	for (auto& module : service.modules) {
		if (ImGui::TreeNodeEx(module, ImGuiTreeNodeFlags_DefaultOpen, "%x", module->GetNameHash())) {
			ImGui::Text("Module flags: %x", module->flags);
			if (ImGui::TreeNode("Resources")) {
				for (auto& resource : module->resourceCollection->resources) {
					ImGui::Text("%s", resource.name);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}
