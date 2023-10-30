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
	class KeyBindButton : public Widget
	{
		shared_ptr<Widget> button;
		WString prefix;
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		virtual bool ProcessEvent(const Event& e);

	public:
		int buttonassigned;
		WString pressmessage;
		virtual WString GetText(const bool selected = false);
		void Clear();
		friend shared_ptr<KeyBindButton> CreateKeyBindButton(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const int defaultbind = -1, WString prefix = "");
	};

	extern shared_ptr<KeyBindButton> CreateKeyBindButton(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const int defaultbind, WString prefix);
}