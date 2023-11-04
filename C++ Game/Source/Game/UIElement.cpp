//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "UIElement.h"

namespace UltraEngine::Game
{
    bool UIElement::EventCallback(const Event& e, shared_ptr<Object> extra)
    {
        return extra->As<UIElement>()->ProcessEvent(e);
    }

    bool UIElement::ProcessEvent(const Event& e)
    {
        return true;
    }

    void UIElement::Listen(const EventId eventid, shared_ptr<Object> widget)
    {
        if (Self() == NULL) return;
        ListenEvent(eventid, widget, EventCallback, Self());
    }
}