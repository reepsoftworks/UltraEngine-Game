#include "UltraEngine.h"
#include "RegisterComponents.h"
#include "Game/Game.h"
using namespace UltraEngine;
using namespace UltraEngine::Game;

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
    ButtonAxis moveaxis =
    {
        BUTTON_KEY_W,
        BUTTON_KEY_S,
        BUTTON_KEY_A,
        BUTTON_KEY_D
    };
    controller->SetAction("Movement", moveaxis, "InGameControls");
    controller->SetAction("Camera", AXIS_MOUSE, "InGameControls");

    controller->SetAction("Sprint", BUTTON_KEY_SHIFT, "InGameControls");
    controller->SetAction("Crouch", BUTTON_KEY_CONTROL, "InGameControls");
    controller->SetAction("Climb", BUTTON_KEY_Q, "InGameControls");
    controller->SetAction("Desent", BUTTON_KEY_E, "InGameControls");
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