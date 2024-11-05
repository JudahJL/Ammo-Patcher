#include "SkyPatcher/ammo.h"
#include "SkyPatcher/dirent.h"

namespace AMMO
{

	line_content create_patch_instruction_ammo(const std::string& line)
	{
		line_content l;

		//// extract ammos
		//std::regex ammos_regex("filterByAmmos\\s*=([^:]+)", regex::icase);
		//std::smatch ammos_match;
		//std::regex_search(line, ammos_match, ammos_regex);
		//std::vector<std::string> ammos;
		//if (ammos_match.empty() || ammos_match[1].str().empty()) {
		//	//empty
		//} else {
		//	std::string ammos_str = ammos_match[1];
		//	std::regex ammos_list_regex("[^,]+[ ]*[|][ ]*[a-zA-Z0-9]{1,8}", regex::icase);
		//	std::sregex_iterator ammos_iterator(ammos_str.begin(), ammos_str.end(), ammos_list_regex);
		//	std::sregex_iterator ammos_end;
		//	while (ammos_iterator != ammos_end) {
		//		std::string tempVar = (*ammos_iterator)[0].str();
		//		tempVar.erase(tempVar.begin(), std::find_if_not(tempVar.begin(), tempVar.end(), ::isspace));
		//		tempVar.erase(std::find_if_not(tempVar.rbegin(), tempVar.rend(), ::isspace).base(), tempVar.end());
		//		//logger::info(FMT_STRING("Race: {}"), race);
		//		if (tempVar != "none") {
		//			ammos.push_back(tempVar);
		//		}
		//		++ammos_iterator;
		//	}
		//	l.ammo = ammos;
		//}

		extractData(line, "filterByAmmos\\s*=([^:]+)", l.ammo);
		extractData(line, "filterByAmmosExcluded\\s*=([^:]+)", l.objectExcluded);

		//// extract weight
		//std::regex weightLess_regex("filterByWeightLessThan\\s*=([^:]+)", regex::icase);
		//std::smatch weightLessmatch;
		//std::regex_search(line, weightLessmatch, weightLess_regex);
		//// extract the value after the equals sign
		//if (weightLessmatch.empty() || weightLessmatch[1].str().empty()) {
		//	l.weightLessThan = "none";
		//} else {
		//	std::string value = weightLessmatch[1].str();
		//	value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
		//	l.weightLessThan = value;
		//}

		extractValueString(line, "filterByWeightLessThan\\s*=([^:]+)", l.weightLessThan);

		//// extract keywords
		//std::regex  keywords_regex("filterByKeywords\\s*=([^:]+)", regex::icase);
		//std::smatch keywords_match;
		//std::regex_search(line, keywords_match, keywords_regex);
		//std::vector<std::string> keywords;
		//if (keywords_match.empty() || keywords_match[1].str().empty()) {
		//	//empty
		//} else {
		//	std::string          keywords_str = keywords_match[1];
		//	std::regex           keywords_list_regex("[^,]+[ ]*[|][ ]*[a-zA-Z0-9]{1,8}", regex::icase);
		//	std::sregex_iterator keywords_iterator(keywords_str.begin(), keywords_str.end(), keywords_list_regex);
		//	std::sregex_iterator keywords_end;
		//	while (keywords_iterator != keywords_end) {
		//		std::string keyword = (*keywords_iterator)[0].str();
		//		keyword.erase(keyword.begin(), std::find_if_not(keyword.begin(), keyword.end(), ::isspace));
		//		keyword.erase(std::find_if_not(keyword.rbegin(), keyword.rend(), ::isspace).base(), keyword.end());
		//		if (keyword != "none") {
		//			keywords.push_back(keyword);
		//		}
		//		++keywords_iterator;
		//	}
		//	l.keywords = keywords;
		//}

		extractData(line, "filterByKeywords\\s*=([^:]+)", l.keywords);

		//// extract keywords
		//std::regex  keywordsOr_regex("filterByKeywordsOr\\s*=([^:]+)", regex::icase);
		//std::smatch keywordsOr_match;
		//std::regex_search(line, keywordsOr_match, keywordsOr_regex);
		//std::vector<std::string> keywordsOr;
		//if (keywordsOr_match.empty() || keywordsOr_match[1].str().empty()) {
		//	//empty
		//} else {
		//	std::string          keywordsOr_str = keywordsOr_match[1];
		//	std::regex           keywordsOr_list_regex("[^,]+[ ]*[|][ ]*[a-zA-Z0-9]{1,8}", regex::icase);
		//	std::sregex_iterator keywordsOr_iterator(keywordsOr_str.begin(), keywordsOr_str.end(), keywordsOr_list_regex);
		//	std::sregex_iterator keywordsOr_end;
		//	while (keywordsOr_iterator != keywordsOr_end) {
		//		std::string keyword = (*keywordsOr_iterator)[0].str();
		//		keyword.erase(keyword.begin(), std::find_if_not(keyword.begin(), keyword.end(), ::isspace));
		//		keyword.erase(std::find_if_not(keyword.rbegin(), keyword.rend(), ::isspace).base(), keyword.end());
		//		if (keyword != "none") {
		//			keywordsOr.push_back(keyword);
		//		}
		//		++keywordsOr_iterator;
		//	}
		//	l.keywordsOr = keywordsOr;
		//}

		extractData(line, "filterByKeywordsOr\\s*=([^:]+)", l.keywordsOr);

		//// extract keywords
		//std::regex  keywordsExcluded_regex("filterByKeywordsExcluded\\s*=([^:]+)", regex::icase);
		//std::smatch keywordsExcluded_match;
		//std::regex_search(line, keywordsExcluded_match, keywordsExcluded_regex);
		//std::vector<std::string> keywordsExcluded;
		//if (keywordsExcluded_match.empty() || keywordsExcluded_match[1].str().empty()) {
		//	//empty
		//} else {
		//	std::string          keywordsExcluded_str = keywordsExcluded_match[1];
		//	std::regex           keywordsExcluded_list_regex("[^,]+[ ]*[|][ ]*[a-zA-Z0-9]{1,8}", regex::icase);
		//	std::sregex_iterator keywordsExcluded_iterator(keywordsExcluded_str.begin(), keywordsExcluded_str.end(), keywordsExcluded_list_regex);
		//	std::sregex_iterator keywordsExcluded_end;
		//	while (keywordsExcluded_iterator != keywordsExcluded_end) {
		//		std::string keyword = (*keywordsExcluded_iterator)[0].str();
		//		keyword.erase(keyword.begin(), std::find_if_not(keyword.begin(), keyword.end(), ::isspace));
		//		keyword.erase(std::find_if_not(keyword.rbegin(), keyword.rend(), ::isspace).base(), keyword.end());
		//		if (keyword != "none") {
		//			keywordsExcluded.push_back(keyword);
		//		}
		//		++keywordsExcluded_iterator;
		//	}
		//	l.keywordsExcluded = keywordsExcluded;
		//}

		extractData(line, "filterByKeywordsExcluded\\s*=([^:]+)", l.keywordsExcluded);

		//				// extract weight
		//std::regex weight_regex("weight\\s*=([^:]+)", regex::icase);
		//std::smatch weightmatch;
		//std::regex_search(line, weightmatch, weight_regex);
		//// extract the value after the equals sign
		//if (weightmatch.empty() || weightmatch[1].str().empty()) {
		//	l.weight = "none";
		//} else {
		//	std::string value = weightmatch[1].str();
		//	value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
		//	l.weight = value;
		//}

		//// extract damage
		//std::regex damage_regex("attackDamage\\s*=([^:]+)", regex::icase);
		//std::smatch damagematch;
		//std::regex_search(line, damagematch, damage_regex);
		//// extract the value after the equals sign
		//if (damagematch.empty() || damagematch[1].str().empty()) {
		//	l.damage = "none";
		//} else {
		//	std::string damagevalue = damagematch[1].str();
		//	damagevalue.erase(std::remove_if(damagevalue.begin(), damagevalue.end(), ::isspace), damagevalue.end());
		//	l.damage = damagevalue;
		//}

		extractValueString(line, "attackDamage\\s*=([^:]+)", l.damage);
		extractValueString(line, "attackDamageToAdd\\s*=([^:]+)", l.damageToAdd);

		//// extract damageMult
		//std::regex  damageMult_regex("attackDamageMult\\s*=([^:]+)", regex::icase);
		//std::smatch damageMultmatch;
		//std::regex_search(line, damageMultmatch, damageMult_regex);
		//// extract the value after the equals sign
		//if (damageMultmatch.empty() || damageMultmatch[1].str().empty()) {
		//	l.damageMult = "none";
		//} else {
		//	std::string damageMultvalue = damageMultmatch[1].str();
		//	damageMultvalue.erase(std::remove_if(damageMultvalue.begin(), damageMultvalue.end(), ::isspace), damageMultvalue.end());
		//	l.damageMult = damageMultvalue;
		//}

		extractValueString(line, "attackDamageMult\\s*=([^:]+)", l.damageMult);

		//// extract projectile
		//std::regex setNewProjectile_regex("setNewProjectile\\s*=([^:]+)", regex::icase);
		//std::smatch setNewProjectilematch;
		//std::regex_search(line, setNewProjectilematch, setNewProjectile_regex);
		//// extract the value after the equals sign
		//if (setNewProjectilematch.empty() || setNewProjectilematch[1].str().empty()) {
		//	l.projectile = "none";
		//} else {
		//	std::string setNewProjectilevalue = setNewProjectilematch[1].str();
		//	setNewProjectilevalue.erase(setNewProjectilevalue.begin(), std::find_if_not(setNewProjectilevalue.begin(), setNewProjectilevalue.end(), ::isspace));
		//	setNewProjectilevalue.erase(std::find_if_not(setNewProjectilevalue.rbegin(), setNewProjectilevalue.rend(), ::isspace).base(), setNewProjectilevalue.end());
		//	l.projectile = setNewProjectilevalue;
		//}

		extractValueString(line, "setNewProjectile\\s*=([^:]+)", l.projectile);

		//// extract formList
		//std::regex formList_regex("addToFormList\\s*=([^:]+)", regex::icase);
		//std::smatch formList_match;
		//std::regex_search(line, formList_match, formList_regex);
		//std::vector<std::string> formList;
		//if (formList_match.empty() || formList_match[1].str().empty()) {
		//	//empty
		//} else {
		//	std::string formList_str = formList_match[1];
		//	std::regex formList_list_regex("[^,]+[ ]*[|][ ]*[a-zA-Z0-9]{1,8}", regex::icase);
		//	std::sregex_iterator formList_iterator(formList_str.begin(), formList_str.end(), formList_list_regex);
		//	std::sregex_iterator formList_end;
		//	while (formList_iterator != formList_end) {
		//		std::string tempVar = (*formList_iterator)[0].str();
		//		tempVar.erase(tempVar.begin(), std::find_if_not(tempVar.begin(), tempVar.end(), ::isspace));
		//		tempVar.erase(std::find_if_not(tempVar.rbegin(), tempVar.rend(), ::isspace).base(), tempVar.end());
		//		//logger::info(FMT_STRING("Race: {}"), race);
		//		if (tempVar != "none") {
		//			formList.push_back(tempVar);
		//		}
		//		++formList_iterator;
		//	}
		//	l.formList = formList;
		//}

		//// extract fullName
		//std::regex fullName_regex("fullName\\s*=\\s*~([^~]+?)\\s*~");
		//std::smatch namematch;
		//std::regex_search(line, namematch, fullName_regex);
		//// extract the value after the equals sign
		//if (namematch.empty() || namematch[1].str().empty()) {
		//	l.fullName = "none";
		//} else {
		//	std::string namevalue = namematch[1].str();
		//	namevalue.erase(namevalue.begin(), std::find_if_not(namevalue.begin(), namevalue.end(), ::isspace));
		//	namevalue.erase(std::find_if_not(namevalue.rbegin(), namevalue.rend(), ::isspace).base(), namevalue.end());
		//	l.fullName = namevalue;
		//}

		//		// extract keywordsToAdd
		//std::regex keywordsToAdd_regex("keywordsToAdd\\s*=([^:]+)", regex::icase);
		//std::smatch keywordsToAdd_match;
		//std::regex_search(line, keywordsToAdd_match, keywordsToAdd_regex);
		//std::vector<std::string> keywordsToAdd;
		//if (keywordsToAdd_match.empty() || keywordsToAdd_match[1].str().empty()) {
		//	//empty
		//} else {
		//	std::string keywordsToAdd_str = keywordsToAdd_match[1];
		//	std::regex keywordsToAdd_list_regex("[^,]+[ ]*[|][ ]*[a-zA-Z0-9]{1,8}", regex::icase);
		//	std::sregex_iterator keywordsToAdd_iterator(keywordsToAdd_str.begin(), keywordsToAdd_str.end(), keywordsToAdd_list_regex);
		//	std::sregex_iterator keywordsToAdd_end;
		//	while (keywordsToAdd_iterator != keywordsToAdd_end) {
		//		std::string keywordToAdd = (*keywordsToAdd_iterator)[0].str();
		//		keywordToAdd.erase(keywordToAdd.begin(), std::find_if_not(keywordToAdd.begin(), keywordToAdd.end(), ::isspace));
		//		keywordToAdd.erase(std::find_if_not(keywordToAdd.rbegin(), keywordToAdd.rend(), ::isspace).base(), keywordToAdd.end());
		//		if (keywordToAdd != "none") {
		//			//logger::info(FMT_STRING("keywordsToAdd: {}"), keywordToAdd);
		//			keywordsToAdd.push_back(keywordToAdd);
		//		}
		//		++keywordsToAdd_iterator;
		//	}
		//	l.keywordsToAdd = keywordsToAdd;
		//}

		//extractData(line, "keywordsToRemove\\s*=([^:]+)", l.keywordsToRemove);
		extractDataStrings(line, "filterByModNames\\s*=([^:]+)", l.modNames);

		extractData(line, "filterByAlternateTextures\\s*=([^:]+)", l.alternateTexturesFind);
		extractDataStrings(line, "filterByEditorIdContains\\s*=([^:]+)", l.filterByEditorIdContains);
		extractDataStrings(line, "filterByEditorIdContainsOr\\s*=([^:]+)", l.filterByEditorIdContainsOr);
		extractDataStrings(line, "filterByEditorIdContainsExcluded\\s*=([^:]+)", l.filterByEditorIdContainsExcluded);
		extractValueString(line, "speed\\s*=([^:]+)", l.speed);
		extractValueString(line, "speedMult\\s*=([^:]+)", l.speedMult);
		extractValueString(line, "gravity\\s*=([^:]+)", l.gravity);
		extractValueString(line, "gravityMult\\s*=([^:]+)", l.gravityMult);
		extractValueString(line, "restrictToBolts\\s*=([^:]+)", l.restrictToBolts);

		return l;
	}

	const std::vector<line_content> readConfig(const std::string& folder)
	{
		constexpr char             skipChar = ';';
		constexpr std::string_view extension = ".ini";

		DIR*                    dir;
		struct dirent*          ent;
		std::list<std::string>  directories{ folder };
		std::string             currentFolder;
		std::vector<line_content> tokens;

		while (!directories.empty()) {
			currentFolder = directories.front();
			directories.pop_front();
			if ((dir = opendir(currentFolder.c_str())) != nullptr) {
				while ((ent = readdir(dir)) != nullptr) {
					if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
						std::string  fullPath = currentFolder + "\\" + ent->d_name;
						struct _stat st;
						if (_stat(fullPath.c_str(), &st) == 0 && ((_S_IFDIR & st.st_mode) != 0)) {
							directories.push_back(fullPath);
						} else {
							std::string  fileName = ent->d_name;
							const size_t pos = fileName.find(extension);
							if (pos != std::string::npos) {
								fileName = fileName.substr(0, pos);
								const char* modname = fileName.c_str();

								if ((strstr(modname, ".esp") != nullptr || strstr(modname, ".esl") != nullptr || strstr(modname, ".esm") != nullptr)) {
									if (!IsPluginInstalled(modname)) {
										logger::info("************************************************************");
										logger::info("{} not found or is not a valid plugin file, skipping config file {}.", modname, fullPath);
										logger::info("************************************************************");
										continue;
									}
								}
								logger::info("************************************************************");
								logger::info("Processing config file {}... ", fullPath);
								logger::info("************************************************************");
								std::string   line;
								std::ifstream infile;
								infile.open(fullPath);
								while (std::getline(infile, line)) {
									if (line[0] == skipChar) {
										continue;
									}

									if (line.empty()) {
										continue;
									}

									tokens.push_back(create_patch_instruction_ammo(line));
								}
								infile.close();
							}
						}
					}
				}
				closedir(dir);
			} else {
				logger::info("Couldn't open directory {}.", currentFolder);
			}
		}
		return tokens;
	}
}
