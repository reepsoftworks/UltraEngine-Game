//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#pragma once
#include "UltraEngine.h"

namespace UltraEngine::Game
{
    class Program;
    class UIElement : public Panel
    {
    protected:
        friend Program;
        static bool EventCallback(const Event& e, shared_ptr<Object> extra);
        virtual bool ProcessEvent(const Event& e);
        virtual void Listen(const EventId eventid = EVENT_NONE, shared_ptr<Object> widget = NULL);
    };
}