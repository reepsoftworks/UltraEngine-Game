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
	class ConsoleWindow : public UIElement
	{
		shared_ptr<UltraEngine::Window> window;
		std::weak_ptr<UltraEngine::Window> parentwindow;
		shared_ptr<Interface> ui;
		String previousactionset;

		shared_ptr<Widget> textarea_log;
		shared_ptr<Widget> panel_entry;
		shared_ptr<Widget> textfield_entry;
		shared_ptr<Widget> button_send;

		std::size_t historyindex;
		std::vector<UltraEngine::WString> history;
		void ManageHistory(const int data);

		void BuildWindow(shared_ptr<UltraEngine::Window> parent);
	public:
		ConsoleWindow();
		virtual void SetHidden(const bool hide);
		virtual bool ProcessEvent(const Event& e);
		static shared_ptr<ConsoleWindow> Create(shared_ptr<UltraEngine::Window> parent);

		friend class Program;
	};

	extern shared_ptr<ConsoleWindow> CreateConsoleWindow(shared_ptr<UltraEngine::Window> parent = NULL);
}