//
//
//
//
#include "pch.h"
#include "Blitz.h"

BlitzApp g_BlitzApp;

//=========================================================
// Engine Entry Point
INT WINAPI wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR    lpCmdLine,
    int       nCmdShow)
{
    return Storm(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
//=========================================================

//---------------------------------------------------------
// Game App
//
void BlitzLogic::VOnUpdate(float time, float elapsedTime)
{
    BaseGameLogic::VOnUpdate(time, elapsedTime);
}

std::unique_ptr<BaseGameLogic> BlitzApp::VCreateGameAndView()
{
    std::unique_ptr<BlitzLogic> ret = std::make_unique<BlitzLogic>();
    return std::move(ret);
}
