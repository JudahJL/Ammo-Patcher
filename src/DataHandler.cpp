#include "DataHandler.h"
#include "Events.h"
#include "utils.h"

auto DataHandler::GetSingleton() -> DataHandler* {
	static DataHandler Singleton;
	return std::addressof(Singleton);
}

void DataHandler::LoadMainJson() {
	std::string   Selected;
	std::ifstream default_preset(std::format("Data/SKSE/Plugins/Ammo Patcher/{}_Default.json", SKSE::PluginDeclaration::GetSingleton()->GetName()));
	try {
		ordered_nJson preset;
		if(default_preset.is_open()) {
			default_preset >> preset;
		}
		Selected = preset.at("Load").get<std::string>();
	} catch(const fs::filesystem_error& e) {
		Utils::customMessageBox(e.what() + "\nDo You Want to Continue?(Please Don't since this uses the Ammo_Patcher_Default.json file)"s);
	} catch(const ordered_nJson::exception& e) {
		Utils::customMessageBox(e.what() + "\nDo You Want to Continue?(Please Don't since this loads a Preset)"s);
	}
	auto constexpr path{ "Data/SKSE/Plugins/Ammo Patcher/Presets/" };
	if(fs::exists(path) && !fs::is_empty(path)) {
		has_files_to_merge_ = true;
		std::set<std::string>    files;
		std::vector<std::thread> threads;
		std::string              errorStr;
		std::mutex               errorStrMutex;
		for(const auto& entry : fs::directory_iterator(path)) {
			fs::path          entry_path{ entry.path() };
			const std::string filename{ entry_path.filename().string() };

			files.insert(filename);

			if(!Utils::resolve_symlink(entry_path)) {
				logger::error("Skipping entry due to symlink loop: {}", fs::absolute(entry_path).generic_string());
				continue;
			}

			if(fs::is_regular_file(entry_path) && entry_path.extension() == ".json") {
				const std::string EntryPathStr(fs::absolute(entry_path).generic_string());
				if(filename == Selected) {
					const std::lock_guard lock(lock_);
					selected_preset_ = EntryPathStr;
				}
				threads.emplace_back([EntryPathStr, entry_path, this, &errorStrMutex, &errorStr] {
					std::ifstream jFile(entry_path);

					if(!jFile.is_open()) {
						logger::error("Failed to open file: {}", EntryPathStr);
						return;
					}
					try {
						const std::lock_guard lock(lock_);
						auto                  Data = ordered_nJson::parse(jFile);
						main_json_data_map_.insert({ EntryPathStr, std::move(Data) });
					} catch(const ordered_nJson::exception& e) {
						logger::error("{} parsing error : {}", EntryPathStr, e.what());
						const std::lock_guard lock(errorStrMutex);
						errorStr += std::format("{}: {}\n", EntryPathStr, e.what());
						return;
					}
					logger::debug("Loaded JSON from file: {}", EntryPathStr);
				});
			}
		}

		for(auto& thread : threads) {
			if(thread.joinable()) {
				thread.join();
			} else {
				util::report_and_fail("Failed to close threads");
			}
		}

		if(!files.contains(fs::path(selected_preset_).filename().string())) {
			Utils::customMessageBox(std::format("File '{}' not found. Operation aborted.\nWarning: Ignoring this may cause issues in most, if not all, SKSE plugins.", Selected));
		}

		if(!errorStr.empty()) {
			Utils::customMessageBox(errorStr + "Do You Want to Continue?(Please Dont)");
		}
	} else {
		util::report_and_fail("Didn't Find any json preset's in the Directory: "s + path + "\n");
	}
}

void DataHandler::LoadExclusionJsonFiles() {
	constexpr auto FolderPath{ "Data/SKSE/Plugins/Ammo Patcher/Exclusions/" };
	if(fs::exists(FolderPath) && !fs::is_empty(FolderPath)) {
		has_files_to_merge_ = true;
		std::vector<std::thread> threads;
		std::string              errorStr;
		std::mutex               errorStrMutex;
		for(const auto& entry : fs::directory_iterator(FolderPath)) {
			fs::path entry_path{ entry.path() };

			if(!Utils::resolve_symlink(entry_path)) {
				logger::error("Skipping entry due to symlink loop: {}", fs::absolute(entry_path).generic_string());
				continue;
			}

			if(fs::is_regular_file(entry_path) && entry_path.extension() == ".json") {
				threads.emplace_back([entry_path, this, &errorStrMutex, &errorStr] {
					std::string EntryPathStr(fs::absolute(entry_path).generic_string());

					std::ifstream jFile(entry_path);

				if (!jFile.is_open()) {
					logger::error("Failed to open file: {}", EntryPathStr);
					return;
				}
				ordered_nJson MergeJsonData;
				try {
					jFile >> MergeJsonData;
				} catch (const ordered_nJson::exception& e) {
					logger::error("{} parsing error : {}", EntryPathStr, e.what());
					std::lock_guard const lock(errorStrMutex);
					errorStr += std::format("{}: {}\n", EntryPathStr, e.what());
					return;
				}
				logger::debug("Loaded JSON from file: {}", EntryPathStr);

				if (MergeJsonData["AMMO FormID to Exclude"].is_array() && MergeJsonData["Mod File(s) to Exclude"].is_array()) {
					const std::lock_guard                              lock(lock_);
					form_id_array_.insert_range(MergeJsonData["AMMO FormID to Exclude"]);
					tes_file_array_.insert_range(MergeJsonData["Mod File(s) to Exclude"]);
				} });
			}
		}
		for(auto& thread : threads) {
			if(thread.joinable()) {
				thread.join();
			} else {
				util::report_and_fail("Failed to close threads");
			}
		}

		if(!errorStr.empty()) {
			Utils::customMessageBox(errorStr + "Do You Want to Continue?(Please Dont)");
		}
	} else {
		logger::info("************No Exclusion will be Done************");
	}
}

void DataHandler::ProcessMainJson() {
	const std::lock_guard lock(lock_);
	const auto&           main_json{ main_json_data_map_.at(selected_preset_) };
	logger::info("*****************Processing Data*****************");
	if(!main_json.empty() && !main_json.is_null()) {
		try {
			constexpr std::array JsonKeys{
				"AMMO",                //00
				"Arrow",               //01
				"Bolt",                //02
				"Sound",               //03
				"Change Sound Level",  //04
				"Enable",              //05
				"Sound Level",         //06
				"Change Speed",        //07
				"Change Gravity",      //08
				"Gravity",             //09
				"Speed",               //10
				"Limit Speed",         //11
				"Limit Damage",        //12
				"Min",                 //13
				"Max",                 //14
				"kLoud",               //15
				"kNormal",             //16
				"kSilent",             //17
				"kVeryLoud",           //18
				"kQuiet",              //19
				"Infinite AMMO",       //20
				"Enable Arrow Patch",  //21
				"Enable Bolt Patch",   //22
				"Player",              //23
				"Teammate",            //24
				"User Details",        //25
				"Username",            //26
				"Randomize Speed"      //27
			};

			const ordered_nJson& AMMO         = main_json.at(JsonKeys.at(0));
			const ordered_nJson& InfiniteAMMO = AMMO[JsonKeys.at(20)];

			const ordered_nJson& Arrow = AMMO[JsonKeys.at(1)];
			const ordered_nJson& Bolt  = AMMO[JsonKeys.at(2)];

			const ordered_nJson& AChangeGravity = Arrow[JsonKeys.at(8)];
			const ordered_nJson& BChangeGravity = Bolt[JsonKeys.at(8)];

			const ordered_nJson& AChangeSpeed = Arrow[JsonKeys.at(7)];
			const ordered_nJson& BChangeSpeed = Bolt[JsonKeys.at(7)];

			const ordered_nJson& ALimitSpeed = Arrow[JsonKeys.at(11)];
			const ordered_nJson& BLimitSpeed = Bolt[JsonKeys.at(11)];

			const ordered_nJson& ARandomizeSpeed = Arrow[JsonKeys.at(27)];
			const ordered_nJson& BRandomizeSpeed = Bolt[JsonKeys.at(27)];

			const ordered_nJson& ALimitDamage = Arrow[JsonKeys.at(12)];
			const ordered_nJson& BLimitDamage = Bolt[JsonKeys.at(12)];

			const ordered_nJson& AChangeSoundLevel = Arrow[JsonKeys.at(3)][JsonKeys.at(4)];
			const ordered_nJson& BChangeSoundLevel = Bolt[JsonKeys.at(3)][JsonKeys.at(4)];

			arrow_patch_ = Arrow[JsonKeys.at(21)].get<bool>();
			bolt_patch_  = Bolt[JsonKeys.at(22)].get<bool>();

			infinite_player_ammo_   = InfiniteAMMO[JsonKeys.at(23)].get<bool>();
			infinite_teammate_ammo_ = InfiniteAMMO[JsonKeys.at(24)].get<bool>();

			//Enable
			constexpr auto Enable{ 5 };
			arrow_speed_enable_       = AChangeSpeed[JsonKeys.at(Enable)].get<bool>();
			bolt_speed_enable_        = BChangeSpeed[JsonKeys.at(Enable)].get<bool>();
			arrow_gravity_enable_     = AChangeGravity[JsonKeys.at(Enable)].get<bool>();
			bolt_gravity_enable_      = BChangeGravity[JsonKeys.at(Enable)].get<bool>();
			limit_arrow_speed_        = ALimitSpeed[JsonKeys.at(Enable)].get<bool>();
			limit_bolt_speed_         = BLimitSpeed[JsonKeys.at(Enable)].get<bool>();
			limit_arrow_damage_       = ALimitDamage[JsonKeys.at(Enable)].get<bool>();
			limit_bolt_damage_        = BLimitDamage[JsonKeys.at(Enable)].get<bool>();
			change_arrow_sound_level_ = AChangeSoundLevel[JsonKeys.at(Enable)].get<bool>();
			change_bolt_sound_level_  = BChangeSoundLevel[JsonKeys.at(Enable)].get<bool>();
			randomize_arrow_speed_    = ARandomizeSpeed[JsonKeys.at(Enable)].get<bool>();
			randomize_bolt_speed_     = BRandomizeSpeed[JsonKeys.at(Enable)].get<bool>();

			//Min
			constexpr auto Min{ 13 };
			arrow_damage_limiter_min_   = ALimitDamage[JsonKeys.at(Min)].get<float>();
			bolt_damage_limiter_min_    = BLimitDamage[JsonKeys.at(Min)].get<float>();
			arrow_speed_limiter_min_    = ALimitSpeed[JsonKeys.at(Min)].get<float>();
			bolt_speed_limiter_min_     = BLimitSpeed[JsonKeys.at(Min)].get<float>();
			arrow_speed_randomizer_min_ = ARandomizeSpeed[JsonKeys.at(Min)].get<float>();
			bolt_speed_randomizer_min_  = BRandomizeSpeed[JsonKeys.at(Min)].get<float>();

			//Max
			constexpr auto Max{ 14 };
			arrow_damage_limiter_max_   = ALimitDamage[JsonKeys.at(Max)].get<float>();
			bolt_damage_limiter_max_    = BLimitDamage[JsonKeys.at(Max)].get<float>();
			arrow_speed_limiter_max_    = ALimitSpeed[JsonKeys.at(Max)].get<float>();
			bolt_speed_limiter_max_     = BLimitSpeed[JsonKeys.at(Max)].get<float>();
			arrow_speed_randomizer_max_ = ARandomizeSpeed[JsonKeys.at(Max)].get<float>();
			bolt_speed_randomizer_max_  = BRandomizeSpeed[JsonKeys.at(Max)].get<float>();

			//Speed
			constexpr auto Speed{ 10 };
			arrow_speed_ = AChangeSpeed[JsonKeys.at(Speed)].get<float>();
			bolt_speed_  = BChangeSpeed[JsonKeys.at(Speed)].get<float>();

			//Gravity
			constexpr auto Gravity{ 9 };
			arrow_gravity_ = AChangeGravity[JsonKeys.at(Gravity)].get<float>();
			bolt_gravity_  = BChangeGravity[JsonKeys.at(Gravity)].get<float>();

			user_name_ = main_json.at(JsonKeys.at(25)).at(JsonKeys.at(26)).get<std::string>();

			if(arrow_speed_limiter_min_ > arrow_speed_limiter_max_) {
				constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Arrow Speed Min is lesser than or Equal to Arrow Speed Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				Utils::customMessageBox(ErrorMessage);
			}

			if(bolt_speed_limiter_min_ > bolt_speed_limiter_max_) {
				constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Bolt Speed Min is lesser than or Equal to Bolt Speed Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				Utils::customMessageBox(ErrorMessage);
			}

			if(arrow_damage_limiter_min_ > arrow_damage_limiter_max_) {
				constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Arrow Damage Min is lesser than or Equal to Arrow Damage Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				Utils::customMessageBox(ErrorMessage);
			}

			if(bolt_damage_limiter_min_ > bolt_damage_limiter_max_) {
				constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Bolt Damage Min is lesser than or Equal to Bolt Damage Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				Utils::customMessageBox(ErrorMessage);
			}

			if(arrow_speed_randomizer_min_ > arrow_speed_randomizer_max_) {
				constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Arrow Randomizer Min is lesser than or Equal to Arrow Randomizer Max.\nPlease Don't ignore this Warning. The Game will Crash" };
				logger::error("{}", ErrorMessage);
				Utils::customMessageBox(ErrorMessage);
			}

			if(arrow_speed_randomizer_min_ > arrow_speed_randomizer_max_) {
				constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Bolt Randomizer Min is lesser than or Equal to Bolt Randomizer Max.\nPlease Don't ignore this Warning. The Game will Crash" };
				logger::error("{}", ErrorMessage);
				Utils::customMessageBox(ErrorMessage);
			}

			auto* const event{ APEventProcessor::GetSingleton() };
			if(infinite_player_ammo_ || infinite_teammate_ammo_) {
				event->RegisterEvent();
			} else {
				event->UnregisterEvent();
			}

			const std::unordered_map<std::string_view, RE::SOUND_LEVEL> AmmoSoundLevelMap{
				{ JsonKeys.at(15),     RE::SOUND_LEVEL::kLoud },
				{ JsonKeys.at(16),   RE::SOUND_LEVEL::kNormal },
				{ JsonKeys.at(17),   RE::SOUND_LEVEL::kSilent },
				{ JsonKeys.at(18), RE::SOUND_LEVEL::kVeryLoud },
				{ JsonKeys.at(19),    RE::SOUND_LEVEL::kQuiet }
			};

			if(const auto iterator{ AmmoSoundLevelMap.find(arrow_sound_level_str_) }; iterator != AmmoSoundLevelMap.end()) {
				arrow_sound_level_ = iterator->second;
			} else {
				logger::error("Invalid Arrow Sound Level specified in the JSON file. Not Patching Arrow Sound Level.");
				change_arrow_sound_level_ = false;
			}

			if(const auto iterator{ AmmoSoundLevelMap.find(bolt_sound_level_str_) }; iterator != AmmoSoundLevelMap.end()) {
				bolt_sound_level_ = iterator->second;
			} else {
				logger::error("Invalid Bolt Sound Level specified in the JSON file. Not Patching Bolt Sound Level.");
				change_bolt_sound_level_ = false;
			}

		} catch(const ordered_nJson::exception& e) {
			logger::error("{}", e.what());
		}
	}

	logger::info("************Finished Processing Data*************");
}

void DataHandler::PatchAMMO() {
	const std::lock_guard lock(lock_);
	done_ammo_patching_ = false;
	const auto startAP  = std::chrono::high_resolution_clock::now();
	logger::info("{} {} is starting to patch", SKSE::PluginDeclaration::GetSingleton()->GetName(), SKSE::PluginDeclaration::GetSingleton()->GetVersion().string("."));

	for(auto* const ammo : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESAmmo>()) {
		if(ammo == nullptr) {
			continue;
		}

		auto* const ammoProjectile = ammo->GetRuntimeData().data.projectile;
		if(ammoProjectile == nullptr) {
			logger::info("PROJ Record with Name '{}' with FormID '{:08X}' from file '{}' is nullptr i.e., NULL", ammo->GetFullName(), ammo->GetRawFormID(), ammo->GetFile()->GetFilename());
			continue;
		}

		{
			ordered_nJson a_json;
			a_json["ModName"]     = ammo->GetFile()->GetFilename();                                               //ModName
			a_json["AmmoName"]    = ammo->GetFullName();                                                          //AmmoName
			a_json["AmmoFormID"]  = ammo->GetRawFormID();                                                         //AmmoFormID
			a_json["AmmoString"]  = Utils::GetStringFromFormIDAndModName(ammo->GetRawFormID(), ammo->GetFile());  //AmmoString
			a_json["AmmoDamage"]  = ammo->GetRuntimeData().data.damage;                                           //AmmoDamage
			a_json["ProjName"]    = ammoProjectile->GetFullName();                                                //ProjName
			a_json["ProjFormID"]  = ammoProjectile->GetRawFormID();                                               //ProjFormID
			a_json["ProjSpeed"]   = ammoProjectile->data.speed;                                                   //ProjSpeed
			a_json["ProjGravity"] = ammoProjectile->data.gravity;                                                 //ProjGravity
			ammo_info_.push_back(std::move(a_json));
		}

		if(arrow_patch_ || bolt_patch_) {
			constexpr auto starString{ Utils::make_filled_char_array<125, '*'>() };
			bool           shouldPatch{ true };
			if(has_files_to_merge_) {
				for(const std::string_view ammoModName : tes_file_array_) {
					if(ammoModName == ammo->GetFile()->GetFilename()) {
						shouldPatch = false;
						logger::debug("{}", starString.data());
						logger::debug("From {} :", ammoModName.data());
						logger::debug("Skipping Ammo : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}", ammo->GetFullName(), ammo->GetRawFormID(),
						              ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity);
						logger::debug("{}", starString.data());
						break;
					}
				}
				if(shouldPatch) {
					for(const std::string_view ammoFormID : form_id_array_) {
						if(const auto* form{ Utils::GetFromIdentifier<RE::TESForm>(ammoFormID) }; form) {
							if(ammo->GetFormID() == form->GetFormID()) {
								shouldPatch = false;
								logger::debug("{}", starString.data());
								logger::debug("Skipping Ammo : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
								              ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
								logger::debug("{}", starString.data());
								break;
							}
						}
					}
				}
			}
			if(shouldPatch) {
				if(!(ammo->GetRuntimeData().data.flags & RE::AMMO_DATA::Flag::kNonPlayable)) {
					const bool ammoPatched{ arrow_speed_enable_ ||
						                    bolt_speed_enable_ ||
						                    arrow_gravity_enable_ ||
						                    bolt_gravity_enable_ ||
						                    limit_arrow_speed_ ||
						                    limit_bolt_speed_ ||
						                    limit_arrow_damage_ ||
						                    limit_bolt_damage_ ||
						                    change_arrow_sound_level_ ||
						                    change_bolt_sound_level_ ||
						                    randomize_arrow_speed_ ||
						                    randomize_bolt_speed_ };
					if(ammoPatched) {
						logger::debug("{}", starString.data());
						logger::debug("Before Patching : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
						              ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
					}

					if(ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt)) {  // for arrow
						if(arrow_patch_) {
							if(change_arrow_sound_level_) {
								ammoProjectile->soundLevel = arrow_sound_level_;
								logger::debug("changed Arrow Sound Level");
							}
							if(arrow_speed_enable_) {
								ammoProjectile->data.speed = arrow_speed_;
								logger::debug("Changed Arrow Speed");
							}
							if(arrow_gravity_enable_) {
								ammoProjectile->data.gravity = arrow_gravity_;
								logger::debug("Changed Arrow Gravity");
							}
							if(limit_arrow_damage_) {
								Utils::limit(ammo->GetRuntimeData().data.damage, arrow_damage_limiter_min_, arrow_damage_limiter_max_);
								logger::debug("Limited Arrow Damage");
							}
							if(limit_arrow_speed_) {
								Utils::limit(ammoProjectile->data.speed, arrow_speed_limiter_min_, arrow_speed_limiter_max_);
								logger::debug("Limited Arrow Level");
							}
							if(randomize_arrow_speed_) {
								float value                = Utils::getRandom(arrow_speed_randomizer_min_, arrow_speed_randomizer_max_);
								ammoProjectile->data.speed = value;
								logger::debug("Randomized Arrow Speed to {}", value);
							}
						}
					}

					if(ammo->GetRuntimeData().data.flags.none(RE::AMMO_DATA::Flag::kNonBolt)) {  // for bolt
						if(bolt_patch_) {
							if(change_bolt_sound_level_) {
								ammoProjectile->soundLevel = bolt_sound_level_;
								logger::debug("changed Bolt Sound Level");
							}
							if(bolt_speed_enable_) {
								ammoProjectile->data.speed = bolt_speed_;
								logger::debug("Changed Bolt Speed");
							}
							if(bolt_gravity_enable_) {
								ammoProjectile->data.gravity = bolt_gravity_;
								logger::debug("Changed Bolt Speed");
							}
							if(limit_bolt_speed_) {
								Utils::limit(ammoProjectile->data.speed, bolt_speed_limiter_min_, bolt_speed_limiter_max_);
								logger::debug("Limited Bolt Speed");
							}
							if(randomize_arrow_speed_) {
								float value                = Utils::getRandom(bolt_speed_randomizer_min_, bolt_speed_randomizer_max_);
								ammoProjectile->data.speed = value;
								logger::debug("Randomized Bolt Speed to {}", value);
							}
							if(limit_bolt_damage_) {
								Utils::limit(ammo->GetRuntimeData().data.damage, bolt_damage_limiter_min_, bolt_damage_limiter_max_);
								logger::debug("Limited Bolt Damage");
							}
						}
					}

					if(ammoPatched) {
						logger::debug("After Patching : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
						              ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
						logger::debug("{}", starString.data());
					}
				}
			}
		}
	}
	logger::info("{} {} has finished Patching", SKSE::PluginDeclaration::GetSingleton()->GetName(), SKSE::PluginDeclaration::GetSingleton()->GetVersion().string("."));

	ammo_mod_files_.clear();

	for(auto& item : ammo_info_) {
		ammo_mod_files_.push_back(item["ModName"].get<std::string>());
	}

	Utils::RemoveAnyDuplicates(ammo_mod_files_);

	done_ammo_patching_ = true;

	const auto nanosecondsTakenForAP = std::chrono::duration(std::chrono::high_resolution_clock::now() - startAP);
	logger::info("Time Taken in {} totally is {} nanoseconds or {} microseconds or {} milliseconds or {} seconds or {} minutes", std::source_location::current().function_name(), nanosecondsTakenForAP.count(),
	             std::chrono::duration_cast<std::chrono::microseconds>(nanosecondsTakenForAP).count(), std::chrono::duration_cast<std::chrono::milliseconds>(nanosecondsTakenForAP).count(),
	             std::chrono::duration_cast<std::chrono::seconds>(nanosecondsTakenForAP).count(), std::chrono::duration_cast<std::chrono::minutes>(nanosecondsTakenForAP).count());
}

void DataHandler::LogDataHandlerContents() {
	constexpr auto starCharPtr{ Utils::make_filled_char_array<50, '*'>() };
	logger::info("Preset: {}", selected_preset_);
	logger::info("{}", starCharPtr.data());
	logger::info("LogLevel: {}", spdlog::level::to_string_view(spdlog::get_level()));
	logger::info("{}", starCharPtr.data());
	logger::info("Patch Arrows : {}", arrow_patch_);
	logger::info("Patch Bolts : {}", bolt_patch_);
	logger::info("{}", starCharPtr.data());
	logger::info("Infinite Player AMMO : {}", infinite_player_ammo_);
	logger::info("Infinite Teammate AMMO : {}", infinite_teammate_ammo_);
	logger::info("{}", starCharPtr.data());
	logger::info("Set Arrow Gravity : {}", arrow_gravity_enable_);
	logger::info("Arrow Gravity : {}", arrow_gravity_);
	logger::info("{}", starCharPtr.data());
	logger::info("Set Bolt Gravity : {}", bolt_gravity_enable_);
	logger::info("Bolt Gravity : {}", bolt_gravity_);
	logger::info("{}", starCharPtr.data());
	logger::info("Set Arrow Speed : {}", arrow_speed_enable_);
	logger::info("Arrow Speed : {}", arrow_speed_);
	logger::info("{}", starCharPtr.data());
	logger::info("Set Bolt Speed : {}", bolt_speed_enable_);
	logger::info("Bolt Speed : {}", bolt_speed_);
	logger::info("{}", starCharPtr.data());
	logger::info("Set Arrow Speed Limit : {}", limit_arrow_speed_);
	logger::info("Arrow Minimum Speed Limit : {}", arrow_speed_limiter_min_);
	logger::info("Arrow Maximum Speed Limit : {}", arrow_speed_limiter_max_);
	logger::info("{}", starCharPtr.data());
	logger::info("Limit Bolt Speed : {}", limit_bolt_speed_);
	logger::info("Bolt Minimum Speed Limit : {}", bolt_speed_limiter_min_);
	logger::info("Bolt Maximum Speed Limit : {}", bolt_speed_limiter_max_);
	logger::info("{}", starCharPtr.data());
	logger::info("Change Arrow Sound Level : {}", change_arrow_sound_level_);
	logger::info("Arrow Sound Level : {}", arrow_sound_level_str_);
	logger::info("{}", starCharPtr.data());
	logger::info("Change Bolt Sound Level : {}", change_bolt_sound_level_);
	logger::info("Bolt Sound Level : {}", bolt_sound_level_str_);
	logger::info("{}", starCharPtr.data());
	logger::info("Randomize Arrow Speed: {}", randomize_arrow_speed_);
	logger::info("Random Arrow Speed Min: {}", arrow_speed_randomizer_min_);
	logger::info("Random Arrow Speed Max: {}", arrow_speed_randomizer_max_);
	logger::info("{}", starCharPtr.data());
	logger::info("Randomize Bolt Speed: {}", randomize_bolt_speed_);
	logger::info("Random Bolt Speed Min: {}", bolt_speed_randomizer_min_);
	logger::info("Random Bolt Speed Max: {}", bolt_speed_randomizer_max_);
	logger::info("{}", starCharPtr.data());
	logger::info("Limit Arrow Damage : {}", limit_arrow_damage_);
	logger::info("Arrow Minimum Damage Limit : {}", arrow_damage_limiter_min_);
	logger::info("Arrow Maximum Damage Limit : {}", arrow_damage_limiter_max_);
	logger::info("{}", starCharPtr.data());
	logger::info("Limit Bolt Damage : {}", limit_bolt_damage_);
	logger::info("Bolt Minimum Damage Limit : {}", bolt_damage_limiter_min_);
	logger::info("Bolt Maximum Damage Limit : {}", bolt_damage_limiter_max_);
	logger::info("{}", starCharPtr.data());
	logger::info("{}", starCharPtr.data());
}

void DataHandler::RevertToDefault() {
	const std::lock_guard lock(lock_);
	logger::info("Starting to Revert");
	if(!ammo_info_.empty()) {
		done_ammo_patching_ = false;

		for(size_t i{}; i < ammo_info_.size(); i++) {
			if(auto* ammo = Utils::GetFromIdentifier<RE::TESAmmo>(ammo_info_[i]["AmmoString"]); ammo) {
				if(auto* ammoProjectile = ammo->GetRuntimeData().data.projectile; (ammoProjectile != nullptr) && (!ammo_info_[i]["ProjFormID"].is_null())) {
					if(ammo->GetRawFormID() == ammo_info_[i]["AmmoFormID"].get<RE::FormID>() && ammoProjectile->GetRawFormID() == ammo_info_[i]["ProjFormID"].get<RE::FormID>()) {
						constexpr auto starCharPtr = Utils::make_filled_char_array<130, '*'>();
						logger::debug("{}", starCharPtr.data());
						logger::debug("Before Reverting : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
						              ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
						ammo->GetRuntimeData().data.damage = ammo_info_[i]["AmmoDamage"].get<float>();
						ammoProjectile->data.speed         = ammo_info_[i]["ProjSpeed"].get<float>();
						ammoProjectile->data.gravity       = ammo_info_[i]["ProjGravity"].get<float>();
						logger::debug("After Reverting : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
						              ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
						logger::debug("{}", starCharPtr.data());
					}
				}
			}
		}
	}
	done_ammo_patching_ = true;
	logger::info("Finished Reverting");
}
