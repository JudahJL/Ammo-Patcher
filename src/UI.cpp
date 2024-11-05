#include "DataHandler.h"
#include "UI.h"
#include "utils.h"

auto SMFRenderer::GetSingleton() -> SMFRenderer* {
	static SMFRenderer Singleton;
	return std::addressof( Singleton );
}

void SMFRenderer::Register() {
	if( !SKSEMenuFramework::IsInstalled() ) {
		logger::error( "Unable to Register For SKSEMenuFramework, Please install SKSEMenuFramework to configure the Json Files(if you want to)" );
		return;
	}

	SMFRenderer* smf_renderer{ SMFRenderer::GetSingleton() };

	SKSEMenuFramework::SetSection( "Ammo Patcher" );

	std::ifstream FileContainingHintsForMain( "Data/SKSE/Plugins/Ammo Patcher/Hints/MainHint.json" );
	FileContainingHintsForMain >> smf_renderer->hints_for_main_;

	SKSEMenuFramework::AddSectionItem( "Change Selected Preset", SMFRenderer::RenderDefaultPreset );

	SKSEMenuFramework::AddSectionItem("Edit Presets", SMFRenderer::RenderEditPresets);

	std::ifstream FileContainingHintsForExclusions( "Data/SKSE/Plugins/Ammo Patcher/Hints/ExclusionHint.json" );
	FileContainingHintsForExclusions >> smf_renderer->hints_for_exclusions_;

	SMFRenderer::log_window_ = SKSEMenuFramework::AddWindow( SMFRenderer::RenderLogWindow );

	SKSEMenuFramework::AddSectionItem( "Exclusions", SMFRenderer::RenderExclusions );

	SKSEMenuFramework::AddSectionItem("Debug", SMFRenderer::RenderDebug);
	logger::info( "Registered For SKSEMenuFramework" );
}

inline void __stdcall SMFRenderer::RenderLogWindow() {
	ImGui::SetNextWindowSize( ImVec2( 1280, 780 ), ImGuiCond_FirstUseEver );
	static std::string const name{ std::format("Log Window##UILogger{}", SKSE::PluginDeclaration::GetSingleton()->GetName()) };

	ImGui::Begin( name.c_str(), nullptr, ImGuiWindowFlags_None);
	// if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
	// 	log_window_->IsOpen = false;
	// }
	UILogger* ui_logger{ UILogger::GetSingleton() };

	static std::array<char,UI::MAX_SIZE> filterInput{ "" };
	ImGui::InputText( "Filter", filterInput.data(), filterInput.size() );

	if( ImGui::Button( "Clear Logs" ) ) {
		ui_logger->ClearLogs();
	}
	ImGui::SameLine();
	if( ImGui::Button( "Close Window" ) ) {
		log_window_->IsOpen = false;
	}

	ImGui::Separator();
	ImGui::BeginChild( "LogScroll" );
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 1 ) );

	auto logs = (filterInput[0] != 0) ? ui_logger->GetFilteredLogs( filterInput.data() ) : ui_logger->GetLogs();
	for( size_t i{}; i < logs.size(); i++ ) {
		ImGui::TextUnformatted( std::format( "[{}] {}", i + 1_sz, logs[i] ).c_str() );
	}

	if( ui_logger->ShouldScrollToBottom() ) {
		ImGui::SetScrollHereY( 1.0F );
		ui_logger->ResetScrollToBottom();
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}

void __stdcall SMFRenderer::RenderDebug() {
	if(ImGui::Button("Quit")) {
		REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_SUCCESS);
	}
	SMFRenderer::RenderLogButton();
}

void SMFRenderer::RenderEditPresets() {
	auto* const data_handler{ DataHandler::GetSingleton() };

	const static std::string Selected{ fs::path(data_handler->get_selected_preset()).filename().string() };
	if(ImGui::BeginCombo("Presets", fs::path(data_handler->get_selected_preset()).filename().string().c_str())) {
		for(const auto& key : data_handler->get_main_json_data_map() | std::views::keys) {
			const bool isSelected = (data_handler->get_selected_preset() == key);
			if(ImGui::Selectable(fs::path(key).filename().string().c_str(), isSelected)) {
				data_handler->set_selected_preset(key);
			}
			if(isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Button(UI::question.c_str());
	if(ImGui::IsItemHovered()) {
		ImGui::SetTooltip("All the presets available for this Mod. Below Details will get loaded Accordingly");
	}

	auto* smf_renderer{ SMFRenderer::GetSingleton() };
	smf_renderer->RenderJsonEditor(Selected, data_handler->get_main_json_data(), smf_renderer->hints_for_main_);

	auto* ui_logger{ UILogger::GetSingleton() };
	if(ImGui::Button("Save JSON")) {
		if(SMFRenderer::SaveJsonToFile(data_handler->get_main_json_data_pair())) {
			ui_logger->AddLog(std::format("{} Saved {} Successfully", data_handler->get_user_name(), data_handler->get_main_json_data_pair()->first));
		} else {
			ui_logger->AddLog(std::format("Failed to Save {}", data_handler->get_main_json_data_pair()->first));
		}
	}
	if(ImGui::IsItemHovered()) {
		ImGui::SetTooltip("You Must Click This to SAVE any Changes");
	}
	if(data_handler->is_done_ammo_patching()) {
		ImGui::SameLine();
		if(ImGui::Button("Revert Changes")) {
			data_handler->RevertToDefault();
		}
		if(ImGui::IsItemHovered()) {
			ImGui::SetTooltip("This Button will Revert ANY changes made to all AMMO Records Safely");
		}
		ImGui::SameLine();
		if(ImGui::Button("RePatch Changes")) {
			if(SMFRenderer::SaveJsonToFile(data_handler->get_main_json_data_pair())) {
				ui_logger->AddLog(std::format("{} Saved {} Successfully", data_handler->get_user_name(), data_handler->get_main_json_data_pair()->first));
			} else {
				ui_logger->AddLog(std::format("Failed to Save {}", data_handler->get_main_json_data_pair()->first));
			}
			DataHandler::ChangeLogLevel(data_handler->get_main_json_data().at("Logging").at("LogLevel"));
			data_handler->ProcessMainJson();
			data_handler->LogDataHandlerContents();
			data_handler->PatchAMMO();
			ui_logger->AddLog("Patched AMMO");
		}
		if(ImGui::IsItemHovered()) {
			ImGui::SetTooltip("This Button will Patch AMMO Records According to The Main Json File and and the Exclusion Files.\nFirst It will Save the file.\nThen Process it.\nThen Log The Details and finally Patch The Records");
		}
	}
	SMFRenderer::RenderLogButton();
}

void SMFRenderer::RenderLogButton() {
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 10.0F);
	static ImVec2 LoggingButtonSize;
	ImGui::GetContentRegionAvail( std::addressof( LoggingButtonSize ) );
	LoggingButtonSize.y = 0;
	if( ImGui::Button( UILogger::GetSingleton()->GetLatestLog().c_str(), LoggingButtonSize ) ) {
		log_window_->IsOpen = true;
	}
	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "Click this to open Log Window" );
	}
}

void SMFRenderer::RenderDefaultPreset() {
	SMFRenderer* smf_renderer{ SMFRenderer::GetSingleton() };

	std::lock_guard const lock( smf_renderer->lock_ );

	DataHandler*       data_handler{ DataHandler::GetSingleton() };
	UILogger*      ui_logger{ UILogger::GetSingleton() };
	static std::string Selected{ fs::path(data_handler->get_selected_preset()).filename().string() };

	if (ImGui::BeginCombo("Selected Preset", Selected.c_str())) {
		for (const auto& key : data_handler->get_main_json_data_map() | std::views::keys) {
			const bool isSelected = (Selected == fs::path(key).filename().string());
			if (ImGui::Selectable(fs::path(key).filename().string().c_str(), isSelected)) {
				Selected = fs::path(key).filename().string();
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("Save to File")) {
		std::ofstream file_stream(std::format("Data/SKSE/Plugins/Ammo Patcher/{}_Default.json", SKSE::PluginDeclaration::GetSingleton()->GetName()));

		if (file_stream.is_open()) {
			file_stream << ordered_nJson{ { "Load", Selected } };
		}
	}
	if(ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Clicking this will save \n{\n\t\"Load\": \"%s\"\n}\n into Data/SKSE/Plugins/Ammo Patcher/%s_Default.json", Selected.c_str(), SKSE::PluginDeclaration::GetSingleton()->GetName().data());
	}
	{
		static std::array<char, UI::MAX_SIZE> buffer{ "" };
		if (ImGui::InputText("Create a Preset?", buffer.data(), buffer.size() , ImGuiInputTextFlags_EnterReturnsTrue)) {
			std::string FileName{ fs::absolute(std::format("Data/SKSE/Plugins/Ammo Patcher/Presets/{}.json", buffer.data())).string() };
			if (!data_handler->get_main_json_data_map().contains(FileName)) {
				auto json_object = ordered_nJson{
					{ "Logging", { { "LogLevel", "info" } } },
					{ "User Details", { { "Username", "User" } } },
					{ "AMMO", { { "Infinite AMMO", { { "Player", false },
													   { "Teammate", false } } },
								  { "Arrow", { { "Enable Arrow Patch", true },
												 { "Change Gravity", { { "Enable", true },
																		 { "Gravity", 0.0F } } },
												 { "Change Speed", { { "Enable", true },
																	   { "Speed", 9000.0F } } },
												 { "Limit Speed", { { "Enable", false },
																	  { "Min", 3000.0F },
																	  { "Max", 12000.0F } } },
												 { "Randomize Speed", { { "Enable", false },
																		  { "Min", 3000.0F },
																		  { "Max", 12000.0F } } },
												 { "Limit Damage", { { "Enable", false },
																	   { "Min", 10.0F },
																	   { "Max", 1000.0F } } },
												 { "Sound", { { "Change Sound Level", { { "Enable", false },
																						  { "Sound Level", "kSilent" } } } } } } },
								  { "Bolt", { { "Enable Bolt Patch", true },
												{ "Change Gravity", { { "Enable", true },
																		{ "Gravity", 0.0 } } },
												{ "Change Speed", { { "Enable", true },
																	  { "Speed", 10800.0F } } },
												{ "Limit Speed", { { "Enable", false },
																	 { "Min", 4000.0F },
																	 { "Max", 12000.0F } } },
												{ "Randomize Speed", { { "Enable", false },
																		 { "Min", 3000.0F },
																		 { "Max", 12000.0F } } },
												{ "Limit Damage", { { "Enable", false },
																	  { "Min", 10.0F },
																	  { "Max", 1000.0F } } },
												{ "Sound", { { "Change Sound Level", { { "Enable", false },
																						 { "Sound Level", "kSilent" } } } } } } } } }
				};

				switch (SMFRenderer::CreateNewJsonFile(FileName, json_object))
				{
				case FileCreationType::OK:
					{
						data_handler->get_main_json_data_map().insert({ FileName, json_object });
						ui_logger->AddLog(std::format("{} Created {} Successfully", data_handler->get_user_name(), FileName));
						break;
					}
				case FileCreationType::Error:
					ui_logger->AddLog(std::format("Error in Creating File {}", FileName));
					break;
				case FileCreationType::Duplicate:
					ui_logger->AddLog(std::format("Not Creating Duplicate File {}", FileName));
					break;
				}
			}
		}
	}
	SMFRenderer::RenderLogButton();
}

void SMFRenderer::RenderExclusions()
{
	SMFRenderer*             smf_renderer{ GetSingleton() };
	UILogger*                ui_logger{ UILogger::GetSingleton() };
	std::unique_lock SMFLock( smf_renderer->lock_ );

	if( !smf_renderer->exclusion_jsons_.empty() ) {
		for (auto& [name, ej] : smf_renderer->exclusion_jsons_) {
			constexpr auto                 key1{ "AMMO FormID to Exclude" };
			constexpr auto                 key2{ "Mod File(s) to Exclude" };
			const std::string              path1(name + "." + key1);
			const std::string              path2(name + "." + key2);
			ordered_nJson&                 AMMOFormIDToExclude{ ej->at(key1) };
			ordered_nJson&                 AMMOFilesToExclude{ ej->at(key2) };
			if( ImGui::TreeNode( fs::path( name ).filename().string().c_str() ) ) {
				[[maybe_unused]] ImGuiIDGuard const path1id( path1.c_str() );
				if (ImGui::TreeNode(key1)) {
					smf_renderer->RenderJsonEditor(path1, AMMOFormIDToExclude, smf_renderer->hints_for_exclusions_.at(key1));
					DataHandler* data_handler{ DataHandler::GetSingleton() };
					if( data_handler->is_done_ammo_patching() ) {
						auto&              currentIndex{ smf_renderer->key1_index_.at(name) };
						auto&              currentValue{ smf_renderer->key1_values_.at(name) };
						static std::string previewValue;
						previewValue = (data_handler->get_ammo_info().at(currentIndex).at("AmmoName").is_null() || data_handler->get_ammo_info()[currentIndex]["AmmoName"].get<std::string>().empty()) ? data_handler->get_ammo_info()[currentIndex]["AmmoString"].get<std::string>() : std::format("{},{}", data_handler->get_ammo_info()[currentIndex]["AmmoName"].get<std::string>(), data_handler->get_ammo_info()[currentIndex]["AmmoString"].get<std::string>());

						if( ImGui::BeginCombo( key1, previewValue.c_str() ) ) {
							for( size_t ii = 0; ii < data_handler->get_ammo_info().size(); ii++ ) {
								const bool   isSelected = (currentIndex == ii);

								static std::string previewOptionsValue;
								previewOptionsValue = (data_handler->get_ammo_info().at( ii ).at( "AmmoName" ).is_null() || data_handler->get_ammo_info().at( ii ).at( "AmmoName" ).get<std::string>().empty()) ? data_handler->get_ammo_info().at( ii ).at( "AmmoString" ).get<std::string>() : std::format( "{},{}", data_handler->get_ammo_info().at( ii ).at( "AmmoName" ).get<std::string>(), data_handler->get_ammo_info().at( ii ).at( "AmmoString" ).get<std::string>() );

								if( ImGui::Selectable( previewOptionsValue.c_str(), isSelected ) ) {
									currentIndex = ii;
									currentValue = data_handler->get_ammo_info().at(ii).at("AmmoString").get<std::string>();
								}
								if( isSelected ) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						if( ImGui::Button( UI::plus.c_str() ) ) {
							AMMOFormIDToExclude.push_back(currentValue);
							data_handler->get_form_id_array().insert(currentValue);
							ui_logger->AddLog(std::format("{} Added '{}' into '{}' in the section {}", data_handler->get_user_name(), currentValue, name, key1));
							ImGui::TreePop();
							ImGui::TreePop();
							SMFLock.unlock();
							return;
						}
					}
					ImGui::TreePop();
				}
				if( ImGui::TreeNode( key2 ) ) {
					smf_renderer->RenderJsonEditor(path2, AMMOFilesToExclude, smf_renderer->hints_for_exclusions_.at(key2));
					DataHandler* data_handler{ DataHandler::GetSingleton() };
					if( data_handler->is_done_ammo_patching() ) {
						auto& pos_of_2{ smf_renderer->key2_index_.at(name) };
						auto& value_of_2{smf_renderer->key2_values_.at(name)};


						if (ImGui::BeginCombo(key2, data_handler->get_ammo_mod_files().at(pos_of_2).c_str())) {
							for( size_t iii = 0; iii < data_handler->get_ammo_mod_files().size(); iii++ ) {
								bool const isSelected = (pos_of_2 == iii);
								if( ImGui::Selectable( data_handler->get_ammo_mod_files().at( iii ).c_str(), isSelected ) ) {
									pos_of_2 = iii;
									value_of_2 = data_handler->get_ammo_mod_files().at(iii);
								}
								if( isSelected ) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						if( ImGui::Button( UI::plus.c_str() ) ) {
							AMMOFilesToExclude.push_back(value_of_2);
							data_handler->get_tes_file_array().insert(value_of_2);
							ui_logger->AddLog(std::format("{} Added '{}' into '{}' in the section {}", data_handler->get_user_name(), value_of_2, name, key2));
							ImGui::TreePop();
							ImGui::TreePop();
							SMFLock.unlock();
							return;
						}
					}
					ImGui::TreePop();
				}

				DataHandler* data_handler{ DataHandler::GetSingleton() };

				if( ImGui::Button( "Save JSON" ) ) {
					if( SMFRenderer::SaveJsonToFile(name, ej) ) {
						ui_logger->AddLog( std::format( "{} Saved {} Successfully", data_handler->get_user_name(), name ) );
					} else {
						ui_logger->AddLog( std::format( "Failed to Save {}", name ) );
}
				}
				ImGui::SameLine();
				if( ImGui::Button( "Delete File" ) ) {
					if( fs::remove( name ) ) {
						ej.reset();
						smf_renderer->exclusion_jsons_.erase(name);
						smf_renderer->key1_index_.erase(name);
						smf_renderer->key1_values_.erase(name);
						smf_renderer->key2_index_.erase(name);
						smf_renderer->key2_values_.erase(name);
						ui_logger->AddLog( std::format( "{} Deleted {} Successfully", data_handler->get_user_name(), name ) );

						ImGui::TreePop();
						SMFLock.unlock();
						return;
					}
					ui_logger->AddLog( std::format( "Failed to Delete {}", name ) );
				}
				ImGui::TreePop();
			}
		}
	}
	static std::array<char,UI::MAX_SIZE> buffer{ "" };
	if( ImGui::InputText( "Create a Exclusion File?", buffer.data(), std::size(buffer), ImGuiInputTextFlags_EnterReturnsTrue ) ) {
		std::string     FileName = std::format( "Data/SKSE/Plugins/Ammo Patcher/Exclusions/{}.json", buffer.data() );
		auto json_object = std::make_shared<ordered_nJson>( ordered_nJson{ { "AMMO FormID to Exclude", ordered_nJson::array() }, { "Mod File(s) to Exclude", ordered_nJson::array() } } );

		switch( SMFRenderer::CreateNewJsonFile(FileName, json_object) ) {
		case FileCreationType::OK:
			{
				smf_renderer->exclusion_jsons_.insert( { FileName, json_object } );
				smf_renderer->key1_values_.insert({ FileName, "" });
				smf_renderer->key2_values_.insert({ FileName, "" });
				smf_renderer->key1_index_.insert({ FileName, 0_sz });
				smf_renderer->key2_index_.insert({ FileName, 0_sz });
				ui_logger->AddLog( std::format( "{} Created {} Successfully", DataHandler::GetSingleton()->get_user_name(), FileName ) );
				break;
			}
		case FileCreationType::Error:
			ui_logger->AddLog( std::format( "Error in Creating File {}", FileName ) );
			json_object.reset();
			break;
		case FileCreationType::Duplicate:
			ui_logger->AddLog( std::format( "Not Creating Duplicate File {}", FileName ) );
			json_object.reset();
			break;
		}
	}
	SMFLock.unlock();
	ImGui::SameLine();
	ImGui::Button( UI::question.c_str() );

	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "Enter a Name with less than 256 characters.\nThe File will be created in the Appropriate Path with the Appropriate Extension" );
	}
	SMFRenderer::RenderLogButton();
}

void SMFRenderer::GetAllExclusionJsons() {
	constexpr auto FolderPath{ "Data/SKSE/Plugins/Ammo Patcher/Exclusions/" };
	if (fs::exists(FolderPath) && !fs::is_empty(FolderPath)) {
		std::vector<std::thread> threads;
		for (const fs::directory_entry& entry : fs::directory_iterator(FolderPath)) {
			fs::path entry_path = entry.path();

			if( !Utils::resolve_symlink( entry_path ) ) {
				auto string{ fs::absolute(entry_path).generic_string() };
				logger::error( "Skipping entry due to symlink loop: {}", string);
				continue;
			}

			if( fs::is_regular_file( entry_path ) && entry_path.extension() == ".json" ) {
				
				threads.emplace_back([entry_path ,this] {
					std::string EntryPathStr( fs::absolute( entry_path ).generic_string() );

				std::ifstream jFile( entry_path );

				if( !jFile.is_open() ) {
					logger::error( "Failed to open file: {}", EntryPathStr );
					return;
				}
				try {
					std::lock_guard const lock(lock_);
					exclusion_jsons_.insert({ EntryPathStr, std::make_shared<ordered_nJson>(ordered_nJson::parse(jFile)) });
					key1_values_.insert({ EntryPathStr, "" });
					key2_values_.insert({ EntryPathStr, "" });
					key1_index_.insert({ EntryPathStr, 0_sz });
					key2_index_.insert({ EntryPathStr, 0_sz });
				} catch( const ordered_nJson::parse_error& e ) {
					logger::error( "{}", e.what() );
				} });
			}
		}
		for (auto& thread : threads) {
			if(thread.joinable()) {
				thread.join();
			} else {
				util::report_and_fail("Failed to close threads");
			}
		}
	} else {
		logger::info("Didn't find any Exclusion files for UI");
	}
}

void SMFRenderer::RenderJsonEditor(const std::string_view Path, ordered_nJson& jsonObject, ordered_nJson& hint) {
	if( jsonObject.is_object() ) {
		RenderJsonObject(Path, jsonObject, hint);
	} else if( jsonObject.is_array() ) {
		RenderJsonArray(Path, "", jsonObject, hint);
	} else {
		logger::error( "Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array" );
		UILogger::GetSingleton()->AddLog( "[error] Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array" );
	}
}

void SMFRenderer::RenderJsonEditor(const std::string_view Path, const std::shared_ptr<ordered_nJson>& jsonObject, ordered_nJson& hint)
{
	const std::string CurrentPath{std::format("{}: ", Path)};
	if (jsonObject->is_object()) {
		RenderJsonObject(CurrentPath, jsonObject, hint);
	} else if (jsonObject->is_array()) {
		RenderJsonArray(CurrentPath, "", jsonObject, hint);
	} else {
		logger::error("Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array");
		UILogger::GetSingleton()->AddLog("[error] Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array");
	}
}

void SMFRenderer::RenderJsonValue(const std::string_view jsonPath, const std::string_view key, ordered_nJson& value, ordered_nJson& hint) { // NOLINT(*-no-recursion)
	UILogger*         ui_logger{ UILogger::GetSingleton() };
	DataHandler*      data_handler{ DataHandler::GetSingleton() };
	const std::string currentPath{ std::format("{}{}.",jsonPath, key) };
	[[maybe_unused]] const ImGuiIDGuard currentPathIDManager( currentPath.c_str() );  // Using jsonPath as unique ID for ImGui

	if( value.is_object() ) {
		// Render object
		if( ImGui::TreeNode( std::format("{} {{ }}", key).c_str() ) ) {
			RenderJsonObject(currentPath, value, hint);
			ImGui::TreePop();
		}
	} else if( value.is_array() ) {
		// Render array
		if( ImGui::TreeNode( std::format("{} [ ]", key).c_str() ) ) {
			RenderJsonArray(currentPath, key, value, hint);
			ImGui::TreePop();
		}
	} else if( value.is_string() ) {
		// Render string input, combo boxes, etc.
		if( key == "LogLevel" ) {
			int                  currentLogLevel{ spdlog::level::from_str(value) };
			constexpr std::array LogLevels{
				"trace",
				"debug",
				"info",
				"warning",
				"error",
				"critical",
				"off"
			};

			if (ImGui::BeginCombo(key.data(), LogLevels[currentLogLevel])) {
				for (int i = 0; i < LogLevels.size(); i++) {
					const bool isSelected = (currentLogLevel == i);
					if (ImGui::Selectable(LogLevels[i], isSelected)) {
						std::string originalStringValue{ value };
						currentLogLevel = i;
						value = std::string(LogLevels[currentLogLevel]);
						ui_logger->AddLog(std::format(
							"{} Selected LogLevel of key '{}' in Path \"{}\" from '{}' to '{}'",
							data_handler->get_user_name(), key, jsonPath, originalStringValue, LogLevels[currentLogLevel]));
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		} else {
			std::array<char,256>   buffer{ "" };
			strcpy_s(buffer.data(), buffer.size(), value.get<std::string>().c_str());
			if (ImGui::InputText(key.data(), buffer.data(), buffer.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::string originalStringValue{ value };
				std::string buffer_data{ buffer.data() };
				value = buffer_data;
				ui_logger->AddLog( std::format( "{} Changed Value of key '{}' in Path \"{}\" from '{}' to '{}'", data_handler->get_user_name(), key, jsonPath, originalStringValue, buffer_data ) );
			}
			if( ImGui::IsItemHovered() ) {
				ImGui::SetTooltip( "Press Enter to save your input.\nMaximum allowed characters: %zu", buffer.size() - 1_sz );
			}
		}
	} else if( value.is_boolean() ) {
		// Render checkbox for boolean
		bool boolValue{ value };
		if( ImGui::Checkbox( key.data(), &boolValue ) ) {
			bool originalBoolValue{ value };
			value = boolValue;
			ui_logger->AddLog( std::format( "{} Changed Value of Key '{}' in Path \"{}\" from {} to {}", data_handler->get_user_name(), key, jsonPath, originalBoolValue, boolValue ) );
		}
	} else if( value.is_number_integer() ) {
		// Render input for integer
		int intValue{ value.get<int>() };
		if( ImGui::InputInt( key.data(), &intValue, ImGuiInputTextFlags_EnterReturnsTrue ) ) {
			int originalIntValue{ value };
			value = intValue;
			ui_logger->AddLog( std::format( "{} Changed Value of Key '{}' in Path \"{}\" from '{}' to '{}'", data_handler->get_user_name(), key, jsonPath, originalIntValue, intValue ) );
		}
	} else if( value.is_number_float() ) {
		// Render input for float
		float floatValue{ value.get<float>() };
		if( ImGui::InputFloat( key.data(), &floatValue, ImGuiInputTextFlags_EnterReturnsTrue ) ) {
			float originalFloatValue{ value };
			value = floatValue;
			ui_logger->AddLog( std::format( "{} Changed Value of Key '{}' in Path \"{}\" from '{}' to '{}'", data_handler->get_user_name(), key, jsonPath, originalFloatValue, floatValue ) );
		}
	}
	RenderHint( hint );
}

void SMFRenderer::RenderJsonObject(const std::string_view jsonPath, ordered_nJson& jsonObject, ordered_nJson& hint) { // NOLINT(*-no-recursion)
	for( const auto& [key, value] : jsonObject.items() ) {
		const std::string currentPath( std::format("{}{}.", jsonPath, key) );
		RenderJsonValue(currentPath, key, value, hint[key]);
	}
}

void SMFRenderer::RenderJsonObject(const std::string_view jsonPath, const std::shared_ptr<ordered_nJson>& jsonObject, ordered_nJson& hint)
{
	for (const auto& [key, value] : jsonObject->items()) {
		const std::string currentPath( std::format("{}{}.", jsonPath, key) );
		RenderJsonValue(currentPath, key, value, hint[key]);
	}
}

inline void SMFRenderer::RenderHint(const ordered_nJson& hint ) {
	const std::string clue{ hint.is_string() ? hint.get<std::string>() : "" };
	if( !clue.empty() ) {
		ImGui::SameLine();
		ImGui::Button(UI::question.c_str());
		if( ImGui::IsItemHovered() ) {
			ImGui::SetTooltip( clue.c_str() );
		}
	}
}

auto SMFRenderer::CreateNewJsonFile(const std::string_view filename, const ordered_nJson& jsonObject) -> SMFRenderer::FileCreationType {
	if(!fs::exists(filename)) {
		if(std::ofstream file(filename.data()); file.is_open()) {
			file << jsonObject.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
			return FileCreationType::OK;
		}
		logger::error("Unable to open file for writing: {}", filename);
		return FileCreationType::Error;
	}
	logger::error("Don't try to create Duplicates of : {}", filename);
	return FileCreationType::Duplicate;
}

auto SMFRenderer::CreateNewJsonFile(const std::string_view filename, const std::shared_ptr<ordered_nJson>& jsonObject) -> SMFRenderer::FileCreationType {
	if(!fs::exists(filename)) {
		std::ofstream file(filename.data());

		if(file.is_open()) {
			file << jsonObject->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
			return FileCreationType::OK;
		}
		logger::error("Unable to open file for writing: {}", filename);
		return FileCreationType::Error;
	}
	logger::error("Don't try to create Duplicates of : {}", filename);
	return FileCreationType::Duplicate;
}

auto SMFRenderer::SaveJsonToFile(const std::string_view filename, const ordered_nJson& jsonObject) -> bool {
	if(std::ofstream file(filename.data()); file.is_open()) {
		file << jsonObject.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		return true;
	}
	logger::error("Unable to open file for writing: {}", filename);
	return false;
}

auto SMFRenderer::SaveJsonToFile(const std::string_view filename, const std::shared_ptr<ordered_nJson>& jsonObject) -> bool {
	if(std::ofstream file(filename.data()); file.is_open()) {
		file << jsonObject->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		return true;
	}
	logger::error("Unable to open file for writing: {}", filename);
	return false;
}

auto SMFRenderer::SaveJsonToFile(const std::map<std::string, ordered_nJson>::iterator& mapIterator) -> bool
{
	if(std::ofstream file(mapIterator->first.data()); file.is_open()) {
		file << mapIterator->second.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		return true;
	}
	logger::error("Unable to open file for writing: {}", mapIterator->first);
	return false;
}

void SMFRenderer::RenderJsonArray(const std::string_view jsonPath, std::string_view key, ordered_nJson& jsonObject, ordered_nJson& hint) { // NOLINT(*-no-recursion)
	UILogger*          ui_logger{ UILogger::GetSingleton() };
	DataHandler*       data_handler{ DataHandler::GetSingleton() };
	const std::string  currentPath( std::format("{}{}.", jsonPath, key) );
	const ImGuiIDGuard current_path_id_manager( currentPath.c_str() );
	for( size_t i{}; i < jsonObject.size(); ++i ) {
		const std::string Path( std::format("{}[{}].", currentPath, i) );

		const ImGuiIDGuard path_id_manager( Path.c_str() );
		if( hint.is_array() ) {
			RenderJsonValue(Path, "[" + std::to_string(i) + "]", jsonObject[i], hint[i]);
		} else {
			RenderJsonValue(Path, "[" + std::to_string(i) + "]", jsonObject[i], hint);
		}
		ImGui::SameLine();
		if( ImGui::Button( UI::cross.c_str() ) ) {
			if( jsonPath.find( "Mod File(s) to Exclude" ) != std::string::npos ) {
				data_handler->get_tes_file_array().erase( jsonObject[i] );
			}
			if( jsonPath.find( "AMMO FormID to Exclude" ) != std::string::npos ) {
				data_handler->get_form_id_array().erase( jsonObject[i] );
			}
			ui_logger->AddLog( std::format( "{} Decided to Remove {} Value in Key '{}' in Path {}", data_handler->get_user_name(), jsonObject[i].dump(), key, jsonPath ) );
			jsonObject.erase( jsonObject.begin() + static_cast<std::remove_reference_t<decltype(jsonObject)>::difference_type>(i) );
			return;  // exit to avoid further processing as the array is modified
		}
		if( ImGui::IsItemHovered() ) {
			ImGui::SetTooltip( std::format("Removes line [{}]", i).c_str() );
		}
		if( i > 0_sz ) {
			ImGui::SameLine();
			if( ImGui::ArrowButton( "##up", ImGuiDir_Up ) ) {  //Move up
				ui_logger->AddLog( std::format( "{} Decided to move {} Value in Key '{}' in Path {} One Step Above", data_handler->get_user_name(), jsonObject[i].dump(), key, jsonPath ) );
				std::swap( jsonObject[i], jsonObject[i - 1_sz] );
				return;
			}
			if( ImGui::IsItemHovered() ) {
				ImGui::SetTooltip( "Click Me to Move me Up" );
			}
		}
		if( i < (jsonObject.size() - 1_sz) ) {
			ImGui::SameLine();
			if( ImGui::ArrowButton( "##down", ImGuiDir_Down ) ) {  //Move Down
				ui_logger->AddLog( std::format( "{} Decided to move {} Value from Key '{}' in Path {} One Step Below", data_handler->get_user_name(), jsonObject[i].dump(), key, jsonPath ) );
				std::swap( jsonObject[i], jsonObject[i + 1_sz] );
				return;
			}
			if( ImGui::IsItemHovered() ) {
				ImGui::SetTooltip( "Click Me to Move me Down" );
			}
		}
	}

	if( ImGui::Button( UI::plus.c_str() ) ) {
		// add a default value to the array
		jsonObject.push_back( "" );
		ui_logger->AddLog( std::format( "{} Added Value \"\" into Last position of Key '{}' in Path {}", data_handler->get_user_name(), key, jsonPath ) );
		return;
	}
	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "Adds a Empty Line" );
	}
}

void SMFRenderer::RenderJsonArray(const std::string_view jsonPath, std::string_view key, const std::shared_ptr<ordered_nJson>& jsonObject, ordered_nJson& hint)
{
	UILogger*         ui_logger{ UILogger::GetSingleton() };
	DataHandler*      data_handler{ DataHandler::GetSingleton() };
	const std::string currentPath{ std::format("{}{}.",jsonPath, key) };
	[[maybe_unused]] const ImGuiIDGuard current_path_id_manager(currentPath.c_str());
	for (size_t i{}; i < jsonObject->size(); ++i) {
		const std::string Path(std::format("{}[{}].", currentPath, i));

		[[maybe_unused]] const ImGuiIDGuard path_id_manager(Path.c_str());
		RenderJsonValue(Path, "[" + std::to_string(i) + "]", jsonObject->at(i), hint.is_array() ? hint[i] : hint);
		ImGui::SameLine();
		if (ImGui::Button(UI::cross.c_str())) {
			if (jsonPath.find("Mod File(s) to Exclude") != std::string::npos) {
				data_handler->get_tes_file_array().erase(jsonObject->at(i));
			}
			if (jsonPath.find("AMMO FormID to Exclude") != std::string::npos) {
				data_handler->get_form_id_array().erase(jsonObject->at(i));
			}
			ui_logger->AddLog(std::format("{} Decided to Remove {} Value in Key '{}' in Path {}", data_handler->get_user_name(), jsonObject->at(i).dump(), key, jsonPath));
			jsonObject->erase(jsonObject->begin() + static_cast<std::remove_reference_t<decltype(*jsonObject)>::difference_type>(i));
			return;  // exit to avoid further processing as the array is modified
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(std::format("Removes line [{}]", i).c_str());
		}
		if (i > 0_sz) {
			ImGui::SameLine();
			if (ImGui::ArrowButton("##up", ImGuiDir_Up)) {  //Move up
				ui_logger->AddLog(std::format("{} Decided to move {} Value in Key '{}' in Path {} One Step Above", data_handler->get_user_name(), jsonObject->at(i).dump(), key, jsonPath));
				std::swap(jsonObject->at(i), jsonObject->at(i - 1_sz));
				return;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Click Me to Move me Up");
			}
		}
		if (i < (jsonObject->size() - 1_sz)) {
			ImGui::SameLine();
			if (ImGui::ArrowButton("##down", ImGuiDir_Down)) {  //Move Down
				ui_logger->AddLog(std::format("{} Decided to move {} Value from Key '{}' in Path {} One Step Below", data_handler->get_user_name(), jsonObject->at(i).dump(), key, jsonPath));
				std::swap(jsonObject->at(i), jsonObject->at(i - 1_sz));
				return;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Click Me to Move me Down");
			}
		}
	}

	if (ImGui::Button(UI::plus.c_str())) {
		// add a default value to the array
		jsonObject->push_back(""s);
		ui_logger->AddLog(std::format("{} Added Value \"\" into Last position of Key '{}' in Path {}", data_handler->get_user_name(), key, jsonPath));
		return;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Adds a Empty Line");
	}
}

inline auto UILogger::GetSingleton() -> UILogger* {
	static UILogger Singleton;
	return std::addressof( Singleton );
}

void UILogger::AddLog( const std::string& message ) {
	const std::string logEntry{ std::format("[{}] {}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()}, message) };
	_logs.push_back(logEntry);

	size_t oldSize = _lineOffsets.empty() ? 0 : _lineOffsets.back();
	for( const auto part : logEntry ) {
		oldSize++;
		if( part == '\n' ) {
			_lineOffsets.push_back( oldSize );
		}
	}
	_scrollToBottom = true;
}

inline void UILogger::ClearLogs() {
	std::lock_guard const lock(_lock);
	_logs.clear();
	_lineOffsets.clear();
	_scrollToBottom = false;
}

inline auto UILogger::GetLogs() -> std::vector<std::string>& {
	std::lock_guard const lock( _lock );
	return _logs;
}

inline auto UILogger::GetLatestLog() const -> std::string {
	std::lock_guard const lock( _lock );

	if( _logs.empty() ) {
		return "Default(No Logs available)";
	}

	const std::string formattedLog = std::format( "[{}] {}", _logs.size(), _logs.back() );
	return formattedLog;
}

auto UILogger::ShouldScrollToBottom() const -> bool {
	std::lock_guard const lock( _lock );
	return _scrollToBottom;
}

void UILogger::ResetScrollToBottom() {
	std::lock_guard const lock( _lock );
	_scrollToBottom = false;
}

auto UILogger::GetFilteredLogs( const std::string& filter ) const -> std::vector<std::string> {
	std::lock_guard const lock( _lock );
	std::vector<std::string>    filteredLogs;
	for( const auto& log : _logs ) {
		if( log.find( filter ) != std::string::npos ) {
			filteredLogs.push_back( log );
		}
	}
	return filteredLogs;
}
