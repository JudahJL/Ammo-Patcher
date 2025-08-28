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

    // std::unordered_map<path, ammoRuleTracker, ModFilesCount, document>
    using BlacklistType = std::unordered_map<std::filesystem::path, std::tuple<std::pair<std::unordered_map<std::string, std::int64_t>, std::int64_t>, std::int64_t, rapidjson::Document>>;
    using WhitelistType = BlacklistType;
    using AmmoInfoType  = std::unordered_map<std::string, std::vector<AmmoInfo>>;

    static Settings&    GetSingleton();
    Settings&           LoadSchema();        // to be called once only
    Settings&           LoadPresets();       // to be called once only
    Settings&           PopulateAmmoInfo();  // to be called once only
    Settings&           PopulateFormIDMap();
    std::optional<bool> IsAllowed(const std::string& file, RE::FormID id);
    Settings&           Patch();
    Settings&           SetLogAndFlushLevel();
    Settings&           RevertToDefault();
    const AmmoInfoType& GetAmmoInfo();

    std::unordered_map<std::filesystem::path, rapidjson::Document> presets;
    std::filesystem::path                                          curr_preset;
    rapidjson::Document                                            presetSchemaDocument;

    enum class Rule : std::uint8_t {
        kNone = 0,
        kWhitelist,
        kBlacklist
    };

    struct FileRule {
        std::optional<Rule>                  global;  // file-wide rule
        std::unordered_map<RE::FormID, Rule> per_id;  // record rules
    };

    std::unordered_map<std::string, FileRule> form_id_map;

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

#endif  // SETTINGS_H
