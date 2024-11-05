#pragma once

class Logging
{
public:
	explicit Logging();

	explicit Logging(spdlog::level::level_enum CommonLevel, std::string_view Name = SKSE::PluginDeclaration::GetSingleton()->GetName());

	Logging(spdlog::level::level_enum SetLevel, spdlog::level::level_enum FlushLevel, std::string_view Name = SKSE::PluginDeclaration::GetSingleton()->GetName());

	explicit Logging(std::string_view Name);

	~Logging() = default;

	Logging(const Logging&) = delete;
	Logging(Logging&&) = delete;
	auto                                     operator=(const Logging&) -> Logging& = delete;
	auto                                     operator=(Logging&&) -> Logging& = delete;
private:
	static void                                         SetupLog(const std::optional<fs::path>& path, std::shared_ptr<spdlog::logger>& log, spdlog::level::level_enum SetLevel = spdlog::level::info, spdlog::level::level_enum FlushLevel = spdlog::level::info);
};
