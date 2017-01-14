#pragma once
//===============================================
// StormSts.h : Precompile Header
//===============================================

#pragma comment(lib, "d3dcompiler.lib")

// Windows Header Files:
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>
#include <windows.h>
#include <windowsx.h>

// C StD Headers :
#include <stdlib.h>
#include <tchar.h>

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <queue>
#include <map>

// DirectX
#include <wrl/client.h>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

// SFML Include :
#include "SDL.h"
#include "SDL_syswm.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Macros
#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

#include "Storm\Storm.h"

extern INT WINAPI Storm(HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPWSTR lpCmdLine,
                        int nCmdShow);
