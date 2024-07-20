#include "Main.h"

SKSEPluginLoad( const SKSE::LoadInterface* a_skse ) {
	/*while (!IsDebuggerPresent()) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}*/
	std::chrono::steady_clock::time_point startSPL = std::chrono::high_resolution_clock::now();

	Logging logger;

	DataHandler* d = DataHandler::GetSingleton();
	d->LoadMainJson();
	d->ReloadLoggingIfNecessary(d->_JsonData["Logging"]["LogLevel"].get<std::string>());
	d->ProcessMainJson();
	d->LogDataHandlerContents();
	d->LoadExclusionJsonFiles();
	SKSE::Init( a_skse );

	SKSEEvent::InitializeMessaging();

	SMFRenderer::Register();

	std::chrono::nanoseconds nanosecondsTakenForSPL = std::chrono::duration( std::chrono::high_resolution_clock::now() - startSPL );

	logger::info("Time Taken in {} totally is {} nanoseconds or {} microseconds or {} milliseconds or {} seconds or {} minutes", std::source_location::current().function_name(), nanosecondsTakenForSPL.count(),
		std::chrono::duration_cast<std::chrono::microseconds>(nanosecondsTakenForSPL).count(), std::chrono::duration_cast<std::chrono::milliseconds>(nanosecondsTakenForSPL).count(),
		std::chrono::duration_cast<std::chrono::seconds>(nanosecondsTakenForSPL).count(), std::chrono::duration_cast<std::chrono::minutes>(nanosecondsTakenForSPL).count() );
	return true;
}
