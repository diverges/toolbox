//===============================================
// Storm.h : Entry point for the application
//===============================================

#include "StormStd.h"

#ifdef _DEBUG
#include "Debugger\DebugConsole.h"
#endif 

using Microsoft::WRL::ComPtr;

StormApp *g_pApp = nullptr;

//===============================================
//
// Storm Implementation
//
//===============================================
StormApp::StormApp()
{
    g_pApp = this;
}

bool StormApp::InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight, int nCmdShow)
{

    // Init Debugger
#ifdef _DEBUG
    Debug::Init();
    CONSOLE_OUT("***\n");
    CONSOLE_OUT("*** Debug Console\n");
    CONSOLE_OUT("***\n");
#endif

    // TODO: Check Resources

    // Init DirectX device resources
    m_deviceResources = std::make_shared<DX::DeviceResources>();

    // 
    // Show Window
    m_window = SDL_CreateWindow(
        "SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (m_window == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create window dependent resources
        m_deviceResources->SetWindow(m_window);

        // Init renderer
        m_D3DRender = std::make_shared<DX::D3DRender>(m_deviceResources);

        m_bIsRunning = true;
    }

    // TODO: Create Game View

    // All resources have been loaded
    // start timer
    m_timer.Reset();
    return TRUE;
}

int StormApp::Close() 
{
    //Destroy window
    SDL_DestroyWindow(m_window);

    return 0;
}

void StormApp::CreateDevice()
{
   
}

void StormApp::CreateResources()
{
   
}

void StormApp::OnDeviceLost()
{

}

//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//
//--------------------------------------------------------------------------------------
void StormApp::Tick()
{
    // Update delta time
    m_timer.Tick();

    //
    //// Update scene 
    // TODO


}

bool StormApp::Render()
{
   
    // Render Functions
    m_D3DRender->Render();

    return true;
}

void StormApp::Present()
{
    m_deviceResources->Present();
}

//----------------------------------------------------------
// SDL Callback Function
//
// Main event handler for SDL
//
int StormApp::MsgProc(SDL_Event* e)
{
    int result = 0;

    switch(e->type)
    {
        case SDL_QUIT:
            g_pApp->m_bIsRunning = false;
            break;
        case SDL_KEYDOWN:
            // TODO: Send to game keydown manager
            break;
    }
    
    return result;
}