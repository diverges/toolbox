#pragma once

enum class location
{
    village_gate,
    village_inn,
    forest,
    cave,
    castle,
};

class Knight : public GameCore::BaseGameEntity
{
public:
    Knight(int id);

    void Update() override;

    location Location() const { return m_Location; }
    void ChangeLocation(const location loc) { m_Location = loc; }

    int  GoldCarried() const { return m_GoldCarried; }
    void SetGoldCarried(const int val) { m_GoldCarried = val; }
    void AddToGoldCarried(const int val);

    bool Fatigued() const;
    void DecreaseFatigue() { m_Fatigue -= 1; }
    void IncreaseFatigue() { m_Fatigue += 1; }

    int  Wealth()const { return m_MoneyInBank; }
    void SetWealth(const int val) { m_MoneyInBank = val; }
    void AddToWealth(const int val);

    bool Healthy() const;
    void Damage(const int val) { m_Health -= val; if (m_Health < 0) m_Health = 0; }
    void Heal() { m_Health = kMaxHealth; }

    AI::StateMachine<Knight>* GetFSM() { return &m_FSM; }

private:
    const int kMaxHealth = 9;
    const int kTirednessThreshold = 5;

    location m_Location;
    int m_GoldCarried;
    int m_MoneyInBank;
    int m_Health;
    int m_Fatigue;

    AI::StateMachine<Knight> m_FSM;
};

//---------------------------------------------------------
// States
//
class GoToInnAndRest : public AI::State<Knight>
{
public:
    GoToInnAndRest() {};
    GoToInnAndRest(const GoToInnAndRest&) = delete;
    GoToInnAndRest& operator=(const GoToInnAndRest&) = delete;

    void Enter(Knight& knight) override;
    void Execute(Knight& knight) override;
    void Exit(Knight& knight) override;

    static GoToInnAndRest* Instance() {
        static GoToInnAndRest _instance;
        return &_instance;
    };
};

class EnterCavesAndFight : public AI::State<Knight>
{
public:
    EnterCavesAndFight() {}
    EnterCavesAndFight(const EnterCavesAndFight&) = delete;
    EnterCavesAndFight& operator=(const EnterCavesAndFight&) = delete;

    void Enter(Knight& knight) override;
    void Execute(Knight& knight) override;
    void Exit(Knight& knight) override;

    static EnterCavesAndFight* Instance() {
        static EnterCavesAndFight _instance;
        return &_instance;
    };
};

class SetupCampAndHeal : public AI::State<Knight>
{
public:
    SetupCampAndHeal() {}
    SetupCampAndHeal(const SetupCampAndHeal&) = delete;
    SetupCampAndHeal& operator=(const SetupCampAndHeal&) = delete;

    void Enter(Knight& knight) override;
    void Execute(Knight& knight) override;
    void Exit(Knight& knight) override;

    static SetupCampAndHeal* Instance() {
        static SetupCampAndHeal _instance;
        return &_instance;
    };
};