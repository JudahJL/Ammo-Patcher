#include "DataHandler.h"
#include "Events.h"
#include "SimpleIni.h"
#include "logging.h"
#include "utils.h"

DataHandler* DataHandler::GetSingleton() {
	static DataHandler Singleton;
	return std::addressof( Singleton );
}

//void DataHandler::ResolveConflictsForSKSEPlugins() {
//}

void DataHandler::LoadMainJson()
{
	std::lock_guard<std::mutex> lock(_lock);
	std::ifstream               jsonFile( std::format( "Data/SKSE/Plugins/{}.json", SKSE::PluginDeclaration::GetSingleton()->GetName() ) );
	try {
		jsonFile >> _JsonData;  // used for parsing main json file
	} catch( const ordered_nJson::exception& e ) {
		_JsonData = {
			{ "Logging", {
                {"LogLevel", "info"}
            }},
			{ "User Details", {
                {"Username", "User"}
            }},
			{ "AMMO", { 
				{ "Infinite AMMO", { 
					{ "Player", false },
				    { "Teammate", false } 
		        } },
				{ "Arrow", { 
					{ "Enable Arrow Patch", true },
					{ "Change Gravity", { 
						{ "Enable", true },
						{ "Gravity", 0.0f } 
		            } },
					{ "Change Speed", { 
						{ "Enable", true },
				        { "Speed", 9000.0f } 
		            } },
					{ "Limit Speed", { 
						{ "Enable", false },
                        {"Min", 3000.0f},
                        {"Max", 12000.0f}
                    }},
					{ "Randomize Speed", { 
						{ "Enable", false },
                        {"Min", 3000.0f},
                        {"Max", 12000.0f}
                    }},
                    {"Limit Damage", { 
						{ "Enable", false },
                        {"Min", 10.0f},
                        {"Max", 1000.0f}
                    }},
                    {"Sound", { 
						{ "Change Sound Level", { 
							{ "Enable", false },
                            {"Sound Level", "kSilent"} 
		                } }
                    }}
                }},
				{ "Bolt", { 
					{ "Enable Bolt Patch", true },
						{ "Change Gravity", { 
							{ "Enable", true },
                            {"Gravity", 0.0f} 
		                } },
						{ "Change Speed", { 
							{ "Enable", true },
                            {"Speed", 10800.0f} 
		                } },
						{ "Limit Speed", { 
							{ "Enable", false },
						    { "Min", 4000.0f },
                            {"Max", 12000.0f}
                        }},
						{ "Randomize Speed", { 
							{ "Enable", false },
                            {"Min", 3000.0f},
							{ "Max", 12000.0f } 
		                } },
						{ "Limit Damage", { 
							{ "Enable", false },
							{ "Min", 10.0f },
							{ "Max", 1000.0f } 
		                } },
                        {"Sound", { 
							{ "Change Sound Level", { 
								{ "Enable", false },
								{ "Sound Level", "kSilent" }
                            }} 
		                } }
                }}
            }}
		};
		logger::error( "{}", e.what() );
		InlineUtils::customMessageBox( std::format( "{}. Are you sure you want to continue? if you continue, The error will be logged and a default json will be used instead.", e.what() ) );
	}
}

void DataHandler::LoadExclusionJsonFiles()
{
	std::lock_guard<std::mutex> lock(_lock);
	_FormIDArray.clear();

	_TESFileArray.clear();

	if (fs::exists(_FolderPath) && !fs::is_empty(_FolderPath)) {
		_HasFilesToMerge = true;
		for (const auto& entry : fs::directory_iterator(_FolderPath)) {
			fs::path entry_path = entry.path();

			if (!InlineUtils::resolve_symlink(entry_path, 10)) {
				logger::error("Skipping entry due to symlink loop: {}", fs::absolute(entry_path).generic_string());
				continue;
			}

			if (fs::is_regular_file(entry_path) && entry_path.extension() == ".json") {
				std::string EntryPathStr(fs::absolute(entry_path).generic_string());

				std::ifstream jFile(entry_path);

				if (!jFile.is_open()) {
					logger::error("Failed to open file: {}", EntryPathStr);
					continue;
				}
				ordered_nJson MergeJsonData;
				try {
					jFile >> MergeJsonData;
				} catch (const ordered_nJson::exception& e) {
					logger::error("{} parsing error : {}", EntryPathStr, e.what());

					InlineUtils::customMessageBox(std::format("{}. Are you sure you want to continue? if you continue, {} will be ignored and the error will be logged.", e.what(), EntryPathStr));
				}
				logger::debug("Loaded JSON from file: {}", EntryPathStr);

				if (MergeJsonData["AMMO FormID to Exclude"].is_array() && MergeJsonData["Mod File(s) to Exclude"].is_array()) {
					for (const std::string& a : MergeJsonData["AMMO FormID to Exclude"]) _FormIDArray.insert(a);
					for (const std::string& a : MergeJsonData["Mod File(s) to Exclude"]) _TESFileArray.insert(a);
				}
			}
		}
	} else
		logger::info("************No Exclusion will be Done************");
}

void DataHandler::ReloadLoggingIfNecessary(const std::string& LogLevelStr)
{
	std::lock_guard<std::mutex>                                      lock(_lock);
	static const std::unordered_map<std::string_view, spdlog::level::level_enum> logLevelMap{
		{ "trace"sv, spdlog::level::trace },
		{ "debug"sv, spdlog::level::debug },
		{ "info"sv, spdlog::level::info },
		{ "warn"sv, spdlog::level::warn },
		{ "err"sv, spdlog::level::err },
		{ "critical"sv, spdlog::level::critical },
		{ "off"sv, spdlog::level::off }
	};
	constexpr const char* Pattern{ "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v" };

	auto it = logLevelMap.find(LogLevelStr);
	if (it != logLevelMap.end()) {
		spdlog::level::level_enum newLevel = it->second;

		if (newLevel != spdlog::level::off) {
			if (spdlog::default_logger()->sinks().empty()) {
				Logging logger(newLevel);
			} else {
				spdlog::set_level(newLevel);
				spdlog::flush_on(newLevel);
				spdlog::set_pattern(Pattern);
			}
		} else {
			spdlog::default_logger()->sinks().clear();
			spdlog::set_pattern("");
		}
	} else {
		logger::error("Invalid log level: {}. Defaulting to info.", LogLevelStr);
		if (spdlog::default_logger()->sinks().empty()) {
			Logging logger;
		} else {
			spdlog::set_level(spdlog::level::info);
			spdlog::flush_on(spdlog::level::info);
			spdlog::set_pattern(Pattern);
		}
	}
}

void DataHandler::ProcessMainJson()
{
	std::lock_guard<std::mutex> lock(_lock);
	logger::info("*****************Processing Data*****************");
	if (!_JsonData.empty() && !_JsonData.is_null()) {
		try { //                                                    0      1        2       3           4                  5         6             7                  8               9          10         11           12              13    14      15       16          17           18         19        20                  21                      22               23       24            25           26             27
			constexpr const std::array<const char*,28> JsonKeys{ "AMMO", "Arrow", "Bolt", "Sound", "Change Sound Level", "Enable", "Sound Level", "Change Speed", "Change Gravity", "Gravity", "Speed", "Limit Speed", "Limit Damage", "Min", "Max", "kLoud", "kNormal", "kSilent", "kVeryLoud", "kQuiet", "Infinite AMMO", "Enable Arrow Patch", "Enable Bolt Patch", "Player", "Teammate", "User Details", "Username", "Randomize Speed" };

			ordered_nJson& AMMO = _JsonData[JsonKeys.at(0)];
			ordered_nJson& InfiniteAMMO = AMMO[JsonKeys.at(20)];

			ordered_nJson& Arrow = AMMO[JsonKeys.at(1)];
			ordered_nJson& Bolt = AMMO[JsonKeys.at(2)];

			ordered_nJson& AChangeGravity = Arrow[JsonKeys.at(8)];
			ordered_nJson& BChangeGravity = Bolt[JsonKeys.at(8)];

			ordered_nJson& AChangeSpeed = Arrow[JsonKeys.at(7)];
			ordered_nJson& BChangeSpeed = Bolt[JsonKeys.at(7)];

			ordered_nJson& ALimitSpeed = Arrow[JsonKeys.at(11)];
			ordered_nJson& BLimitSpeed = Bolt[JsonKeys.at(11)];

			ordered_nJson& ARandomizeSpeed = Arrow[JsonKeys.at(27)];
			ordered_nJson& BRandomizeSpeed = Bolt[JsonKeys.at(27)];

			ordered_nJson& ALimitDamage = Arrow[JsonKeys.at(12)];
			ordered_nJson& BLimitDamage = Bolt[JsonKeys.at(12)];

			ordered_nJson& AChangeSoundLevel = Arrow[JsonKeys.at(3)][JsonKeys.at(4)];
			ordered_nJson& BChangeSoundLevel = Bolt[JsonKeys.at(3)][JsonKeys.at(4)];

			_ArrowPatch = Arrow[JsonKeys.at(21)].get<bool>();
			_BoltPatch = Bolt[JsonKeys.at(22)].get<bool>();

			_InfinitePlayerAmmo = InfiniteAMMO[JsonKeys.at(23)].get<bool>();
			_InfiniteTeammateAmmo = InfiniteAMMO[JsonKeys.at(24)].get<bool>();

			//Enable
			constexpr const auto Enable{ 5 };
			_ArrowSpeedEnable = AChangeSpeed[JsonKeys.at(Enable)].get<bool>();
			_BoltSpeedEnable = BChangeSpeed[JsonKeys.at(Enable)].get<bool>();
			_ArrowGravityEnable = AChangeGravity[JsonKeys.at(Enable)].get<bool>();
			_BoltGravityEnable = BChangeGravity[JsonKeys.at(Enable)].get<bool>();
			_LimitArrowSpeed = ALimitSpeed[JsonKeys.at(Enable)].get<bool>();
			_LimitBoltSpeed = BLimitSpeed[JsonKeys.at(Enable)].get<bool>();
			_LimitArrowDamage = ALimitDamage[JsonKeys.at(Enable)].get<bool>();
			_LimitBoltDamage = BLimitDamage[JsonKeys.at(Enable)].get<bool>();
			_ChangeArrowSoundLevel = AChangeSoundLevel[JsonKeys.at(Enable)].get<bool>();
			_ChangeBoltSoundLevel = BChangeSoundLevel[JsonKeys.at(Enable)].get<bool>();
			_RandomizeArrowSpeed = ARandomizeSpeed[JsonKeys.at(Enable)].get<bool>();
			_RandomizeBoltSpeed = BRandomizeSpeed[JsonKeys.at(Enable)].get<bool>();

			//Min
			constexpr const auto Min{ 13 };
			_ArrowDamageLimiterMin = ALimitDamage[JsonKeys.at(Min)].get<float>();
			_BoltDamageLimiterMin = BLimitDamage[JsonKeys.at(Min)].get<float>();
			_ArrowSpeedLimiterMin = ALimitSpeed[JsonKeys.at(Min)].get<float>();
			_BoltSpeedLimiterMin = BLimitSpeed[JsonKeys.at(Min)].get<float>();
			_ArrowSpeedRandomizerMin = ARandomizeSpeed[JsonKeys.at(Min)].get<float>();
			_BoltSpeedRandomizerMin = BRandomizeSpeed[JsonKeys.at(Min)].get<float>();

			//Max
			constexpr const auto Max{ 14 };
			_ArrowDamageLimiterMax = ALimitDamage[JsonKeys.at(Max)].get<float>();
			_BoltDamageLimiterMax = BLimitDamage[JsonKeys.at(Max)].get<float>();
			_ArrowSpeedLimiterMax = ALimitSpeed[JsonKeys.at(Max)].get<float>();
			_BoltSpeedLimiterMax = BLimitSpeed[JsonKeys.at(Max)].get<float>();
			_ArrowSpeedRandomizerMax = ARandomizeSpeed[JsonKeys.at(Max)].get<float>();
			_BoltSpeedRandomizerMax = BRandomizeSpeed[JsonKeys.at(Max)].get<float>();

			//Speed
			constexpr const auto Speed{ 10 };
			_ArrowSpeed = AChangeSpeed[JsonKeys.at(Speed)].get<float>();
			_BoltSpeed = BChangeSpeed[JsonKeys.at(Speed)].get<float>();

			//Gravity
			constexpr const auto Gravity{ 9 };
			_ArrowGravity = AChangeGravity[JsonKeys.at(Gravity)].get<float>();
			_BoltGravity = BChangeGravity[JsonKeys.at(Gravity)].get<float>();
			
			_UserName = _JsonData[JsonKeys.at(25)][JsonKeys.at(26)].get<std::string>();

			if (_ArrowSpeedLimiterMin > _ArrowSpeedLimiterMax) {
				constexpr const char* ErrorMessage{ "Error Detected in Json. Make Sure Arrow Speed Min is lesser than or Equal to Arrow Speed Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}",ErrorMessage);
				InlineUtils::customMessageBox(ErrorMessage);
			}

			if (_BoltSpeedLimiterMin > _BoltSpeedLimiterMax) {
				constexpr const char* ErrorMessage{ "Error Detected in Json. Make Sure Bolt Speed Min is lesser than or Equal to Bolt Speed Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				InlineUtils::customMessageBox(ErrorMessage);
			}

			if (_ArrowDamageLimiterMin > _ArrowDamageLimiterMax) {
				constexpr const char* ErrorMessage{ "Error Detected in Json. Make Sure Arrow Damage Min is lesser than or Equal to Arrow Damage Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				InlineUtils::customMessageBox(ErrorMessage);
			}

			if (_BoltDamageLimiterMin > _BoltDamageLimiterMax) {
				constexpr const char* ErrorMessage{ "Error Detected in Json. Make Sure Bolt Damage Min is lesser than or Equal to Bolt Damage Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
				logger::error("{}", ErrorMessage);
				InlineUtils::customMessageBox(ErrorMessage);
			}

			if (_ArrowSpeedRandomizerMin > _ArrowSpeedRandomizerMax) {
				constexpr const char* ErrorMessage{ "Error Detected in Json. Make Sure Arrow Randomizer Min is lesser than or Equal to Arrow Randomizer Max.\nPlease Don't ignore this Warning. The Game will Crash" };
				logger::error("{}", ErrorMessage);
				InlineUtils::customMessageBox(ErrorMessage);
			}

			if (_ArrowSpeedRandomizerMin > _ArrowSpeedRandomizerMax) {
				constexpr const char* ErrorMessage{ "Error Detected in Json. Make Sure Bolt Randomizer Min is lesser than or Equal to Bolt Randomizer Max.\nPlease Don't ignore this Warning. The Game will Crash" };
				logger::error("{}", ErrorMessage);
				InlineUtils::customMessageBox(ErrorMessage);
			}

			
		    if (_InfinitePlayerAmmo || _InfiniteTeammateAmmo) {
				APEventProcessor::RegisterEvent();
			} else {
				APEventProcessor::UnregisterEvent();
			}

			const std::unordered_map<std::string, RE::SOUND_LEVEL> AmmoSoundLevelMap{
				{ JsonKeys.at(15), RE::SOUND_LEVEL::kLoud },
				{ JsonKeys.at(16), RE::SOUND_LEVEL::kNormal },
				{ JsonKeys.at(17), RE::SOUND_LEVEL::kSilent },
				{ JsonKeys.at(18), RE::SOUND_LEVEL::kVeryLoud },
				{ JsonKeys.at(19), RE::SOUND_LEVEL::kQuiet }
			};

			{
				auto it = AmmoSoundLevelMap.find(_ArrowSoundLevelStr);
				if (it != AmmoSoundLevelMap.end()) {
					_ArrowSoundLevel = it->second;
				} else {
					logger::error("Invalid Arrow Sound Level specified in the JSON file. Not Patching Arrow Sound Level.");
					_ChangeArrowSoundLevel = false;
				}
			}

			{
				auto it = AmmoSoundLevelMap.find(_BoltSoundLevelStr);
				if (it != AmmoSoundLevelMap.end()) {
					_BoltSoundLevel = it->second;
				} else {
					logger::error("Invalid Bolt Sound Level specified in the JSON file. Not Patching Bolt Sound Level.");
					_ChangeBoltSoundLevel = false;
				}
			}
		} catch (const ordered_nJson::exception& e) {
			logger::error("{}", e.what());
		}
	}

	logger::info("************Finished Processing Data*************");
}

void DataHandler::PatchAMMO() {
	std::lock_guard<std::mutex>           lock( _lock );
	_DoneAmmoPatching = false;
	std::chrono::steady_clock::time_point startAP = std::chrono::high_resolution_clock::now();
	logger::info( "{} {} is starting to patch", SKSE::PluginDeclaration::GetSingleton()->GetName(), SKSE::PluginDeclaration::GetSingleton()->GetVersion() );
	ordered_nJson j;
	RE::TESDataHandler*     DataHandler = RE::TESDataHandler::GetSingleton();

	for (const auto ammo : DataHandler->GetFormArray<RE::TESAmmo>()) {
		if (ammo) {
			constexpr const char* starString = "******************************************************************************************************************************";
			bool                  shouldPatch = true;

			j.clear();
			j["ModName"] = ammo->GetFile()->GetFilename();                                                          //ModName
			j["AmmoName"] = ammo->GetFullName();                                                                    //AmmoName
			j["AmmoFormID"] = ammo->GetRawFormID();                                                                 //AmmoFormID
			j["AmmoString"] = InlineUtils::GetStringFromFormIDAndModName( ammo->GetRawFormID(), ammo->GetFile() );  //AmmoString
			j["AmmoDamage"] = ammo->GetRuntimeData().data.damage; //AmmoDamage
			j["ProjName"] = nullptr; //ProjName
			j["ProjFormID"] = nullptr;  //ProjFormID
			j["ProjSpeed"] = nullptr;   //ProjSpeed
			j["ProjGravity"] = nullptr;  //ProjGravity

			if (const auto ammoProjectile = ammo->GetRuntimeData().data.projectile; ammoProjectile) {
				j["ProjName"] = ammoProjectile->GetFullName();  //ProjName
				j["ProjFormID"] = ammoProjectile->GetRawFormID();  //ProjFormID
				j["ProjSpeed"] = ammoProjectile->data.speed;   //ProjSpeed
				j["ProjGravity"] = ammoProjectile->data.gravity;  //ProjGravity
				if( _ArrowPatch || _BoltPatch ) {
					shouldPatch = true;
					if( _HasFilesToMerge ) {
						for( const std::string& ammoModName : _TESFileArray ) {
							if( ammoModName.c_str() == ammo->GetFile()->GetFilename() ) {
								shouldPatch = false;
								logger::debug( "{}", starString );
								logger::debug( "From {} :", ammoModName.c_str() );
								logger::debug( "Skipping Ammo : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}", ammo->GetFullName(), ammo->GetRawFormID(),
									ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity );
								logger::debug( "{}", starString );
								break;
							}
						}
						if( shouldPatch ) {
							for( const auto& ammoFormID : _FormIDArray ) {
								if (auto form = InlineUtils::GetFormFromIdentifier(ammoFormID); form) {
									if (ammo->GetFormID() == form->GetFormID()) {
										shouldPatch = false;
										logger::debug("{}", starString);
										logger::debug("Skipping Ammo : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
											ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
										logger::debug("{}", starString);
										break;
									}
								}
							}
						}
					}
					if( shouldPatch ) {
						if( !(ammo->GetRuntimeData().data.flags & RE::AMMO_DATA::Flag::kNonPlayable) ) {
							bool ammoPatched = false;
							if( _ChangeArrowSoundLevel || _ArrowSpeedEnable || _ArrowGravityEnable || _LimitArrowDamage || _LimitArrowSpeed || _ChangeBoltSoundLevel || _BoltSpeedEnable || _BoltGravityEnable || _LimitBoltDamage || _LimitBoltSpeed )
								ammoPatched = true;
							if( (_ArrowPatch || _BoltPatch) && ammoPatched ) {
								logger::debug( "{}", starString );
								logger::debug( "Before Patching : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
									ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename() );
							}

							if( ammo->GetRuntimeData().data.flags & RE::AMMO_DATA::Flag::kNonBolt ) {  // for arrow
								if( _ArrowPatch ) {
									if( _ChangeArrowSoundLevel ) {  // set sound level
										ammoProjectile->soundLevel = _ArrowSoundLevel;
										logger::debug( "changed Arrow Sound Level" );
									}
									if( _ArrowSpeedEnable ) {  // set speed
										ammoProjectile->data.speed = _ArrowSpeed;
										logger::debug( "Changed Arrow Speed" );
									}
									if( _ArrowGravityEnable ) {  // set gravity
										ammoProjectile->data.gravity = _ArrowGravity;
										logger::debug( "Changed Arrow Gravity" );
									}
									if( _LimitArrowDamage ) {  // limit damage
										InlineUtils::limit( ammo->GetRuntimeData().data.damage, _ArrowDamageLimiterMin, _ArrowDamageLimiterMax );
										logger::debug( "Limited Arrow Damage" );
									}
									if( _LimitArrowSpeed ) {  // limit speed
										InlineUtils::limit( ammoProjectile->data.speed, _ArrowSpeedLimiterMin, _ArrowSpeedLimiterMax );
										logger::debug( "Limited Arrow Level" );
									}
									if( _RandomizeArrowSpeed ) {
										float value = InlineUtils::getRandom(_ArrowSpeedRandomizerMin, _ArrowSpeedRandomizerMax);
										ammoProjectile->data.speed = value;
										logger::debug("Randomized Arrow Speed to {}", value);
									}
								}
							}

							if( !(ammo->GetRuntimeData().data.flags & RE::AMMO_DATA::Flag::kNonBolt) ) {  // for bolt
								if( _BoltPatch ) {
									if( _ChangeBoltSoundLevel ) {  // set sound level of bolt
										ammoProjectile->soundLevel = _BoltSoundLevel;
										logger::debug( "changed Bolt Sound Level" );
									}
									if( _BoltSpeedEnable ) {  // set speed of bolt
										ammoProjectile->data.speed = _BoltSpeed;
										logger::debug( "Changed Bolt Speed" );
									}
									if( _BoltGravityEnable ) {  // set gravity of bolt
										ammoProjectile->data.gravity = _BoltGravity;
										logger::debug( "Changed Bolt Speed" );
									}
									if( _LimitBoltSpeed ) {  // limit speed of bolt
										InlineUtils::limit( ammoProjectile->data.speed, _BoltSpeedLimiterMin, _BoltSpeedLimiterMax );
										logger::debug( "Limited Bolt Speed" );
									}
									if (_RandomizeArrowSpeed) {
										float value = InlineUtils::getRandom(_BoltSpeedRandomizerMin, _BoltSpeedRandomizerMax);
										ammoProjectile->data.speed = value;
										logger::debug("Randomized Bolt Speed to {}", value);
									}
									if( _LimitBoltDamage ) {  // limit damage of bolt
										InlineUtils::limit( ammo->GetRuntimeData().data.damage, _BoltDamageLimiterMin, _BoltDamageLimiterMax );
										logger::debug( "Limited Bolt Damage" );
									}
								}
							}

							if( (_ArrowPatch || _BoltPatch) && ammoPatched ) {
								logger::debug( "After Patching : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
									ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename() );
								logger::debug( "{}", starString );
							}
						}
					}
				}
			} else
				logger::info( "PROJ Record with Name '{}' with FormID '{:08X}' from file '{}' is nullptr i.e., NULL", ammo->GetFullName(), ammo->GetRawFormID(), ammo->GetFile()->GetFilename() );
			_AmmoInfo.push_back( j );
		}
	}
	logger::info( "{} {} has finished Patching", SKSE::PluginDeclaration::GetSingleton()->GetName(), SKSE::PluginDeclaration::GetSingleton()->GetVersion() );
	j.clear();

	_AmmoModFiles.clear();

	for( auto& item : _AmmoInfo ) _AmmoModFiles.push_back( item["ModName"].get<std::string>() );

	InlineUtils::RemoveAnyDuplicates( _AmmoModFiles );

	_DoneAmmoPatching = true;

	std::chrono::nanoseconds nanosecondsTakenForAP = std::chrono::duration( std::chrono::high_resolution_clock::now() - startAP );
	logger::info("Time Taken in {} totally is {} nanoseconds or {} microseconds or {} milliseconds or {} seconds or {} minutes", std::source_location::current().function_name(), nanosecondsTakenForAP.count(),
		std::chrono::duration_cast<std::chrono::microseconds>(nanosecondsTakenForAP).count(), std::chrono::duration_cast<std::chrono::milliseconds>(nanosecondsTakenForAP).count(),
		std::chrono::duration_cast<std::chrono::seconds>(nanosecondsTakenForAP).count(), std::chrono::duration_cast<std::chrono::minutes>(nanosecondsTakenForAP).count() );
}

void DataHandler::LogDataHandlerContents()
{
	constexpr const char* data{ "*************************************************" };

	logger::info("{}", data);
	logger::info("{}", data);
	logger::info("Patch Arrows : {}", _ArrowPatch);
	logger::info("Patch Bolts : {}", _BoltPatch);
	logger::info("{}", data);
	logger::info("Infinite Player AMMO : {}", _InfinitePlayerAmmo);
	logger::info("Infinite Teammate AMMO : {}", _InfiniteTeammateAmmo);
	logger::info("{}", data);
	logger::info("Set Arrow Gravity : {}", _ArrowGravityEnable);
	logger::info("Arrow Gravity : {}", _ArrowGravity);
	logger::info("{}", data);
	logger::info("Set Bolt Gravity : {}", _BoltGravityEnable);
	logger::info("Bolt Gravity : {}", _BoltGravity);
	logger::info("{}", data);
	logger::info("Set Arrow Speed : {}", _ArrowSpeedEnable);
	logger::info("Arrow Speed : {}", _ArrowSpeed);
	logger::info("{}", data);
	logger::info("Set Bolt Speed : {}", _BoltSpeedEnable);
	logger::info("Bolt Speed : {}", _BoltSpeed);
	logger::info("{}", data);
	logger::info("Set Arrow Speed Limit : {}", _LimitArrowSpeed);
	logger::info("Arrow Minimum Speed Limit : {}", _ArrowSpeedLimiterMin);
	logger::info("Arrow Maximum Speed Limit : {}", _ArrowSpeedLimiterMax);
	logger::info("{}", data);
	logger::info("Limit Bolt Speed : {}", _LimitBoltSpeed);
	logger::info("Bolt Minimum Speed Limit : {}", _BoltSpeedLimiterMin);
	logger::info("Bolt Maximum Speed Limit : {}", _BoltSpeedLimiterMax);
	logger::info("{}", data);
	logger::info("Change Arrow Sound Level : {}", _ChangeArrowSoundLevel);
	logger::info("Arrow Sound Level : {}", _ArrowSoundLevelStr);
	logger::info("{}", data);
	logger::info("Change Bolt Sound Level : {}", _ChangeBoltSoundLevel);
	logger::info("Bolt Sound Level : {}", _BoltSoundLevelStr);
	logger::info("{}", data);
	logger::info("Randomize Arrow Speed: {}", _RandomizeArrowSpeed);
	logger::info("Random Arrow Speed Min: {}", _ArrowSpeedRandomizerMin);
	logger::info("Random Arrow Speed Max: {}", _ArrowSpeedRandomizerMax);
	logger::info("{}", data);
	logger::info("Randomize Bolt Speed: {}", _RandomizeBoltSpeed);
	logger::info("Random Bolt Speed Min: {}", _BoltSpeedRandomizerMin);
	logger::info("Random Bolt Speed Max: {}", _BoltSpeedRandomizerMax);
	logger::info("{}", data);
	logger::info("Limit Arrow Damage : {}", _LimitArrowDamage);
	logger::info("Arrow Minimum Damage Limit : {}", _ArrowDamageLimiterMin);
	logger::info("Arrow Maximum Damage Limit : {}", _ArrowDamageLimiterMax);
	logger::info("{}", data);
	logger::info("Limit Bolt Damage : {}", _LimitBoltDamage);
	logger::info("Bolt Minimum Damage Limit : {}", _BoltDamageLimiterMin);
	logger::info("Bolt Maximum Damage Limit : {}", _BoltDamageLimiterMax);
	logger::info("{}", data);
	logger::info("{}", data);
}

void DataHandler::RevertToDefault()
{
	std::lock_guard<std::mutex> lock(_lock);
	logger::info("Starting to Revert");
	if (!_AmmoInfo.empty()) {
		_DoneAmmoPatching = false;
		constexpr const char* starString = "******************************************************************************************************************************";

		for( size_t i = 0sz; i < _AmmoInfo.size(); i++ ) {
			auto ammo = InlineUtils::GetFormFromIdentifier<RE::TESAmmo>(_AmmoInfo[i]["AmmoString"]);
			if (ammo) {
				auto ammoProjectile = ammo->GetRuntimeData().data.projectile;
				if (ammoProjectile && (!_AmmoInfo[i]["ProjFormID"].is_null())) {
					if (ammo->GetRawFormID() == _AmmoInfo[i]["AmmoFormID"].get<RE::FormID>() && ammoProjectile->GetRawFormID() == _AmmoInfo[i]["ProjFormID"].get<RE::FormID>()) {
						logger::debug("{}", starString);
						logger::debug("Before Reverting : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
							ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
						ammo->GetRuntimeData().data.damage = _AmmoInfo[i]["AmmoDamage"].get<float>();
						ammoProjectile->data.speed = _AmmoInfo[i]["ProjSpeed"].get<float>();
						ammoProjectile->data.gravity = _AmmoInfo[i]["ProjGravity"].get<float>();
						logger::debug("After Reverting : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
							ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
						logger::debug("{}", starString);
					}
				}
			}
		}
	}
	_DoneAmmoPatching = true;
	logger::info("Finished Reverting");
}
