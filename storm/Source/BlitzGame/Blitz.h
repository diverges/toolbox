#pragma once

class BlitzApp : public StormApp
{
public:
    virtual TCHAR *VGetGameTitle() { return _T("Teapot Wars"); }
    virtual TCHAR *VGetGameAppDirectory() { return _T("Game Coding Complete 4\\Teapot Wars\\4.0"); }
    virtual HICON VGetIcon(){ return 0; } 
};