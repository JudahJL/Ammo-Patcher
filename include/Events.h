#pragma once

class APEventProcessor final: public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
	static auto GetSingleton() -> APEventProcessor*;
	void RegisterEvent();
	void UnregisterEvent();

	APEventProcessor(const APEventProcessor&)                    = delete;
	APEventProcessor(APEventProcessor&&)                         = delete;
	auto operator=(const APEventProcessor&) -> APEventProcessor& = delete;
	auto operator=(APEventProcessor&&) -> APEventProcessor&      = delete;


	[[nodiscard]] auto get_current_follower_faction() const -> RE::TESFaction*;
	auto               set_current_follower_faction(RE::TESFaction* current_follower_faction) -> void;

private:
	APEventProcessor()           = default;
	~APEventProcessor() override = default;

	RE::TESFaction* current_follower_faction_{ nullptr };
	bool event_deployed_{ false };

	auto ProcessEvent(const RE::TESContainerChangedEvent* event, RE::BSTEventSource<RE::TESContainerChangedEvent>* /*a_eventSource*/) -> RE::BSEventNotifyControl override;
};

class SKSEEvent
{
public:
	static void InitializeMessaging();

private:
	static void MessageListener(SKSE::MessagingInterface::Message* message);
};
