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
    class UIPanel;
    class UIWindow : public UIElement
    {
    protected:
        std::weak_ptr<UltraEngine::Window> parentwindow;
        shared_ptr<Interface> ui;
        shared_ptr<UIPanel> panel;

    public:
        String callaction;
        shared_ptr<UltraEngine::Window> root;

        UIWindow();
        ~UIWindow();
        virtual void SetHidden(const bool hide);
        virtual bool ProcessEvent(const Event& e);

        virtual const bool BuildWindow(const WString title, const int w = 800, const int h = 600, shared_ptr<UltraEngine::Window> parent = NULL);

        template <class T>
        shared_ptr<T> SetUIPanel()
        {
            if (ui == NULL) return NULL;
            auto o = panel;
            if (o) return o->As<T>();
            o = std::make_shared<T>();
            std::shared_ptr<UIPanel> c = std::dynamic_pointer_cast<UIPanel>(o);
            if (c == NULL) RuntimeError("Type must be a UIPanel.");
            c->BuildWidgets(ui->root->GetSize(), ui->root);
            panel = c;
            return c->As<T>();
        }

        friend class Program;
    };
}