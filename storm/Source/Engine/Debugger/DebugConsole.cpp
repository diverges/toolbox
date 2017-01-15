#include "StormStd.h"
#include "DebugConsole.h"

namespace Debug 
{

void DebugConsole::Init()
{
    AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
    hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
}

void DebugConsole::Destroy()
{
    fclose(hf_out);
    SAFE_DELETE(hf_out);
}

}