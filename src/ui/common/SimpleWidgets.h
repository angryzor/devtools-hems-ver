#pragma once

void MatrixValues(const Eigen::Matrix4f& mat);
void ViewportDataInfo(const hh::gfnd::ViewportData& viewport);
void VectorEditor(const char* label, csl::math::Vector3& vec);
void VectorEditor(const char* label, csl::math::Vector4& vec);
void QuaternionEditor(const char* label, csl::math::Quaternion& quat);
void WorldPositionEditor(hh::fnd::WorldPosition& worldPos);
void TransformEditor(csl::math::Transform& transform);
bool BeginVectorViewerTable(const char* id, bool withWAxis);
void EndVectorViewerTable();
void VectorViewerTableItem(const char* label, const csl::math::Vector3& vec);
void VectorViewerTableItem(const char* label, const csl::math::Vector4& vec);
void VectorViewerTableItem(const char* label, const csl::math::Quaternion& quat);
void WorldPositionViewer(const char* id, const hh::fnd::WorldPosition& worldPos);
void TransformViewer(const char* id, csl::math::Transform& transform);
void InputText(const char* label, csl::ut::VariableString* str, ImGuiInputTextFlags flags = 0);
void InputText(const char* label, csl::ut::String* str, ImGuiInputTextFlags flags = 0);
void InputText(const char* label, hh::needle::intrusive_ptr<hh::needle::CNameIDObject>& str, ImGuiInputTextFlags flags = 0);
void InputObjectId(const char* label, hh::game::ObjectId* id);
void RsFlagMaskEditor(const char* label, hh::needle::RsFlagMask* mask);

template<typename T, typename U>
void CheckboxFlags(const char* label, csl::ut::Bitset<T, U>* v, T value) {
	ImGui::CheckboxFlags(label, &v->m_dummy, static_cast<U>(1) << static_cast<U>(value));
}

template<typename T, typename U>
void CheckboxFlagsLT32(const char* label, csl::ut::Bitset<T, U>* v, T value) {
	int v2 = v->m_dummy;
	ImGui::CheckboxFlags(label, &v2, static_cast<U>(1) << static_cast<U>(value));
	*v = static_cast<U>(v2);
}

template<typename T, size_t count>
void ComboEnum(const char* label, T* v, const char* (&names)[count]) {
	int x = static_cast<std::underlying_type_t<T>>(*v);
	ImGui::Combo(label, &x, names, count);
	*v = static_cast<T>(x);
}
