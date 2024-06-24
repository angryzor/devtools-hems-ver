#include "ResMaterialEditor.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/inputs/Needle.h>
#include <ui/common/editors/Needle.h>

using namespace hh::needle;

ResMaterialEditor::ResMaterialEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResMaterial* resource) : StandaloneWindow{ allocator }, resource { resource }
{
	char title[400];
	snprintf(title, sizeof(title), "%s - %s @ 0x%zx (MaterialResource @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)&resource->resource);
	SetTitle(title);
}

void ResMaterialEditor::RenderContents()
{
	ImGui::BeginGroup();
	void* data = resource->resource->GetMaterialParameterValues();
	for (size_t i = 0; i < resource->resource->GetMaterialParameterCount(); i++) {
		auto& info = resource->resource->GetMaterialParameterInfos()[i];

		ImGui::PushID(i);
		ImGui::Text("PARAM IDX %d", i);
		switch (info.type) {
		case hh::needle::ParameterType::FLOAT:
			RenderFloatParameterEditor(i, static_cast<ParameterFloatValue*>(data));
			break;
		case hh::needle::ParameterType::BOOL:
			RenderBoolParameterEditor(i, static_cast<ParameterBoolValue*>(data));
			break;
		case hh::needle::ParameterType::INT:
			RenderIntParameterEditor(i, static_cast<ParameterIntValue*>(data));
			break;
		case hh::needle::ParameterType::SAMPLER:
			RenderSamplerParameterEditor(i, static_cast<ParameterSamplerValue*>(data));
			break;
		case hh::needle::ParameterType::RS_FLAG_MASK:
			RenderRsFlagMaskParameterEditor(i, static_cast<ParameterRsFlagMaskValue*>(data));
			break;
		case hh::needle::ParameterType::SHADER_NAME:
			RenderShaderNameParameterEditor(i, static_cast<ParameterShaderNameValue*>(data));
			break;
		case hh::needle::ParameterType::TEXTURE_NAME:
			RenderTextureNameParameterEditor(i, static_cast<ParameterTextureNameValue*>(data));
			break;
		default:
			RenderUnimplementedTypeParameterEditor(i);
			break;
		}
		ImGui::PopID();

		data = reinterpret_cast<void*>(reinterpret_cast<size_t>(data) + info.sizeInDwords * 4);
	}
	ImGui::EndGroup();
	if (ImGui::IsItemEdited()) {
		hh::game::GameManager::GetInstance()->PreResourceReloadCallback(resource);
		hh::game::GameManager::GetInstance()->PostResourceReloadCallback(resource);
		//resource->resource->RefreshInstanceParameter();
		//MaterialChunkBuilder::RegisterUniqueMaterialResource(&resource->resource);
	}
}

ResMaterialEditor* ResMaterialEditor::Create(csl::fnd::IAllocator* allocator, hh::gfx::ResMaterial* resource)
{
	return new (allocator) ResMaterialEditor(allocator, resource);
}

void ResMaterialEditor::RenderFloatParameterEditor(size_t idx, ParameterFloatValue* data)
{
	auto& info = reinterpret_cast<MaterialChunkBuilder::ParameterInfo*>(reinterpret_cast<size_t>(resource->resource->data) + 0x28)[idx];

	size_t itemCount = (info.sizeInDwords - 2) / 4;

	if (itemCount > 1) {
		for (size_t i = 0; i < itemCount; i++) {
			char buf[256];
			snprintf(buf, sizeof(buf), "%s[%zd]", data->name->name, i);
			ImGui::DragFloat4(buf, reinterpret_cast<float*>(&(&data->values)[i]), 0.01);
		}
	}
	else {
		ImGui::DragFloat4(data->name->name, reinterpret_cast<float*>(&(&data->values)[0]), 0.01);
	}
}

void ResMaterialEditor::RenderBoolParameterEditor(size_t idx, hh::needle::ParameterBoolValue* data)
{
	ImGui::Text("%s, %x", data->name->name, &data->value);
}

void ResMaterialEditor::RenderIntParameterEditor(size_t idx, hh::needle::ParameterIntValue* data)
{
	auto& info = reinterpret_cast<MaterialChunkBuilder::ParameterInfo*>(reinterpret_cast<size_t>(resource->resource->data) + 0x28)[idx];

	for (size_t i = 0; i < (info.sizeInDwords - 2) / 4; i++) {
		char buf[256];
		snprintf(buf, sizeof(buf), "%s[%zd]", data->name->name, i);
		ImGui::DragInt4(buf, reinterpret_cast<int*>(&(&data->uintVector)[i]));
	}
}

static const char* blendModes[] = { "2/4/0", "2/1/0", "0/7/0", "2/1/2" };
static const char* wrapModes[] = { "WRAP", "MIRROR", "CLAMP", "MIRRORONCE", "BORDER_HALF", "MIRRORONCE_BORDER_HALF", "BORDER" };

void ResMaterialEditor::RenderSamplerParameterEditor(size_t idx, hh::needle::ParameterSamplerValue* data)
{
	if (ImGui::TreeNode(data->name->name)) {
		ComboEnum("Wrap mode U", data->wrapModeU, wrapModes);
		ComboEnum("Wrap mode V", data->wrapModeV, wrapModes);
		ImGui::DragScalar("TexCoord index", ImGuiDataType_U8, &data->texCoordIndex);
		ImGui::Text("TexCoord index name %s", data->texCoordIndexName->name);
		ImGui::Text("TexCoord mtx name %s", data->texCoordMtxName->name);
		ImGui::TreePop();
	}
}

void ResMaterialEditor::RenderRsFlagMaskParameterEditor(size_t idx, hh::needle::ParameterRsFlagMaskValue* data)
{
	Editor("Render options", data->rsFlagMask);
}

void ResMaterialEditor::RenderShaderNameParameterEditor(size_t idx, hh::needle::ParameterShaderNameValue* data)
{
	InputText("Shader Name", data->name);
}

void ResMaterialEditor::RenderTextureNameParameterEditor(size_t idx, hh::needle::ParameterTextureNameValue* data)
{
	if (ImGui::TreeNode("Texture name")) {
		InputText("Type", data->type);
		InputText("Name", data->name);
		ImGui::TreePop();
	}
}

void ResMaterialEditor::RenderUnimplementedTypeParameterEditor(size_t idx)
{
	ImGui::Text("editor not yet implemented");
}
