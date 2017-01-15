#pragma once

class BaseGameLogic : public IGameLogic
{
public:
    BaseGameLogic();
    virtual ~BaseGameLogic();

    // Logic Update
    virtual void VOnUpdate(float time, float elapsedTime);

protected:
    float m_Lifetime;   // game session length
};