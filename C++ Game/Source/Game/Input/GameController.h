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
#include "Input.h"

namespace UltraEngine::Game
{
	struct InputAction
	{
		std::vector<ButtonCode> buttons;
		AxisCode axis = AXIS_INVAILD;
		ButtonAxis btnaxis =
		{
			BUTTON_INVAILD,
			BUTTON_INVAILD,
			BUTTON_INVAILD,
			BUTTON_INVAILD
		};
	};

	class GameController : public Object
	{
		String activeset;
		std::weak_ptr<UltraEngine::Window> window;
		shared_ptr<Input> input;

		std::vector<String> actionsets;
		void RegisterActionSet(const String& setname);
	public:
		table config;

		GameController();

		void LoadConfig(const WString& filepath);
		bool SaveConfig(const WString& filepath);

		virtual void SetAction(const String& actionname, ButtonCode button);
		virtual void SetAction(const String& actionname, ButtonCode button, const String& setname);
		virtual void SetAction(const String& actionname, std::vector<ButtonCode> buttons);
		virtual void SetAction(const String& actionname, std::vector<ButtonCode> buttons, const String& setname);
		virtual void SetAction(const String& actionname, AxisCode axis);
		virtual void SetAction(const String& actionname, AxisCode axis, const String& setname);
		virtual void SetAction(const String& actionname, ButtonAxis axis);
		virtual void SetAction(const String& actionname, ButtonAxis axis, const String& setname);
		virtual void SetAction(const String& actionname, InputAction action, const String& setname = "");
		InputAction GetActionData(const String& actionname, const String& setname = "");

		virtual void SetActiveSet(const String& setname);
		virtual bool Hit(const String& actionname);
		virtual bool Down(const String& actionname);
		virtual bool Released(const String& actionname);
		Vec2 Axis(const String& actionname);
		const std::vector<String> GetActionSets();
		const table GetActionSetTable(const String& setname);
		const bool IsRegisteredActionSet(const String& setname);
		void SetDisabled(const bool mode);

		virtual void SetCursorPosition(const int x, const int y);
		virtual void SetCursorPosition(const iVec2 xy);
		void CenterCursor();
		void SetCursorHidden(const bool hide);
		iVec2 GetCursorPosition();
		Vec2 GetCursorFloatPosition();

		virtual void SetSetting(const String& setting, const int value);
		virtual void SetSetting(const String& setting, const float value);
		virtual void SetSetting(const String& setting, const double value);
		virtual void SetSetting(const String& setting, const bool value);
		virtual void SetSetting(const String& setting, const String& value);

		int GetSettingInt(const String& setting, const int defaultvalue = 0);
		float GetSettingFloat(const String& setting, const float defaultvalue = 0.0f);
		bool GetSettingBool(const String& setting, const bool defaultvalue = false);
		String GetSettingString(const String& setting, const String& defaultvalue = "");

		void AttachWindow(const shared_ptr<UltraEngine::Window> window);
		shared_ptr<UltraEngine::Window> GetWindow();

		void Flush();
		bool ProcessEvent(const Event& e);
	};

	extern shared_ptr<GameController> CreateGameController();
	extern shared_ptr<GameController> LoadGameController(const WString& config);
}