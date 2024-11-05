#pragma once
#include "SKSE-MCP/SKSEMenuFramework.h"

class UILogger
{
public:
	static auto GetSingleton() -> UILogger*;
	void        AddLog(const std::string& message);
	void        ClearLogs();
	auto        GetLogs() -> std::vector<std::string>&;
	auto        GetLatestLog() const -> std::string;
	auto        ShouldScrollToBottom() const -> bool;
	void        ResetScrollToBottom();
	auto        GetFilteredLogs(const std::string& filter) const -> std::vector<std::string>;

	UILogger(const UILogger&) = delete;
	UILogger(UILogger&&) = delete;
	auto operator=(const UILogger&) -> UILogger& = delete;
	auto operator=(UILogger&&) -> UILogger& = delete;
private:
	UILogger() = default;
	~UILogger() = default;

	std::vector<std::string> _logs;
	std::vector<size_t>      _lineOffsets;
	bool                     _scrollToBottom{ false };
	mutable std::mutex       _lock;
};

namespace UI
{
	constexpr auto MAX_SIZE{ 256 };
	inline const std::string plus{ FontAwesome::UnicodeToUtf8(0x2b) };
	inline const std::string cross{ FontAwesome::UnicodeToUtf8(0x58) };
	inline const std::string question{ FontAwesome::UnicodeToUtf8(0x3f) };
}

class SMFRenderer
{
public:
	static auto GetSingleton() -> SMFRenderer*;
	static void __stdcall Register();
	void GetAllExclusionJsons();

	SMFRenderer(const SMFRenderer&) = delete;
	SMFRenderer(SMFRenderer&&) = delete;
	auto operator=(const SMFRenderer&) -> SMFRenderer& = delete;
	auto operator=(SMFRenderer&&) -> SMFRenderer& = delete;

private:
	enum FileCreationType
	{
		OK,
		Duplicate,
		Error
	};

	SMFRenderer() = default;
	~SMFRenderer() = default;

	inline static MENU_WINDOW log_window_;

	static void __stdcall     RenderDefaultPreset();
	static void __stdcall     RenderExclusions();
	static void __stdcall     RenderLogWindow();
	static void __stdcall     RenderDebug();
	static void __stdcall     RenderEditPresets();
	static void               RenderLogButton();
	void                      RenderJsonEditor(std::string_view Path, ordered_nJson& jsonObject, ordered_nJson& hint);
	void                      RenderJsonEditor(std::string_view Path, const std::shared_ptr<ordered_nJson>& jsonObject, ordered_nJson& hint);
	void                      RenderJsonValue(std::string_view jsonPath, std::string_view key, ordered_nJson& value, ordered_nJson& hint);
	void                      RenderJsonObject(std::string_view jsonPath, ordered_nJson& jsonObject, ordered_nJson& hint);
	void                      RenderJsonObject(std::string_view jsonPath, const std::shared_ptr<ordered_nJson>& jsonObject, ordered_nJson& hint);
	void                      RenderJsonArray(std::string_view jsonPath, std::string_view key, ordered_nJson& jsonObject, ordered_nJson& hint);
	void                      RenderJsonArray(std::string_view jsonPath, std::string_view key, const std::shared_ptr<ordered_nJson>& jsonObject, ordered_nJson& hint);
	static void               RenderHint(const ordered_nJson& hint);
	[[nodiscard]] static auto CreateNewJsonFile(std::string_view filename, const ordered_nJson& jsonObject) -> FileCreationType;
	[[nodiscard]] static auto CreateNewJsonFile(std::string_view filename, const std::shared_ptr<ordered_nJson>& jsonObject) -> FileCreationType;
	[[nodiscard]] static auto SaveJsonToFile(std::string_view filename, const ordered_nJson& jsonObject) -> bool;
	[[nodiscard]] static auto SaveJsonToFile(std::string_view filename, const std::shared_ptr<ordered_nJson>& jsonObject) -> bool;
	[[nodiscard]] static auto SaveJsonToFile(const std::map<std::string, ordered_nJson>::iterator& mapIterator) -> bool;

	ordered_nJson                                         hints_for_main_;
	ordered_nJson                                         hints_for_exclusions_;
	std::set<std::string>                                 mod_names_;
	std::map<std::string, std::shared_ptr<ordered_nJson>> exclusion_jsons_;
	std::map<std::string, std::string>                    key1_values_;
	std::map<std::string, std::string>                    key2_values_;
	std::map<std::string, size_t>                         key1_index_;
	std::map<std::string, size_t>                         key2_index_;
	std::mutex                                            lock_;
};

class ImGuiIDGuard  //using RAII for ImGui ID management
{
public:
	explicit ImGuiIDGuard(const char* str_id) {
		ImGui::PushID(str_id);
	}
	explicit ImGuiIDGuard(const char* str_id_begin, const char* str_id_end) {
		ImGui::PushID(str_id_begin, str_id_end);
	}
	// ReSharper disable once CppParameterMayBeConst
	explicit ImGuiIDGuard(int int_id) {
		ImGui::PushID(int_id);
	}
	explicit ImGuiIDGuard(const void* ptr_id) {
		ImGui::PushID(ptr_id);
	}
	~ImGuiIDGuard() {
		ImGui::PopID();
	}

	ImGuiIDGuard(const ImGuiIDGuard&) = delete;
	ImGuiIDGuard(ImGuiIDGuard&&) = delete;
	auto operator=(const ImGuiIDGuard&) -> ImGuiIDGuard& = delete;
	auto operator=(ImGuiIDGuard&&) -> ImGuiIDGuard& = delete;
};
