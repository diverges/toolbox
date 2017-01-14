#pragma once
//===============================================
// Storm.h : Entry point for the application
//===============================================
#include "..\GraphicsCommon\DeviceResources.h"
#include "..\Graphics3D\D3DRender.h"
#include "..\Utilities\GameTimer.h"
#include "..\Utilities\Type.h"

//
// class GameCode App
//
class StormApp 
{
private:

    void CreateDevice();
    void CreateResources();
    void OnDeviceLost();

protected:
    SDL_Window*         m_window;         // SDL Window instance

    // Device Resources
    StormMath::Point m_screenSize;	                // game screen size

    bool m_bIsRunning;                              // true if everything is initialized and the game is in the main loop
    StormMath::Rect m_rcDesktop;			        // current desktop size - not necessarily the client window size

public: 
    const StormMath::Point &GetScreenSize() { return m_screenSize; }

protected:
    std::map<std::wstring, std::wstring> m_textResource;
    std::map<std::wstring, UINT> m_hotkeys;

    // Rendering Resources
    std::shared_ptr<DX::DeviceResources> m_deviceResources;
    std::shared_ptr<DX::D3DRender>       m_D3DRender;
    DX::GameTimer m_timer;

public:
    
    StormApp();

    // Initialization
    virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight, int nCmdShow);

    // Game Loop
    void Tick();
    bool Render();
    void Present();

    // Util
    static int MsgProc(SDL_Event* e);
    int Close();   // Close GameWindow
    bool IsRunning() { return m_bIsRunning; }

    // Game Application Data
    // Must be defined in an inherited class
    virtual TCHAR *VGetGameTitle() = 0;
    virtual TCHAR *VGetGameAppDirectory() = 0;
    virtual HICON VGetIcon() = 0;
};

extern StormApp *g_pApp;