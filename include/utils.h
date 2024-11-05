#pragma once

namespace Utils {
	[[nodiscard]] inline auto GetStringFromFormIDAndModName(const RE::FormID formID, const RE::TESFile* File) -> std::string {
		const RE::FormID FormID = File->IsLight() ? formID & 0xF'FF : formID & 0xFF'FF'FF;
		return std::format("{}|0x{:X}", File->GetFilename(), FormID);
	}

	template<typename StringType>
	requires std::is_same_v<StringType, std::string> || std::is_same_v<StringType, std::string_view>
	// ReSharper disable once CppNotAllPathsReturnValue
	[[nodiscard]] auto toWideString(const StringType& str) -> std::wstring {
		if constexpr(std::is_same_v<StringType, std::string_view>) {
			const size_t wideLength = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), nullptr, 0);

			std::wstring wideStr(wideLength, L'\0');

			MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.length()), wideStr.data(), static_cast<int>(wideLength));

			return wideStr;
		} else if constexpr(std::is_same_v<StringType, std::string>) {
			const size_t wideLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);

			std::wstring wideStr(wideLength, L'\0');

			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), wideStr.data(), static_cast<int>(wideLength));

			return wideStr;
		}
	}

	inline void customMessageBox(const std::string_view errorString) {
		const std::string confirmationMessage{ std::format("{}\nClick 'NO' to Close the Game", errorString) };
		switch(REX::W32::MessageBoxA(nullptr, confirmationMessage.c_str(), SKSE::PluginDeclaration::GetSingleton()->GetName().data(), MB_YESNO | MB_ICONQUESTION)) {
			case IDNO:
				REX::W32::TerminateProcess(REX::W32::GetCurrentProcess(), EXIT_FAILURE);
				break;
			default:
				break;
		}
	}

	template<typename T>
	requires std::is_arithmetic_v<T>
	void limit(T& value, T min_value, T max_value) {
		value = std::clamp(value, min_value, max_value);
	}

	template<class T>
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
				uint32_t               formID{ static_cast<uint32_t>(std::stoul(modForm.data(), nullptr, 16) & 0xFF'FF'FF) };
				const auto* const      mod{ dataHandler->LookupLoadedModByName(modName) };
				if(mod) {
					if(mod->IsLight()) {
						formID = std::stoul(modForm.data(), nullptr, 16) & 0xF'FF;
					}
				} else {
					logger::trace("File With Name: '{}' was nullptr or Null", modName);
				}
				return dataHandler->LookupForm<T>(formID, modName.data());
			}
			return nullptr;
		} catch(const std::invalid_argument& e) {
			logger::error("Invalid argument: {}", e.what());
			return nullptr;
		} catch(const std::out_of_range& e) {
			logger::error("Out of range: {}", e.what());
			return nullptr;
		}
	}

	template<typename T>
	requires std::is_integral_v<T> || std::is_floating_point_v<T>
	// ReSharper disable once CppNotAllPathsReturnValue
	[[nodiscard]] auto getRandom(T lower_bound, T upper_bound) -> T {
		static std::random_device random_device;
		static std::mt19937       gen(random_device());

		if constexpr(std::is_integral_v<T>) {
			std::uniform_int_distribution<T> distributor(lower_bound, upper_bound);
			return distributor(gen);
		} else if constexpr(std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> distributor(lower_bound, upper_bound);
			return distributor(gen);
		}
	}

	[[nodiscard]] inline auto resolve_symlink(fs::path& entry_path, const size_t max_depth = 10) -> bool {
		size_t depth{};
		while(fs::is_symlink(entry_path)) {
			if(depth >= max_depth) {
				logger::error("Maximum symlink depth reached: {}", fs::absolute(entry_path).generic_string());
				return false;
			}
			entry_path = fs::read_symlink(entry_path);
			depth++;
		}
		return true;
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
}  // namespace Utils
