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
	class InputBindPanel : public Widget
	{
		shared_ptr<Widget> panel;
		shared_ptr<Widget> label;
		shared_ptr<Widget> clearbutton;
		std::vector<shared_ptr<Widget>> bindbutton;
		String action;
		String actionset;

		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		virtual bool ProcessEvent(const Event& e);
	public:
		bool savetoaxis;
		void WriteBinding();
		//virtual void SetLayout(const int left, const int right, const int top, const int bottom);
		friend shared_ptr<InputBindPanel> CreateInputBindPanel(const String& label, table table, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const String& set);
	};

	extern shared_ptr<InputBindPanel> CreateInputBindPanel(const String& label, table table, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const String& set = "");

	class InputSettingsPanel : public Widget
	{
		shared_ptr<Widget> panel;
		shared_ptr<Widget> label;
		shared_ptr<Widget> valuewidget;
		String setting;
		int type;

		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		virtual bool ProcessEvent(const Event& e);
	public:
		void WriteSetting();
		friend shared_ptr<InputSettingsPanel> CreateInputSettingsPanel(const String& label, table config, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
	};

	extern shared_ptr<InputSettingsPanel> CreateInputSettingsPanel(const String& label, table config, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent);
}