#pragma once
#include <algorithm>
#include <codecvt>
#include <format>
#include <random>
#include <RE/B/BSCoreTypes.h>
#include <RE/T/TESDataHandler.h>
#include <spdlog/spdlog.h>

namespace Utils {
    [[nodiscard]] inline auto GetStringFromFormIDAndModName(const RE::FormID formID, const RE::TESFile* file) -> std::string {
        const RE::FormID FormID{ formID & (file->IsLight() ? 0xF'FF : 0xFF'FF'FF) };
        return std::format("{}|0x{:X}", file->GetFilename(), FormID);
    }

    template<typename T>
    requires std::is_arithmetic_v<T>
    constexpr void limit(T& value, T min_value, T max_value) {
        value = std::clamp(value, min_value, max_value);
    }

    template<class T>
    requires std::sortable<typename std::vector<T>::iterator>
    void RemoveAnyDuplicates(std::vector<T>& vec) {
        // Sort the vector
        std::sort(vec.begin(), vec.end());

        // Use std::unique to move duplicates to the end
        auto last = std::unique(vec.begin(), vec.end());

        // Erase the duplicates
        vec.erase(last, vec.end());
    }

    template<class T>
    [[nodiscard]] auto GetFromIdentifier(const std::string_view identifier) -> T* {
        try {
            auto* const  dataHandler{ RE::TESDataHandler::GetSingleton() };
            const size_t delimiter{ identifier.find('|') };
            if(delimiter != std::string::npos) {
                const std::string_view modName{ identifier.substr(0, delimiter) };
                const std::string_view modForm{ identifier.substr(delimiter + 1) };
                uint32_t               formID{ static_cast<uint32_t>(std::stoul(modForm.data(), nullptr, 16)) };
                if(const auto* const mod{ dataHandler->LookupLoadedModByName(modName) }) {
                    if(mod->IsLight()) {
                        formID = formID & 0xF'FF;
                    } else {
                        formID = formID & 0xFF'FF'FF;
                    }
                } else {
                    SPDLOG_TRACE("File With Name: '{}' was nullptr or Null", modName);
                }
                return dataHandler->LookupForm<T>(formID, modName.data());
            }
            SPDLOG_TRACE("Couldn't find delimiter in '{}'", identifier);
            return nullptr;
        } catch(const std::invalid_argument& e) {
            SPDLOG_ERROR("Invalid argument: {}", e.what());
            return nullptr;
        } catch(const std::out_of_range& e) {
            SPDLOG_ERROR("Out of range: {}", e.what());
            return nullptr;
        }
    }

    static inline std::random_device        random_device;
    static thread_local inline std::mt19937 gen(random_device());

    template<typename T>
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    // ReSharper disable once CppNotAllPathsReturnValue
    [[nodiscard]] auto getRandom(T lower_bound, T upper_bound) -> T {
        if constexpr(std::is_integral_v<T>) {
            std::uniform_int_distribution<T> distributor(lower_bound, upper_bound);
            return distributor(gen);
        } else if constexpr(std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> distributor(lower_bound, upper_bound);
            return distributor(gen);
        }
    }

    /**
	 * \brief Creates a null-terminated std::array filled with a specified character.
	 *
	 * This function creates a `std::array` of size `N + 1` where the first `N` elements
	 * are filled with `FillChar`, and the last element is set to `'\0'` to make the array
	 * a null-terminated C-style string.
	 *
	 * \tparam N The size of the array excluding the null terminator.
	 * \tparam FillChar The character used to fill the array.
	 *
	 * \return A `std::array<char, N + 1>` with `N` elements filled with `FillChar` and
	 *         a null terminator at the end.
	 */
    template<std::size_t N, char FillChar>
    [[nodiscard]] constexpr auto make_filled_char_array() -> std::array<char, N + 1> {
        std::array<char, N + 1> arr{};
        arr.fill(FillChar);  // Fill the array with the specified character
        arr.back() = '\0';
        return arr;
    }

    inline std::string wstringToString(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }
}  // namespace Utils
