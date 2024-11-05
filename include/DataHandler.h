#pragma once

namespace AP_Constants {
	// Damage-related constants
	constexpr float DAMAGE_MIN{ 10.0F };
	constexpr float DAMAGE_MAX{ 1000.0F };

	// Speed-related constants
	constexpr float SPEED_MIN{ 3000.0F };
	constexpr float SPEED_MAX{ 12000.0F };
	constexpr float RANDOM_SPEED_MIN{ 4000.0F };

	// Specific speed constants
	constexpr float ARROW_SPEED{ 9000.0F };
	constexpr float BOLT_SPEED{ 10800.0F };
}  // namespace AP_Constants

class DataHandler
{
public:
	static auto GetSingleton() -> DataHandler*;
	void        LoadMainJson();
	void        LoadExclusionJsonFiles();
	void        ProcessMainJson();
	void        PatchAMMO();
	void        LogDataHandlerContents();
	void        RevertToDefault();

	static auto ChangeLogLevel(const std::string& LogLevelStr) -> void {
		const spdlog::level::level_enum level{ spdlog::level::from_str(LogLevelStr) };
		spdlog::set_level(level);
		spdlog::flush_on(level);
		logger::trace("Set LogLevel to {}", spdlog::level::to_string_view(level));
	}

	[[nodiscard]] auto get_selected_preset() -> std::string& {
		const std::lock_guard lock(lock_);
		return selected_preset_;
	}

	auto set_selected_preset(const std::string& selected_preset) -> void {
		const std::lock_guard lock(lock_);
		selected_preset_ = selected_preset;
	}

	[[nodiscard]] auto get_ammo_info() -> ordered_nJson& {
		const std::lock_guard lock(lock_);
		return ammo_info_;
	}

	[[nodiscard]] auto get_ammo_mod_files() -> std::vector<std::string>& {
		const std::lock_guard lock(lock_);
		return ammo_mod_files_;
	}

	[[nodiscard]] auto get_main_json_data_map() -> std::map<std::string, ordered_nJson>& {
		const std::lock_guard lock(lock_);
		return main_json_data_map_;
	}

	[[nodiscard]] auto get_main_json_data() -> ordered_nJson& {
		const std::lock_guard lock(lock_);
		try {
			return main_json_data_map_.at(selected_preset_);
		} catch(std::out_of_range&) {
			return main_json_data_map_.at(fs::absolute("Data/SKSE/Plugins/Ammo Patcher/Presets/Default.json").string());
		}
	}

	[[nodiscard]] auto get_main_json_data_pair() -> std::map<std::string, ordered_nJson>::iterator {
		const std::lock_guard lock(lock_);
		auto                  iterator = main_json_data_map_.find(selected_preset_);
		if(iterator != main_json_data_map_.end()) {
			return iterator;
		}
		iterator = main_json_data_map_.find(fs::absolute("Data/SKSE/Plugins/Ammo Patcher/Presets/Default.json").string());
		return iterator;
	}

	[[nodiscard]] auto get_user_name() -> std::string& {
		const std::lock_guard lock(lock_);
		return user_name_;
	}

	[[nodiscard]] auto is_done_ammo_patching() const -> bool {
		const std::lock_guard lock(lock_);
		return done_ammo_patching_;
	}

	[[nodiscard]] auto get_form_id_array() -> std::unordered_set<std::string>& {
		const std::lock_guard lock(lock_);
		return form_id_array_;
	}

	[[nodiscard]] auto get_tes_file_array() -> std::unordered_set<std::string>& {
		const std::lock_guard lock(lock_);
		return tes_file_array_;
	}

	[[nodiscard]] auto is_infinite_player_ammo() const -> bool {
		const std::lock_guard lock(lock_);
		return infinite_player_ammo_;
	}

	[[nodiscard]] auto is_infinite_teammate_ammo() const -> bool {
		const std::lock_guard lock(lock_);
		return infinite_teammate_ammo_;
	}

		 DataHandler(const DataHandler&)                = delete;
		 DataHandler(DataHandler&&)                     = delete;
	auto operator= (const DataHandler&) -> DataHandler& = delete;
	auto operator= (DataHandler&&) -> DataHandler&      = delete;

private:
	 DataHandler() = default;
	~DataHandler() = default;

	bool                                 arrow_patch_{ true };
	bool                                 bolt_patch_{ true };
	bool                                 arrow_speed_enable_{ true };
	bool                                 bolt_speed_enable_{ true };
	bool                                 arrow_gravity_enable_{ true };
	bool                                 bolt_gravity_enable_{ true };
	bool                                 limit_arrow_speed_{ false };
	bool                                 limit_bolt_speed_{ false };
	bool                                 limit_arrow_damage_{ false };
	bool                                 limit_bolt_damage_{ false };
	bool                                 change_arrow_sound_level_{ false };
	bool                                 change_bolt_sound_level_{ false };
	bool                                 randomize_arrow_speed_{ false };
	bool                                 randomize_bolt_speed_{ false };
	bool                                 infinite_player_ammo_{ false };
	bool                                 infinite_teammate_ammo_{ false };
	bool                                 has_files_to_merge_{ false };
	bool                                 done_ammo_patching_{ false };
	float                                arrow_damage_limiter_min_{ AP_Constants::DAMAGE_MIN };
	float                                bolt_damage_limiter_min_{ AP_Constants::DAMAGE_MIN };
	float                                arrow_speed_limiter_min_{ AP_Constants::SPEED_MIN };
	float                                bolt_speed_limiter_min_{ AP_Constants::RANDOM_SPEED_MIN };
	float                                arrow_speed_randomizer_min_{ AP_Constants::SPEED_MIN };
	float                                bolt_speed_randomizer_min_{ AP_Constants::RANDOM_SPEED_MIN };
	float                                arrow_damage_limiter_max_{ AP_Constants::DAMAGE_MAX };
	float                                bolt_damage_limiter_max_{ AP_Constants::DAMAGE_MAX };
	float                                arrow_speed_limiter_max_{ AP_Constants::SPEED_MAX };
	float                                bolt_speed_limiter_max_{ AP_Constants::SPEED_MAX };
	float                                arrow_speed_randomizer_max_{ AP_Constants::SPEED_MAX };
	float                                bolt_speed_randomizer_max_{ AP_Constants::SPEED_MAX };
	float                                arrow_speed_{ AP_Constants::ARROW_SPEED };
	float                                bolt_speed_{ AP_Constants::BOLT_SPEED };
	float                                arrow_gravity_{};
	float                                bolt_gravity_{};
	mutable std::mutex                   lock_;
	std::string                          arrow_sound_level_str_{ "kSilent" };
	std::string                          bolt_sound_level_str_{ "kSilent" };
	RE::SOUND_LEVEL                      arrow_sound_level_{ RE::SOUND_LEVEL::kSilent };
	RE::SOUND_LEVEL                      bolt_sound_level_{ RE::SOUND_LEVEL::kSilent };
	std::string                          user_name_{ "User" };
	ordered_nJson                        ammo_info_;  // store all AMMO info for exclusion files
	std::vector<std::string>             ammo_mod_files_;
	std::map<std::string, ordered_nJson> main_json_data_map_;
	std::string                          selected_preset_;
	std::unordered_set<std::string>      form_id_array_;
	std::unordered_set<std::string>      tes_file_array_;
};
