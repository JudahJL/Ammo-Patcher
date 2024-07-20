#pragma once

class DataHandler
{
public:
	static DataHandler* GetSingleton();

	// void ResolveConflictsForSKSEPlugins();

	void LoadMainJson();

	void LoadExclusionJsonFiles();

	void ReloadLoggingIfNecessary(const std::string& LogLevelStr);

	void ProcessMainJson();

	void PatchAMMO();

	void LogDataHandlerContents();

	void RevertToDefault();

private:
	DataHandler() = default;
	~DataHandler() = default;
	DataHandler(const DataHandler&) = delete;
	DataHandler(DataHandler&&) = delete;
	DataHandler& operator=(const DataHandler&) = delete;
	DataHandler& operator=(DataHandler&&) = delete;

	mutable bool _ArrowPatch{ true };
	mutable bool _BoltPatch{ true };
	mutable bool _ArrowSpeedEnable{ true };
	mutable bool _BoltSpeedEnable{ true };
	mutable bool _ArrowGravityEnable{ true };
	mutable bool _BoltGravityEnable{ true };
	mutable bool _LimitArrowSpeed{ false };
	mutable bool _LimitBoltSpeed{ false };
	mutable bool _LimitArrowDamage{ false };
	mutable bool _LimitBoltDamage{ false };
	mutable bool _ChangeArrowSoundLevel{ false };
	mutable bool _ChangeBoltSoundLevel{ false };
	mutable bool _RandomizeArrowSpeed{ false };
	mutable bool _RandomizeBoltSpeed{ false };

public:
	mutable bool _InfinitePlayerAmmo{ false };
	mutable bool _InfiniteTeammateAmmo{ false };
	mutable bool _HasFilesToMerge{ false };
	mutable bool _DoneAmmoPatching{ false };

private:
	mutable float           _ArrowDamageLimiterMin{ 10.0f };
	mutable float           _BoltDamageLimiterMin{ 10.0f };
	mutable float           _ArrowSpeedLimiterMin{ 3000.0f };
	mutable float           _BoltSpeedLimiterMin{ 4000.0f };
	mutable float           _ArrowSpeedRandomizerMin{ 3000.0f };
	mutable float           _BoltSpeedRandomizerMin{ 4000.0f };
	mutable float           _ArrowDamageLimiterMax{ 1000.0f };
	mutable float           _BoltDamageLimiterMax{ 1000.0f };
	mutable float           _ArrowSpeedLimiterMax{ 12000.0f };
	mutable float           _BoltSpeedLimiterMax{ 12000.0f };
	mutable float           _ArrowSpeedRandomizerMax{ 12000.0f };
	mutable float           _BoltSpeedRandomizerMax{ 12000.0f };
	mutable float           _ArrowSpeed{ 9000.0f };
	mutable float           _BoltSpeed{ 10800.0f };
	mutable float           _ArrowGravity{ 0.0f };
	mutable float           _BoltGravity{ 0.0f };
	mutable std::string     _ArrowSoundLevelStr{ "kSilent" };
	mutable std::string     _BoltSoundLevelStr{ "kSilent" };
	mutable RE::SOUND_LEVEL _ArrowSoundLevel{ RE::SOUND_LEVEL::kSilent };
	mutable RE::SOUND_LEVEL _BoltSoundLevel{ RE::SOUND_LEVEL::kSilent };

public:
	mutable std::string             _UserName{ "User" };
	const char*                     _FolderPath{ "Data/SKSE/Plugins/Ammo Patcher/" };
	mutable ordered_nJson           _AmmoInfo;  // store all AMMO info for exclusion files
	std::vector<std::string>        _AmmoModFiles;
	mutable std::mutex              _lock;
	mutable ordered_nJson           _JsonData;   // used for main json file
	std::unordered_set<std::string> _FormIDArray;
	std::unordered_set<std::string> _TESFileArray;
};
