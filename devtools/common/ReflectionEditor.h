#pragma once

class ReflectionEditor {
    static const char* GetRflMemberName(const hh::fnd::RflClassMember* member);

    template<typename T>
    static void RenderSliderlessScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName) {
        const hh::fnd::RflCustomAttribute* rangeAttr;

        if (rangeName && (rangeAttr = member->GetAttribute(rangeName))) {
            const T* rangeValues = reinterpret_cast<const T*>(rangeAttr->GetData());

            ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n, 1.0f, &rangeValues[0], &rangeValues[1]);
        }
        else {
            ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n);
        }
    }

    template<typename T>
    static void RenderSliderlessScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        RenderSliderlessScalarRflParamEditor<T>(obj, member, dtype, n, nullptr);
    }

    template<typename T>
    static void RenderScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName, const T& minSliderRange, const T& maxSliderRange) {
        const hh::fnd::RflCustomAttribute* rangeAttr;

        if (rangeName && (rangeAttr = member->GetAttribute(rangeName))) {
            const T* rangeValues = reinterpret_cast<const T*>(rangeAttr->GetData());

            if (minSliderRange <= rangeValues[0] && rangeValues[1] <= maxSliderRange)
                ImGui::SliderScalarN(GetRflMemberName(member), dtype, obj, n, &rangeValues[0], &rangeValues[1]);
            else
                ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n, 1.0f, &rangeValues[0], &rangeValues[1]);
        }
        else {
            ImGui::DragScalarN(GetRflMemberName(member), dtype, obj, n, 1.0f);
        }
    }

    template<typename T>
    static void RenderScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n, const char* rangeName) {
        RenderScalarRflParamEditor<T>(obj, member, dtype, n, rangeName, std::numeric_limits<T>::lowest() / 2, std::numeric_limits<T>::max() / 2);
    }

    template<typename T>
    static void RenderScalarRflParamEditor(void* obj, const hh::fnd::RflClassMember* member, ImGuiDataType dtype, int n) {
        RenderScalarRflParamEditor<T>(obj, member, dtype, n, nullptr);
    }

    static int64_t ReadPrimitiveInt(void* obj, const hh::fnd::RflClassMember::Type type);
    static void WritePrimitiveInt(void* obj, int64_t value, const hh::fnd::RflClassMember::Type type);
    static void RenderEnumEditor(void* obj, const hh::fnd::RflClassMember* member);

    static void RenderStubRflParamEditor(const hh::fnd::RflClassMember* member);
    static void RenderStructRflParamEditor(void* obj, const char* name, const hh::fnd::RflClass* rflClass);
    static void RflClassMembersEditor(void* obj, const hh::fnd::RflClass* rflClass);

public:
    static void Render(void* reflectionData, const hh::fnd::RflClass* rflClass) {
        RenderStructRflParamEditor(reflectionData, rflClass->m_pName, rflClass);
    }

    template<typename T> static void Render(T& reflectionData) {
        Render(&reflectionData, rangerssdk::bootstrap::GetAddress(&T::rflClass));
    }
};
