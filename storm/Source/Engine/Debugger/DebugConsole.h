#pragma once
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#define CONSOLE_OUT(...) fprintf((Debug::hf_out), __VA_ARGS__);
#else
#define CONSOLE_OUT
#endif

//--------------------------------------------------------------------------------------
//  Debug console, redirects print statements
//  Writes to console by using CONSOLE_OUT 
//--------------------------------------------------------------------------------------
namespace Debug
{
    void Init();
    void Destroy(void);

    extern FILE* hf_out; // console output
}

