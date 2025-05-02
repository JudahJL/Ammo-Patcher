#pragma once

class APEventProcessor final: public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
    static auto GetSingleton() -> APEventProcessor&;
    void        RegisterEvent(bool infiniteArrowForPlayer, bool infiniteArrowForTeammate);
    void        UnregisterEvent();

    APEventProcessor(const APEventProcessor&)                     = delete;
    APEventProcessor(APEventProcessor&&)                          = delete;
    auto operator= (const APEventProcessor&) -> APEventProcessor& = delete;
    auto operator= (APEventProcessor&&) -> APEventProcessor&      = delete;

    RE::TESFaction* current_follower_faction_{ nullptr };

private:
    static APEventProcessor Singleton;

    APEventProcessor()           = default;
    ~APEventProcessor() override = default;

    bool event_deployed_{ false };
    bool infinite_ammo_for_player{ false };
    bool infinite_ammo_for_teammate{ false };

    auto ProcessEvent(const RE::TESContainerChangedEvent* event, RE::BSTEventSource<RE::TESContainerChangedEvent>* /*a_eventSource*/) -> RE::BSEventNotifyControl override;
};

class SKSEEvent
{
public:
    static void InitializeMessaging();

private:
    static void MessageListener(SKSE::MessagingInterface::Message* message);
};
