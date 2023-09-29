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

For a deeper insight on everything, check out the Wiki!

## Support
This code is free to use for those who wish to develop and ship their commerical or non-commerical projects using Ultra Engine. Pull requests and suggestions are all welcomed! 

Donations are appreciated, but not necessary via [Patreon](https://www.patreon.com/reepblue).
