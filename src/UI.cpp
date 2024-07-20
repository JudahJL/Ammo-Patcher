#include "UI.h"
#include "DataHandler.h"

void UI::Register()
{
	if (!SKSEMenuFramework::IsInstalled()) {
		logger::error("Unable to Register For SKSEMenuFramework, Please install SKSEMenuFramework to configure the Json Files(if you want to)");
		return;
	}

	CustomLogger*                c = CustomLogger::GetSingleton();
	SMFRenderer*                 s = GetSingleton();
	std::unique_lock<std::mutex> lock(s->_lock);

	std::ifstream FileContainingHintsForMain("Data/SKSE/Plugins/Ammo Patcher Hints/MainHint.json");
	if (FileContainingHintsForMain.is_open()) {
		FileContainingHintsForMain >> s->_HintsForMain;
	} else {
		c->AddLog("Failed to open MainHint.json");
		lock.unlock();
		return;
	}

	
	std::ifstream FileContainingHintsForExclusions("Data/SKSE/Plugins/Ammo Patcher Hints/ExclusionHint.json");
	if (FileContainingHintsForExclusions.is_open()) {
		FileContainingHintsForExclusions >> s->_HintsForExclusions;
	} else {
		c->AddLog("Failed to open ExclusionHint.json");
		lock.unlock();
		return;
	}

	s->GetAllExclusionJsons();

	SKSEMenuFramework::SetSection("Ammo Patcher");
	SKSEMenuFramework::AddSectionItem("Main", SMFRenderer::RenderMain);
	SKSEMenuFramework::AddSectionItem("Exclusions", SMFRenderer::RenderExclusions);

	s->_LogWindow = SKSEMenuFramework::AddWindow(s->RenderLogWindow);

	logger::info("Registered For SKSEMenuFramework");
}

void __stdcall UI::AP::Render()
{
	DataHandler* d = DataHandler::GetSingleton();
	if (d->_Done) {
		jsoning::RenderJsonEditor(d->_JsonData);
	}
	ImGui::SameLine();
	if (ImGui::Button("Close Window")) {
		_LogWindow->IsOpen = false;
		DataHandler*                d = DataHandler::GetSingleton();
		std::lock_guard<std::mutex> lock(d->_lock);
	}

	ImGui::Separator();
	ImGui::BeginChild("LogScroll");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

	auto logs = filterInput[0] ? c->GetFilteredLogs(filterInput) : c->GetLogs();
	for (size_t i = 0; i < logs.size(); i++) {
		ImGui::TextUnformatted(std::format("[{}] {}", i + 1sz, logs[i]).c_str());
	}

	if (c->ShouldScrollToBottom()) {
		ImGui::SetScrollHereY(1.0f);
		c->ResetScrollToBottom();
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}

void SMFRenderer::RenderMain()
{
	SMFRenderer* s = GetSingleton();

	std::lock_guard<std::mutex> lock(s->_lock);
	constexpr const char*       CurrentPath{ "Main." };

	std::string                 MainFileName(std::format("Data/SKSE/Plugins/{}.json", SKSE::PluginDeclaration::GetSingleton()->GetName()));

	ImGui::PushID(CurrentPath);

	DataHandler* d = DataHandler::GetSingleton();

	CustomLogger* c = CustomLogger::GetSingleton();

	{
		std::lock_guard<std::mutex> dlock(d->_lock);

		s->RenderJsonEditor(CurrentPath, d->_JsonData, s->_HintsForMain);
	}

	if (ImGui::Button("Save JSON")) {
		if (s->SaveJsonToFile(MainFileName, d->_JsonData))
			c->AddLog(std::format("{} Saved {} Successfully", d->_UserName, MainFileName));
		else
			c->AddLog(std::format("Failed to Save {}", MainFileName));
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("You Must Click This to SAVE any Changes");
	}
	{
		if( d->_DoneAmmoPatching ) {
			ImGui::SameLine();
			if (ImGui::Button("Revert Changes")) {
				d->RevertToDefault();
			}
			if(ImGui::IsItemHovered()){
				ImGui::SetTooltip("This Button will Revert ANY changes made to all AMMO Records Safely");
			}
			ImGui::SameLine();
			if (ImGui::Button("RePatch Changes")) {
				if (s->SaveJsonToFile(MainFileName, d->_JsonData)){
					c->AddLog(std::format("{} Saved {} Successfully", d->_UserName, MainFileName));
				} else {
					c->AddLog(std::format("Failed to Save {}", MainFileName));
				}
				d->ReloadLoggingIfNecessary(d->_JsonData["Logging"]["LogLevel"].get<std::string>());
				d->ProcessMainJson();
				d->LogDataHandlerContents();
				d->PatchAMMO();
				c->AddLog("Repatched AMMO");
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("This Button will Patch AMMO Records According to The Main Json File and and the Exclusion Files.\nFirst It will Save the file.\nThen Process it.\nThen Log The Details and finally Patch The Records");
			}
		}
	}
	ImGui::Separator();
	static ImVec2 LoggingButtonSize;
	ImGui::GetContentRegionAvail(std::addressof(LoggingButtonSize));
	LoggingButtonSize.y = 0;
	if (ImGui::Button(CustomLogger::GetSingleton()->GetLatestLog().c_str(), LoggingButtonSize)) {
		_LogWindow->IsOpen = true;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Click this to open Log Window");
	}

void __stdcall UI::Exclusions::Render()
{
	SMFRenderer*                 s = GetSingleton();
	CustomLogger*                c = CustomLogger::GetSingleton();
	std::unique_lock<std::mutex> SMFLock(s->_lock);

	if (!s->_ExclusionJsons.empty()) {
		size_t ExclusionJsonsSize{ s->_ExclusionJsons.size() };
		for (size_t index = 0sz; index < ExclusionJsonsSize; index++) {
			std::string                  name(s->_ExclusionJsons[index].first);
			std::shared_ptr<ordered_nJson>       ej{ s->_ExclusionJsons[index].second };
			constexpr static const char* key1{ "AMMO FormID to Exclude" };
			constexpr static const char* key2{ "Mod File(s) to Exclude" };
			std::string                  path1(name + "." + key1);
			std::string                  path2(name + "." + key2);
			ordered_nJson&                       AMMOFormIDToExclude{ ej->at(key1) };
			ordered_nJson&                       ModFilesToExclude{ ej->at(key2) };
			if (ImGui::TreeNode(fs::path(name).filename().string().c_str())) {
				ImGui::PushID(path1.c_str());
				if (ImGui::TreeNode(key1)) {
					s->RenderJsonEditor(path1, AMMOFormIDToExclude, s->_HintsForExclusions);
					DataHandler*                 d = DataHandler::GetSingleton();
					std::unique_lock<std::mutex> dlock(d->_lock);
					if (d->_DoneAmmoPatching) {
						static std::vector<std::string> a_value_for_FormID(ExclusionJsonsSize, d->_AmmoInfo[0]["AmmoString"].get<std::string>());  //used to store the selected value of _AmmoInfo of the currently traversed json
						static std::vector<size_t>      CurrentIterationForFormID(ExclusionJsonsSize, 0sz);                                        //used to store the selected index of _AmmoInfo of the currently traversed json

						if (a_value_for_FormID.size() != ExclusionJsonsSize) {
							a_value_for_FormID.resize(ExclusionJsonsSize, d->_AmmoInfo[0]["AmmoString"].get<std::string>());
						}
						if (CurrentIterationForFormID.size() != ExclusionJsonsSize) {
							CurrentIterationForFormID.resize(ExclusionJsonsSize, 0sz);
						}

						InlineUtils::limit(CurrentIterationForFormID[index], 0sz, d->_AmmoInfo.size());
						static std::string previewValue;
						previewValue = (d->_AmmoInfo[CurrentIterationForFormID[index]]["AmmoName"].is_null() || d->_AmmoInfo[CurrentIterationForFormID[index]]["AmmoName"].get<std::string>().empty()) ? d->_AmmoInfo[CurrentIterationForFormID[index]]["AmmoString"].get<std::string>() : std::format("{},{}", d->_AmmoInfo[CurrentIterationForFormID[index]]["AmmoName"].get<std::string>(), d->_AmmoInfo[CurrentIterationForFormID[index]]["AmmoString"].get<std::string>());

						if (ImGui::BeginCombo(key1, previewValue.c_str())) {
							for (size_t ii = 0; ii < d->_AmmoInfo.size(); ii++) {
								bool        isSelected = (CurrentIterationForFormID[index] == ii);
								static std::string previewOptionsValue;
								previewOptionsValue = (d->_AmmoInfo[ii]["AmmoName"].is_null() || d->_AmmoInfo[ii]["AmmoName"].get<std::string>().empty()) ? d->_AmmoInfo[ii]["AmmoString"].get<std::string>() : std::format("{},{}", d->_AmmoInfo[ii]["AmmoName"].get<std::string>(), d->_AmmoInfo[ii]["AmmoString"].get<std::string>());

								if (ImGui::Selectable(previewOptionsValue.c_str(), isSelected)) {
									CurrentIterationForFormID[index] = ii;
									a_value_for_FormID[index] = d->_AmmoInfo[CurrentIterationForFormID[index]]["AmmoString"].get<std::string>();
								}
								if (isSelected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						if (ImGui::Button(UI::plus.c_str())) {
							AMMOFormIDToExclude.push_back(a_value_for_FormID[index]);
							d->_FormIDArray.insert(a_value_for_FormID[index]);
							c->AddLog(std::format("{} Added '{}' into '{}' in the section {}", d->_UserName, a_value_for_FormID[index], name, key1));
							ImGui::TreePop();
							ImGui::PopID();
							ImGui::TreePop();
							SMFLock.unlock();
							dlock.unlock();
							return;
						}
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode(key2)) {
					s->RenderJsonEditor(path1, ModFilesToExclude, s->_HintsForExclusions);
					DataHandler*                 d = DataHandler::GetSingleton();
					std::unique_lock<std::mutex> ddlock(d->_lock);
					if (d->_DoneAmmoPatching) {
						static std::vector<std::string> a_value(ExclusionJsonsSize, d->_AmmoModFiles[0]);
						static std::vector<size_t>      CurrentIteration(ExclusionJsonsSize, 0sz);

						if (a_value.size() != ExclusionJsonsSize) {
							a_value.resize(ExclusionJsonsSize, d->_AmmoModFiles[0]);
						}
						if (CurrentIteration.size() != ExclusionJsonsSize) {
							CurrentIteration.resize(ExclusionJsonsSize, 0sz);
						}

						InlineUtils::limit(CurrentIteration[index], 0sz, d->_AmmoModFiles.size());

						if (ImGui::BeginCombo(key2, d->_AmmoModFiles[CurrentIteration[index]].c_str())) {
							for (size_t iii = 0; iii < d->_AmmoModFiles.size(); iii++) {
								bool isSelected = (CurrentIteration[index] == iii);
								if (ImGui::Selectable(d->_AmmoModFiles[iii].c_str(), isSelected)) {
									CurrentIteration[index] = iii;
									a_value[index] = d->_AmmoModFiles[CurrentIteration[index]];
								}
								if (isSelected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						if (ImGui::Button(UI::plus.c_str())) {
							ModFilesToExclude.push_back(a_value[index]);
							d->_TESFileArray.insert(a_value[index]);
							c->AddLog(std::format("{} Added '{}' into '{}' in the section {}", d->_UserName, a_value[index], name, key2));
							ImGui::TreePop();
							ImGui::PopID();
							ImGui::TreePop();
							SMFLock.unlock();
							ddlock.unlock();
							return;
						}
					}
					ImGui::TreePop();
				}

				if (ImGui::Button("Save JSON")) {
					DataHandler*                d = DataHandler::GetSingleton();
					std::lock_guard<std::mutex> lock(d->_lock);
					if (s->SaveJsonToFile(name, ej))
						c->AddLog(std::format("{} Saved {} Successfully", d->_UserName, name));
					else
						c->AddLog(std::format("Failed to Save {}", name));
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete File")) {
					if (fs::remove(name)) {
						DataHandler*                 d = DataHandler::GetSingleton();
						std::unique_lock<std::mutex> lock(d->_lock);
						ej.reset();
						s->_ExclusionJsons.erase(s->_ExclusionJsons.begin() + index);
						c->AddLog(std::format("{} Deleted {} Successfully", d->_UserName, name));

						ImGui::PopID();
						ImGui::TreePop();
						lock.unlock();
						SMFLock.unlock();
						return;
					} else {
						c->AddLog(std::format("Failed to Delete {}", name));
					}
				}
				ImGui::PopID();
				ImGui::TreePop();
			}
		}
	}
	static char buffer[256] = "";
	if (ImGui::InputText("Create a Exclusion File?", buffer, ((int)(sizeof(buffer) / sizeof(*buffer))), ImGuiInputTextFlags_EnterReturnsTrue)) {
		std::string                    FileName = std::format("Data/SKSE/Plugins/Ammo Patcher/{}.json", buffer);
		std::shared_ptr<ordered_nJson> j = std::make_shared<ordered_nJson>(ordered_nJson{ { "AMMO FormID to Exclude", ordered_nJson::array() }, { "Mod File(s) to Exclude", ordered_nJson::array() } });

		switch (s->CreateNewJsonFile(FileName, j)) {
		case FileCreationType::OK:
			{
				s->_ExclusionJsons.push_back({ FileName, j });
				DataHandler*                d = DataHandler::GetSingleton();
				std::lock_guard<std::mutex> lock(d->_lock);
				c->AddLog(std::format("{} Created {} Successfully", d->_UserName, FileName));
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
	SMFLock.unlock();
	ImGui::SameLine();
	ImGui::Button(UI::question.c_str());

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enter a Name with less than 256 characters.\nThe File will be created in the Appropriate Path with the Appropriate Extension");
	}
	static ImVec2 LoggingButtonSize;
	ImGui::GetContentRegionAvail(std::addressof(LoggingButtonSize));
	LoggingButtonSize.y = 0;
	if (ImGui::Button(CustomLogger::GetSingleton()->GetLatestLog().c_str(), LoggingButtonSize)) {
		_LogWindow->IsOpen = true;
	}
}

void SMFRenderer::GetAllExclusionJsons()
{
	DataHandler* d = DataHandler::GetSingleton();
	if (fs::exists(d->_FolderPath) && !fs::is_empty(d->_FolderPath)) {
		for (const fs::directory_entry& entry : fs::directory_iterator(d->_FolderPath)) {
			fs::path entry_path = entry.path();

			if (!InlineUtils::resolve_symlink(entry_path, 10)) {
				logger::error("Skipping entry due to symlink loop: {}", fs::absolute(entry_path).generic_string());
				continue;
			}

			if (fs::is_regular_file(entry_path) && entry_path.extension() == ".json") {
				std::string   EntryPathStr(fs::absolute(entry_path).generic_string());

				std::ifstream jFile(entry_path);

				if (!jFile.is_open()) {
					logger::error("Failed to open file: {}", EntryPathStr);
					continue;
				}

				try {
					std::shared_ptr<ordered_nJson> MergeData = std::make_shared<ordered_nJson>(ordered_nJson::parse(jFile));
					_ExclusionJsons.push_back({ EntryPathStr, MergeData });
				} catch (const ordered_nJson::exception& e) {
					logger::error("{}", e.what());
				}
			}
		}
	}
}

void SMFRenderer::RenderJsonEditor(const std::string_view Path, ordered_nJson& jsonObject, ordered_nJson& hint)
{
	if (jsonObject.is_object()) {
		RenderJsonObject(Path, jsonObject, hint);
	} else if (jsonObject.is_array()) {
		RenderJsonArray(Path, "", jsonObject, hint);
	} else {
		logger::error("Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array");
		CustomLogger::GetSingleton()->AddLog("[error] Weird Error Detected at SMFRenderer::RenderJsonEditor. Given json object is neither object nor array");
	}
}

void SMFRenderer::RenderJsonValue(const std::string_view jsonPath, const std::string& key, ordered_nJson& value, ordered_nJson& hint)
{
	CustomLogger* c = CustomLogger::GetSingleton();
	DataHandler*  d = DataHandler::GetSingleton();
	std::string   currentPath(jsonPath);
	currentPath += key + ".";
	ImGui::PushID(currentPath.c_str());  // Using jsonPath as unique ID for ImGui

	if (value.is_object()) {
		if (ImGui::TreeNode((key + " { }").c_str())) {
			RenderJsonObject(value);
			ImGui::TreePop();
		}
	} else if (value.is_array()) {
		if (ImGui::TreeNode((key + " [ ]").c_str())) {
			RenderJsonArray(value);
			ImGui::TreePop();
		}
	} else if (value.is_string()) {
		// Render string input, combo boxes, etc.
		if (key == "LogLevel") {
			std::string           originalStringValue = value;
			static int            currentLogLevel = 2;
			constexpr const char* LogLevels[7] = { "trace", "debug", "info", "warn", "err", "critical", "off" };
			if (ImGui::Combo(key.c_str(), &currentLogLevel, LogLevels, ((int)(sizeof(LogLevels) / sizeof(*LogLevels))))) {
				value = std::string(LogLevels[currentLogLevel]);
				c->AddLog(std::format("{} Selected LogLevel of key '{}' in Path {} from '{}' to '{}'", d->_UserName, key, jsonPath, originalStringValue, LogLevels[currentLogLevel]));
			}
		} else if (key == "Sound Level") {
			enum AmmoType
			{
				Arrow,
				Bolt,
				None
			};
			AmmoType a = AmmoType::None;
			if (currentPath.find(".Arrow.") != std::string::npos) {
				a = AmmoType::Arrow;
			} else if (currentPath.find(".Bolt.") != std::string::npos) {
				a = AmmoType::Bolt;
			}
			static int            CurrentSoundLevel[2] = { 2, 2 };
			constexpr const char* SoundLevel[5] = { "kLoud", "kNormal", "kSilent", "kVeryLoud", "kQuiet" };
			switch (a) {
			case AmmoType::Arrow:
			case AmmoType::Bolt:
				if (ImGui::Combo(key.c_str(), &CurrentSoundLevel[a], SoundLevel, ((int)(sizeof(SoundLevel) / sizeof(*SoundLevel))))) {
					std::string originalStringValue = value;
					value = std::string(SoundLevel[CurrentSoundLevel[a]]);
					c->AddLog(std::format("{} Selected SoundLevel of key '{}' in Path {} from '{}' to '{}'", d->_UserName, key, jsonPath, originalStringValue, SoundLevel[CurrentSoundLevel[a]]));
				}
				break;
			default:
				break;
			}
		} else {
			char   buffer[256];
			size_t size = sizeof(buffer);
			strncpy(buffer, value.get<std::string>().c_str(), size);
			buffer[size - 1] = 0;
			if (ImGui::InputText(key.c_str(), buffer, size, ImGuiInputTextFlags_EnterReturnsTrue)) {
				std::string originalStringValue = value;
				value = std::string(buffer);
				c->AddLog(std::format("{} Changed Value of key '{}' in Path {} from '{}' to '{}'", d->_UserName, key, jsonPath, originalStringValue, buffer));
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Press Enter to save your input.\nMaximum allowed characters: %zu", size - 1sz);
			}
		}
	} else if (value.is_boolean()) {
		// Render checkbox for boolean
		bool boolValue = value;
		if (ImGui::Checkbox(key.c_str(), &boolValue)) {
			bool originalBoolValue = value;
			value = boolValue;
			c->AddLog(std::format("{} Changed Value of Key '{}' in Path {} from {} to {}", d->_UserName, key, jsonPath, originalBoolValue, boolValue));
		}
	} else if (value.is_number_integer()) {
		int intValue = value.get<int>();
		if (ImGui::InputInt(key.c_str(), &intValue, ImGuiInputTextFlags_EnterReturnsTrue)) {
			int originalIntValue = value;
			value = intValue;
			c->AddLog(std::format("{} Changed Value of Key '{}' in Path from '{}' to '{}'", d->_UserName, key, jsonPath, originalIntValue, intValue));
		}
	} else if (value.is_number_float()) {
		float floatValue = value.get<float>();
		if (ImGui::InputFloat(key.c_str(), &floatValue, ImGuiInputTextFlags_EnterReturnsTrue)) {
			float originalFloatValue = value;
			value = floatValue;
			c->AddLog(std::format("{} Changed Value of Key '{}' in Path from '{}' to '{}'", d->_UserName, key, jsonPath, originalFloatValue, floatValue));
		}
	}
	ImGui::PopID();
}

void SMFRenderer::RenderJsonObject(const std::string_view jsonPath, ordered_nJson& j, ordered_nJson& hint)
{
	for (auto& [key, value] : j.items()) {
		std::string currentPath(jsonPath);
		currentPath += key + ".";
		RenderJsonValue(currentPath, key, value, hint[key]);
	}
}

void SMFRenderer::RenderHint(ordered_nJson& hint)
{
	std::string clue = hint.is_string() ? hint.get<std::string>() : "";
	if (!clue.empty()) {
		ImGui::SameLine();
		if (ImGui::Button(UI::question.c_str())) {}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(clue.c_str());
		}
	}
}

SMFRenderer::FileCreationType SMFRenderer::CreateNewJsonFile(const std::string_view filename, const ordered_nJson& jsonObject)
{
	if (!fs::exists(filename)) {
		std::ofstream file(filename.data());

		if (file.is_open()) {
			file << jsonObject.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
			file.close();
			return FileCreationType::OK;
		} else {
			logger::error("Unable to open file for writing: {}", filename);
			return FileCreationType::Error;
		}
	} else {
		logger::error("Don't try to create Duplicates of : {}", filename);
		return FileCreationType::Duplicate;
	}
}

SMFRenderer::FileCreationType SMFRenderer::CreateNewJsonFile(const std::string_view filename, std::shared_ptr<ordered_nJson> jsonObject)
{
	if (!fs::exists(filename)) {
		std::ofstream file(filename.data());

		if (file.is_open()) {
			file << jsonObject->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
			file.close();
			return FileCreationType::OK;
		} else {
			logger::error("Unable to open file for writing: {}", filename);
			return FileCreationType::Error;
		}
	} else {
		logger::error("Don't try to create Duplicates of : {}", filename);
		return FileCreationType::Duplicate;
	}
}

bool SMFRenderer::SaveJsonToFile(const std::string_view filename, const ordered_nJson& jsonObject)
{
	std::ofstream file(filename.data());

	if (file.is_open()) {
		file << jsonObject.dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		file.close();
		return true;
	} else {
		logger::error("Unable to open file for writing: {}", filename);
	}
	return false;
}

bool SMFRenderer::SaveJsonToFile(const std::string_view filename, std::shared_ptr<ordered_nJson> jsonObject)
{
	std::ofstream file(filename.data());

	if (file.is_open()) {
		file << jsonObject->dump();  // no need to Dump JSON with indentation of 4 spaces because user will(should) not manually edit json
		file.close();
		return true;
	} else {
		logger::error("Unable to open file for writing: {}", filename);
	}
}

void SMFRenderer::RenderJsonArray(const std::string_view jsonPath, std::string_view key, ordered_nJson& j, ordered_nJson& hint)
{
	CustomLogger*                c = CustomLogger::GetSingleton();
	DataHandler*                 d = DataHandler::GetSingleton();
	std::unique_lock<std::mutex> dlock(d->_lock);
	std::string                  currentPath(jsonPath);
	currentPath += key;
	currentPath += ".";
	ImGui::PushID(currentPath.c_str());
	for (size_t i = 0; i < j.size(); ++i) {
		ImGui::PushID(static_cast<int>(i));
		RenderJsonValue("[" + std::to_string(i) + "]", j[i]);
		ImGui::SameLine();
		if (ImGui::Button(UI::x.c_str())) {
			if (jsonPath.find("Mod File(s) to Exclude") != std::string::npos) {
				d->_TESFileArray.erase(j[i]);
			}
			if (jsonPath.find("AMMO FormID to Exclude") != std::string::npos) {
				d->_FormIDArray.erase(j[i]);
			}
			c->AddLog(std::format("{} Decided to Remove {} Value in Key '{}' in Path {}", d->_UserName, j[i].dump(), key, jsonPath));
			j.erase(j.begin() + i);
			ImGui::PopID();
			ImGui::PopID();
			dlock.unlock();
			return;  // exit to avoid further processing as the array is modified
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(std::string("Removes line [" + std::to_string(i) + "]").c_str());
		}
		if (i > 0sz) {
			ImGui::SameLine();
			if (ImGui::ArrowButton("##up", ImGuiDir_Up)) {  //Move up
				c->AddLog(std::format("{} Decided to move {} Value in Key '{}' in Path {} One Step Above", d->_UserName, j[i].dump(), key, jsonPath));
				std::swap(j[i], j[i - 1sz]);
				ImGui::PopID();
				ImGui::PopID();
				dlock.unlock();
				return;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Click Me to Move me Up");
			}
		}
		if (i < (j.size() - 1sz)) {
			ImGui::SameLine();
			if (ImGui::ArrowButton("##down", ImGuiDir_Down)) {  //Move Down
				c->AddLog(std::format("{} Decided to move {} Value from Key '{}' in Path {} One Step Below", d->_UserName, j[i].dump(), key, jsonPath));
				std::swap(j[i], j[i + 1sz]);
				ImGui::PopID();
				ImGui::PopID();
				dlock.unlock();
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
		j.push_back("");
		c->AddLog(std::format("{} Added Value \"\" into Last position of Key '{}' in Path {}", d->_UserName, key, jsonPath));
		ImGui::PopID();
		ImGui::PopID();
		dlock.unlock();
		return;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Adds a Empty Line");
	}
	ImGui::PopID();
}

inline CustomLogger* CustomLogger::GetSingleton()
{
	static CustomLogger Singleton;
	return std::addressof(Singleton);
}

void CustomLogger::AddLog(const std::string& message)
{
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::time_t             now_time = std::chrono::system_clock::to_time_t(now);
	std::tm                 local_tm = *std::localtime(&now_time);
	std::ostringstream      oss;

	oss << "[" << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << "] " << message;
	_logs.push_back(oss.str());

	size_t oldSize = _lineOffsets.empty() ? 0 : _lineOffsets.back();
	for (char c : oss.str()) {
		oldSize++;
		if (c == '\n') {
			_lineOffsets.push_back(oldSize);
		}
	}
	_scrollToBottom = true;
}

inline void CustomLogger::ClearLogs()
{
	std::lock_guard<std::mutex> lock(_lock);
	_logs.clear();
	_lineOffsets.clear();
	_scrollToBottom = false;
}

inline const std::vector<std::string>& CustomLogger::GetLogs() const
{
	std::lock_guard<std::mutex> lock(_lock);
	return _logs;
}

inline const std::string CustomLogger::GetLatestLog() const
{
	std::lock_guard<std::mutex> lock(_lock);

	constexpr const char* emptyLog = "Default(No Logs available)";
	if (_logs.empty()) {
		return emptyLog;
	}

	std::string formattedLog = std::format("[{}] {}", _logs.size(), _logs.back());
	return formattedLog;
}

bool CustomLogger::ShouldScrollToBottom() const
{
	std::lock_guard<std::mutex> lock(_lock);
	return _scrollToBottom;
}

void jsoning::RenderJsonEditor(nJson& jsonObject)
{
	RenderJsonObject(jsonObject);
}

void jsoning::SaveJsonToFile(const std::string& filename, const nJson& jsonObject)
{
	std::ofstream file(filename);
	if (file.is_open()) {
		file << jsonObject.dump(4);  // Dump JSON with indentation of 4 spaces
		file.close();
	} else {
		logger::error("Unable to open file for writing: {}", filename);
	}
}
