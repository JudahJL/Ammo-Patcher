#pragma once

class Logging
{
public:
	Logging();

	Logging(spdlog::level::level_enum CommonLevel, std::string_view Name = SKSE::PluginDeclaration::GetSingleton()->GetName());

	Logging(spdlog::level::level_enum SetLevel, spdlog::level::level_enum FlushLevel, std::string_view Name = SKSE::PluginDeclaration::GetSingleton()->GetName());

	Logging(std::string_view Name);

	~Logging() = default;

private:
	Logging(const Logging&) = delete;
	Logging(Logging&&) = delete;
	Logging&                                     operator=(const Logging&) = delete;
	Logging&                                     operator=(Logging&&) = delete;
	void                                         SetupLog(std::optional<fs::path> path, std::shared_ptr<spdlog::logger>& log, spdlog::level::level_enum SetLevel = spdlog::level::info, spdlog::level::level_enum FlushLevel = spdlog::level::info);
};
