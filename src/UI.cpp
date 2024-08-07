#include "DataHandler.h"
#include "UI.h"
#include "utils.h"

SMFRenderer* SMFRenderer::GetSingleton() {
	static SMFRenderer Singleton;
	return std::addressof( Singleton );
}

void SMFRenderer::Register() {
	if( !SKSEMenuFramework::IsInstalled() ) {
		logger::error( "Unable to Register For SKSEMenuFramework, Please install SKSEMenuFramework to configure the Json Files(if you want to)" );
		return;
	}

	SMFRenderer* s{ SMFRenderer::GetSingleton() };

	SKSEMenuFramework::SetSection( "Ammo Patcher" );

	std::ifstream FileContainingHintsForMain( "Data/SKSE/Plugins/Ammo Patcher/Hints/MainHint.json" );
	s->_HintsForMain << FileContainingHintsForMain;

	SKSEMenuFramework::AddSectionItem( "Main", SMFRenderer::RenderMain );

	std::ifstream FileContainingHintsForExclusions( "Data/SKSE/Plugins/Ammo Patcher/Hints/ExclusionHint.json" );
	s->_HintsForExclusions << FileContainingHintsForExclusions;

	s->_LogWindow = SKSEMenuFramework::AddWindow( s->RenderLogWindow );

	SKSEMenuFramework::AddSectionItem( "Exclusions", SMFRenderer::RenderExclusions );

	/*SKSEMenuFramework::AddSectionItem("Debug", SMFRenderer::RenderDebug);*/
	logger::info( "Registered For SKSEMenuFramework" );
}

inline void __stdcall SMFRenderer::RenderLogWindow() {
	ImGui::SetNextWindowSize( ImVec2( 900, 900 ), ImGuiCond_FirstUseEver );
	static std::string name{ std::format("Log Window##CustomLogger{}", SKSE::PluginDeclaration::GetSingleton()->GetName()) };

	ImGui::Begin( name.c_str(), nullptr, ImGuiWindowFlags_None );

	CustomLogger* c = CustomLogger::GetSingleton();

	static char filterInput[128] = "";
	ImGui::InputText( "Filter", filterInput, ((int)(sizeof( filterInput ) / sizeof( *filterInput ))) );

	if( ImGui::Button( "Clear Logs" ) ) {
		c->ClearLogs();
	}
	ImGui::SameLine();
	if( ImGui::Button( "Close Window" ) ) {
		_LogWindow->IsOpen = false;
	}

	ImGui::Separator();
	ImGui::BeginChild( "LogScroll" );
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 1 ) );

	auto logs = filterInput[0] ? c->GetFilteredLogs( filterInput ) : c->GetLogs();
	for( size_t i = 0; i < logs.size(); i++ ) {
		ImGui::TextUnformatted( std::format( "[{}] {}", i + 1sz, logs[i] ).c_str() );
	}

	if( c->ShouldScrollToBottom() ) {
		ImGui::SetScrollHereY( 1.0f );
		c->ResetScrollToBottom();
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}

void __stdcall SMFRenderer::RenderDebug()
{
	if (ImGui::Button("Quit")) {
		REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_FAILURE);
	}
}

void SMFRenderer::RenderMain() {
	SMFRenderer* s{ SMFRenderer::GetSingleton() };

	std::lock_guard<std::mutex> lock( s->_lock );
	constexpr const char* CurrentPath{ "Main." };

	ImGui::PushID( CurrentPath );

	DataHandler*       d{ DataHandler::GetSingleton() };
	CustomLogger*      c{ CustomLogger::GetSingleton() };
	static std::string Selected{ fs::path(d->GetUnmodifiableSelectedPreset()).filename().string() };
	
	if (ImGui::BeginCombo("Selected Preset", Selected.c_str())) {
		for (const auto& [k,_] : d->GetUnmodifiableMainJsonDataMap()) {
			bool isSelected = (Selected == fs::path(k).filename().string());
			if (ImGui::Selectable(fs::path(k).filename().string().c_str(), isSelected)) {
				Selected = fs::path(k).filename().string();
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("Save to File")) {
		std::ofstream m(std::format("Data/SKSE/Plugins/Ammo Patcher/{}_Default.json", SKSE::PluginDeclaration::GetSingleton()->GetName()));

		ordered_nJson n{ { "Load", Selected } };
		if (m.is_open()) {
			m << n;
		}
	}
	if(ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Clicking this will save \n{\n\t\"Load\": \"%s\"\n}\n into Data/SKSE/Plugins/Ammo Patcher/%s_Default.json", Selected.c_str(), SKSE::PluginDeclaration::GetSingleton()->GetName().data());
	}
	{
		static char buffer[256]{ "" };
		if (ImGui::InputText("Create a Preset?", buffer, ((int)(sizeof(buffer) / sizeof(*buffer))), ImGuiInputTextFlags_EnterReturnsTrue)) {
			std::string FileName = fs::absolute(std::format("Data/SKSE/Plugins/Ammo Patcher/Presets/{}.json", buffer)).string();
			if (!d->GetUnmodifiableMainJsonDataMap().contains(FileName)) {
				auto j = std::make_shared<ordered_nJson>(ordered_nJson{
					{ "Logging", { { "LogLevel", "info" } } },
					{ "User Details", { { "Username", "User" } } },
					{ "AMMO", { { "Infinite AMMO", { { "Player", false },
													   { "Teammate", false } } },
								  { "Arrow", { { "Enable Arrow Patch", true },
												 { "Change Gravity", { { "Enable", true },
																		 { "Gravity", 0.0 } } },
												 { "Change Speed", { { "Enable", true },
																	   { "Speed", 9000.0 } } },
												 { "Limit Speed", { { "Enable", false },
																	  { "Min", 3000.0 },
																	  { "Max", 12000.0 } } },
												 { "Randomize Speed", { { "Enable", false },
																		  { "Min", 3000.0 },
																		  { "Max", 12000.0 } } },
												 { "Limit Damage", { { "Enable", false },
																	   { "Min", 10.0 },
																	   { "Max", 1000.0 } } },
												 { "Sound", { { "Change Sound Level", { { "Enable", false },
																						  { "Sound Level", "kSilent" } } } } } } },
								  { "Bolt", { { "Enable Bolt Patch", true },
												{ "Change Gravity", { { "Enable", true },
																		{ "Gravity", 0.0 } } },
												{ "Change Speed", { { "Enable", true },
																	  { "Speed", 10800.0 } } },
												{ "Limit Speed", { { "Enable", false },
																	 { "Min", 4000.0 },
																	 { "Max", 12000.0 } } },
												{ "Randomize Speed", { { "Enable", false },
																		 { "Min", 3000.0 },
																		 { "Max", 12000.0 } } },
												{ "Limit Damage", { { "Enable", false },
																	  { "Min", 10.0 },
																	  { "Max", 1000.0 } } },
												{ "Sound", { { "Change Sound Level", { { "Enable", false },
																						 { "Sound Level", "kSilent" } } } } } } } } }
				});

				switch (s->CreateNewJsonFile(FileName, j))
				{
				case FileCreationType::OK:
					{
						d->GetModifiableMainJsonDataMap().insert({ FileName, j });
						c->AddLog(std::format("{} Created {} Successfully", d->GetUsername(), FileName));
						break;
					}
				case FileCreationType::Error:
					c->AddLog(std::format("Error in Creating File {}", FileName));
					j.reset();
					break;
				case FileCreationType::Duplicate:
					c->AddLog(std::format("Not Creating Duplicate File {}", FileName));
					j.reset();
					break;
				}
			}
		}
	}

	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 10.0);

	if (ImGui::BeginCombo("Presets", fs::path(d->GetUnmodifiableSelectedPreset()).filename().string().c_str())) {
		for (const auto& [k, _] : d->GetUnmodifiableMainJsonDataMap()) {
			bool isSelected = (d->GetUnmodifiableSelectedPreset() == k);
			if (ImGui::Selectable(fs::path(k).filename().string().c_str(), isSelected)) {
				d->SetSelectedPreset(k);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Button(UI::question.c_str());
	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip("All the presets available for this Mod. Below Details will get loaded Accordingly");
	}
	s->RenderJsonEditor( CurrentPath, d->GetModifiableMainJsonData(), s->_HintsForMain );

	if( ImGui::Button( "Save JSON" ) ) {
		if (s->SaveJsonToFile(d->GetUnmodifiableMainJsonPair()))
			c->AddLog(std::format("{} Saved {} Successfully", d->GetUsername(), d->GetUnmodifiableMainJsonPair()->first));
		else
			c->AddLog(std::format("Failed to Save {}", d->GetUnmodifiableMainJsonPair()->first));
	}
	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "You Must Click This to SAVE any Changes" );
	}
	if( d->GetDoneAmmoPatching() ) {
		ImGui::SameLine();
		if( ImGui::Button( "Revert Changes" ) ) {
			d->RevertToDefault();
		}
		if( ImGui::IsItemHovered() ) {
			ImGui::SetTooltip( "This Button will Revert ANY changes made to all AMMO Records Safely" );
		}
		ImGui::SameLine();
		if( ImGui::Button( "RePatch Changes" ) ) {
			if (s->SaveJsonToFile(d->GetUnmodifiableMainJsonPair())) {
				c->AddLog(std::format("{} Saved {} Successfully", d->GetUsername(), d->GetUnmodifiableMainJsonPair()->first));
			} else {
				c->AddLog(std::format("Failed to Save {}", d->GetUnmodifiableMainJsonPair()->first));
			}
			d->ChangeLogLevel( d->GetUnmodifiableMainJsonData()->at( "Logging" ).at( "LogLevel" ) );
			d->ProcessMainJson();
			d->LogDataHandlerContents();
			d->PatchAMMO();
			c->AddLog( "Repatched AMMO" );
		}
		if( ImGui::IsItemHovered() ) {
			ImGui::SetTooltip( "This Button will Patch AMMO Records According to The Main Json File and and the Exclusion Files.\nFirst It will Save the file.\nThen Process it.\nThen Log The Details and finally Patch The Records" );
		}
	}
	static ImVec2 LoggingButtonSize;
	ImGui::GetContentRegionAvail( std::addressof( LoggingButtonSize ) );
	LoggingButtonSize.y = 0;
	if( ImGui::Button( CustomLogger::GetSingleton()->GetLatestLog().c_str(), LoggingButtonSize ) ) {
		_LogWindow->IsOpen = true;
	}
	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "Click this to open Log Window" );
	}

	ImGui::PopID();
}

void SMFRenderer::RenderExclusions()
{
	SMFRenderer*                 s{ GetSingleton() };
	CustomLogger*                c{ CustomLogger::GetSingleton() };
	std::unique_lock<std::mutex> SMFLock( s->_lock );

	if( !s->_ExclusionJsons.empty() ) {
		for (auto& [name, ej] : s->_ExclusionJsons) {
			constexpr static const char*   key1{ "AMMO FormID to Exclude" };
			constexpr static const char*   key2{ "Mod File(s) to Exclude" };
			std::string                    path1(name + "." + key1);
			std::string                    path2(name + "." + key2);
			ordered_nJson&                 AMMOFormIDToExclude{ ej->at(key1) };
			ordered_nJson&                 AMMOFilesToExclude{ ej->at(key2) };
			if( ImGui::TreeNode( fs::path( name ).filename().string().c_str() ) ) {
				ImGui::PushID( path1.c_str() );
				if (ImGui::TreeNode(key1)) {
					s->RenderJsonEditor( path1, AMMOFormIDToExclude, s->_HintsForExclusions.at( key1 ) );
					DataHandler* d{ DataHandler::GetSingleton() };
					if( d->GetDoneAmmoPatching() ) {
						auto&              currentIndex{ s->_Key1Index.at(name) };
						auto&              currentValue{ s->_Key1Values.at(name) };
						static std::string previewValue;
						previewValue = (d->GetUnmodifiableAmmoInfo().at(currentIndex).at("AmmoName").is_null() || d->GetUnmodifiableAmmoInfo()[currentIndex]["AmmoName"].get<std::string>().empty()) ? d->GetUnmodifiableAmmoInfo()[currentIndex]["AmmoString"].get<std::string>() : std::format("{},{}", d->GetUnmodifiableAmmoInfo()[currentIndex]["AmmoName"].get<std::string>(), d->GetUnmodifiableAmmoInfo()[currentIndex]["AmmoString"].get<std::string>());

						if( ImGui::BeginCombo( key1, previewValue.c_str() ) ) {
							for( size_t ii = 0; ii < d->GetUnmodifiableAmmoInfo().size(); ii++ ) {
								bool        isSelected = (currentIndex == ii);

								static std::string previewOptionsValue;
								previewOptionsValue = (d->GetUnmodifiableAmmoInfo().at( ii ).at( "AmmoName" ).is_null() || d->GetUnmodifiableAmmoInfo().at( ii ).at( "AmmoName" ).get<std::string>().empty()) ? d->GetUnmodifiableAmmoInfo().at( ii ).at( "AmmoString" ).get<std::string>() : std::format( "{},{}", d->GetUnmodifiableAmmoInfo().at( ii ).at( "AmmoName" ).get<std::string>(), d->GetUnmodifiableAmmoInfo().at( ii ).at( "AmmoString" ).get<std::string>() );

								if( ImGui::Selectable( previewOptionsValue.c_str(), isSelected ) ) {
									currentIndex = ii;
									currentValue = d->GetUnmodifiableAmmoInfo().at(ii).at("AmmoString").get<std::string>();
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
							d->GetModifiableFormIDArray().insert(currentValue);
							c->AddLog(std::format("{} Added '{}' into '{}' in the section {}", d->GetUsername(), currentValue, name, key1));
							ImGui::TreePop();
							ImGui::PopID();
							ImGui::TreePop();
							SMFLock.unlock();
							return;
						}
					}
					ImGui::TreePop();
				}
				if( ImGui::TreeNode( key2 ) ) {
					s->RenderJsonEditor( path1, AMMOFilesToExclude, s->_HintsForExclusions.at( key2 ) );
					DataHandler* d = DataHandler::GetSingleton();
					if( d->GetDoneAmmoPatching() ) {
						auto& sjdnfo{ s->_Key2Index.at(name) };
						auto& sjdbsbdj{s->_Key2Values.at(name)};


						if (ImGui::BeginCombo(key2, d->GetUnmodifiableAmmoModFiles().at(sjdnfo).c_str())) {
							for( size_t iii = 0; iii < d->GetUnmodifiableAmmoModFiles().size(); iii++ ) {
								bool isSelected = (sjdnfo == iii);
								if( ImGui::Selectable( d->GetUnmodifiableAmmoModFiles().at( iii ).c_str(), isSelected ) ) {
									sjdnfo = iii;
									sjdbsbdj = d->GetUnmodifiableAmmoModFiles().at(iii);
								}
								if( isSelected ) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						if( ImGui::Button( UI::plus.c_str() ) ) {
							AMMOFilesToExclude.push_back(sjdbsbdj);
							d->GetModifiableTESFileArray().insert(sjdbsbdj);
							c->AddLog(std::format("{} Added '{}' into '{}' in the section {}", d->GetUsername(), sjdbsbdj, name, key2));
							ImGui::TreePop();
							ImGui::PopID();
							ImGui::TreePop();
							SMFLock.unlock();
							return;
						}
					}
					ImGui::TreePop();
				}

				DataHandler* d{ DataHandler::GetSingleton() };

				if( ImGui::Button( "Save JSON" ) ) {
					if( s->SaveJsonToFile( name, ej ) )
						c->AddLog( std::format( "{} Saved {} Successfully", d->GetUsername(), name ) );
					else
						c->AddLog( std::format( "Failed to Save {}", name ) );
				}
				ImGui::SameLine();
				if( ImGui::Button( "Delete File" ) ) {
					if( fs::remove( name ) ) {
						ej.reset();
						s->_ExclusionJsons.erase(name);
						s->_Key1Index.erase(name);
						s->_Key1Values.erase(name);
						s->_Key2Index.erase(name);
						s->_Key2Values.erase(name);
						c->AddLog( std::format( "{} Deleted {} Successfully", d->GetUsername(), name ) );

						ImGui::PopID();
						ImGui::TreePop();
						SMFLock.unlock();
						return;
					} else {
						c->AddLog( std::format( "Failed to Delete {}", name ) );
					}
				}
				ImGui::PopID();
				ImGui::TreePop();
			}
		}
	}
	static char buffer[256]{ "" };
	if( ImGui::InputText( "Create a Exclusion File?", buffer, ((int)(sizeof( buffer ) / sizeof( *buffer ))), ImGuiInputTextFlags_EnterReturnsTrue ) ) {
		std::string     FileName = std::format( "Data/SKSE/Plugins/Ammo Patcher/Exclusions/{}.json", buffer );
		std::shared_ptr j = std::make_shared<ordered_nJson>( ordered_nJson{ { "AMMO FormID to Exclude", ordered_nJson::array() }, { "Mod File(s) to Exclude", ordered_nJson::array() } } );

		switch( s->CreateNewJsonFile( FileName, j ) ) {
		case FileCreationType::OK:
			{
				s->_ExclusionJsons.insert( { FileName, j } );
				s->_Key1Values.insert({ FileName, "" });
				s->_Key2Values.insert({ FileName, "" });
				s->_Key1Index.insert({ FileName, 0sz });
				s->_Key2Index.insert({ FileName, 0sz });
				DataHandler* d{ DataHandler::GetSingleton() };
				c->AddLog( std::format( "{} Created {} Successfully", d->GetUsername(), FileName ) );
				break;
			}
		case FileCreationType::Error:
			c->AddLog( std::format( "Error in Creating File {}", FileName ) );
			j.reset();
			break;
		case FileCreationType::Duplicate:
			c->AddLog( std::format( "Not Creating Duplicate File {}", FileName ) );
			j.reset();
			break;
		}
	}
	SMFLock.unlock();
	ImGui::SameLine();
	ImGui::Button( UI::question.c_str() );

	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "Enter a Name with less than 256 characters.\nThe File will be created in the Appropriate Path with the Appropriate Extension" );
	}
	static ImVec2 LoggingButtonSize;
	ImGui::GetContentRegionAvail( std::addressof( LoggingButtonSize ) );
	LoggingButtonSize.y = 0;
	if( ImGui::Button( CustomLogger::GetSingleton()->GetLatestLog().c_str(), LoggingButtonSize ) ) {
		_LogWindow->IsOpen = true;
	}
}

void SMFRenderer::GetAllExclusionJsons() {
	constexpr const char* FolderPath{ "Data/SKSE/Plugins/Ammo Patcher/Exclusions/" };
	if (fs::exists(FolderPath) && !fs::is_empty(FolderPath)) {
		std::vector<std::thread> threads;
		for (const fs::directory_entry& entry : fs::directory_iterator(FolderPath)) {
			fs::path entry_path = entry.path();

			if( !InlineUtils::resolve_symlink( entry_path, 10 ) ) {
				auto s{ fs::absolute(entry_path).generic_string() };
				logger::error( "Skipping entry due to symlink loop: {}", s);
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
					{
						std::lock_guard<std::mutex> lock(_lock);
						_ExclusionJsons.insert({ EntryPathStr, std::make_shared<ordered_nJson>(ordered_nJson::parse(jFile)) });
						_Key1Values.insert({ EntryPathStr, "" });
						_Key2Values.insert({ EntryPathStr, "" });
						_Key1Index.insert({ EntryPathStr, 0sz });
						_Key2Index.insert({ EntryPathStr, 0sz });
						
					}
				} catch( const ordered_nJson::parse_error& e ) {
					logger::error( "{}", e.what() );
				} });
			}
		}
		for (auto& thread : threads) {
			thread.join();
		}
	} else {
		logger::info("Didn't find any Exclusion files for UI");
	}
}

void SMFRenderer::RenderJsonEditor( const std::string_view Path, ordered_nJson& jsonObject, ordered_nJson& hint ) {
	if( jsonObject.is_object() ) {
		RenderJsonObject( Path, jsonObject, hint );
	} else if( jsonObject.is_array() ) {
		RenderJsonArray( Path, "", jsonObject, hint );
	} else {
		logger::error( "Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array" );
		CustomLogger::GetSingleton()->AddLog( "[error] Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array" );
	}
}

void SMFRenderer::RenderJsonEditor(const std::string_view Path, std::shared_ptr<ordered_nJson> jsonObject, ordered_nJson& hint)
{
	if (jsonObject->is_object()) {
		RenderJsonObject(Path, jsonObject, hint);
	} else if (jsonObject->is_array()) {
		RenderJsonArray(Path, "", jsonObject, hint);
	} else {
		logger::error("Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array");
		CustomLogger::GetSingleton()->AddLog("[error] Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array");
	}
}

void SMFRenderer::RenderJsonValue( const std::string_view jsonPath, const std::string& key, ordered_nJson& value, ordered_nJson& hint ) {
	CustomLogger* c{ CustomLogger::GetSingleton() };
	DataHandler*  d{ DataHandler::GetSingleton() };
	std::string   currentPath( jsonPath );
	currentPath += key + ".";
	ImGui::PushID( currentPath.c_str() );  // Using jsonPath as unique ID for ImGui

	if( value.is_object() ) {
		// Render object
		if( ImGui::TreeNode( (key + " { }").c_str() ) ) {
			RenderJsonObject( currentPath, value, hint );
			ImGui::TreePop();
		}
	} else if( value.is_array() ) {
		// Render array
		if( ImGui::TreeNode( (key + " [ ]").c_str() ) ) {
			RenderJsonArray( currentPath, key, value, hint );
			ImGui::TreePop();
		}
	} else if( value.is_string() ) {
		// Render string input, combo boxes, etc.
		if( key == "LogLevel" ) {
			std::string_view                     originalStringValue{ value };
			int                                  currentLogLevel{ spdlog::level::from_str(value) };
			constexpr std::array<const char*, 7> LogLevels{
				"trace",
				"debug",
				"info",
				"warning",
				"error",
				"critical",
				"off"
			};

			if (ImGui::BeginCombo(key.c_str(), LogLevels[currentLogLevel])) {
				for (int i = 0; i < LogLevels.size(); i++) {
					const bool isSelected = (currentLogLevel == i);
					if (ImGui::Selectable(LogLevels[i], isSelected)) {
						currentLogLevel = i;
						value = std::string(LogLevels[currentLogLevel]);
						c->AddLog(std::format(
							"{} Selected LogLevel of key '{}' in Path {} from '{}' to '{}'",
							d->GetUsername(), key, jsonPath, originalStringValue, LogLevels[currentLogLevel]));
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		} else {
			char   buffer[256];
			constexpr auto size{ sizeof(buffer) };
			strcpy_s(buffer, size, value.get<std::string>().c_str());
			if (ImGui::InputText(key.c_str(), buffer, size, ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::string_view originalStringValue( value );
				value = std::string( buffer );
				c->AddLog( std::format( "{} Changed Value of key '{}' in Path {} from '{}' to '{}'", d->GetUsername(), key, jsonPath, originalStringValue, buffer ) );
			}
			if( ImGui::IsItemHovered() ) {
				ImGui::SetTooltip( "Press Enter to save your input.\nMaximum allowed characters: %zu", size - 1sz );
			}
		}
	} else if( value.is_boolean() ) {
		// Render checkbox for boolean
		bool boolValue{ value };
		if( ImGui::Checkbox( key.c_str(), &boolValue ) ) {
			bool originalBoolValue{ value };
			value = boolValue;
			c->AddLog( std::format( "{} Changed Value of Key '{}' in Path {} from {} to {}", d->GetUsername(), key, jsonPath, originalBoolValue, boolValue ) );
		}
	} else if( value.is_number_integer() ) {
		// Render input for integer
		int intValue{ value.get<int>() };
		if( ImGui::InputInt( key.c_str(), &intValue, ImGuiInputTextFlags_EnterReturnsTrue ) ) {
			int originalIntValue{ value };
			value = intValue;
			c->AddLog( std::format( "{} Changed Value of Key '{}' in Path from '{}' to '{}'", d->GetUsername(), key, jsonPath, originalIntValue, intValue ) );
		}
	} else if( value.is_number_float() ) {
		// Render input for float
		float floatValue{ value.get<float>() };
		if( ImGui::InputFloat( key.c_str(), &floatValue, ImGuiInputTextFlags_EnterReturnsTrue ) ) {
			float originalFloatValue{ value };
			value = floatValue;
			c->AddLog( std::format( "{} Changed Value of Key '{}' in Path from '{}' to '{}'", d->GetUsername(), key, jsonPath, originalFloatValue, floatValue ) );
		}
	}

	RenderHint( hint );
	ImGui::PopID();
}

void SMFRenderer::RenderJsonObject( const std::string_view jsonPath, ordered_nJson& j, ordered_nJson& hint ) {
	for( auto& [key, value] : j.items() ) {
		std::string currentPath( jsonPath );
		currentPath += key + ".";
		RenderJsonValue( currentPath, key, value, hint[key] );
	}
}

void SMFRenderer::RenderJsonObject(const std::string_view jsonPath, std::shared_ptr<ordered_nJson> j, ordered_nJson& hint)
{
	for (auto& [key, value] : j->items()) {
		std::string currentPath(jsonPath);
		currentPath += key + ".";
		RenderJsonValue(currentPath, key, value, hint[key]);
	}
}

void SMFRenderer::RenderHint( ordered_nJson& hint ) {
	std::string clue = hint.is_string() ? hint.get<std::string>() : "";
	if( !clue.empty() ) {
		ImGui::SameLine();
		if( ImGui::Button( UI::question.c_str() ) ) { }
		if( ImGui::IsItemHovered() ) {
			ImGui::SetTooltip( clue.c_str() );
		}
	}
}

SMFRenderer::FileCreationType SMFRenderer::CreateNewJsonFile( const std::string_view filename, const ordered_nJson& jsonObject ) {
	if( !fs::exists( filename ) ) {
		std::ofstream file( filename.data() );

		if( file.is_open() ) {
			file << jsonObject.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
			return FileCreationType::OK;
		} else {
			logger::error( "Unable to open file for writing: {}", filename );
			return FileCreationType::Error;
		}
	} else {
		logger::error( "Don't try to create Duplicates of : {}", filename );
		return FileCreationType::Duplicate;
	}
}

SMFRenderer::FileCreationType SMFRenderer::CreateNewJsonFile( const std::string_view filename, std::shared_ptr<ordered_nJson>& jsonObject ) {
	if( !fs::exists( filename ) ) {
		std::ofstream file( filename.data() );

		if( file.is_open() ) {
			file << jsonObject->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
			return FileCreationType::OK;
		} else {
			logger::error( "Unable to open file for writing: {}", filename );
			return FileCreationType::Error;
		}
	} else {
		logger::error( "Don't try to create Duplicates of : {}", filename );
		return FileCreationType::Duplicate;
	}
}

bool SMFRenderer::SaveJsonToFile( const std::string_view filename, const ordered_nJson& jsonObject ) {
	std::ofstream file( filename.data() );

	if( file.is_open() ) {
		file << jsonObject.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		return true;
	} else {
		logger::error( "Unable to open file for writing: {}", filename );
	}
	return false;
}

bool SMFRenderer::SaveJsonToFile( const std::string_view filename, std::shared_ptr<ordered_nJson>& jsonObject ) {
	std::ofstream file( filename.data() );

	if( file.is_open() ) {
		file << jsonObject->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		return true;
	} else {
		logger::error( "Unable to open file for writing: {}", filename );
	}
	return false;
}

bool SMFRenderer::SaveJsonToFile(std::map<std::string, std::shared_ptr<ordered_nJson>>::iterator p)
{
	std::ofstream file(p->first.data());

	if (file.is_open()) {
		file << p->second->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		return true;
	} else {
		logger::error("Unable to open file for writing: {}", p->first);
	}
	return false;
}

void SMFRenderer::RenderJsonArray( const std::string_view jsonPath, std::string_view key, ordered_nJson& j, ordered_nJson& hint ) {
	CustomLogger* c = CustomLogger::GetSingleton();
	DataHandler* d = DataHandler::GetSingleton();
	std::string                  currentPath( jsonPath );
	currentPath += key;
	currentPath += ".";
	ImGui::PushID( currentPath.c_str() );
	for( size_t i = 0; i < j.size(); ++i ) {
		std::string Path( currentPath );
		Path += "[" + std::to_string( i ) + "].";

		ImGui::PushID( Path.c_str() );
		if( hint.is_array() ) {
			RenderJsonValue( Path, "[" + std::to_string( i ) + "]", j[i], hint[i] );
		} else {
			RenderJsonValue( Path, "[" + std::to_string( i ) + "]", j[i], hint );
		}
		ImGui::SameLine();
		if( ImGui::Button( UI::x.c_str() ) ) {
			if( jsonPath.find( "Mod File(s) to Exclude" ) != std::string::npos ) {
				d->GetModifiableTESFileArray().erase( j[i] );
			}
			if( jsonPath.find( "AMMO FormID to Exclude" ) != std::string::npos ) {
				d->GetModifiableFormIDArray().erase( j[i] );
			}
			c->AddLog( std::format( "{} Decided to Remove {} Value in Key '{}' in Path {}", d->GetUsername(), j[i].dump(), key, jsonPath ) );
			j.erase( j.begin() + i );
			ImGui::PopID();
			ImGui::PopID();
			return;  // exit to avoid further processing as the array is modified
		}
		if( ImGui::IsItemHovered() ) {
			ImGui::SetTooltip( std::string( "Removes line [" + std::to_string( i ) + "]" ).c_str() );
		}
		if( i > 0sz ) {
			ImGui::SameLine();
			if( ImGui::ArrowButton( "##up", ImGuiDir_Up ) ) {  //Move up
				c->AddLog( std::format( "{} Decided to move {} Value in Key '{}' in Path {} One Step Above", d->GetUsername(), j[i].dump(), key, jsonPath ) );
				std::swap( j[i], j[i - 1sz] );
				ImGui::PopID();
				ImGui::PopID();
				return;
			}
			if( ImGui::IsItemHovered() ) {
				ImGui::SetTooltip( "Click Me to Move me Up" );
			}
		}
		if( i < (j.size() - 1sz) ) {
			ImGui::SameLine();
			if( ImGui::ArrowButton( "##down", ImGuiDir_Down ) ) {  //Move Down
				c->AddLog( std::format( "{} Decided to move {} Value from Key '{}' in Path {} One Step Below", d->GetUsername(), j[i].dump(), key, jsonPath ) );
				std::swap( j[i], j[i + 1sz] );
				ImGui::PopID();
				ImGui::PopID();
				return;
			}
			if( ImGui::IsItemHovered() ) {
				ImGui::SetTooltip( "Click Me to Move me Down" );
			}
		}
		ImGui::PopID();
	}

	if( ImGui::Button( UI::plus.c_str() ) ) {
		// add a default value to the array
		j.push_back( "" );
		c->AddLog( std::format( "{} Added Value \"\" into Last position of Key '{}' in Path {}", d->GetUsername(), key, jsonPath ) );
		ImGui::PopID();
		ImGui::PopID();
		return;
	}
	if( ImGui::IsItemHovered() ) {
		ImGui::SetTooltip( "Adds a Empty Line" );
	}
	ImGui::PopID();
}

void SMFRenderer::RenderJsonArray(const std::string_view jsonPath, std::string_view key, std::shared_ptr<ordered_nJson> j, ordered_nJson& hint)
{
	CustomLogger* c = CustomLogger::GetSingleton();
	DataHandler*  d = DataHandler::GetSingleton();
	std::string   currentPath(jsonPath);
	currentPath += key;
	currentPath += ".";
	ImGui::PushID(currentPath.c_str());
	for (size_t i = 0; i < j->size(); ++i) {
		std::string Path(currentPath);
		Path += "[" + std::to_string(i) + "].";

		ImGui::PushID(Path.c_str());
		if (hint.is_array()) {
			RenderJsonValue(Path, "[" + std::to_string(i) + "]", j->at(i), hint[i]);
		} else {
			RenderJsonValue(Path, "[" + std::to_string(i) + "]", j->at(i), hint);
		}
		ImGui::SameLine();
		if (ImGui::Button(UI::x.c_str())) {
			if (jsonPath.find("Mod File(s) to Exclude") != std::string::npos) {
				d->GetModifiableTESFileArray().erase(j->at(i));
			}
			if (jsonPath.find("AMMO FormID to Exclude") != std::string::npos) {
				d->GetModifiableFormIDArray().erase(j->at(i));
			}
			c->AddLog(std::format("{} Decided to Remove {} Value in Key '{}' in Path {}", d->GetUsername(), j->at(i).dump(), key, jsonPath));
			j->erase(j->begin() + i);
			ImGui::PopID();
			ImGui::PopID();
			return;  // exit to avoid further processing as the array is modified
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(std::string("Removes line [" + std::to_string(i) + "]").c_str());
		}
		if (i > 0sz) {
			ImGui::SameLine();
			if (ImGui::ArrowButton("##up", ImGuiDir_Up)) {  //Move up
				c->AddLog(std::format("{} Decided to move {} Value in Key '{}' in Path {} One Step Above", d->GetUsername(), j->at(i).dump(), key, jsonPath));
				std::swap(j->at(i), j->at(i - 1sz));
				ImGui::PopID();
				ImGui::PopID();
				return;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Click Me to Move me Up");
			}
		}
		if (i < (j->size() - 1sz)) {
			ImGui::SameLine();
			if (ImGui::ArrowButton("##down", ImGuiDir_Down)) {  //Move Down
				c->AddLog(std::format("{} Decided to move {} Value from Key '{}' in Path {} One Step Below", d->GetUsername(), j->at(i).dump(), key, jsonPath));
				std::swap(j->at(i), j->at(i - 1sz));
				ImGui::PopID();
				ImGui::PopID();
				return;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Click Me to Move me Down");
			}
		}
		ImGui::PopID();
	}

	if (ImGui::Button(UI::plus.c_str())) {
		// add a default value to the array
		j->push_back("");
		c->AddLog(std::format("{} Added Value \"\" into Last position of Key '{}' in Path {}", d->GetUsername(), key, jsonPath));
		ImGui::PopID();
		ImGui::PopID();
		return;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Adds a Empty Line");
	}
	ImGui::PopID();
}

inline CustomLogger* CustomLogger::GetSingleton() {
	static CustomLogger Singleton;
	return std::addressof( Singleton );
}

void CustomLogger::AddLog( const std::string& message ) {
	std::time_t             now_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
	std::ostringstream      oss;

	oss << "[" << std::put_time( std::localtime( &now_time ), "%Y-%m-%d %H:%M:%S" ) << "] " << message;
	_logs.push_back( oss.str() );

	size_t oldSize = _lineOffsets.empty() ? 0 : _lineOffsets.back();
	for( char c : oss.str() ) {
		oldSize++;
		if( c == '\n' ) {
			_lineOffsets.push_back( oldSize );
		}
	}
	_scrollToBottom = true;
}

inline void CustomLogger::ClearLogs() {
	std::lock_guard<std::mutex> lock( _lock );
	_logs.clear();
	_lineOffsets.clear();
	_scrollToBottom = false;
}

inline const std::vector<std::string>& CustomLogger::GetLogs() const {
	std::lock_guard<std::mutex> lock( _lock );
	return _logs;
}

inline const std::string CustomLogger::GetLatestLog() const {
	std::lock_guard<std::mutex> lock( _lock );

	if( _logs.empty() ) {
		return "Default(No Logs available)";
	}

	std::string formattedLog = std::format( "[{}] {}", _logs.size(), _logs.back() );
	return formattedLog;
}

bool CustomLogger::ShouldScrollToBottom() const {
	std::lock_guard<std::mutex> lock( _lock );
	return _scrollToBottom;
}

void CustomLogger::ResetScrollToBottom() {
	std::lock_guard<std::mutex> lock( _lock );
	_scrollToBottom = false;
}

std::vector<std::string> CustomLogger::GetFilteredLogs( const std::string& filter ) const {
	std::lock_guard<std::mutex> lock( _lock );
	std::vector<std::string>    filteredLogs;
	for( const auto& log : _logs ) {
		if( log.find( filter ) != std::string::npos ) {
			filteredLogs.push_back( log );
		}
	}
	return filteredLogs;
}
