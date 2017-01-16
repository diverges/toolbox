#include "pch.h"
#include "Knight.h"

Knight::Knight(int id) :
    BaseGameEntity(id),
    m_Location(location::village_inn),
    m_GoldCarried(0),
    m_MoneyInBank(0),
    m_Thirst(0),
    m_Fatigue(0),
    m_FSM(std::make_unique<AI::StateMachine<Knight>>(*this))
{}

void Knight::Update()
{
    m_Thirst += 1;

    m_FSM->Update();
}

void Knight::AddToGoldCarried(const int val)
{
    m_GoldCarried += val;

    if (m_GoldCarried < 0) m_GoldCarried = 0;
}

bool Knight::Fatigued() const
{
    return (m_Fatigue > TirednessThreshold);
}

void Knight::AddToWealth(const int val)
{
    m_MoneyInBank += val;
    if (m_MoneyInBank < 0) m_MoneyInBank = 0;
}

bool Knight::Thirsty() const
{
   return (m_Thirst >= ThirstLevel);
}

void EnterDungeonAndFight::Enter(Knight & knight)
{
    if (knight.Location() != location::cave)
    {
        g_pApp->DebugLog("Knight : Walking to caves");
        knight.ChangeLocation(location::cave);
    }
}

void EnterDungeonAndFight::Execute(Knight & knight)
{
    knight.AddToGoldCarried(10);
    knight.IncreaseFatigue();

    g_pApp->DebugLog("Knight : Fighting enemies, made 10 gold.");
}

void EnterDungeonAndFight::Exit(Knight & knight)
{
}
