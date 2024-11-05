#include "Events.h"
#include "DataHandler.h"
#include "SKSE-MCP/SKSEMenuFramework.h"
#include "UI.h"

void SKSEEvent::InitializeMessaging() {
	if(!SKSE::GetMessagingInterface()->RegisterListener("SKSE", MessageListener)) {
		util::report_and_fail("Unable to register message listener.");
	}
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void SKSEEvent::MessageListener(SKSE::MessagingInterface::Message* message) {
	constexpr RE::FormID       CURRENT_FOLLOWER_FACTION{ 0x5'C8'4E };
	DataHandler*               data_handler{ DataHandler::GetSingleton() };
	auto*                      ap_event_processor{ APEventProcessor::GetSingleton() };
	std::array<std::thread, 2> threads;
	switch(message->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			ap_event_processor->set_current_follower_faction(RE::TESForm::LookupByID<RE::TESFaction>(CURRENT_FOLLOWER_FACTION));
			if(ap_event_processor->get_current_follower_faction() == nullptr) {
				util::report_and_fail("CurrentFollowerFaction Can't be null, Something Went Wrong");
			}
			data_handler->PatchAMMO();
			break;
		case SKSE::MessagingInterface::kPostLoad:
			// threads[0] = std::thread([&data_handler]() -> void { data_handler->LoadMainJson(); });
			threads[0] = std::thread(&DataHandler::LoadMainJson, data_handler);
			threads[1] = std::thread(&DataHandler::LoadExclusionJsonFiles, data_handler);
			for(auto& thread : threads) {
				if(thread.joinable()) {
					thread.join();
				} else {
					util::report_and_fail("Failed to close threads");
				}
			}
			if(SKSEMenuFramework::IsInstalled()) {
				SMFRenderer::GetSingleton()->GetAllExclusionJsons();
			}

			logger::trace("Loaded Main Preset: {}", data_handler->get_selected_preset());

			data_handler->ProcessMainJson();

			DataHandler::ChangeLogLevel(data_handler->get_main_json_data().at("Logging").at("LogLevel").get<std::string>());

			data_handler->LogDataHandlerContents();
			break;
		default:
			break;
	}
}

void APEventProcessor::RegisterEvent() {
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

auto APEventProcessor::get_current_follower_faction() const -> RE::TESFaction* { return current_follower_faction_; }

auto APEventProcessor::set_current_follower_faction(RE::TESFaction* current_follower_faction) -> void { current_follower_faction_ = current_follower_faction; }

auto APEventProcessor::GetSingleton() -> APEventProcessor* {
	static APEventProcessor Singleton;
	return std::addressof(Singleton);
}

auto APEventProcessor::ProcessEvent(const RE::TESContainerChangedEvent* event, RE::BSTEventSource<RE::TESContainerChangedEvent>* /*a_eventSource*/) -> RE::BSEventNotifyControl {
	// this event handles all object transfers between containers in the game
	// this can be derived into multiple base events: OnItemRemoved and OnItemAdded
	if(event != nullptr && event->baseObj != 0 && event->itemCount != 0) {
		auto* const              oldCont = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer);
		const RE::TESObjectREFR* newCont = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer);
		auto*                    baseObj = RE::TESForm::LookupByID<RE::TESBoundObject>(event->baseObj);
		auto*                    skyrim_ui{ RE::UI::GetSingleton() };
		if((baseObj != nullptr) && (oldCont != nullptr) && (skyrim_ui != nullptr) && (newCont == nullptr)) {
			RE::Actor*         actor{ oldCont->As<RE::Actor>() };
			const DataHandler* data_handler{ DataHandler::GetSingleton() };
			if(actor != nullptr) {
				if((actor->IsPlayerRef() && data_handler->is_infinite_player_ammo()) || (actor->IsInFaction(current_follower_faction_) && data_handler->is_infinite_teammate_ammo())) {
					if(!(
						   skyrim_ui->IsMenuOpen(RE::GiftMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::FavoritesMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::InventoryMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::LoadWaitSpinner::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::TweenMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::MagicMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::BookMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::JournalMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) ||
						   skyrim_ui->IsMenuOpen(RE::StatsMenu::MENU_NAME))) {
						if(baseObj->IsAmmo()) {
							actor->AddObjectToContainer(baseObj, baseObj->As<RE::ExtraDataList>(), 1, baseObj->AsReference());
							logger::debug("{} added to {}", baseObj->As<RE::TESAmmo>()->GetFullName(), actor->GetName());
						}
					}
				}
			}
		}
	}

	return RE::BSEventNotifyControl::kContinue;
}
