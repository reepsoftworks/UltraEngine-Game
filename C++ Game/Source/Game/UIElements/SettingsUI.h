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
    class SettingsUI : public UIPanel
    {
		std::shared_ptr<UltraEngine::Widget> listbox;
		std::shared_ptr<UltraEngine::Widget> mainpanel;
		std::array<std::shared_ptr<UltraEngine::Widget>, 4> panel;
		std::shared_ptr<UltraEngine::Widget> button_ok;
		std::shared_ptr<UltraEngine::Widget> button_cancel;
		std::shared_ptr<UltraEngine::Widget> button_apply;

		// Window Settings
		std::shared_ptr<UltraEngine::Widget> label_display, combo_display;
		std::shared_ptr<UltraEngine::Widget> label_resoultion;
		std::shared_ptr<UltraEngine::Widget> combo_resoultion;
		std::shared_ptr<UltraEngine::Widget> label_windowmode, combo_windowmode;
		std::shared_ptr<UltraEngine::Widget> label_windowtheme, combo_windowtheme;

		// Graphic Settings
		std::shared_ptr<UltraEngine::Widget> label_fov, slider_fov;
		std::shared_ptr<UltraEngine::Widget> label_lightquality, combo_lightquality;
		std::shared_ptr<UltraEngine::Widget> label_tessellation, combo_tessellation;
		std::shared_ptr<UltraEngine::Widget> label_postfx, combo_postfx;
		std::shared_ptr<UltraEngine::Widget> label_ssr, combo_ssr;

		// Controls
		std::shared_ptr<UltraEngine::Widget> tabber_actionsets;
		std::vector<std::shared_ptr<UltraEngine::Widget>> tabber_panels;
		std::vector<std::shared_ptr<UltraEngine::Widget>> button_binds;
		std::vector<std::shared_ptr<UltraEngine::Widget>> settings_panels;

		// Sound
		std::shared_ptr<UltraEngine::Widget> label_hrtf, combo_hrtf;

		void ApplySettings();

    public:
        SettingsUI();
        ~SettingsUI();

        virtual void BuildWidgets(const iVec2& size, const shared_ptr<Widget> parent);
        virtual bool ProcessEvent(const Event& e);
    };
}