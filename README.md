# Ultra Game System

Application source code for turning any [Ultra Engine](https://www.ultraengine.com) project into what players expect from a modern game. This project is to ensure game developers have a stable foundation to build their next title on without worrying about the mundane backend features.

## What does this do?
- Window Management: Game windows can at anytime be resized or change modes (Fullscreen to Windowed, etc) with ease.
- Scene Management: Load, change, or clear a map from the world with a working loading screen.
- Input System: Dynamic, configurable and easy-to-use. The action based system for any type of game!
- Built-In Settings: Engine settings and keybindings are handled and can be easily changed with the the built-in settings window.
- Event Based Console: A simple but flexible console system for invoking additional functionality with components.

This requires no additional dependencies outside the game engine and it's designed to be isolated within it's own folder. Getting started is easy! Drag and drop the "Game" folder (Source/Game) into your projects "Source" directory. You'll also need to copy the "config.json" file as it's needed for the system to know the details of your application such as the name and appid (if applicable).

Set your main.cpp file to look like the below:
```
#include "UltraEngine.h"
#include "Game/Game.h"
using namespace UltraEngine;
using namespace UltraEngine::Game;

// Components
#include "Components/Motion/Mover.hpp"
#include "Components/Player/CameraControls.hpp"

// Define Components.
static void RegisterComponents()
{
    RegisterComponent<Mover>();
    RegisterComponent<CameraControls>();
}

// Define default controls.
static void InstallControls(shared_ptr<GameController> controller)
{
    if (controller == NULL) return;

    // Program actions
    controller->SetAction("Pause", BUTTON_KEY_ESCAPE);
    controller->SetAction("Terminate", BUTTON_KEY_END);
    controller->SetAction("ConsoleApp", BUTTON_KEY_F1);
    controller->SetAction("Fullscreen", BUTTON_KEY_F11);
    controller->SetAction("Screenshot", BUTTON_KEY_F2);

    // Camera
    ButtonAxis moveaxis =
    {
        BUTTON_KEY_W,
        BUTTON_KEY_S,
        BUTTON_KEY_A,
        BUTTON_KEY_D
    };
    controller->SetAction("Movement", moveaxis, "InGameControls");
    controller->SetAction("Sprint", BUTTON_KEY_SHIFT, "InGameControls");
    controller->SetAction("Slowdown", BUTTON_KEY_CONTROL, "InGameControls");
    controller->SetAction("Climb", BUTTON_KEY_Q, "InGameControls");
    controller->SetAction("Desent", BUTTON_KEY_E, "InGameControls");
    controller->SetAction("Camera", AXIS_MOUSE, "InGameControls");

    // Settings
    controller->SetSetting("Raw Mouse", false);
    controller->SetSetting("Inverse Mouse", false);
    controller->SetSetting("Mouse Smoothing", 0.0f);
    controller->SetSetting("Mouse Look Speed", 1.0f);
}

int main(int argc, const char* argv[])
{
    // Register Components.
    RegisterComponents();

    // Init the program!
    auto app = GetProgram();
    app->commandline = ParseCommandLine(argc, argv);

    if (!app->Initialize(true))
        return 1;

    // Init the game controller.
    app->gamecontroller = CreateGameController();
    InstallControls(app->gamecontroller);
    if (!app->LoadControllerBindings())
    {
        Print("Warning: Failed to load controller bindings.");
    }

    // Launch the app.
    if (app->Launch() == false) return 1;

    // Install the loading screen.
    if (app->stage) app->stage->SetLoadingScreen<LoadingScreen>();

    // Main Loop
    bool running = true;
    while (running)
    {
        while (PeekEvent())
        {
            const auto e = WaitEvent();
            if (!app->ProcessEvent(e))
            {
                EmitEvent(EVENT_QUIT);
            }

            if (e.id == EVENT_QUIT)
            {
                if (app->Shutdown())
                {
                    if (!app->SaveSettings())
                    {
                        Print("Error: Failed to save program settings.");
                    }

                    running = false;
                    break;
                }
            }
        }

        app->Update();
    }

    // Write this line before we terminate!
    Print("Application terminated successfully!");

    return 0;
}
```

## Game Controller (Input)

The days of using Window::KeyHit() is over! Every input command is an action and can be seperated in action sets. 
```
			if (GetInput()->Down("Sprint"))
			{
				speed *= 10.0f;
			}
```

Keys like W, A, S and D can be assigned to an axis making your movement code compatible with joysticks if you chose to opt-in for gamepad support.
```
			const float speed = 10.0f;
			auto axis = GetInput()->Axis("Movement");
			entity->Move(axis.x * speed, 0, axis.y * speed);
```

You can seperate your actions into sets (Like running, swiming, flying, etc) and have "global" actions that will always register regardless on what action set is active. Changing action sets is as simple as using GameController::SetActiveSet().

## Components

Your components can be 100% isolated from the game system. However, consider using the included GameObject and GamePlayer classes when making your components.

The reasons for doing so are:
1. There is a new function called UpdateStage() which is called everytime the Stage is updated (When the game isn't paused). Update() still gets called by the engine regardless if the game is paused or not. Rule of thumb is to have Update() before input calls and UpdateStage() be for everything else.
2. Direct return functions for getting the Stage and Game Controller.
3. GamePlayer has code in ProcessEvent() to handle when the settings are changed.

Here's what the CameraControls.hpp can look like when intergrated with the GamePlayer class. 
```
#pragma once
#include "UltraEngine.h"
#include "../../Game/Game.h"

using namespace UltraEngine;
using namespace UltraEngine::Game;

class CameraControls : public GamePlayer
{
    bool freelookstarted{ false };
	Vec2 freelookmousepos;
	Vec2 lookchange;
	Vec3 freelookrotation;
	float mousesmoothing;
	float mouselookspeed;
public:
	bool lockmouse{ false };
	bool allowmovement{ true };
	float movespeed = 4.0f;
	float topangle = -90.0f;
	float bottomangle = 90.0f;

	CameraControls()
	{
		name = "CameraControls";
		mousesmoothing = 0.0f;
		mouselookspeed = 1.0f;
	}

	virtual void Start()
	{
		GamePlayer::Start();

		// Enable sound.
		GetEntity()->Listen();

		Listen(EVENT_PAUSESTATE, GetProgram());
		Listen(EVENT_GRAPHICSWINDOW, GetProgram());
		GetInput()->CenterCursor();
		GetInput()->SetCursorHidden(true);

		GetInput()->SetActiveSet("InGameControls");
	}

	void RawMouseLook()
	{
		auto entity = GetEntity();
		if (lockmouse)
		{
			freelookstarted = false;
			return;
		}

		if (!freelookstarted)
		{
			freelookstarted = true;
			freelookrotation = entity->GetRotation(true);
			freelookmousepos = GetInput()->Axis("Camera");
		}

		int inverse = 1;
		if (GetInput()->GetSettingBool("Inverse Mouse")) inverse = -1;

		auto newmousepos = GetInput()->Axis("Camera");
		lookchange.x = lookchange.x * mousesmoothing + (newmousepos.y - freelookmousepos.y) * 100.0f * mouselookspeed * (1.0f - mousesmoothing);
		lookchange.y = lookchange.y * mousesmoothing + (newmousepos.x - freelookmousepos.x) * 100.0f * mouselookspeed * (1.0f - mousesmoothing);
		if (Abs(lookchange.x) < 0.001f) lookchange.x = 0.0f;
		if (Abs(lookchange.y) < 0.001f) lookchange.y = 0.0f;
		if (lookchange.x != 0.0f or lookchange.y != 0.0f)
		{
			freelookrotation.x += lookchange.x * inverse;
			freelookrotation.x = Clamp(freelookrotation.x, topangle, bottomangle);
			freelookrotation.y += lookchange.y;
			entity->SetRotation(freelookrotation, true);
		}
		freelookmousepos = newmousepos;
	}

	void RelativeMouseLook()
	{
		if (lockmouse)
		{
			freelookstarted = false;
			return;
		}

		int inverse = 1;
		auto entity = GetEntity();
		if (GetInput()->GetSettingBool("Inverse Mouse")) inverse = -1;

		auto window = GetInput()->GetWindow();
		auto cx = Round((float)window->GetFramebuffer()->GetSize().x / 2);
		auto cy = Round((float)window->GetFramebuffer()->GetSize().y / 2);
		auto mpos = GetInput()->GetCursorFloatPosition();
		window->SetMousePosition(cx, cy);
		auto centerpos = GetInput()->GetCursorFloatPosition();

		if (freelookstarted)
		{
			float looksmoothing = mousesmoothing;
			float lookspeed = mouselookspeed / 10.0f;

			if (looksmoothing > 0.00f)
			{
				mpos.x = mpos.x * looksmoothing + freelookmousepos.x * (1 - looksmoothing);
				mpos.y = mpos.y * looksmoothing + freelookmousepos.y * (1 - looksmoothing);
			}

			auto dx = (mpos.x - centerpos.x) * lookspeed;
			auto dy = (mpos.y - centerpos.y) * lookspeed;

			freelookrotation.x = freelookrotation.x + dy * inverse;
			freelookrotation.x = Clamp(freelookrotation.x, topangle, bottomangle);
			freelookrotation.y = freelookrotation.y + dx;
			entity->SetRotation(freelookrotation, true);
			freelookmousepos = Vec2((float)mpos.x, (float)mpos.y);
		}
		else
		{
			freelookstarted = true;
			freelookrotation = entity->GetRotation(true);
			freelookmousepos = Vec2(GetInput()->GetCursorFloatPosition().x, GetInput()->GetCursorFloatPosition().y);
		}
	}

	virtual void UpdateStage()
	{
		auto window = GetInput()->GetWindow();
		if (window == NULL) return;

		auto entity = GetEntity();
		if (entity == NULL) return;

		// Camera look
		mousesmoothing = GetInput()->GetSettingFloat("Mouse Smoothing", mousesmoothing);
		mouselookspeed = GetInput()->GetSettingFloat("Mouse Look Speed", mouselookspeed);
		const bool rawmouse = GetInput()->GetSettingBool("Raw Mouse", true);
		if (rawmouse)
			RawMouseLook();
		else
			RelativeMouseLook();

		// Movement
		if (allowmovement)
		{
			float speed = movespeed / 60.0f;
			if (GetInput()->Down("Sprint"))
			{
				speed *= 10.0f;
			}
			else if (GetInput()->Down("Crouch"))
			{
				speed *= 0.25f;
			}

			if (GetInput()->Down("Climb")) entity->Translate(0, speed, 0);
			if (GetInput()->Down("Desent")) entity->Translate(0, -speed, 0);
			auto axis = GetInput()->Axis("Movement");
			entity->Move(axis.x * speed, 0, axis.y * speed);
		}
	}

	virtual void Update()
    	{
		if (GetInput()->Hit("Pause"))
		{
			GetStage()->Pause(!GetStage()->GetPaused());
		}

		GamePlayer::Update();
    	}

	//This method will work with simple components
	virtual shared_ptr<Component> Copy()
	{
		return std::make_shared<CameraControls>(*this);
	}

	virtual bool ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_PAUSESTATE)
		{
			const bool state = (bool)e.data;
			GetInput()->CenterCursor();
			GetInput()->SetCursorHidden(!state);
			freelookstarted = false;
		}
		else if (e.id == EVENT_GRAPHICSWINDOW)
		{
			GetInput()->Flush();
			GetInput()->CenterCursor();
			freelookstarted = false;
		}
		
		return GamePlayer::ProcessEvent(e);
	}

	virtual bool Load(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const LoadFlags flags)
	{
		if (properties["lockmouse"].is_boolean()) lockmouse = properties["lockmouse"];
		if (properties["allowmovement"].is_boolean()) allowmovement = properties["allowmovement"];
		if (properties["movespeed"].is_number()) movespeed = properties["movespeed"];
		if (properties["topangle"].is_number()) topangle = properties["topangle"];
		if (properties["bottomangle"].is_number()) bottomangle = properties["bottomangle"];
	
		return true;
	}
		
	virtual bool Save(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const SaveFlags flags)
	{
		properties["lockmouse"] = lockmouse;
		properties["allowmovement"] = allowmovement;
		properties["topangle"] = topangle;
		properties["bottomangle"] = bottomangle;
	
		return true;
	}
};
```

## Support
This code is free to use for those who wish to develop and ship their commerical or non-commerical projects using Ultra Engine. Pull requests and suggestions are all welcomed! 

Donations are appreciated, but not necessary via [Patreon](https://www.patreon.com/reepblue).
