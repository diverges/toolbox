#pragma once

//#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//// Required

// Windows Header Files:
#include <WinSDKVer.h>
#include <SDKDDKVer.h>
#include <wrl.h>
#include <wrl/client.h>

// C StD Headers :
#include <stdlib.h>
#include <memory>
#include <vector>
#include <agile.h>
#include <concrt.h>

// DirectX
#include <dxgi1_4.h>
#include <d3d12.h>
#include <pix.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include "d3dx12.h"

//// Debug

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif
