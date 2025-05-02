#include "Events.h"
#include "Settings.h"
#include "logging.h"
#include "timeit.h"

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    // while(REX::W32::IsDebuggerPresent() == 0) {
    //     using std::chrono_literals::operator""s;
    //     constexpr auto wait_time{ 5s };
    //     std::this_thread::sleep_for(wait_time);
    // }
    InitializeLogging();

    [[maybe_unused]] const timeit t;
    SKSE::Init(a_skse, false);
    Settings::GetSingleton().LoadSchema().LoadPresets().SetLogAndFlushLevel().LoadExclusions();
    SKSEEvent::InitializeMessaging();
    return true;
}
