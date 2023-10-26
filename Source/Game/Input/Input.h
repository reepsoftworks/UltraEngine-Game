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
#include "ButtonCode.h"

namespace UltraEngine::Game
{
	enum InputDevice
	{
		INPUT_KBM,
		INPUT_GAMEPAD
	};

	class Input : public Object
	{
		std::weak_ptr<UltraEngine::Window> window;

		bool anybuttonhit;
		ButtonCode lastbutton;
		bool suspend;
		std::map<ButtonCode, bool> buttondownstate;
		std::map<ButtonCode, bool> buttonhitstate;
		std::map<ButtonCode, bool> buttonreleasedstate;
		InputDevice device;

		void PumpButtonCodeDown(const ButtonCode code);
		void PumpButtonCodeUp(const ButtonCode code);
		void PumpMouseWheelDir(const int dir);

	public:
		Input();

		bool ButtonDown(const ButtonCode code);
		bool ButtonHit(const ButtonCode code);
		bool ButtonReleased(const ButtonCode code);
		bool ButtonAnyDown();
		bool ButtonAnyHit();
		Vec2 ButtonAxis(const ButtonCode& up, const ButtonCode& down, const ButtonCode& left, const ButtonCode& right);
		ButtonCode LastButtonPressed();

		Vec2 GetAxis(const AxisCode axis);
		virtual void SetCursorPosition(const int x, const int y);
		virtual void SetCursorPosition(iVec2 xy);
		iVec2 GetCursorPosition();

		void Flush();
		void Suspend(const bool mode);
		virtual bool ProcessEvent(const Event& e);
		void AttachWindow(const shared_ptr<UltraEngine::Window> window);
		shared_ptr<UltraEngine::Window> GetWindow();

		void SetDevice(InputDevice type);
		InputDevice GetDevice();

		static shared_ptr<Input> Create(shared_ptr<Window> window);
	};

	extern shared_ptr<Input> CreateInput(shared_ptr<Window> window);
}