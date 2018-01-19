#include "StormStd.h"
#include "BaseGameLogic.h"

using namespace GameCore;

BaseGameLogic::BaseGameLogic()
{
    m_Lifetime = 0.0f;
}

BaseGameLogic::~BaseGameLogic()
{
}

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{
    int deltaMilliseconds = static_cast<int>(elapsedTime * 1000.0f);
    m_Lifetime += elapsedTime;

    // Update Components
    // TODO
}
