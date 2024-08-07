#pragma once

class DataHandler
{
public:
	static DataHandler* GetSingleton();
	void                LoadMainJson();
	void                LoadExclusionJsonFiles();
	void                ProcessMainJson();
	void                PatchAMMO();
	void                LogDataHandlerContents();
	void                RevertToDefault();

	inline void ChangeLogLevel(const std::string& LogLevelStr) const
	{
		spdlog::level::level_enum level{ spdlog::level::from_str(LogLevelStr) };
		spdlog::set_level(level);
		spdlog::flush_on(level);
		logger::trace("Set LogLevel to {}", spdlog::level::to_string_view(level));
	}

	inline bool GetDoneAmmoPatching() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _DoneAmmoPatching;
	}

	inline void SetDoneAmmoPatching()
	{
		std::lock_guard<std::mutex> lock(_lock);
		_DoneAmmoPatching = true;
	}

	inline void ResetDoneAmmoPatching()
	{
		std::lock_guard<std::mutex> lock(_lock);
		_DoneAmmoPatching = false;
	}

	inline const auto& GetUnmodifiableMainJsonDataMap() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _MainJsonDataMap;
	}
	inline auto& GetModifiableMainJsonDataMap()
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _MainJsonDataMap;
	}

	inline const std::shared_ptr<ordered_nJson> GetUnmodifiableMainJsonData()
	{
		std::lock_guard<std::mutex> lock(_lock);
		auto                        it = _MainJsonDataMap.find(_SelectedPreset);
		if (it != _MainJsonDataMap.end()) {
			return it->second;
		} else {
			_SelectedPreset = fs::absolute(R"(Data\SKSE\Plugins\Ammo Patcher\Presets\Default.json)").string();
			return _MainJsonDataMap.at(_SelectedPreset);
		}
	}

	inline const auto GetUnmodifiableMainJsonPair()
	{
		std::lock_guard<std::mutex> lock(_lock);
		auto                        it = _MainJsonDataMap.find(_SelectedPreset);
		if (it != _MainJsonDataMap.end()) {
			return it;
		} else {
			_SelectedPreset = fs::absolute(R"(Data\SKSE\Plugins\Ammo Patcher\Presets\Default.json)").string();
			return _MainJsonDataMap.find(_SelectedPreset);
		}
	}

	inline auto& GetModifiableMainJsonData()
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _MainJsonDataMap.at(_SelectedPreset);
	}

	inline void SetUserName(const std::string_view& UserName)
	{
		std::lock_guard<std::mutex> lock(_lock);
		_UserName = UserName;
	}

	inline const std::string_view GetUsername() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _UserName;
	}

	inline const ordered_nJson& GetUnmodifiableAmmoInfo() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _AmmoInfo;
	}

	inline ordered_nJson& GetModifiableAmmoInfo()
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _AmmoInfo;
	}

	inline const std::vector<std::string>& GetUnmodifiableAmmoModFiles() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _AmmoModFiles;
	}

	inline std::vector<std::string>& GetModifiableAmmoModFiles()
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _AmmoModFiles;
	}

	inline const std::unordered_set<std::string>& GetUnmodifiableFormIDArray() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _FormIDArray;
	}

	inline std::unordered_set<std::string>& GetModifiableFormIDArray()
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _FormIDArray;
	}

	inline const std::unordered_set<std::string>& GetUnmodifiableTESFileArray() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _TESFileArray;
	}

	inline std::unordered_set<std::string>& GetModifiableTESFileArray()
	{
		return _TESFileArray;
	}

	inline const std::string& GetUnmodifiableSelectedPreset() const
	{
		std::lock_guard<std::mutex> lock(_lock);
		return _SelectedPreset;
	}

	inline const void SetSelectedPreset(const std::string& value)
	{
		std::lock_guard<std::mutex> lock(_lock);
		_SelectedPreset = value;
	}

private:
	DataHandler() = default;
	~DataHandler() = default;
	DataHandler(const DataHandler&) = delete;
	DataHandler(DataHandler&&) = delete;
	DataHandler& operator=(const DataHandler&) = delete;
	DataHandler& operator=(DataHandler&&) = delete;

	bool _ArrowPatch{ true };
	bool _BoltPatch{ true };
	bool _ArrowSpeedEnable{ true };
	bool _BoltSpeedEnable{ true };
	bool _ArrowGravityEnable{ true };
	bool _BoltGravityEnable{ true };
	bool _LimitArrowSpeed{ false };
	bool _LimitBoltSpeed{ false };
	bool _LimitArrowDamage{ false };
	bool _LimitBoltDamage{ false };
	bool _ChangeArrowSoundLevel{ false };
	bool _ChangeBoltSoundLevel{ false };
	bool _RandomizeArrowSpeed{ false };
	bool _RandomizeBoltSpeed{ false };

public:
	bool _InfinitePlayerAmmo{ false };
	bool _InfiniteTeammateAmmo{ false };
	bool _HasFilesToMerge{ false };
	bool _DoneAmmoPatching{ false };

private:
	float                                                 _ArrowDamageLimiterMin{ 10.0f };
	float                                                 _BoltDamageLimiterMin{ 10.0f };
	float                                                 _ArrowSpeedLimiterMin{ 3000.0f };
	float                                                 _BoltSpeedLimiterMin{ 4000.0f };
	float                                                 _ArrowSpeedRandomizerMin{ 3000.0f };
	float                                                 _BoltSpeedRandomizerMin{ 4000.0f };
	float                                                 _ArrowDamageLimiterMax{ 1000.0f };
	float                                                 _BoltDamageLimiterMax{ 1000.0f };
	float                                                 _ArrowSpeedLimiterMax{ 12000.0f };
	float                                                 _BoltSpeedLimiterMax{ 12000.0f };
	float                                                 _ArrowSpeedRandomizerMax{ 12000.0f };
	float                                                 _BoltSpeedRandomizerMax{ 12000.0f };
	float                                                 _ArrowSpeed{ 9000.0f };
	float                                                 _BoltSpeed{ 10800.0f };
	float                                                 _ArrowGravity{ 0.0f };
	float                                                 _BoltGravity{ 0.0f };
	std::string                                           _ArrowSoundLevelStr{ "kSilent" };
	std::string                                           _BoltSoundLevelStr{ "kSilent" };
	RE::SOUND_LEVEL                                       _ArrowSoundLevel{ RE::SOUND_LEVEL::kSilent };
	RE::SOUND_LEVEL                                       _BoltSoundLevel{ RE::SOUND_LEVEL::kSilent };
	std::string                                           _UserName{ "User" };
	ordered_nJson                                         _AmmoInfo;  // store all AMMO info for exclusion files
	std::vector<std::string>                              _AmmoModFiles;
	mutable std::mutex                                    _lock;
	std::map<std::string, std::shared_ptr<ordered_nJson>> _MainJsonDataMap;
	std::string                                           _SelectedPreset;
	std::unordered_set<std::string>                       _FormIDArray;
	std::unordered_set<std::string>                       _TESFileArray;
};
