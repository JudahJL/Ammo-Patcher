//
// Created by judah on 23-02-2025.
//

#include "UI.h"

#include "APUtils.h"
#include "FilePtrManager.h"
#include "ImGuiIDGuard.h"
#include "ImGuiTreeNodeGuard.h"
#include "Settings.h"
#include <ctre.hpp>

SMFRenderer SMFRenderer::Singleton;

SMFRenderer& SMFRenderer::GetSingleton() { return Singleton; }

void SMFRenderer::Register() {
    if(!SKSEMenuFramework::IsInstalled()) {
        SPDLOG_WARN("Unable to Register For SKSEMenuFramework, Please install SKSEMenuFramework to "
                    "configure the Json Files(if you want to)");
        return;
    }

    SKSEMenuFramework::SetSection("Ammo Patcher");

    SKSEMenuFramework::AddSectionItem("Edit Presets", RenderEditPresets);

#ifndef NDEBUG
    SKSEMenuFramework::AddSectionItem("Debug", RenderDebug);
#endif
    SPDLOG_INFO("Registered For SKSEMenuFramework");
}

#pragma push_macro("GetObject")
#undef GetObject

#ifndef NDEBUG
void SMFRenderer::RenderDebug() {
    if(ImGui::Button("Quit Game?")) {
        REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_SUCCESS);
    }
}
#endif

void SMFRenderer::RenderEditPresets() {
    auto& settings{ Settings::GetSingleton() };
    if(ImGui::Button("Save Default Preset")) {
        if(FilePtrManager f{ "data/SKSE/Plugins/Ammo_Patcher.json", "wb" }) {
            auto                u8_path{ Utils::wstringToString(settings.curr_preset.filename()) };
            rapidjson::Document doc{ rapidjson::kObjectType };
            doc.AddMember("Load", rapidjson::Value{}.SetString(u8_path.c_str(), u8_path.size(), doc.GetAllocator()), doc.GetAllocator());
            char                       writeBuffer[65'536]{ "" };
            rapidjson::FileWriteStream os{ f.get(), writeBuffer, std::size(writeBuffer) };
            rapidjson::PrettyWriter    writer{ os };

            doc.Accept(writer);
        }
    }
    ImGui::SameLine();
    if(ImGui::BeginCombo("Selected Presets", Utils::wstringToString(settings.curr_preset.filename()).c_str())) {
        for(const auto& key : settings.presets | std::views::keys) {
            const bool isSelected = (settings.curr_preset == key);
            if(ImGui::Selectable(Utils::wstringToString(key.filename()).c_str(), isSelected)) {
                settings.curr_preset = key;
            }
            if(isSelected) { ImGui::SetItemDefaultFocus(); }
        }
        ImGui::EndCombo();
    }
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 20.0f);
    for(auto& [path, doc] : settings.presets) {
        const auto u8_filename{ Utils::wstringToString(path.filename()) };
        char       button_uid_buff[1'024]{};
        std::snprintf(button_uid_buff, std::size(button_uid_buff), "Save JSON##%s", u8_filename.c_str());
        if(ImGui::Button(button_uid_buff)) {
            if(FilePtrManager f{ path.c_str(), L"wb" }) {
                char                       writeBuffer[65'536];
                rapidjson::FileWriteStream os{ f.get(), writeBuffer, sizeof(writeBuffer) };

                rapidjson::PrettyWriter writer{ os };
                doc.Accept(writer);
            }
        }
        ImGui::SameLine();
        if(ImGuiTreeNodeGuard i{ u8_filename.c_str() }) { RenderJsonEditor(u8_filename, doc); }
    }
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 20.0f);
    static char warning[2'048];
    static char buff[MAX_PATH]{};
    if(warning[0] != '\0') {
        ImGui::TextColored(ImGui::GetStyle()->Colors[ImGuiCol_PlotHistogramHovered], warning);
    }
    if(ImGui::InputText("Create a Preset?", buff, std::size(buff), ImGuiInputTextFlags_EnterReturnsTrue)) {
        if(buff[0] == '\0') {
            std::snprintf(warning, std::size(warning), "Type in something!");
            return;
        }
        constexpr auto filename_regex = ctll::fixed_string{ R"(^(?!.*(PRN|AUX|NUL|CO(N|M[0-9¹²³])|LPT[0-9¹²³]|[<>:"/\|?*]))(?=\S).*)" };
        if(!ctre::match<filename_regex>(buff)) {
            std::snprintf(warning, std::size(warning), "Invalid filename! Avoid reserved names or special characters or whitespaces at the start.");
            return;
        }
        char created_file_path[1'024]{};
        std::snprintf(created_file_path, std::size(created_file_path), "data/SKSE/Plugins/Ammo Patcher/Presets/%s.json", buff);
        if(FilePtrManager f{ created_file_path, "wb" }) {
            char                       writeBuffer[65'536];
            rapidjson::FileWriteStream os(f.get(), writeBuffer, sizeof(writeBuffer));
            rapidjson::PrettyWriter    writer(os);
            rapidjson::Document        doc;
            doc.Parse(R"({"Logging":{"LogLevel":"info"},"AMMO":{"Infinite AMMO":{"Player":false,"Teammate":false},"Arrow":{"Enable Arrow Patch":true,"Change Gravity":{"Enable":true,"Gravity":0.0},"Change Speed":{"Enable":true,"Speed":9000.0},"Limit Speed":{"Enable":false,"Min":3000.0,"Max":12000.0},"Randomize Speed":{"Enable":false,"Min":3000.0,"Max":12000.0},"Limit Damage":{"Enable":false,"Min":10.0,"Max":1000.0},"Sound":{"Change Sound Level":{"Enable":false,"Sound Level":"kSilent"}}},"Bolt":{"Enable Bolt Patch":true,"Change Gravity":{"Enable":true,"Gravity":0.0},"Change Speed":{"Enable":true,"Speed":10800.0},"Limit Speed":{"Enable":false,"Min":4000.0,"Max":12000.0},"Randomize Speed":{"Enable":false,"Min":3000.0,"Max":12000.0},"Limit Damage":{"Enable":false,"Min":10.0,"Max":1000.0},"Sound":{"Change Sound Level":{"Enable":false,"Sound Level":"kSilent"}}}}})");
            doc.Accept(writer);
            Settings::GetSingleton().presets.try_emplace(created_file_path, std::move(doc));
            warning[0] = '\0';
        } else {
            std::snprintf(warning, std::size(warning), "Failed to create preset! check the log for the reason.");
        }
    }
    if(ImGui::Button("Revert Ammo Records to Default Values")) { settings.RevertToDefault(); }
    if(ImGui::Button("Patch All Ammo Records")) { settings.SetLogAndFlushLevel().Patch(); }
}

void SMFRenderer::RenderJsonEditor(const std::string_view filePath, rapidjson::Document& doc) noexcept {
    if(doc.IsObject()) {
        for(auto& kv_pair : doc.GetObject()) {
            const auto temp{ std::format("{}/{}", filePath, kv_pair.name.GetString()) };
            RenderJsonValue(temp.c_str(), kv_pair.name.GetString(), kv_pair.value, doc.GetAllocator());
        }
    } else if(doc.IsArray()) {
        RenderJsonArray(filePath.data(), "[ ]", doc, doc.GetAllocator());
    } else {
        RenderJsonValue(filePath.data(), "", doc, doc.GetAllocator());
    }
}

void SMFRenderer::RenderJsonValue(const char* uniqueID, std::string_view key, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& alloc) noexcept {  // NOLINT(*-no-recursion)
    const ImGuiIDGuard currentPathIDManager{ uniqueID };
    if(value.IsObject()) {
        const auto temp{ std::format("{} {{ }}", key) };
        if(ImGuiTreeNodeGuard i{ temp.c_str() }) {
            const std::string temp1{ std::format("{}/{}", uniqueID, key) };
            RenderJsonObject(temp1.c_str(), value, alloc);
        }
    } else if(value.IsArray()) {
        const auto temp{ std::format("{} [ ]", key) };
        if(ImGuiTreeNodeGuard i{ temp.c_str() }) {
            const std::string temp1{ std::format("{}/{}", uniqueID, key) };
            RenderJsonArray(temp1.c_str(), key, value, alloc);
        }
    } else if(value.IsString()) {
        if(key == "LogLevel") [[unlikely]] {
            int                  currentLogLevel{ spdlog::level::from_str(value.GetString()) };
            constexpr std::array LogLevels{ "trace"sv, "debug"sv,    "info"sv, "warning"sv,
                                            "error"sv, "critical"sv, "off"sv };

            if(ImGui::BeginCombo(key.data(), LogLevels[currentLogLevel].data())) {
                for(int i{}; i < LogLevels.size(); i++) {
                    const bool isSelected = (currentLogLevel == i);
                    if(ImGui::Selectable(LogLevels[i].data(), isSelected)) {
                        currentLogLevel = i;
                        value           = rapidjson::Value{}.SetString(
                            LogLevels[currentLogLevel].data(), LogLevels[currentLogLevel].size(), alloc);
                    }
                    if(isSelected) { ImGui::SetItemDefaultFocus(); }
                }
                ImGui::EndCombo();
            }
        } else {
            char buffer[256];
            strcpy_s(buffer, std::size(buffer), value.GetString());
            if(ImGui::InputText(key.data(), buffer, std::size(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                value = rapidjson::Value{}.SetString(buffer, strlen(buffer), alloc);
            }
            if(ImGui::IsItemHovered()) {
                ImGui::SetTooltip(
                    "Press Enter to save your input.\nMaximum allowed characters: %zu", std::size(buffer) - 1);
            }
        }
    } else if(value.IsBool()) {
        auto boolValue{ value.GetBool() };
        if(ImGui::Checkbox(key.data(), &boolValue)) { value = boolValue; }
    } else if(value.IsInt()) {
        auto intValue{ value.GetInt() };
        if(ImGui::InputInt(key.data(), &intValue, ImGuiInputTextFlags_EnterReturnsTrue)) {
            value = intValue;
        }
    } else if(value.IsInt64()) {
        auto int64Value{ value.GetInt64() };
        if(ImGui::InputScalar(key.data(), ImGuiDataType_S64, &int64Value, nullptr, nullptr, "%lld", ImGuiInputTextFlags_EnterReturnsTrue)) {
            value = int64Value;
        }
    } else if(value.IsUint()) {
        auto uintValue = value.GetUint();
        if(ImGui::InputScalar(key.data(), ImGuiDataType_U32, &uintValue, nullptr, nullptr, "%u", ImGuiInputTextFlags_EnterReturnsTrue)) {
            value = uintValue;
        }
    } else if(value.IsUint64()) {
        auto uint64Value = value.GetUint64();
        if(ImGui::InputScalar(key.data(), ImGuiDataType_U64, &uint64Value, nullptr, nullptr, "%llu", ImGuiInputTextFlags_EnterReturnsTrue)) {
            value = uint64Value;
        }
    } else if(value.IsFloat()) {
        auto floatValue{ value.GetFloat() };
        if(ImGui::InputFloat(key.data(), &floatValue, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
            value = floatValue;
        }
    } else if(value.IsDouble()) {
        double doubleValue = value.GetDouble();
        if(ImGui::InputDouble(key.data(), &doubleValue, 0, 0, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue)) {
            value = doubleValue;
        }
    }
}

void SMFRenderer::RenderJsonObject(const char* uniqueID, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& alloc) noexcept {  // NOLINT(*-no-recursion)
    for(auto& kv_pair : value.GetObject()) {
        auto temp{ std::format("{}/{}", uniqueID, kv_pair.name.GetString()) };
        RenderJsonValue(temp.c_str(), kv_pair.name.GetString(), kv_pair.value, alloc);
    }
}

#pragma pop_macro("GetObject")

void SMFRenderer::RenderJsonArray(const char* uniqueID, std::string_view key, rapidjson::Value& value, rapidjson::MemoryPoolAllocator<>& alloc) noexcept {  // NOLINT(*-no-recursion)
    const ImGuiIDGuard current_path_id_manager{ uniqueID };
    for(auto it{ value.Begin() }; it != value.End();) {
        const auto i{ static_cast<rapidjson::SizeType>(std::distance(value.Begin(), it)) };
        auto       Path{ std::format("{}/{}", uniqueID, key) };

        const ImGuiIDGuard path_id_manager{ Path.c_str() };
        char               buff[64];
        std::snprintf(buff, std::size(buff), "[%u]", i);
        RenderJsonValue(std::format("{}/{}", Path, i).c_str(), buff, *it, alloc);

        ImGui::SameLine();
        if(ImGui::Button(UI::cross.c_str())) {
            it = value.Erase(it);
            continue;
        }
        if(ImGui::IsItemHovered()) {
            char buffer[64];
            sprintf_s(buffer, sizeof(buffer), "Removes line [%u]", i);
            ImGui::SetTooltip(buffer);
        }
        if(it != value.Begin()) {
            ImGui::SameLine();
            if(ImGui::ArrowButton("##up", ImGuiDir_Up)) {  // Move up
                std::iter_swap(it, std::prev(it));
                continue;
            }
            if(ImGui::IsItemHovered()) { ImGui::SetTooltip("Click Me to Move me Up"); }
        }
        if(std::next(it) != value.End()) {
            ImGui::SameLine();
            if(ImGui::ArrowButton("##down", ImGuiDir_Down)) {  // Move Down
                std::iter_swap(it, std::next(it));
                continue;
            }
            if(ImGui::IsItemHovered()) { ImGui::SetTooltip("Click Me to Move me Down"); }
        }
        ++it;
    }

    if(ImGui::Button(UI::plus.c_str())) {
        value.PushBack(rapidjson::Value{}.SetString("", 0, alloc), alloc);
    }
    if(ImGui::IsItemHovered()) { ImGui::SetTooltip("Adds a Empty Line"); }
}

