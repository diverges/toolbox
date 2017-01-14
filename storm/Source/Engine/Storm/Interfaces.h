#pragma once
//=============================================================================
// Interfaces.h : Defines interface linking engine and game
//
//
//=============================================================================

///////////////////////////////////////////////////////////////////////////////
//
//  Input Managers : Public APIs for any object that implements reactions
//                   to events sent by hardware to user interface.
//
///////////////////////////////////////////////////////////////////////////////
class IKeyboardHandler
{
public:
    virtual bool VOnKeyDown(const BYTE c)=0;
    virtual bool VOnKeyUp(const BYTE c)=0;
};