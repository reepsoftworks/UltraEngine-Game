# Ultra Game System

Application source code for turning any [Ultra Engine](https://www.ultraengine.com) project into what players expect from a modern game. This project is to ensure game developers have a stable foundation to build their next title on without worrying about the mundane backend features.

## What does this do?
- Window Management: Game windows can at anytime be resized or change modes (Fullscreen to Windowed, etc) with ease.
- Scene Management: Load, change, or clear a map from the world with a working loading screen.
- Input System: Dynamic, configurable and easy-to-use. The action based system for any type of game!
- Built-In Settings: Engine settings and keybindings are handled and can be easily changed with the the built-in settings window.
- Event Based Console: A simple but flexible console system for invoking additional functionality with components.
- Sound Managment: Load sound via JSON scripts with the GameSpeaker class.

This requires no additional dependencies outside the game engine and it's designed to be isolated within it's own folder. Getting started is easy! Run the "Install Template" batch file as administrator and the script will install (and update) to the engine's template directory. Then you'll be able to create a new project using the newly installed template.

*The default location is set to C:\Program Files\Ultra Engine. If you've installed Ultra Engine to a diffrent location, edit [ULTRAENGINEPATH] in the batch file to where you've installed the engine.*

**NOTE: You may get an error regarding the Component Preprocessor (error MSB3073: :VCEnd" exited with code 9009). Windows may be blocking the direct execution because it's seen as a foreign appm Manually execute the exe *Tools/Preprocessor.exe* and tell Windows UAC to trust it.**

You can also preform a manual install by drag and droping the "Game" folder (C++ Game/Source/Game) into your projects "Source" directory and add all files to your project. You'll also need to copy the "config.json" file as it's needed for the system to know the details of your application such as the name and appid (if applicable).

Then set your main.cpp file to look like the below:
```
#include "UltraEngine.h"
#include "Game/Game.h"
using namespace UltraEngine;
using namespace UltraEngine::Game;

// Components
#include "Components/Motion/Mover.hpp"
#include "Components/Player/CameraControls.hpp"
#include "Components/Player/FirstPersonControls.hpp"

// Define Components.
static void RegisterComponents()
{
    // Stock Components
    RegisterComponent<Mover>();
    RegisterComponent<CameraControls>();
    RegisterComponent<FirstPersonControls>();
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
    controller->SetAction("Quick Save", BUTTON_KEY_F5);
    controller->SetAction("Quick Load", BUTTON_KEY_F6);

    // Camera
    controller->SetAction("Camera", AXIS_MOUSE, "InGameControls");
    controller->SetActionSetCursor("InGameControls", CURSOR_NONE);

    // Movement
    ButtonAxis moveaxis =
    {
        BUTTON_KEY_W,
        BUTTON_KEY_S,
        BUTTON_KEY_A,
        BUTTON_KEY_D
    };
    controller->SetAction("Movement", moveaxis, "InGameControls");

    // Keybinds
    controller->SetAction("Use", BUTTON_KEY_E, "InGameControls");
    controller->SetAction("Jump", BUTTON_KEY_SPACE, "InGameControls");

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

## Support
This code is free to use for those who wish to develop and ship their commerical or non-commerical game projects using Ultra Engine. Pull requests and suggestions are all welcomed! 

Donations are appreciated, but not necessary via [Patreon](https://www.patreon.com/reepblue).
