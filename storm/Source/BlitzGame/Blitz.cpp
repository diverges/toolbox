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

using namespace GameCore;

//---------------------------------------------------------
// Game App
//
void BlitzLogic::VOnUpdate(float time, float elapsedTime)
{
    BaseGameLogic::VOnUpdate(time, elapsedTime);
    m_knight.Update();
    Sleep(800);
}

std::unique_ptr<BaseGameLogic> BlitzApp::VCreateGameAndView()
{
    std::unique_ptr<BlitzLogic> ret = std::make_unique<BlitzLogic>();
    return std::move(ret);
}
