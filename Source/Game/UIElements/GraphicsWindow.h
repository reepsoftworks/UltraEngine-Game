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
    enum GraphicWindowStyles
    {
        GRAPHICSWINDOW_TITLEBAR,
        GRAPHICSWINDOW_BORDERLESS,
        GRAPHICSWINDOW_FULLSCREEN,
        GRAPHICSWINDOW_FULLSCREENNATIVE,
    };

    class SplashScreen;
    class GraphicsWindow : public UIElement
    {
        WString windowtitle;
        iVec2 framebuffersize;
        GraphicWindowStyles style;
    public:
        shared_ptr<SplashScreen> splashscreen;
        shared_ptr<Window> window;
        shared_ptr<Framebuffer> framebuffer;

        GraphicsWindow();
        virtual bool Initialize(const WString& title, const int width, const int height, shared_ptr<Display> display, const GraphicWindowStyles style);
        virtual bool ProcessEvent(const Event& e);

        void Resize(shared_ptr<Display> display, const int width, const int height, const GraphicWindowStyles style);
        void Render(shared_ptr<World> world);
        const iVec2 GetFramebufferSize();
    };

    extern shared_ptr<GraphicsWindow> CreateGraphicsWindow(const WString& title, const int width, const int height, shared_ptr<Display> display, const GraphicWindowStyles style = GRAPHICSWINDOW_TITLEBAR);
}