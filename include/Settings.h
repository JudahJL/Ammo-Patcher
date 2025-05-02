//
// Created by judah on 18-02-2025.
//

#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    struct AmmoInfo {
        static constexpr auto                    size{ 19 };
        RE::FormID                               AmmoFormID;
        RE::FormID                               ProjFormID;
        std::string                              AmmoName;
        std::string                              ProjName;
        decltype(RE::AMMO_DATA::damage)          damage;
        decltype(RE::BGSProjectileData::gravity) ProjGravity;
        decltype(RE::BGSProjectileData::speed)   ProjSpeed;
        bool                                     IsLight;
        char                                     AmmoFormIDShort[size];
    };

    // std::unordered_map<path, ammoExclusionTracker, ModFilesCount, document>
    using ExclusionType = std::unordered_map<std::filesystem::path, std::tuple<std::pair<std::unordered_map<std::string, std::int64_t>, std::int64_t>, std::int64_t, rapidjson::Document>>;
    using AmmoInfoType  = std::unordered_map<std::string, std::vector<AmmoInfo>>;

    static Settings&    GetSingleton();
    Settings&           LoadSchema();        // to be called once only
    Settings&           LoadPresets();       // to be called once only
    Settings&           LoadExclusions();    // to be called once only
    Settings&           PopulateAmmoInfo();  // to be called once only
    Settings&           PopulateFormIDMapFromExclusions();
    Settings&           Patch();
    Settings&           Clear();  //clears form_id_map
    Settings&           SetLogAndFlushLevel();
    Settings&           RevertToDefault();
    const AmmoInfoType& GetAmmoInfo();

    std::unordered_map<std::filesystem::path, rapidjson::Document> presets;
    ExclusionType                                                  exclusions;
    std::filesystem::path                                          curr_preset;
    rapidjson::Document                                            presetSchemaDocument;

    // Maps a filename to a pair containing:
    // 1. A set of FormIDs associated with the file to blacklist.
    // 2. A boolean flag indicating whether the file should be blacklisted.
    std::unordered_map<std::string, std::pair<std::unordered_set<RE::FormID>, bool>> form_id_map;

private:
    AmmoInfoType ammo_info_;

public:
    Settings(Settings&&)                 = delete;
    Settings(const Settings&)            = delete;
    Settings operator= (Settings&&)      = delete;
    Settings operator= (const Settings&) = delete;

private:
    static Settings Singleton;
    Settings()  = default;
    ~Settings() = default;
};

#endif  //SETTINGS_H
