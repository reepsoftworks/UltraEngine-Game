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
    class SplashScreen : public UIElement
    {
    public:
        WString splashimagefile;
        shared_ptr<Timer> timer;
        shared_ptr<Window> window;
        shared_ptr<Interface> ui;
        shared_ptr<Widget> mainpanel, label;

        SplashScreen();
        virtual bool Initialize(shared_ptr<Window> window, const int time);
        void SetHidden(const bool hide);

        virtual bool ProcessEvent(const Event& e);
    };
}