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
	GameController::GameController()
	{
		activeset = "";
		input = std::make_shared<Input>();
	}

	void GameController::LoadConfig(const WString& filepath)
	{
		if (FileType(filepath) == 1)
		{
			config.clear();
			config = LoadTable(filepath);
		}
	}

	bool GameController::SaveConfig(const WString& filepath)
	{
		return SaveTable(config, filepath);
	}

	void GameController::RegisterActionSet(const String& setname)
	{
		if (!setname.empty())
		{
			auto exists = std::find(actionsets.begin(), actionsets.end(), setname) != actionsets.end();
			if (!exists) actionsets.push_back(setname);
		}
	}

	const bool GameController::IsRegisteredActionSet(const String& setname)
	{
		return std::find(actionsets.begin(), actionsets.end(), setname) != actionsets.end();
	}

	void GameController::SetDisabled(const bool mode)
	{
		input->Suspend(mode);
	}

	void GameController::SetAction(const String& actionname, ButtonCode button)
	{
		config[actionname] = button;
	}

	void GameController::SetAction(const String& actionname, ButtonCode button, const String& setname)
	{
		if (setname.empty())
		{
			config[actionname] = button;
			return;
		}

		RegisterActionSet(setname);
		config[setname][actionname] = button;
	}

	void GameController::SetAction(const String& actionname, std::vector<ButtonCode> buttons)
	{
		config[actionname] = {};
		for (const auto& p : buttons)
		{
			config[actionname].push_back(p);
		}
	}

	void GameController::SetAction(const String& actionname, std::vector<ButtonCode> buttons, const String& setname)
	{
		if (setname.empty())
		{
			SetAction(actionname, buttons);
			return;
		}

		RegisterActionSet(setname);
		config[setname][actionname] = {};
		for (const auto& p : buttons)
		{
			config[setname][actionname].push_back(p);
		}
	}

	void GameController::SetAction(const String& actionname, AxisCode axis)
	{
		if (config["axes"].is_null()) config["axes"] = {};
		config["axes"][actionname] = axis;
	}

	void GameController::SetAction(const String& actionname, AxisCode axis, const String& setname)
	{
		if (setname.empty())
		{
			SetAction(actionname, axis);
			return;
		}

		RegisterActionSet(setname);
		if (config[setname]["axes"].is_null()) config[setname]["axes"] = {};
		config[setname]["axes"][actionname] = axis;
	}

	void GameController::SetAction(const String& actionname, ButtonAxis axis)
	{
		if (config["axes"].is_null()) config["axes"] = {};

		config["axes"][actionname] = {};
		config["axes"][actionname].push_back(axis.up);
		config["axes"][actionname].push_back(axis.down);
		config["axes"][actionname].push_back(axis.left);
		config["axes"][actionname].push_back(axis.right);
	}

	void GameController::SetAction(const String& actionname, ButtonAxis axis, const String& setname)
	{
		if (setname.empty())
		{
			SetAction(actionname, axis);
			return;
		}

		RegisterActionSet(setname);
		if (config[setname]["axes"].is_null()) config[setname]["axes"] = {};

		config[setname]["axes"][actionname] = {};
		config[setname]["axes"][actionname].push_back(axis.up);
		config[setname]["axes"][actionname].push_back(axis.down);
		config[setname]["axes"][actionname].push_back(axis.left);
		config[setname]["axes"][actionname].push_back(axis.right);
	}

	void GameController::SetAction(const String& actionname, InputAction action, const String& setname)
	{
		if (setname.empty())
		{
			if (!action.buttons.empty())
			{
				if (action.buttons.size() == 1)
				{
					SetAction(actionname, action.buttons[0]);
				}
				else
				{
					SetAction(actionname, action.buttons);
				}
			}
			else if (action.axis != AXIS_INVAILD)
			{
				SetAction(actionname, action.axis);
			}
			else if (!action.btnaxis.empty())
			{
				SetAction(actionname, action.btnaxis);
			}
		}
		else
		{
			RegisterActionSet(setname);
			if (!action.buttons.empty())
			{
				if (action.buttons.size() == 1)
				{
					SetAction(actionname, action.buttons[0], setname);
				}
				else
				{
					SetAction(actionname, action.buttons, setname);
				}
			}
			else if (action.axis != AXIS_INVAILD)
			{
				SetAction(actionname, action.axis, setname);
			}
			else if (!action.btnaxis.empty())
			{
				SetAction(actionname, action.btnaxis, setname);
			}
		}
	}

	InputAction GameController::GetActionData(const String& actionname, const String& setname)
	{
		InputAction data;

		if (setname.empty())
		{
			if (config[actionname].is_integer())
			{
				int d = config[actionname];
				data.buttons.push_back((ButtonCode)d);
			}
			else if (config[actionname].is_array())
			{
				for (int n = 0; n < config[actionname].size(); ++n)
				{
					data.buttons.push_back((ButtonCode)n);
				}
			}
			else if (config["axes"][actionname].is_integer())
			{
				int d = config["axes"][actionname];
				data.axis = (AxisCode)d;
			}
			else if (config["axes"][actionname].is_array() && config["axes"][actionname].size() == 4)
			{
				auto v = config["axes"][actionname];
				int up = v[0];
				int down = v[1];
				int left = v[2];
				int right = v[3];

				data.btnaxis.up = (ButtonCode)up;
				data.btnaxis.down = (ButtonCode)down;
				data.btnaxis.left = (ButtonCode)left;
				data.btnaxis.right = (ButtonCode)right;
			}
		}
		else
		{
			RegisterActionSet(setname);
			if (config[setname][actionname].is_integer())
			{
				int d = config[setname][actionname];
				data.buttons.push_back((ButtonCode)d);
			}
			else if (config[setname][actionname].is_array())
			{
				for (int n = 0; n < config[setname][actionname].size(); ++n)
				{
					data.buttons.push_back((ButtonCode)n);
				}
			}
			else if (config[setname]["axes"][actionname].is_integer())
			{
				int d = config[setname]["axes"][actionname];
				data.axis = (AxisCode)d;
			}
			else if (config[setname]["axes"][actionname].is_array() && config[setname]["axes"][actionname].size() == 4)
			{
				auto v = config[setname]["axes"][actionname];
				int up = v[0];
				int down = v[1];
				int left = v[2];
				int right = v[3];

				data.btnaxis.up = (ButtonCode)up;
				data.btnaxis.down = (ButtonCode)down;
				data.btnaxis.left = (ButtonCode)left;
				data.btnaxis.right = (ButtonCode)right;
			}
		}

		return data;
	}

	void GameController::SetActiveSet(const String& setname)
	{
		if (setname != activeset)
		{
			input->Flush();
			activeset = setname;
		}
	}

	bool GameController::Hit(const String& actionname)
	{
		bool ret = false;

		// Setless Actions
		if (config[actionname].is_integer())
		{
			int button = config[actionname];
			ret = input->ButtonHit((ButtonCode)button);
		}
		else if (config[actionname].is_array())
		{
			for (int n = 0; n < config[actionname].size(); ++n)
			{
				int button = config[actionname][n];
				ret = input->ButtonHit((ButtonCode)button);

				if (ret) break;					
			}
		}

		// Actions within Sets
		if (!activeset.empty() && !ret)
		{
			if (config[activeset][actionname].is_integer())
			{
				int button = config[activeset][actionname];
				ret = input->ButtonHit((ButtonCode)button);
			}
			else if (config[activeset][actionname].is_array())
			{
				for (int n = 0; n < config[activeset][actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = input->ButtonHit((ButtonCode)button);

					if (ret) break;
				}
			}
		}
		return ret;
	}

	bool GameController::Down(const String& actionname)
	{
		bool ret = false;

		// Setless Actions
		if (config[actionname].is_integer())
		{
			int button = config[actionname];
			ret = input->ButtonDown((ButtonCode)button);
		}
		else if (config[actionname].is_array())
		{
			for (int n = 0; n < config[actionname].size(); ++n)
			{
				int button = config[actionname][n];
				ret = input->ButtonDown((ButtonCode)button);

				if (ret) break;
			}
		}

		// Actions within Sets
		if (!activeset.empty() && !ret)
		{
			if (config[activeset][actionname].is_integer())
			{
				int button = config[activeset][actionname];
				ret = input->ButtonDown((ButtonCode)button);
			}
			else if (config[activeset][actionname].is_array())
			{
				for (int n = 0; n < config[activeset][actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = input->ButtonDown((ButtonCode)button);

					if (ret) break;
				}
			}
		}
		return ret;
	}

	bool GameController::Released(const String& actionname)
	{
		bool ret = false;

		// Setless Actions
		if (config[actionname].is_integer())
		{
			int button = config[actionname];
			ret = input->ButtonReleased((ButtonCode)button);
		}
		else if (config[actionname].is_array())
		{
			for (int n = 0; n < config[actionname].size(); ++n)
			{
				int button = config[actionname][n];
				ret = input->ButtonReleased((ButtonCode)button);

				if (ret) break;
			}
		}

		// Actions within Sets
		if (!activeset.empty() && !ret)
		{
			if (config[activeset][actionname].is_integer())
			{
				int button = config[activeset][actionname];
				ret = input->ButtonReleased((ButtonCode)button);
			}
			else if (config[activeset][actionname].is_array())
			{
				for (int n = 0; n < config[activeset][actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = input->ButtonReleased((ButtonCode)button);

					if (ret) break;
				}
			}
		}
		return ret;
	}

	Vec2 GameController::Axis(const String& actionname)
	{
		Vec2 ret = Vec2(0);
		if (GetWindow() == NULL) return ret;

		// Setless Actions
		if (config["axes"][actionname].is_integer())
		{
			int code = config["axes"][actionname];
			ret = input->GetAxis((AxisCode)code);
		}
		else if (config["axes"][actionname].is_array() && config["axes"][actionname].size() == 4)
		{
			auto v = config["axes"][actionname];
			int up = v[0];
			int down = v[1];
			int left = v[2];
			int right = v[3];
			ret = input->ButtonAxis((ButtonCode)up, (ButtonCode)down, (ButtonCode)left, (ButtonCode)right);
		}

		// Actions within Sets
		if (!activeset.empty() && ret == Vec2(0))
		{
			if (config[activeset]["axes"][actionname].is_integer())
			{
				int code = config[activeset]["axes"][actionname];
				ret = input->GetAxis((AxisCode)code);
			}
			else if (config[activeset]["axes"][actionname].is_array() && config[activeset]["axes"][actionname].size() == 4)
			{
				auto v = config[activeset]["axes"][actionname];
				int up = v[0];
				int down = v[1];
				int left = v[2];
				int right = v[3];
				ret = input->ButtonAxis((ButtonCode)up, (ButtonCode)down, (ButtonCode)left, (ButtonCode)right);
			}
		}
		return ret;
	}

	const std::vector<String> GameController::GetActionSets()
	{
		return actionsets;
	}

	const table GameController::GetActionSetTable(const String& setname)
	{
		if (setname.empty())
		{
			return config[setname];
		}
		else
		{
			return config;
		}
	}

	void GameController::SetCursorPosition(const int x, const int y)
	{
		input->SetCursorPosition(x, y);
	}

	void GameController::SetCursorPosition(const iVec2 xy)
	{
		input->SetCursorPosition(xy);
	}

	void GameController::CenterCursor()
	{
		if (input->GetWindow() != NULL)
		{
			auto center = iVec2(input->GetWindow()->GetSize().x / 2, input->GetWindow()->GetSize().y / 2);
			input->SetCursorPosition(center.x, center.y);
		}
	}

	void GameController::SetCursorHidden(const bool hide)
	{
		if (input->GetWindow() != NULL)
		{
			if (hide)
			{
				currentcursor = CURSOR_NONE;
				input->GetWindow()->SetCursor(currentcursor);
			}
			else
			{
				currentcursor = CURSOR_DEFAULT;
				input->GetWindow()->SetCursor(currentcursor);
			}
		}
		else
		{
			Print("Warning: input->GetWindow() is null!");
		}
	}

	iVec2 GameController::GetCursorPosition()
	{
		return input->GetCursorPosition();
	}

	Vec2 GameController::GetCursorFloatPosition()
	{
		auto v = input->GetCursorPosition();
		return Vec2((float)v.x, (float)v.y);
	}


	void GameController::SetSetting(const String& setting, const int value)
	{
		config["settings"][setting] = value;
	}

	void GameController::SetSetting(const String& setting, const float value)
	{
		config["settings"][setting] = value;
	}

	void GameController::SetSetting(const String& setting, const double value)
	{
		config["settings"][setting] = value;
	}

	void GameController::SetSetting(const String& setting, const bool value)
	{
		config["settings"][setting] = value;
	}

	void GameController::SetSetting(const String& setting, const String& value)
	{
		config["settings"][setting] = value;
	}

	int GameController::GetSettingInt(const String& setting, const int defaultvalue)
	{
		if (config["settings"][setting].is_integer()) return config["settings"][setting];
		return defaultvalue;
	}

	float GameController::GetSettingFloat(const String& setting, const float defaultvalue)
	{
		if (config["settings"][setting].is_number()) return config["settings"][setting];
		return defaultvalue;
	}

	bool GameController::GetSettingBool(const String& setting, const bool defaultvalue)
	{
		if (config["settings"][setting].is_boolean()) return config["settings"][setting];
		return defaultvalue;
	}

	String GameController::GetSettingString(const String& setting, const String& defaultvalue)
	{
		if (config["settings"][setting].is_string()) return config["settings"][setting];
		return defaultvalue;
	}

	void GameController::AttachWindow(const shared_ptr<UltraEngine::Window> window)
	{
		input->AttachWindow(window);
	}

	shared_ptr<UltraEngine::Window> GameController::GetWindow()
	{
		return input->GetWindow();
	}

	void GameController::Flush()
	{
		Print("Debug: Flushed controller.");
		input->Flush();
	}

	bool GameController::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_STAGELOADED || e.id == EVENT_STAGECLEARED || e.id == EVENT_PAUSESTATE)
        {
            Flush();
        }

		return true;
	}

	static bool GameControllerCallback(const Event& e, shared_ptr<Object> extra)
	{
		return extra->As<GameController>()->ProcessEvent(e);
	}

	shared_ptr<GameController> CreateGameController()
	{
		auto gamecontroller = std::make_shared<GameController>();

		// Enable callbacks
		ListenEvent(EVENT_STAGELOADED, GetProgram(), GameControllerCallback, gamecontroller);
		ListenEvent(EVENT_STAGECLEARED, GetProgram(), GameControllerCallback, gamecontroller);
		ListenEvent(EVENT_PAUSESTATE, GetProgram(), GameControllerCallback, gamecontroller);

		return gamecontroller;
	}

	shared_ptr<GameController> LoadGameController(const WString& config)
	{
		auto gamecontroller = CreateGameController();
		gamecontroller->LoadConfig(config);
		return gamecontroller;
	}
}