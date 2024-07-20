#pragma once

namespace AP
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
	const char*                                  _Pattern{ "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v" };
	std::shared_ptr<spdlog::sinks::msvc_sink_mt> _MSVCSink{ std::make_shared<spdlog::sinks::msvc_sink_mt>() };
};
