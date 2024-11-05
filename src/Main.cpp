#include "Events.h"
#include "logging.h"
#include "UI.h"

SKSEPluginLoad( const SKSE::LoadInterface* a_skse ) {
	// while (IsDebuggerPresent() == 0) {
	// 	using namespace std::chrono_literals;
	// 	constexpr auto wait_time{5s};
	// 	std::this_thread::sleep_for(wait_time);
	// }
	const auto startSPL = std::chrono::high_resolution_clock::now();

	[[ maybe_unused ]] const Logging logger(spdlog::level::trace, spdlog::level::off);

	logger::trace("Logger was initialized");

	SKSE::Init( a_skse );

	logger::trace("SKSE::Init(const SKSE::LoadInterface* a_intfc) was Called");

	SKSEEvent::InitializeMessaging();


	SMFRenderer::Register();

	const auto nanosecondsTakenForSPL = std::chrono::duration( std::chrono::high_resolution_clock::now() - startSPL );

	logger::info("Time Taken in {} totally is {} nanoseconds or {} microseconds or {} milliseconds or {} seconds or {} minutes", std::source_location::current().function_name(), nanosecondsTakenForSPL.count(),
		std::chrono::duration_cast<std::chrono::microseconds>(nanosecondsTakenForSPL).count(), std::chrono::duration_cast<std::chrono::milliseconds>(nanosecondsTakenForSPL).count(),
		std::chrono::duration_cast<std::chrono::seconds>(nanosecondsTakenForSPL).count(), std::chrono::duration_cast<std::chrono::minutes>(nanosecondsTakenForSPL).count() );
	return true;
}
