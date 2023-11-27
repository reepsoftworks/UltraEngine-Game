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
    class UIWindow;
    class UIPanel : public UIElement
    {
        virtual void OnActivate() {};
    public:
        shared_ptr<Widget> root;

        UIPanel();
        ~UIPanel();

        virtual void BuildWidgets(const iVec2& size, const shared_ptr<Widget> parent) {};

        friend UIWindow;
    };
}