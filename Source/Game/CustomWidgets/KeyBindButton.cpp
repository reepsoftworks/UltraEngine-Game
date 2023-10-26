//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "KeyBindButton.h"
#include "../Game.h"

namespace UltraEngine::Game
{
	bool KeyBindButton::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<KeyBindButton>();
		return elem->ProcessEvent(e);
	}

	bool KeyBindButton::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_WIDGETACTION)
		{
			const bool state = button->GetState();
			if (state)
			{
				button->Unlock();
				button->SetText(pressmessage);
				button->Disable();
				button->Lock();

				auto window = ActiveWindow();
				if (window)
				{
					window->FlushKeys();
					window->FlushMouse();
				}
			}

		}
		else if (e.id == EVENT_KEYDOWN || e.id == EVENT_MOUSEDOWN)
		{
			const bool state = button->GetState();
			if (state)
			{
				buttonassigned = e.data;
				button->Unlock();
				String text = Game::ButtonCodeToString((Game::ButtonCode)buttonassigned);
				button->SetText(prefix + text);
				button->Enable();
				button->SetState(false);

				auto window = ActiveWindow();
				if (window)
				{
					window->FlushKeys();
					window->FlushMouse();
				}
			}
		}
		else if (e.id == EVENT_MOUSEWHEEL)
		{
			const bool state = button->GetState();
			if (state)
			{
				if (e.data > 0)
				{
					buttonassigned = Game::BUTTON_MOUSE_WHEELDOWN;
				}
				else
				{
					buttonassigned = Game::BUTTON_MOUSE_WHEELUP;
				}

				button->Unlock();
				String text = Game::ButtonCodeToString((Game::ButtonCode)buttonassigned);
				button->SetText(prefix+text);
				button->Enable();
				button->SetState(false);

				auto window = ActiveWindow();
				if (window)
				{
					window->FlushKeys();
					window->FlushMouse();
				}
			}
		}

		return true;
	}

	void KeyBindButton::Clear()
	{
		buttonassigned = 0;
		button->Unlock();
		String text = Game::ButtonCodeToString((Game::ButtonCode)buttonassigned);
		button->SetText(prefix+text);
		button->Enable();
		button->SetState(false);
	}

	WString KeyBindButton::GetText(const bool selected)
	{
		return button->GetText(selected);
	}

	shared_ptr<KeyBindButton> CreateKeyBindButton(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const int defaultbind, WString prefix)
	{
		auto widget = std::make_shared<KeyBindButton>();
		widget->Initialize("", x, y, width, height, parent, 0);
		widget->buttonassigned = defaultbind;
		String text = Game::ButtonCodeToString((Game::ButtonCode)widget->buttonassigned);
		widget->prefix = prefix;
		widget->button = CreateButton(prefix+text, x, y, width, height, parent, BUTTON_TOGGLE);
		widget->button->SetRadius(0);
		widget->pressmessage = "Press button to bind.";
		
		ListenEvent(EVENT_WIDGETACTION, widget->button, KeyBindButton::EventCallback, widget);

		// Listen to window events from the program as we want this to still work if this is
		// used within the graphics window.
		ListenEvent(EVENT_KEYDOWN, Game::GetProgram(), KeyBindButton::EventCallback, widget);
		ListenEvent(EVENT_MOUSEDOWN, Game::GetProgram(), KeyBindButton::EventCallback, widget);
		ListenEvent(EVENT_MOUSEWHEEL, Game::GetProgram(), KeyBindButton::EventCallback, widget);

		return widget;
	}
}