#pragma once
#include "Knight.h"

class BlitzApp : public GameCore::StormApp
{
public:
    virtual TCHAR *VGetGameTitle() { return _T("AI Test"); }
    virtual TCHAR *VGetGameAppDirectory() { return _T("FSM Logic"); }
    virtual HICON  VGetIcon(){ return 0; }

protected:
    virtual std::unique_ptr<GameCore::BaseGameLogic> VCreateGameAndView();
};

class BlitzLogic : public GameCore::BaseGameLogic
{
public:
    BlitzLogic() : m_knight(0) {}
    virtual void VOnUpdate(float time, float elapsedTime);

private:
    Knight m_knight;
};