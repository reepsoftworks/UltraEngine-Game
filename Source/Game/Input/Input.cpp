//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "../Game.h"

namespace UltraEngine::Game
{
	static bool InputEventCallback(const Event& e, shared_ptr<Object> extra)
	{
		return extra->As<Input>()->ProcessEvent(e);
	}

	Input::Input()
	{
		buttondownstate.clear();
		buttonhitstate.clear();
		buttonreleasedstate.clear();
		anybuttonhit = false;
		lastbutton = BUTTON_INVAILD;
		suspend = false;
		device = INPUT_KBM;

		auto BUTTON_LAST = BUTTON_MOUSE_WHEELRIGHT;
		for (int i = 0; i < BUTTON_LAST + 1; i++)
		{
			auto code = (ButtonCode)i;
			buttonreleasedstate[code] = true;
		}
	}

	void Input::PumpButtonCodeDown(const ButtonCode code)
	{
		if (GetWindow() == NULL) return;

		bool actually_down = false;
		if (code < BUTTON_FIRST_KEY)
			actually_down = GetWindow()->MouseDown(MouseButton(code));
		else
			actually_down = GetWindow()->KeyDown(KeyCode(code));

		buttondownstate[code] = actually_down;
		buttonhitstate[code] = actually_down;
		buttonreleasedstate[code] = !actually_down;
		anybuttonhit = actually_down;
		if (lastbutton != code) lastbutton = code;
	}

	void Input::PumpButtonCodeUp(const ButtonCode code)
	{
		if (GetWindow() == NULL) return;

		bool actually_up = false;
		if (code < BUTTON_FIRST_KEY)
			actually_up = GetWindow()->MouseDown(MouseButton(code));
		else
			actually_up = GetWindow()->KeyDown(KeyCode(code));

		buttondownstate[code] = actually_up;
		buttonhitstate[code] = actually_up;
		//buttonreleasedstate[code] = !actually_up; // Handled in ButtonReleased()
		anybuttonhit = actually_up;
	}

	void Input::PumpMouseWheelDir(const int dir)
	{
		// Reset the mousewheel.
		if (buttondownstate[BUTTON_MOUSE_WHEELDOWN])
		{
			buttondownstate[BUTTON_MOUSE_WHEELDOWN] = false;
			buttonhitstate[BUTTON_MOUSE_WHEELDOWN] = false;
			anybuttonhit = false;
		}

		if (buttondownstate[BUTTON_MOUSE_WHEELUP])
		{
			buttondownstate[BUTTON_MOUSE_WHEELUP] = false;
			buttonhitstate[BUTTON_MOUSE_WHEELUP] = false;
			anybuttonhit = false;
		}

		float fixed_dir = Sign(static_cast<float>(dir));
		if (static_cast<float>(fixed_dir) != 0)
		{
			if (fixed_dir > 0)
			{
				if (!buttondownstate[BUTTON_MOUSE_WHEELDOWN])
				{
					buttondownstate[BUTTON_MOUSE_WHEELDOWN] = true;
					buttonhitstate[BUTTON_MOUSE_WHEELDOWN] = true;
					anybuttonhit = true;
					if (lastbutton != BUTTON_MOUSE_WHEELDOWN) lastbutton = BUTTON_MOUSE_WHEELDOWN;
				}
			}
			else if (fixed_dir < 0)
			{
				if (!buttondownstate[BUTTON_MOUSE_WHEELUP])
				{
					buttondownstate[BUTTON_MOUSE_WHEELUP] = true;
					buttonhitstate[BUTTON_MOUSE_WHEELUP] = true;
					anybuttonhit = true;
					if (lastbutton != BUTTON_MOUSE_WHEELUP) lastbutton = BUTTON_MOUSE_WHEELUP;
				}
			}
		}
	}

	bool Input::ButtonDown(const ButtonCode code)
	{
		return buttondownstate[code];
	}

	bool Input::ButtonHit(const ButtonCode code)
	{
		const bool is_button_down = buttonhitstate[code];
		buttonhitstate[code] = false;
		return is_button_down;
	}

	bool Input::ButtonReleased(const ButtonCode code)
	{
		const bool is_button_down = buttondownstate[code];
		const bool is_button_up = buttonreleasedstate[code];

		if (!is_button_down && !is_button_up)
		{
			buttonreleasedstate[code] = true;
			return true;
		}

		return false;
	}

	bool Input::ButtonAnyDown()
	{
		return std::any_of(buttondownstate.begin(), buttondownstate.end(),
			[](const auto& p) { return p.second; });
	}

	bool Input::ButtonAnyHit()
	{
		const bool is_button_down = anybuttonhit;
		anybuttonhit = false;
		return is_button_down;
	}

	Vec2 Input::ButtonAxis(const ButtonCode& up, const ButtonCode& down, const ButtonCode& left, const ButtonCode& right)
	{
		const bool bUp = ButtonDown(up);
		const bool bDwn = ButtonDown(down);
		const bool bLft = ButtonDown(left);
		const bool bRt = ButtonDown(right);
		return Vec2(static_cast<float>(bRt - bLft), static_cast<float>(bUp - bDwn));
	}

	ButtonCode Input::LastButtonPressed()
	{
		return lastbutton;
	}

	Vec2 Input::GetAxis(const AxisCode axis)
	{
		Vec2 ret = Vec2(0);
		if (GetWindow() == NULL) return ret;
		if (axis == AXIS_MOUSE)
		{
			auto mouseaxis = GetWindow()->GetMouseAxis();
			ret = Vec2(mouseaxis.x, mouseaxis.y);
		}

		return ret;
	}

	void Input::SetCursorPosition(const int x, const int y)
	{
		if (device == INPUT_KBM)
		{
			if (GetWindow() != NULL)
			{
				GetWindow()->FlushMouse();
				GetWindow()->SetMousePosition(x, y);
			}
		}
	}

	void Input::SetCursorPosition(iVec2 xy)
	{
		if (GetWindow() != NULL)
		{
			GetWindow()->FlushMouse();
			GetWindow()->SetMousePosition(xy.x, xy.y);
		}
	}

	iVec2 Input::GetCursorPosition()
	{
		if (GetWindow() == NULL) return iVec2(0);
		auto mousepos = GetWindow()->GetMousePosition();
		return iVec2(mousepos.x, mousepos.y);
	}

	void Input::Flush()
	{
		buttondownstate.clear();
		buttonhitstate.clear();
		anybuttonhit = false;

		if (GetWindow() != NULL)
		{
			GetWindow()->FlushKeys();
			GetWindow()->FlushMouse();
		}	
	}

	void Input::Suspend(const bool mode)
	{
		suspend = mode;
	}

	void Input::AttachWindow(const shared_ptr<UltraEngine::Window> window)
	{
		this->window = window;
		ListenEvent(EVENT_KEYDOWN, this->window.lock(), InputEventCallback, Self());
		ListenEvent(EVENT_KEYUP, this->window.lock(), InputEventCallback, Self());
		ListenEvent(EVENT_MOUSEWHEEL, this->window.lock(), InputEventCallback, Self());
		ListenEvent(EVENT_MOUSEDOWN, this->window.lock(), InputEventCallback, Self());
		ListenEvent(EVENT_MOUSEUP, this->window.lock(), InputEventCallback, Self());
	}

	shared_ptr<UltraEngine::Window> Input::GetWindow()
	{
		return window.lock();
	}

	bool Input::ProcessEvent(const Event& e)
	{
		if (!suspend)
		{
			if (e.id == EVENT_KEYDOWN)
			{
				SetDevice(INPUT_KBM);
				PumpButtonCodeDown((ButtonCode)e.data);
			}
			else if (e.id == EVENT_KEYUP)
			{
				SetDevice(INPUT_KBM);
				PumpButtonCodeUp((ButtonCode)e.data);
			}
			else if (e.id == EVENT_MOUSEWHEEL)
			{
				SetDevice(INPUT_KBM);
				PumpMouseWheelDir(e.data);
			}
			else if (e.id == EVENT_MOUSEDOWN)
			{
				SetDevice(INPUT_KBM);
				PumpButtonCodeDown((ButtonCode)e.data);
			}
			else if (e.id == EVENT_MOUSEUP)
			{
				SetDevice(INPUT_KBM);
				PumpButtonCodeUp((ButtonCode)e.data);
			}
			else if (e.id == EVENT_MOUSEMOVE)
			{
				SetDevice(INPUT_KBM);
			}
		}

		return true;
	}

	void Input::SetDevice(InputDevice type)
	{
		if (device != type)
		{
			EmitEvent(EVENT_INPUTDEVICE, GetProgram(), (int)type);
			device = type;
		}
	}

	InputDevice Input::GetDevice()
	{
		return device;
	}

	shared_ptr<Input> Input::Create(shared_ptr<Window> window)
	{
		shared_ptr<Input> input = std::make_shared<Input>();
		input->AttachWindow(window);
		return input;
	}

	shared_ptr<Input> CreateInput(shared_ptr<Window> window)
	{
		return Input::Create(window);
	}
}