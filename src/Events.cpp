#include "Events.h"

#include "Settings.h"
#include "UI.h"
#include "SKSE-MCP/SKSEMenuFramework.h"

void SKSEEvent::InitializeMessaging() {
    if(!SKSE::GetMessagingInterface()->RegisterListener("SKSE", MessageListener)) {
        SKSE::stl::report_and_fail("Unable to register message listener.");
    }
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void SKSEEvent::MessageListener(SKSE::MessagingInterface::Message* message) {
    constexpr RE::FormID CURRENT_FOLLOWER_FACTION{ 0x5'C8'4E };
    auto&                ap_event_processor{ APEventProcessor::GetSingleton() };
    switch(message->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            if((ap_event_processor.current_follower_faction_ = RE::TESForm::LookupByID<RE::TESFaction>(CURRENT_FOLLOWER_FACTION)) == nullptr) {
                SKSE::stl::report_and_fail("CurrentFollowerFaction Can't be null, Something Went "
                                           "Wrong");
            }
            Settings::GetSingleton().PopulateAmmoInfo().PopulateFormIDMapFromExclusions().Patch();
            break;
        case SKSE::MessagingInterface::kPostLoad: SMFRenderer::Register(); break;
        default:                                  break;
    }
}

void APEventProcessor::RegisterEvent(const bool infiniteArrowForPlayer, const bool infiniteArrowForTeammate) {
    infinite_ammo_for_player   = infiniteArrowForPlayer;
    infinite_ammo_for_teammate = infiniteArrowForTeammate;
    if(!this->event_deployed_) {
        logger::debug("Registering APEventProcessor's RE::TESContainerChangedEvent");
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESContainerChangedEvent>(this);
    }
}

void APEventProcessor::UnregisterEvent() {
    if(this->event_deployed_) {
        logger::debug("Unregistering APEventProcessor's RE::TESContainerChangedEvent");
        RE::ScriptEventSourceHolder::GetSingleton()->RemoveEventSink<RE::TESContainerChangedEvent>(this);
    }
}

constinit APEventProcessor APEventProcessor::Singleton;

auto APEventProcessor::GetSingleton() -> APEventProcessor& { return Singleton; }

template<typename... MenuNames>
requires(sizeof...(MenuNames) > 0) && (std::convertible_to<std::decay_t<MenuNames>, std::string_view> && ...)
bool IsAnyMenuOpen(RE::UI* skyrim_ui, MenuNames&&... menus) {
    return (... || skyrim_ui->IsMenuOpen(menus));
}

auto APEventProcessor::ProcessEvent(const RE::TESContainerChangedEvent* event, RE::BSTEventSource<RE::TESContainerChangedEvent>* /*a_eventSource*/) -> RE::BSEventNotifyControl {
    // this event handles all object transfers between containers in the game
    // this can be derived into multiple base events: OnItemRemoved and OnItemAdded
    if(event != nullptr && event->baseObj != 0 && event->itemCount != 0) {
        auto* const oldCont{ RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer) };
        const auto* const newCont{ RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer) };
        auto* const baseObj{ RE::TESForm::LookupByID<RE::TESBoundObject>(event->baseObj) };
        auto* const skyrim_ui{ RE::UI::GetSingleton() };
        if((baseObj != nullptr) && (oldCont != nullptr) && (skyrim_ui != nullptr) && (newCont == nullptr) &&
           baseObj->IsAmmo()) {
            if(RE::Actor * actor{ oldCont->As<RE::Actor>() }) {
                if((actor->IsPlayerRef() && infinite_ammo_for_player) || (actor->IsInFaction(current_follower_faction_) && infinite_ammo_for_teammate)) {
                    if(!IsAnyMenuOpen(skyrim_ui, RE::GiftMenu::MENU_NAME, RE::FavoritesMenu::MENU_NAME, RE::ContainerMenu::MENU_NAME, RE::InventoryMenu::MENU_NAME, RE::DialogueMenu::MENU_NAME, RE::LoadWaitSpinner::MENU_NAME, RE::TweenMenu::MENU_NAME, RE::SleepWaitMenu::MENU_NAME, RE::MagicMenu::MENU_NAME, RE::BookMenu::MENU_NAME, RE::JournalMenu::MENU_NAME, RE::LoadingMenu::MENU_NAME, RE::StatsMenu::MENU_NAME)) {
                        actor->AddObjectToContainer(
                            baseObj, baseObj->As<RE::ExtraDataList>(), 1, baseObj->AsReference());
                        logger::debug(
                            "{} added to {}", baseObj->As<RE::TESAmmo>()->GetFullName(), actor->GetName());
                    }
                }
            }
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}
