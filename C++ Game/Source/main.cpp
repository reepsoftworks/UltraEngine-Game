#include "UltraEngine.h"
#include "ComponentSystem.h"
#include "Controls.h"
#include "Game/Game.h"
using namespace UltraEngine;
using namespace UltraEngine::Game;

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
        // Terminate the app if called.
        if (app->gamecontroller->Hit("Terminate")) app->Terminate(false);

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