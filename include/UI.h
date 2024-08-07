#pragma once
#include "SKSEMenuFramework.h"

class CustomLogger
{
public:
	static CustomLogger*            GetSingleton();
	void                            AddLog(const std::string& message);
	void                            ClearLogs();
	const std::vector<std::string>& GetLogs() const;
	const std::string               GetLatestLog() const;
	bool                            ShouldScrollToBottom() const;
	void                            ResetScrollToBottom();
	std::vector<std::string>        GetFilteredLogs(const std::string& filter) const;

private:
	CustomLogger() = default;
	~CustomLogger() = default;
	CustomLogger(const CustomLogger&) = delete;
	CustomLogger(CustomLogger&&) = delete;
	CustomLogger& operator=(const CustomLogger&) = delete;
	CustomLogger& operator=(CustomLogger&&) = delete;

	std::vector<std::string> _logs;
	std::vector<size_t>      _lineOffsets;
	mutable bool             _scrollToBottom{ false };
	mutable std::mutex       _lock;
};

namespace UI
{
	inline const std::string plus{ FontAwesome::UnicodeToUtf8(0x2b) };
	inline const std::string x{ FontAwesome::UnicodeToUtf8(0x58) };
	inline const std::string question{ FontAwesome::UnicodeToUtf8(0x3f) };
}

class SMFRenderer
{
public:
	static SMFRenderer* GetSingleton();
	static void __stdcall Register();
	void GetAllExclusionJsons();

private:
	enum FileCreationType
	{
		OK,
		Duplicate,
		Error
	};
	SMFRenderer() = default;
	~SMFRenderer() = default;
	SMFRenderer(const SMFRenderer&) = delete;
	SMFRenderer(SMFRenderer&&) = delete;
	SMFRenderer& operator=(const SMFRenderer&) = delete;
	SMFRenderer& operator=(SMFRenderer&&) = delete;

	inline static MENU_WINDOW _LogWindow;

	static void __stdcall RenderMain();
	static void __stdcall RenderExclusions();
	static void __stdcall RenderLogWindow();
	static void __stdcall RenderDebug();
	void                           RenderJsonEditor(const std::string_view Path, ordered_nJson& jsonObject, ordered_nJson& hint);
	void                           RenderJsonEditor(const std::string_view Path, std::shared_ptr<ordered_nJson> jsonObject, ordered_nJson& hint);
	void                           RenderJsonValue(const std::string_view jsonPath, const std::string& key, ordered_nJson& value, ordered_nJson& hint);
	void                           RenderJsonObject(const std::string_view jsonPath, ordered_nJson& j, ordered_nJson& hint);
	void                           RenderJsonObject(const std::string_view jsonPath, std::shared_ptr<ordered_nJson> j, ordered_nJson& hint);
	void                           RenderJsonArray(const std::string_view jsonPath, std::string_view key, ordered_nJson& j, ordered_nJson& hint);
	void                           RenderJsonArray(const std::string_view jsonPath, std::string_view key, std::shared_ptr<ordered_nJson> j, ordered_nJson& hint);
	void                           RenderHint(ordered_nJson& hint);
	[[nodiscard]] FileCreationType CreateNewJsonFile(const std::string_view filename, const ordered_nJson& jsonObject);
	[[nodiscard]] FileCreationType CreateNewJsonFile(const std::string_view filename, std::shared_ptr<ordered_nJson>& jsonObject);
	[[nodiscard]] bool             SaveJsonToFile(const std::string_view filename, const ordered_nJson& jsonObject);
	[[nodiscard]] bool             SaveJsonToFile(const std::string_view filename, std::shared_ptr<ordered_nJson>& jsonObject);
	[[nodiscard]] bool             SaveJsonToFile(std::map<std::string, std::shared_ptr<ordered_nJson>>::iterator p);

	ordered_nJson                                                       _HintsForMain;
	ordered_nJson                                                       _HintsForExclusions;
	std::set<std::string>                                               _ModNames;
	std::map<std::string, std::shared_ptr<ordered_nJson>>               _ExclusionJsons;
	std::map<std::string, std::string>                                 _Key1Values;
	std::map<std::string, std::string>                                 _Key2Values;
	std::map<std::string, size_t>                                      _Key1Index;
	std::map<std::string, size_t>                                      _Key2Index;


public:
	std::mutex                                                          _lock;
};
