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

    bool Thirsty() const;
    void BuyAndDrinkAWhiskey() { m_Thirst = 0; m_MoneyInBank -= 2; }

private:
    //the amount of gold a miner must have before he feels comfortable
    const int ComfortLevel = 5;
    //the amount of nuggets a miner can carry
    const int MaxNuggets = 3;
    //above this value a miner is thirsty
    const int ThirstLevel = 5;
    //above this value a miner is sleepy
    const int TirednessThreshold = 5;

    location m_Location;
    int m_GoldCarried;
    int m_MoneyInBank;
    int m_Thirst;
    int m_Fatigue;

    std::unique_ptr<AI::StateMachine<Knight>> m_FSM;
};

//---------------------------------------------------------
// States
//
class EnterDungeonAndFight : public AI::State<Knight>
{
public:
    EnterDungeonAndFight() = delete;
    EnterDungeonAndFight(const EnterDungeonAndFight&) = delete;
    EnterDungeonAndFight& operator=(const EnterDungeonAndFight&) = delete;

    void Enter(Knight& knight) override;
    void Execute(Knight& knight) override;
    void Exit(Knight& knight) override;

    static EnterDungeonAndFight* Instance() {
        return &_instance;
    };
private:
    static EnterDungeonAndFight _instance;
};