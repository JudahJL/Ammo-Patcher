#include "Events.h"
#include "Settings.h"
#include <spdlog/spdlog.h>
#include "timeit.h"

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    // while(REX::W32::IsDebuggerPresent() == 0) {
    //     using std::chrono_literals::operator""s;
    //     constexpr auto wait_time{ 5s };
    //     std::this_thread::sleep_for(wait_time);
    // }
    {
        [[maybe_unused]] const timeit t;
#ifndef NDEBUG
        auto pattern = std::format("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [{}.dll,%s:%#] %v", SKSE::GetPluginName());  // TODO: make this compile time later
#endif

        SKSE::Init(a_skse, {
                               .logLevel = spdlog::level::trace,
#ifndef NDEBUG
                               .logPattern = pattern.data(),
#else
                               .logPattern = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v",
#endif
                           });
        spdlog::flush_on(spdlog::level::off);
        auto plugin = SKSE::PluginDeclaration::GetSingleton();
        logger::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion().string("."));
        Settings::GetSingleton().PopulateFormIDMap().LoadSchema().LoadPresets().SetLogAndFlushLevel();
        SKSEEvent::InitializeMessaging();
    }
    return true;
}
