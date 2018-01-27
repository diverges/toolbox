#pragma once

// Core game methods
namespace GameCore
{
    // Definition from Game DLL
    struct GameExport {
        DX::DeviceResources * deviceResources;
        // render
        // cmd prmpt
    };

    // Definition from .EXE
    struct GameImport {
        BaseGameLogic * game;
    };


}