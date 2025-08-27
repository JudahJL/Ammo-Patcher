//
// Created by judah on 18-02-2025.
//

#include "Settings.h"

#include "APUtils.h"
#include "Events.h"
#include "FilePtrManager.h"
#include "timeit.h"
#include <ctre.hpp>

Settings Settings::Singleton;

Settings& Settings::GetSingleton() {
    return Singleton;
}

Settings& Settings::LoadSchema() {
    constexpr auto schema_path{ "data/SKSE/Plugins/APConfig_schema.json" };
    FilePtrManager schema{ schema_path };
    if(schema) {
        char                      readBuffer[65'535];
        rapidjson::Document       sd;
        rapidjson::FileReadStream bis(schema.get(), readBuffer, std::size(readBuffer));
        if(rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis); sd.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis).HasParseError()) {
            SPDLOG_CRITICAL("Error(offset {}): {}", sd.GetErrorOffset(), rapidjson::GetParseError_En(sd.GetParseError()));
            SPDLOG_CRITICAL("Loading {} failed", schema_path);
            SKSE::stl::report_and_fail("Please Check the Ammo_Patcher.log. Seems like there is a issue with loading APConfig_schema.json");
        }
        presetSchemaDocument = std::move(sd);
        SPDLOG_TRACE("Loaded Schema");
    } else {
        SKSE::stl::report_and_fail("Please Check the Ammo_Patcher.log. Seems like there is a issue with loading APConfig_schema.json");
    }
    return *this;
}

Settings& Settings::LoadPresets() {
    [[maybe_unused]] const timeit t;
    constexpr auto                file_path{ "data/SKSE/Plugins/Ammo_Patcher.json" };
    FilePtrManager                file{ file_path };
    char                          readBuffer[65'535];
    if(file) {
        rapidjson::Document       sd;
        rapidjson::FileReadStream bis(file.get(), readBuffer, std::size(readBuffer));
        if(rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis); sd.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis).HasParseError()) [[unlikely]] {
            SPDLOG_ERROR("Error(offset {}): {}", sd.GetErrorOffset(), rapidjson::GetParseError_En(sd.GetParseError()));
            SPDLOG_ERROR("Loading {} failed", file_path);
        } else {
            if(auto itr{ sd.FindMember("Load") }; itr != sd.MemberEnd() && itr->value.IsString()) {
                curr_preset  = "data/SKSE/Plugins/Ammo Patcher/Presets";
                curr_preset /= itr->value.GetString();
                if(curr_preset.extension() != ".json" || !std::filesystem::exists(curr_preset) || !std::filesystem::is_regular_file(curr_preset)) {
                    SPDLOG_ERROR("Couldn't Find '{}'. Will load Default.json", itr->value.GetString());
                    curr_preset = "data/SKSE/Plugins/Ammo Patcher/Presets/Default.json";
                }
            }
        }
    }
    constexpr auto            presets_path{ "data/SKSE/Plugins/Ammo Patcher/Presets/" };
    rapidjson::SchemaDocument schemaDoc{ presetSchemaDocument };

    if(fs::exists(presets_path) && !fs::is_empty(presets_path)) {
        for(const auto& entry : std::filesystem::directory_iterator(presets_path)) {
            const auto& path{ entry.path() };
            if(fs::is_regular_file(path) && path.extension() == ".json") {
                file = FilePtrManager{ path.c_str() };
                if(file) {
                    memset(readBuffer, 0, std::size(readBuffer));
                    rapidjson::Document       sd;
                    rapidjson::FileReadStream bis(file.get(), readBuffer, std::size(readBuffer));
                    if(rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis); sd.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis).HasParseError()) {
                        char buff[1'024];
                        sprintf_s(buff, std::size(buff), "File '%ls':Error(offset %zu): %s", path.c_str(), sd.GetErrorOffset(), rapidjson::GetParseError_En(sd.GetParseError()));
                        SPDLOG_ERROR(buff);
                        continue;
                    }
                    if(rapidjson::SchemaValidator validator{ schemaDoc }; !sd.Accept(validator)) {
                        wchar_t buff[2'048];
                        swprintf_s(buff, std::size(buff), L"File: %ls", path.c_str());
                        SPDLOG_ERROR(buff);
                        rapidjson::StringBuffer sb;
                        rapidjson::PrettyWriter writer(sb);
                        const auto              invalidSchemaPointer{ validator.GetInvalidSchemaPointer() };
                        invalidSchemaPointer.StringifyUriFragment(sb);
                        SPDLOG_ERROR("Invalid schema: {}", sb.GetString());
                        SPDLOG_ERROR("Invalid keyword: {}", validator.GetInvalidSchemaKeyword());
                        sb.Clear();
                        const auto invalidDocumentPointer{ validator.GetInvalidDocumentPointer() };
                        invalidDocumentPointer.StringifyUriFragment(sb);
                        SPDLOG_ERROR("Invalid document: {}", sb.GetString());
                        sb.Clear();
                        if(auto* err_value_ptr{ invalidDocumentPointer.Get(sd) }) {
                            err_value_ptr->Accept(writer);
                            SPDLOG_ERROR("Error at: {}", sb.GetString());
                            sb.Clear();
                        }
                        if(auto* err_values_schema_pointer{ invalidSchemaPointer.Get(sd) }) {
                            err_values_schema_pointer->Accept(writer);
                            SPDLOG_ERROR("Schema Definition of Error: {}", sb.GetString());
                        }
                        continue;
                    }
                    presets.try_emplace(path, std::move(sd));
                    SPDLOG_TRACE("Found Preset: {}", Utils::wstringToString(path));
                }
            }
        }
    }
    if(!presets.contains(curr_preset)) {
        if(!presets.contains("data/SKSE/Plugins/Ammo Patcher/Presets/Default.json")) {
            SKSE::stl::report_and_fail("Failed to load a preset");
        }
        curr_preset = "data/SKSE/Plugins/Ammo Patcher/Presets/Default.json";
    }
    return *this;
}

Settings& Settings::LoadExclusions() {
    [[maybe_unused]] const timeit t;
    // ReSharper disable once CppTooWideScope
    char                          readBuffer[65'535];
    constexpr auto                exclusions_path{ "data/SKSE/Plugins/Ammo Patcher/Exclusions/" };
    if(fs::exists(exclusions_path) && !fs::is_empty(exclusions_path)) {
        for(const auto& entry : std::filesystem::directory_iterator(exclusions_path)) {
            const auto& path{ entry.path() };
            if(path.extension() == ".json" && fs::is_regular_file(path)) {
                if(const FilePtrManager file{ path.c_str() }) {
                    memset(readBuffer, 0, std::size(readBuffer));
                    rapidjson::Document       sd;
                    rapidjson::FileReadStream bis(file.get(), readBuffer, std::size(readBuffer));
                    if(rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis); sd.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis).HasParseError()) {
                        char buff[2'048]{};

                        sprintf_s(buff, std::size(buff), "File '%ls':Error(offset %zu): %hs", path.c_str(), sd.GetErrorOffset(), rapidjson::GetParseError_En(sd.GetParseError()));
                        SPDLOG_ERROR(buff);
                        continue;
                    }
                    exclusions.try_emplace(path, std::make_tuple(std::make_pair(std::unordered_map<std::string, std::int64_t>{}, std::int64_t{}), std::int64_t{}, std::move(sd)));
                }
            }
        }
    }
    return *this;
}

inline std::string DiscardFormDigits(const std::string_view formID, const RE::TESFile* mod) {
    const std::size_t offset = (formID.starts_with("0x") || formID.starts_with("0X")) ? 2 : 0;
    const std::size_t length{ formID.length() - offset };

    char temp[9]{ "00000000" };
    std::memcpy(temp + (8 - length), formID.data() + offset, length);
    return { temp + (mod->IsLight() ? 5 : 2) };
}

#pragma push_macro("GetObject")
#undef GetObject

Settings& Settings::PopulateAmmoInfo() {
    for(const auto* const ammo : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESAmmo>()) {
        if(!ammo) {
            continue;
        }

        const auto* const ammoProjectile{ ammo->GetRuntimeData().data.projectile };
        if(!ammoProjectile) {
            continue;
        }

        if(ammo->GetRuntimeData().data.flags & RE::AMMO_DATA::Flag::kNonPlayable) {
            continue;
        }

        const auto* const tes_file{ ammo->GetFile() };
        const auto        filename{ tes_file->GetFilename() };
        const RE::FormID  f{ ammo->GetRawFormID() & (tes_file->IsLight() ? 0xF'FF : 0xFF'FF'FF) };
        std::snprintf(ammo_info_[filename.data()].emplace_back(
                                                     ammo->GetRawFormID(),
                                                     ammoProjectile->GetRawFormID(),
                                                     ammo->GetFullName(),
                                                     ammoProjectile->GetFullName(),
                                                     ammo->GetRuntimeData().data.damage,
                                                     ammoProjectile->data.gravity,
                                                     ammoProjectile->data.speed,
                                                     tes_file->IsLight())
                          .AmmoFormIDShort,
                      AmmoInfo::size, "0x%X", f);
    }
    for(auto& item : ammo_info_ | std::views::values) {
        std::ranges::sort(item, {}, &AmmoInfo::AmmoFormID);
    }
    return *this;
}

Settings& Settings::PopulateFormIDMapFromExclusions() {
    auto* const datahandler{ RE::TESDataHandler::GetSingleton() };
    using ctre::literals::operator""_ctre;

    for(const auto& exclusion_json : exclusions | std::views::values | std::views::elements<2>) {
        auto       files_to_exclude{ exclusion_json.FindMember("Mod File(s) to Exclude") };
        auto       formID_to_exclude{ exclusion_json.FindMember("AMMO FormID to Exclude") };
        const auto end{ exclusion_json.MemberEnd() };

        if(formID_to_exclude != end) {
            for(const auto& kv_pair : formID_to_exclude->value.GetObject()) {
                const char* filename{ kv_pair.name.GetString() };
                if(const RE::TESFile * modfile{ datahandler->LookupModByName(filename) }) {
                    constexpr auto FormID{ R"(^(0[xX])?[0-9A-Fa-f]{3,8}$)"_ctre };
                    for(const auto& formid : kv_pair.value.GetArray()) {
                        if(FormID.match(formid.GetString())) {
                            std::string formID{ DiscardFormDigits(formid.GetString(), modfile) };
                            uint32_t    hexnumber;
                            sscanf_s(formID.data(), "%x", &hexnumber);
                            const auto ammo_form{ datahandler->LookupForm<RE::TESAmmo>(hexnumber, filename) };
                            if(!ammo_form) {
                                continue;
                            }
                            form_id_map[filename].first.insert(ammo_form->GetRawFormID());
                        }
                    }
                }
            }
        }
        if(files_to_exclude != end) {
            for(const auto& filename : files_to_exclude->value.GetArray()) {
                if(datahandler->LookupModByName(filename.GetString())) {
                    form_id_map[filename.GetString()].second = true;
                }
            }
        }
    }
    return *this;
}

Settings& Settings::Patch() {
    [[maybe_unused]] const timeit t;
    std::size_t                   patched{};
    SPDLOG_INFO("*****************Processing data*****************");
    const auto& main_json{ presets[curr_preset] };
    SPDLOG_INFO("Selected Preset: {}", Utils::wstringToString(curr_preset));

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
        "Randomize Speed"      //25
    };

    const auto& AMMO         = main_json[JsonKeys.at(0)];
    const auto& InfiniteAMMO = AMMO[JsonKeys.at(20)];

    const auto& Arrow = AMMO[JsonKeys.at(1)];
    const auto& Bolt  = AMMO[JsonKeys.at(2)];

    const auto& AChangeGravity = Arrow[JsonKeys.at(8)];
    const auto& BChangeGravity = Bolt[JsonKeys.at(8)];

    const auto& AChangeSpeed = Arrow[JsonKeys.at(7)];
    const auto& BChangeSpeed = Bolt[JsonKeys.at(7)];

    const auto& ALimitSpeed = Arrow[JsonKeys.at(11)];
    const auto& BLimitSpeed = Bolt[JsonKeys.at(11)];

    const auto& ARandomizeSpeed = Arrow[JsonKeys.at(25)];
    const auto& BRandomizeSpeed = Bolt[JsonKeys.at(25)];

    const auto& ALimitDamage = Arrow[JsonKeys.at(12)];
    const auto& BLimitDamage = Bolt[JsonKeys.at(12)];

    const auto& AChangeSoundLevel = Arrow[JsonKeys.at(3)][JsonKeys.at(4)];
    const auto& BChangeSoundLevel = Bolt[JsonKeys.at(3)][JsonKeys.at(4)];

    const auto arrow_patch_ = Arrow[JsonKeys.at(21)].GetBool();
    const auto bolt_patch_  = Bolt[JsonKeys.at(22)].GetBool();

    const auto infinite_player_ammo_   = InfiniteAMMO[JsonKeys.at(23)].GetBool();
    const auto infinite_teammate_ammo_ = InfiniteAMMO[JsonKeys.at(24)].GetBool();

    //Enable
    constexpr auto Enable{ 5 };
    const auto     arrow_speed_enable_       = AChangeSpeed[JsonKeys.at(Enable)].GetBool();
    const auto     bolt_speed_enable_        = BChangeSpeed[JsonKeys.at(Enable)].GetBool();
    const auto     arrow_gravity_enable_     = AChangeGravity[JsonKeys.at(Enable)].GetBool();
    const auto     bolt_gravity_enable_      = BChangeGravity[JsonKeys.at(Enable)].GetBool();
    const auto     limit_arrow_speed_        = ALimitSpeed[JsonKeys.at(Enable)].GetBool();
    const auto     limit_bolt_speed_         = BLimitSpeed[JsonKeys.at(Enable)].GetBool();
    const auto     limit_arrow_damage_       = ALimitDamage[JsonKeys.at(Enable)].GetBool();
    const auto     limit_bolt_damage_        = BLimitDamage[JsonKeys.at(Enable)].GetBool();
    auto           change_arrow_sound_level_ = AChangeSoundLevel[JsonKeys.at(Enable)].GetBool();
    auto           change_bolt_sound_level_  = BChangeSoundLevel[JsonKeys.at(Enable)].GetBool();
    const auto     randomize_arrow_speed_    = ARandomizeSpeed[JsonKeys.at(Enable)].GetBool();
    const auto     randomize_bolt_speed_     = BRandomizeSpeed[JsonKeys.at(Enable)].GetBool();

    //Min
    constexpr auto Min{ 13 };
    const auto     arrow_damage_limiter_min_   = ALimitDamage[JsonKeys.at(Min)].GetFloat();
    const auto     bolt_damage_limiter_min_    = BLimitDamage[JsonKeys.at(Min)].GetFloat();
    const auto     arrow_speed_limiter_min_    = ALimitSpeed[JsonKeys.at(Min)].GetFloat();
    const auto     bolt_speed_limiter_min_     = BLimitSpeed[JsonKeys.at(Min)].GetFloat();
    const auto     arrow_speed_randomizer_min_ = ARandomizeSpeed[JsonKeys.at(Min)].GetFloat();
    const auto     bolt_speed_randomizer_min_  = BRandomizeSpeed[JsonKeys.at(Min)].GetFloat();

    //Max
    constexpr auto Max{ 14 };
    const auto     arrow_damage_limiter_max_   = ALimitDamage[JsonKeys.at(Max)].GetFloat();
    const auto     bolt_damage_limiter_max_    = BLimitDamage[JsonKeys.at(Max)].GetFloat();
    const auto     arrow_speed_limiter_max_    = ALimitSpeed[JsonKeys.at(Max)].GetFloat();
    const auto     bolt_speed_limiter_max_     = BLimitSpeed[JsonKeys.at(Max)].GetFloat();
    const auto     arrow_speed_randomizer_max_ = ARandomizeSpeed[JsonKeys.at(Max)].GetFloat();
    const auto     bolt_speed_randomizer_max_  = BRandomizeSpeed[JsonKeys.at(Max)].GetFloat();

    //Speed
    constexpr auto Speed{ 10 };
    const auto     arrow_speed_ = AChangeSpeed[JsonKeys.at(Speed)].GetFloat();
    const auto     bolt_speed_  = BChangeSpeed[JsonKeys.at(Speed)].GetFloat();

    //Gravity
    constexpr auto Gravity{ 9 };
    const auto     arrow_gravity_ = AChangeGravity[JsonKeys.at(Gravity)].GetFloat();
    const auto     bolt_gravity_  = BChangeGravity[JsonKeys.at(Gravity)].GetFloat();

    //Sound Level
    constexpr auto SoundLevel{ 6 };
    const auto     arrow_sound_level_str_ = AChangeSoundLevel[JsonKeys.at(SoundLevel)].GetString();
    const auto     bolt_sound_level_str_  = BChangeSoundLevel[JsonKeys.at(SoundLevel)].GetString();

    if(arrow_speed_limiter_min_ > arrow_speed_limiter_max_) {
        constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Arrow Speed Min is lesser than or Equal to Arrow Speed Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
        SPDLOG_WARN("{}", ErrorMessage);
    }

    if(bolt_speed_limiter_min_ > bolt_speed_limiter_max_) {
        constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Bolt Speed Min is lesser than or Equal to Bolt Speed Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
        SPDLOG_WARN("{}", ErrorMessage);
    }

    if(arrow_damage_limiter_min_ > arrow_damage_limiter_max_) {
        constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Arrow Damage Min is lesser than or Equal to Arrow Damage Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
        SPDLOG_WARN("{}", ErrorMessage);
    }

    if(bolt_damage_limiter_min_ > bolt_damage_limiter_max_) {
        constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Bolt Damage Min is lesser than or Equal to Bolt Damage Max.\nThis is ignorable but not expected or proper(i.e., it will still work)" };
        SPDLOG_WARN("{}", ErrorMessage);
    }

    if(arrow_speed_randomizer_min_ > arrow_speed_randomizer_max_) {
        constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Arrow Randomizer Min is lesser than or Equal to Arrow Randomizer Max.\nPlease Don't ignore this Warning. The Game will Crash" };
        SPDLOG_CRITICAL("{}", ErrorMessage);
    }

    if(arrow_speed_randomizer_min_ > arrow_speed_randomizer_max_) {
        constexpr auto ErrorMessage{ "Error Detected in Json. Make Sure Bolt Randomizer Min is lesser than or Equal to Bolt Randomizer Max.\nPlease Don't ignore this Warning. The Game will Crash" };
        SPDLOG_CRITICAL("{}", ErrorMessage);
    }

    auto& event{ APEventProcessor::GetSingleton() };
    if(infinite_player_ammo_ || infinite_teammate_ammo_) {
        event.RegisterEvent(infinite_player_ammo_, infinite_teammate_ammo_);
    } else {
        event.UnregisterEvent();
    }

    const std::unordered_map<std::string_view, RE::SOUND_LEVEL> AmmoSoundLevelMap{
        { JsonKeys.at(15),     RE::SOUND_LEVEL::kLoud },
        { JsonKeys.at(16),   RE::SOUND_LEVEL::kNormal },
        { JsonKeys.at(17),   RE::SOUND_LEVEL::kSilent },
        { JsonKeys.at(18), RE::SOUND_LEVEL::kVeryLoud },
        { JsonKeys.at(19),    RE::SOUND_LEVEL::kQuiet }
    };

    auto arrow_sound_level_{ RE::SOUND_LEVEL::kSilent };
    auto bolt_sound_level_{ RE::SOUND_LEVEL::kSilent };

    if(const auto iterator{ AmmoSoundLevelMap.find(arrow_sound_level_str_) }; iterator != AmmoSoundLevelMap.end()) {
        arrow_sound_level_ = iterator->second;
    } else {
        SPDLOG_ERROR("Invalid Arrow Sound Level specified in the JSON file. Not Patching Arrow Sound Level.");
        change_arrow_sound_level_ = false;
    }

    if(const auto iterator{ AmmoSoundLevelMap.find(bolt_sound_level_str_) }; iterator != AmmoSoundLevelMap.end()) {
        bolt_sound_level_ = iterator->second;
    } else {
        SPDLOG_ERROR("Invalid Bolt Sound Level specified in the JSON file. Not Patching Bolt Sound Level.");
        change_bolt_sound_level_ = false;
    }
    SPDLOG_INFO("{} {} is starting to patch", SKSE::PluginDeclaration::GetSingleton()->GetName(), SKSE::PluginDeclaration::GetSingleton()->GetVersion().string("."));

    for(auto* const ammo : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESAmmo>()) {
        if(!ammo) {
            continue;
        }

        auto* const ammoProjectile = ammo->GetRuntimeData().data.projectile;
        if(!ammoProjectile) {
            SPDLOG_INFO("PROJ Record with Name '{}' with FormID '{:08X}' from file '{}' is nullptr i.e., NULL", ammo->GetFullName(), ammo->GetRawFormID(), ammo->GetFile()->GetFilename());
            continue;
        }

        if(arrow_patch_ || bolt_patch_) {
            // for(const auto& ammoModName : form_id_map) {
            //     if(ammoModName == ammo->GetFile()->GetFilename()) {
            //         shouldPatch = false;
            //         SPDLOG_DEBUG("{}", starString.data());
            //         SPDLOG_DEBUG("From {} :", ammoModName.data());
            //         SPDLOG_DEBUG("Skipping Ammo : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}", ammo->GetFullName(), ammo->GetRawFormID(),
            //                      ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity);
            //         SPDLOG_DEBUG("{}", starString.data());
            //         break;
            //     }
            // }

            constexpr auto starString{ Utils::make_filled_char_array<125, '*'>() };
            if(!form_id_map.empty()) {
                if(const auto itr{ form_id_map.find(ammo->GetFile()->GetFilename().data()) }; itr != form_id_map.end()) {
                    const auto& [formIDs, blacklistFile]{ itr->second };
                    if(blacklistFile || formIDs.contains(ammo->GetRawFormID())) {
                        SPDLOG_DEBUG("{}", starString.data());
                        SPDLOG_DEBUG("Skipping Ammo : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}", ammo->GetFullName(), ammo->GetRawFormID(),
                                     ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity);
                        SPDLOG_DEBUG("{}", starString.data());
                        continue;
                    }
                }
            }

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
                    patched++;
                    SPDLOG_DEBUG("{}", starString.data());
                    SPDLOG_DEBUG("Before Patching : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
                                 ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
                }

                if(ammo->GetRuntimeData().data.flags.all(RE::AMMO_DATA::Flag::kNonBolt)) {  // for arrow
                    if(arrow_patch_) {
                        if(change_arrow_sound_level_) {
                            ammoProjectile->soundLevel = arrow_sound_level_;
                            SPDLOG_DEBUG("changed Arrow Sound Level");
                        }
                        if(arrow_speed_enable_) {
                            ammoProjectile->data.speed = arrow_speed_;
                            SPDLOG_DEBUG("Changed Arrow Speed");
                        }
                        if(arrow_gravity_enable_) {
                            ammoProjectile->data.gravity = arrow_gravity_;
                            SPDLOG_DEBUG("Changed Arrow Gravity");
                        }
                        if(limit_arrow_damage_) {
                            Utils::limit(ammo->GetRuntimeData().data.damage, arrow_damage_limiter_min_, arrow_damage_limiter_max_);
                            SPDLOG_DEBUG("Limited Arrow Damage");
                        }
                        if(limit_arrow_speed_) {
                            Utils::limit(ammoProjectile->data.speed, arrow_speed_limiter_min_, arrow_speed_limiter_max_);
                            SPDLOG_DEBUG("Limited Arrow Level");
                        }
                        if(randomize_arrow_speed_) {
                            float value                = Utils::getRandom(arrow_speed_randomizer_min_, arrow_speed_randomizer_max_);
                            ammoProjectile->data.speed = value;
                            SPDLOG_DEBUG("Randomized Arrow Speed to {}", value);
                        }
                    }
                }

                if(ammo->GetRuntimeData().data.flags.none(RE::AMMO_DATA::Flag::kNonBolt)) {  // for bolt
                    if(bolt_patch_) {
                        if(change_bolt_sound_level_) {
                            ammoProjectile->soundLevel = bolt_sound_level_;
                            SPDLOG_DEBUG("changed Bolt Sound Level");
                        }
                        if(bolt_speed_enable_) {
                            ammoProjectile->data.speed = bolt_speed_;
                            SPDLOG_DEBUG("Changed Bolt Speed");
                        }
                        if(bolt_gravity_enable_) {
                            ammoProjectile->data.gravity = bolt_gravity_;
                            SPDLOG_DEBUG("Changed Bolt Speed");
                        }
                        if(limit_bolt_speed_) {
                            Utils::limit(ammoProjectile->data.speed, bolt_speed_limiter_min_, bolt_speed_limiter_max_);
                            SPDLOG_DEBUG("Limited Bolt Speed");
                        }
                        if(randomize_arrow_speed_) {
                            float value                = Utils::getRandom(bolt_speed_randomizer_min_, bolt_speed_randomizer_max_);
                            ammoProjectile->data.speed = value;
                            SPDLOG_DEBUG("Randomized Bolt Speed to {}", value);
                        }
                        if(limit_bolt_damage_) {
                            Utils::limit(ammo->GetRuntimeData().data.damage, bolt_damage_limiter_min_, bolt_damage_limiter_max_);
                            SPDLOG_DEBUG("Limited Bolt Damage");
                        }
                    }
                }

                if(ammoPatched) {
                    SPDLOG_DEBUG("After Patching : Name:{}|FormID:{:08X}|Damage:{}|Projectile Name:{}|Projectile FormID:{:08X}|Projectile Speed:{}|Projectile Gravity:{}|File:{}", ammo->GetFullName(), ammo->GetRawFormID(),
                                 ammo->GetRuntimeData().data.damage, ammoProjectile->GetFullName(), ammoProjectile->GetRawFormID(), ammoProjectile->data.speed, ammoProjectile->data.gravity, ammo->GetFile()->GetFilename());
                    SPDLOG_DEBUG("{}", starString.data());
                }
            }
        }
    }
    SPDLOG_INFO("{} {} has finished Patching {} records", SKSE::PluginDeclaration::GetSingleton()->GetName(), SKSE::PluginDeclaration::GetSingleton()->GetVersion().string("."), patched);
    return *this;
}

Settings& Settings::Clear() {
    form_id_map.clear();
    return *this;
}

Settings& Settings::SetLogAndFlushLevel() {
    const auto level = spdlog::level::from_str(presets[curr_preset]["Logging"]["LogLevel"].GetString());
    spdlog::set_level(level);
    spdlog::flush_on(level);
    spdlog::default_logger()->flush(); // initially set was flush to off. now flush everything since we got the loglevel
    SPDLOG_DEBUG("LogLevel: {}", spdlog::level::to_string_view(level));
    return *this;
}

Settings& Settings::RevertToDefault() {
    [[maybe_unused]] const timeit t;
    std::size_t reverted{};
    for(auto* const ammo : RE::TESDataHandler::GetSingleton()->GetFormArray<RE::TESAmmo>()) {
        if(!ammo) {
            continue;
        }

        auto* const ammoProjectile{ ammo->GetRuntimeData().data.projectile };
        if(!ammoProjectile) {
            continue;
        }

        if(ammo->GetRuntimeData().data.flags & RE::AMMO_DATA::Flag::kNonPlayable) {
            continue;
        }
        const auto* const tes_file{ ammo->GetFile() };
        const auto        filename{ tes_file->GetFilename() };
        const auto&       ammo_info_vec = ammo_info_[filename.data()];

        if(auto it = std::ranges::lower_bound(ammo_info_vec, ammo->GetRawFormID(),
                                              {}, &AmmoInfo::AmmoFormID);
           it != ammo_info_vec.end() && it->AmmoFormID == ammo->GetRawFormID()) {
            reverted++;
            ammo->GetRuntimeData().data.damage = it->damage;
            ammoProjectile->data.gravity       = it->ProjGravity;
            ammoProjectile->data.speed         = it->ProjSpeed;
        }
    }
    SPDLOG_DEBUG("Revered {} records to default", reverted);
    return *this;
}

const Settings::AmmoInfoType& Settings::GetAmmoInfo() {
    return ammo_info_;
}

#pragma pop_macro("GetObject")
