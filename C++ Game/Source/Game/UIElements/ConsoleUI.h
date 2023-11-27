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
#include "../Game.h"

namespace UltraEngine::Game
{
    class ConsoleUI : public UIPanel
    {
        shared_ptr<Widget> textarea_log;
        shared_ptr<Widget> panel_entry;
        shared_ptr<Widget> textfield_entry;
        shared_ptr<Widget> button_send;

        virtual void OnActivate();
    public:
        ConsoleUI();
        ~ConsoleUI();

        virtual void BuildWidgets(const iVec2& size, const shared_ptr<Widget> parent);
        virtual bool ProcessEvent(const Event& e);
    };
}