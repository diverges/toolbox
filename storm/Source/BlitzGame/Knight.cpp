#include "pch.h"
#include "Knight.h"

Knight::Knight(int id) :
    BaseGameEntity(id),
    m_Location(location::village_inn),
    m_GoldCarried(0),
    m_MoneyInBank(0),
    m_Health(0),
    m_Fatigue(0),
    m_FSM(*this)
{
    m_FSM.SetCurrentState(GoToInnAndRest::Instance());
}

void Knight::Update()
{
    m_FSM.Update();
}

void Knight::AddToGoldCarried(const int val)
{
    m_GoldCarried += val;

    if (m_GoldCarried < 0) m_GoldCarried = 0;
}

bool Knight::Fatigued() const
{
    return (m_Fatigue > kTirednessThreshold);
}

void Knight::AddToWealth(const int val)
{
    m_MoneyInBank += val;
    if (m_MoneyInBank < 0) m_MoneyInBank = 0;
}

bool Knight::Healthy() const
{
   return (m_Health > 0);
}

// GoToInnAndRest
void GoToInnAndRest::Enter(Knight & knight)
{
    if (knight.Location() != location::village_inn)
    {
        g_pApp->DebugLog("Knight : Walking to the village inn.\n");
        knight.ChangeLocation(location::village_inn);
    }
}

void GoToInnAndRest::Execute(Knight & knight)
{
    if (!knight.Fatigued())
    {
        g_pApp->DebugLog("Knight : All rested, time for some adventure.\n");
        knight.Heal();
        knight.GetFSM()->ChangeState(EnterCavesAndFight::Instance());
    }
    else
    {
        g_pApp->DebugLog("Knight : ZzZzZz...\n");
        knight.DecreaseFatigue();
    }
}

void GoToInnAndRest::Exit(Knight & knight)
{
    g_pApp->DebugLog("Knight : Leaving the inn.\n");
}

// EnterCavesAndFight
void EnterCavesAndFight::Enter(Knight & knight)
{
    if (knight.Location() != location::cave)
    {
        g_pApp->DebugLog("Knight : Walking to the cave.\n");
        knight.ChangeLocation(location::cave);
    }
}

void EnterCavesAndFight::Execute(Knight & knight)
{

    g_pApp->DebugLog("Knight : There are rats in the cave!\n");
    g_pApp->DebugLog("Knight : Damaged by rats, received 2 gold.\n");
    knight.AddToWealth(2);
    knight.Damage(3);
    if (!knight.Healthy())
    {
        knight.GetFSM()->ChangeState(SetupCampAndHeal::Instance());
    } 
    else if (knight.Fatigued())
    {
        knight.GetFSM()->ChangeState(GoToInnAndRest::Instance());
    }
}

void EnterCavesAndFight::Exit(Knight & knight)
{
    if (knight.Location() == location::cave)
    {
        g_pApp->DebugLog("Knight : Walking out of the cave.\n");
    }
}

// SetupCampAndHeal
void SetupCampAndHeal::Enter(Knight & knight)
{
    if (knight.Location() != location::forest)
    {
        g_pApp->DebugLog("Knight : Setting up camp in the woods.\n");
        knight.ChangeLocation(location::forest);
    }
}

void SetupCampAndHeal::Execute(Knight & knight)
{
    g_pApp->DebugLog("Knight : Ate some food, feeling replenished.\n");
    knight.Heal();
    knight.IncreaseFatigue();
    knight.GetFSM()->ChangeState(EnterCavesAndFight::Instance());
}

void SetupCampAndHeal::Exit(Knight & knight)
{
    if (knight.Location() == location::forest)
    {
        g_pApp->DebugLog("Knight : Packing up camp.\n");
    }
}
