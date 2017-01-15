#pragma once
#include <io.h>
#include <fcntl.h>

//--------------------------------------------------------------------------------------
//  Debug console, redirects print statements
//  Writes to console by using CONSOLE_OUT 
//--------------------------------------------------------------------------------------
namespace Debug
{
    class DebugConsole
    {
    public:
        void Init();
        void Destroy();
        FILE* hf_out;
    };
}

