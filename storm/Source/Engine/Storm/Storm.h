#pragma once
//===============================================
// Storm.h : Entry point for the application
//===============================================
#include "..\GraphicsCommon\DeviceResources.h"
#include "..\Graphics3D\D3DRender.h"
#include "..\Storm\BaseGameLogic.h"
#include "..\Utilities\GameTimer.h"
#include "..\Utilities\Type.h"

//
// class GameCode App
//
class StormApp 
{
public:
    StormApp();
    virtual ~StormApp();

    // Win32 Specific
    virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight, int nCmdShow);

    const StormMath::Point &GetScreenSize() { return m_screenSize; }

    // Game Specific
    std::unique_ptr<BaseGameLogic>   m_pGame;

    void Tick();
    bool Render();
    void Present();

    void DebugLog(char* fmt, ...);

    // Util
    static int MsgProc(SDL_Event* e);
    int Close();   // Close GameWindow
    bool IsRunning() { return m_bIsRunning; }

protected:
    SDL_Window*         m_window;                   // SDL Window instance

                                                    // Device Resources
    StormMath::Point    m_screenSize;               // game screen size

    bool m_bIsRunning;                              // true if everything is initialized and the game is in the main loop
    StormMath::Rect m_rcDesktop;                    // current desktop size - not necessarily the client window size

    std::map<std::wstring, std::wstring> m_textResource;
    std::map<std::wstring, UINT> m_hotkeys;

    // Rendering Resources
    std::shared_ptr<DX::DeviceResources> m_deviceResources;
    std::shared_ptr<DX::D3DRender>       m_D3DRender;
    DX::GameTimer m_timer;

private:
    void CreateDevice();
    void CreateResources();
    void OnDeviceLost();

#ifdef _DEBUG
    Debug::DebugConsole m_debugConsole;
#endif

    // Game Application Interface
public:

    // Must be defined in an inherited class
    virtual TCHAR *VGetGameTitle() = 0;
    virtual TCHAR *VGetGameAppDirectory() = 0;
    virtual HICON VGetIcon() = 0;

    virtual std::unique_ptr<BaseGameLogic> VCreateGameAndView() = 0;
};

extern StormApp *g_pApp;