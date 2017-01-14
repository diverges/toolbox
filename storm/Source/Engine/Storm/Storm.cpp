//===============================================
// Storm.cpp : Entry point for the application
//===============================================


#include "StormStd.h"

INT WINAPI Storm(HINSTANCE hInstance,
                 HINSTANCE hPrevInstance,
                 LPWSTR lpCmdLine,
                 int nCmdShow) 
{
    UNREFERENCED_PARAMETER(hPrevInstance);

    // TODO: Init Memory Leak Detection

    // TODO: Init Logger

    
    // TODO: Perform Application Initialization
    // Perform application initialization

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return FALSE;
    } 
    else
    {
        // Initialize app and start timer
        if (!g_pApp->InitInstance(hInstance, lpCmdLine, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nCmdShow))
        {
            // [rez] Note: Fix memory leaks if we hit this branch.  Also, print an error.
            return FALSE;
        }
    }

    // Main Loop
    SDL_Event event;
    while(g_pApp->IsRunning()) 
    {
        // process event queue until empty
        if(SDL_PollEvent(&event) != 0)
        {
            StormApp::MsgProc(&event);
        } else {
            //
            // Game update loop
            g_pApp->Tick();

            // Render Frame
            if (g_pApp->Render())
            {
                g_pApp->Present();
            }
        }
    }

    // Shutdown
    g_pApp->Close();    // Close game window
    SDL_Quit();

    return TRUE;
}