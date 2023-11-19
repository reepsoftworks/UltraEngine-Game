//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "Game.h"

namespace UltraEngine::Game
{
    // This member is used by the game controller and the graphics window to fix a terrible bug
    // involving the cursor being reset when the window is rebuilt.
    MouseCursor currentcursor = CURSOR_DEFAULT;

    static void ExecuteDir(const WString& path)
    {
        auto dir = LoadDir(path);
        for (auto file : dir)
        {
            WString filepath = path + "/" + file;
            switch (FileType(filepath))
            {
            case 1:
                if (ExtractExt(file).Lower() == "lua")  RunScript(filepath);
                break;
            case 2:
                ExecuteDir(filepath);
                break;
            }
        }
    }

	bool Program::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
        auto program = extra->As<Program>();
		if (e.id == EVENT_PRINT)
		{
#ifdef _WIN32
            //https://learn.microsoft.com/en-us/windows/console/getstdhandle#handle-disposal
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            if (e.text.Left(8) == "Warning:")
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }
            else if (e.text.Left(6) == "Error:")
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }
            else if (e.text.Left(6) == "Debug:")
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
#endif
            if (program->consolelog != NULL)
            {
                program->consolelog->WriteWLine(e.text);
            }

            if (program->log != NULL)
            {
                auto time = TimeStamp();
                program->log->WriteLine("[" + time + "] " + e.text.ToString());
            }
		}

        return true;
	}

	Program::Program()
	{
        title = "Untitled";
        author = "Unknown";
        version = "0.0.0";

        config = NULL;
        log = NULL;
        commandline = NULL;
        plugins.clear();
        debugtimer = NULL;
        gamecontroller = NULL;
        console = NULL;
        controllerconfigpath.clear();
        screenshotpath.clear();

        maindisplay = NULL;
        mainapp = NULL;
        stage = NULL;
        luaapp = false;
        pausewhenunselected = false;
        consolemode = false;
        vrenabled = false;

        Initialized = false;
	}

	bool Program::Initialize(const bool fullload)
	{
        if (Initialized)
            return true;

        const bool editormode = CheckArgument(GetProgram()->commandline, CFLAG_EDITOR);

        // Start CMD window if flag is set and if we're on Windows.
#if defined (_WIN32)
        if (CheckArgument(GetProgram()->commandline, CFLAG_CMD) || editormode) CallCMDWindow();
#endif
		// Attach event hooks.
		ListenEvent(EVENT_PRINT, NULL, Program::EventCallback, Self());

        // Load the config.
        if (FileType("config.json") != 1)
        {
            Notify("Unable to locate config file!", "Error", true);
            Print("Error: Unable to locate config file!");
            return false;
        }

        config = LoadTable("config.json");
        Assert(!config.empty(), "Failed to load config.json!");

        title = config["appTitle"];
        author = config["appAuthor"];
        version = config["appVersion"];

        // Write Program Data Folder
        WString user_programdata_dir = GetPath(PATH_PROGRAMDATA);
        user_programdata_dir = RealPath(user_programdata_dir + "\\" + title);
        if (FileType(user_programdata_dir) != 2)
        {
            auto success = CreateDir(user_programdata_dir);
            if (!success)
            {
                Notify("Unable to create Program Data Folder!", "Error", true);
                Print("Error: Unable to create Program Data Folder!");
                return false;
            }
        }

        // Logging
        consolelog = CreateBufferStream();
        if (commandline[CFLAG_NOLOG].is_boolean() && commandline[CFLAG_NOLOG])
        {
            log = WriteFile(user_programdata_dir + "/" + title + ".log");
            if (!log) Print("Warning: Unable to create log file!");
        }

        Initialized = true;

        // Optional: Load Plugins, Packages, Settings, etc here to cut down the amount of code in main.
        if (fullload)
        {
            if (!LoadPlugins())
            {
                Print("Warning: Failed to successfully load all plugins!");
            }

            if (!LoadPackages())
            {
                Print("Warning: Failed to load one or more packages.");
            }

            if (!LoadSettings())
            {
                Print("Warning: Failed to load program settings.");
            }
        }

        const bool engineinit = InitializeEngineSettings();
        return Initialized && engineinit;
	}

    bool Program::InitializeEngineSettings()
    {
        Assert(!config.empty(), "Failed to load config.json!");

        // This is an interesting idea with terrible consiquences..
        // Allow the engine settings to be changed with the config file.
        // We're not going to save anything. This is so people who know what they 
        // are doing to have better access to the engine easier.
        // If you're one of these people... Hi! :)
        EngineSettings settings;

        if (!config["engine"].is_null())
        {
            auto i = config["engine"]["maxskeletons"];
            if (!i.is_null()) settings.maxskeletons = i;

            i = config["engine"]["maxcubemaps"];
            if (!i.is_null()) settings.maxcubemaps = i;

            i = config["engine"]["maxshadowmaps"];
            if (!i.is_null()) settings.maxshadowmaps = i;

            i = config["engine"]["maxtextures2d"];
            if (!i.is_null()) settings.maxtextures2d = i;

            i = config["engine"]["maxindices"];
            if (!i.is_null()) settings.maxindices = i;

            i = config["engine"]["maxvertices"];
            if (!i.is_null()) settings.maxvertices = i;

            i = config["engine"]["maxmaterials"];
            if (!i.is_null()) settings.maxmaterials = i;

            i = config["engine"]["usediscretegpu"];
            if (!i.is_null()) settings.usediscretegpu = i;

            // For VR, just look for vr as OpenVR might become OpenXR someday.
            i = config["engine"]["vr"];
            if (!i.is_null())
            {
                // Tell me we're a VR app!
                vrenabled = i;

                settings.openvr = vrenabled;
                //settings.openxr = vrenabled;
            }
        }

        // Ok, if VR isn't in the config, still allow VR to be toggled on for
        // hybrid applications.
        if (!vrenabled)
        {
            const bool forcevr = CheckArgument(commandline, CFLAG_VR);
            if (forcevr)
            {
                // Enable VR
                vrenabled = true;

                settings.openvr = vrenabled;
                //settings.openxr = vrenabled;   
            }
        }
        else
        {
            const bool disablevr = CheckArgument(commandline, CFLAG_NOVR);
            if (disablevr)
            {
                // Disable VR
                vrenabled = false;

                settings.openvr = vrenabled;
                //settings.openxr = vrenabled;   
            }
        }

        // Tell the engine to Initialize the settings.
        if (!UltraEngine::Initialize(settings)) return false;

        if (vrenabled) Print("Virtual Reality: Enabled!");

        return true;
    }

    bool Program::LoadPlugins()
    {
        if (!Initialized)
            return false;

        // Load all Plugins within the Plugins directory.
        WString plugins_path = "Plugins";
        if (config["pluginsPath"].is_string()) plugins_path = config["pluginsPath"];
        if (!config["defaultSettings"]["pluginFiles"].is_null() && config["defaultSettings"]["pluginFiles"].is_array())
        {
            for (const auto& p : config["defaultSettings"]["pluginFiles"])
            {
                String s = p.second;
                auto plgin = LoadPlugin(plugins_path + "/" + s);
                if (plgin) plugins.push_back(plgin);
            }

            return (plugins.size() == config["defaultSettings"]["pluginFiles"].size());
        }

        return true;
    }
    
    bool Program::LoadPackages()
    {
        if (!Initialized)
            return false;

        // Load all packages provided.
        if (!config["defaultSettings"]["packageFiles"].is_null() && config["defaultSettings"]["packageFiles"].is_array())
        {
            for (const auto& p : config["defaultSettings"]["packageFiles"])
            {
                String s = p.second;
                auto pak = LoadPackage(s);
                if (pak) packages.push_back(pak);
            }

            return (packages.size() == config["defaultSettings"]["packageFiles"].size());
        }

        return true;
    }

    bool Program::LoadSettings()
    {
        if (!Initialized)
            return false;

        WString settingsfile = RealPath(GetPath(PATH_PROGRAMDATA) + "\\" + title) + "/settings.json";
        if (FileType(settingsfile) == 0) return true;
        Print("Loading settings file \"" + settingsfile + "\"");
        settings = LoadTable(settingsfile);
        return !settings.empty();
    }

    bool Program::SaveSettings()
    {
        if (!Initialized)
            return false;

        if (settings.empty()) return true;
        WString settingsfile = RealPath(GetPath(PATH_PROGRAMDATA) + "\\" + title) + "/settings.json";
        Print("Writing settings file \"" + settingsfile + "\"");
        return SaveTable(settings, settingsfile);
    }

    bool Program::SetSetting(const String& name, const int value)
    {
        if (!Initialized)
            return false;

        settings[name] = value;
        return EmitEvent(EVENT_SETTINGCHANGED, Self(), value, 0, 0, 0, 0, 0, name);
    }

    const int Program::GetSetting(const String& name, const int defaultvalue)
    {
        if (!Initialized)
            return false;

        if (settings.is_null())
            LoadSettings();

        if (settings[name].is_null()) return defaultvalue;
        return settings[name];
    }

    bool Program::LoadControllerBindings()
    {
        if (!Initialized)
            return false;

        // if the path is null, generate a default one!
        if (controllerconfigpath.empty()) controllerconfigpath = RealPath(GetPath(PATH_PROGRAMDATA) + "\\" + title) + "/controls.json";
        if (FileType(controllerconfigpath) != 1) return false;
        if (gamecontroller) gamecontroller->LoadConfig(controllerconfigpath);
        return true;
    }

    bool Program::SaveControllerBindings()
    {
        if (!Initialized)
            return false;

        bool ret = false;
        // if the path is null, generate a default one!
        if (controllerconfigpath.empty()) controllerconfigpath = RealPath(GetPath(PATH_PROGRAMDATA) + "\\" + title) + "/controls.json";
        if (gamecontroller) ret = gamecontroller->SaveConfig(controllerconfigpath);
        return ret;
    }

    bool Program::Launch()
    {
        // Tell the user we need a game controller in-order to continue!
        Assert(gamecontroller != NULL, "No game controller detected. Please create class before Program::LaunchApp()!!");

        // Bind Lua
        auto L = GetLuaState();
        InitGameLuaBindings(L);

        // Set the correct program app type based on the commandline.
        ProgramApp programtype = PROGRAMAPP_GAME;
        if (commandline[CFLAG_SETTINGSAPP] == true)
        {
            programtype = PROGRAMAPP_SETTINGS;
        }
        else if (commandline[CFLAG_LUASCRIPT] == true)
        {
            programtype = PROGRAMAPP_LUASCRIPT;
        }

        // Load window settings
        int screenwidth = GetSetting(SETTING_SCREENWIDTH, 1280);
        int screenheight = GetSetting(SETTING_SCREENHEIGHT, 768);
        int windowmode = GetSetting(SETTING_WINDOWMODE, 0);
        int displayindex = GetSetting(SETTING_DISPLAY, 0);
        consolemode = CheckArgument(commandline, CFLAG_CONSOLEWINDOW);

        // Load editor settings.
        if (CheckArgument(GetProgram()->commandline, CFLAG_EDITOR))
        {
            auto editorsettingsfile = RealPath(GetPath(PATH_PROGRAMDATA) + "\\" + title) + "/editorsettings.json";

            // Create editor settings if none exists!
            if (FileType(editorsettingsfile) != 1)
            {
                table f = table();
                f["window"] = {};
                f["window"]["display"] = 0;
                f["window"]["screenwidth"] = 1280;
                f["window"]["screenheight"] = 768;
                f["window"]["windowmode"] = 0;
                f["window"]["forceconsole"] = true;
                SaveTable(f, editorsettingsfile);
                f = NULL;
            }

            auto editorsettings = LoadTable(editorsettingsfile);
            if (!editorsettings.is_null())
            {
                // if we're launched with the editor flag, override window settings with this.
                if (editorsettings["window"].is_object())
                {
                    Print("Forcing editor settings!");

                    if (editorsettings["window"]["display"].is_integer()) displayindex = editorsettings["window"]["display"];
                    if (editorsettings["window"]["screenwidth"].is_integer()) screenwidth = editorsettings["window"]["screenwidth"];
                    if (editorsettings["window"]["screenheight"].is_integer()) screenheight = editorsettings["window"]["screenheight"];
                    if (editorsettings["window"]["windowmode"].is_integer()) windowmode = editorsettings["window"]["windowmode"];
                    if (editorsettings["window"]["forceconsole"].is_boolean()) consolemode = editorsettings["window"]["forceconsole"];
                }
            }
            editorsettings = NULL;
        }

        // Init the display
        if (maindisplay == NULL)
        {
            // Init Display
            auto displays = GetDisplays();
            maindisplay = displays[displayindex];
            if (maindisplay == NULL)
            {
                Print("Warning: Unable to detect display index " + WString(displayindex));

                // Force setting the display to 0.
                displayindex = 0;
                maindisplay = displays[displayindex];

                // Try again...
                if (maindisplay == NULL)
                {
                    MessageError("Unable to detect display index " + WString(displayindex), "Error");
                    return false;
                }
            }

            Print("Target Display: " + WString(displays[displayindex]->GetSize()) + " at scale " + WString(displays[displayindex]->GetScale()));
        }

        if (programtype == PROGRAMAPP_GAME)
        {
            // Create graphics window
            mainapp = CreateGraphicsWindow(title, screenwidth, screenheight, maindisplay, (GraphicWindowStyles)windowmode);
            Assert(mainapp != NULL);
            auto game = mainapp->As<GraphicsWindow>();
            gamecontroller->AttachWindow(game->window);

#ifdef OPTION_USE_CONSOLEWINDOW
            // Create Console Window
            if (consolemode)
            {
                console = CreateConsoleWindow(game->window);
            }
#endif

            // Create stage
            stage = CreateStage();

        }
        else if (programtype == PROGRAMAPP_SETTINGS)
        {
            mainapp = CreateSettingWindow();
             
        }
        else if (programtype == PROGRAMAPP_LUASCRIPT)
        {
            if (FileType("Scripts/Main.lua") == 0)
            {
                MessageError("Unable to detect " + QuoteWString("Scripts/Main.lua"), "Error");
                return false;
            }

            luaapp = true;
        }

        if (!luaapp) Assert(mainapp != NULL, "No main application was created!");

        EmitEvent(EVENT_PROGRAMSTART, GetProgram());

        return true;
    }

    void Program::Update()
    {
        if (mainapp != NULL)
        {
            auto game = mainapp->As<GraphicsWindow>();
            if (game != NULL)
            {
                // Console App
                if (gamecontroller->Hit("Console") && console != NULL)
                {
                    // Show the console.
                    console->SetHidden(false);
                }

                if (gamecontroller->Hit("Screenshot"))
                {
                    TakeScreenshot();
                }

                if (gamecontroller->Hit("Fullscreen"))
                {
                    static bool fullscreen = false;
                    if (!fullscreen)
                    {
                        ResizeApp(1920, 1080, GRAPHICSWINDOW_FULLSCREEN);
                    }
                    else
                    {
                        ResizeApp(1280, 768, GRAPHICSWINDOW_TITLEBAR);
                    }

                    fullscreen = !fullscreen;
                }

                auto caps = game->framebuffer->GetCaptures();
                if (!caps.empty())
                {
                    for (auto pixmap : caps)
                    {
                        if (screenshotpath.empty()) screenshotpath = RealPath(GetPath(PATH_DOCUMENTS) + "/" + title) + "/Screenshots";
                        if (FileType(screenshotpath) != 1) CreateDir(screenshotpath, true);

                        // Timestamp
                        time_t now = time(0);
                        struct tm tstruct;
                        char buf[80];
                        tstruct = *localtime(&now);
                        strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
                        std::string result = buf;

                        auto path = screenshotpath + "/screenshot_" + result + ".jpg";
                        pixmap->Save(path);
                    }
                }

                // Save/Load
                if (gamecontroller->Hit("Quick Save"))
                {
                    auto quicksavepath = RealPath(GetPath(PATH_PROGRAMDATA) + "/" + GetProgram()->title) + "/Save";
                    if (FileType(quicksavepath) != 2)
                    {
                        Print("Debug: Creating save directory.");
                        CreateDir(quicksavepath, true);
                    }

                    if (stage->Save(quicksavepath + "/quick.sav"))
                    {
                        Print("Game saved.");
                    }
                }

                if (gamecontroller->Hit("Quick Load"))
                {
                    auto quickloadpath = RealPath(GetPath(PATH_PROGRAMDATA) + "/" + GetProgram()->title) + "/Save/quick.sav";
                    if (FileType(quickloadpath) != 0)
                    {
                        if (stage->Load(quickloadpath))
                        {
                            Print("Loaded quick save.");
                        }
                    }
                }

                if (stage != NULL)
                {
                    // Update Stage 
                    stage->Update();
                }
            }
        }
    }

    bool Program::Shutdown()
    {
        if (!plugins.empty())
        {
            Print("Unloading plugins.");
            plugins.clear();
        }

        if (log != NULL)
        {
            log->Close();
            log = NULL;
        }

        return true;
    }

    void Program::Terminate(const bool force)
    {
        if (stage)
        {
            if (!force)
            {
                auto gamecontroller = GetProgram()->GetGameController();
                String current_set = "";
                if (gamecontroller)
                {
                    current_set = gamecontroller->GetActiveSet();
                    gamecontroller->SetActiveSet("");
                }

                auto answer = MessageContinue("Terminate", "Do you wish to terminate this application?");
                if (answer == MESSAGE_ID_YES)
                {
                    EmitEvent(EVENT_QUIT);
                }
                else
                {
                    if (gamecontroller)
                    {
                        gamecontroller->SetActiveSet(current_set);
                        current_set.clear();
                    }
                }
            }
            else
            {
                EmitEvent(EVENT_QUIT);
            }
  
        }
        else
        {
            if (!force)
            {
                auto answer = MessageContinue("Terminate", "Do you wish to terminate this application?");
                if (answer == MESSAGE_ID_YES)
                {
                    EmitEvent(EVENT_QUIT);
                }
            }
            else
            {
                EmitEvent(EVENT_QUIT);
            }
        }
    }

    bool Program::ProcessEvent(const Event& e)
    {
        if (e.id == EVENT_PROGRAMSTART)
        {
            if (CheckArgument(commandline, CFLAG_DEBUG))
            {
                RunScript("Scripts/System/Debugger.lua");
                debugtimer = CreateTimer(510);
                ListenEvent(EVENT_TIMERTICK, debugtimer, std::bind(PollDebugger, 500));
            }

            // Enable the entity component system
            RunScript("Scripts/System/ComponentSystem.lua");

            // Run user start scripts
            ExecuteDir("Scripts/Start");

            // If we're told to run as a lua app, execute the Main.lua script!
            if (luaapp) RunScript("Scripts/Main.lua");
        }
        else if (e.id == EVENT_GRAPHICSWINDOW)
        {
            // After a new graphic window has been built, reasign the controller's window pointer.
            // The input system needs a window pointer to reference for input states and cursor modifications.
            auto window = e.extra->As<Window>();
            if (gamecontroller != NULL && window != NULL) gamecontroller->AttachWindow(window);
        }
        else if (e.id == EVENT_CONSOLEEXECUTE)
        {
            // The Quit command...
            auto line = e.text.Split(" ");
            auto cmd = line[0].ToString();
            if (cmd == COMMAND_QUIT) Terminate();
        }
        else if (e.id == EVENT_WINDOWDESELECT)
        {
            if (e.source == this->ActiveWindow() && pausewhenunselected)
            {
                // Pause the stage if we lost focus.
                if (stage)
                {
                    if (!stage->GetPaused() && stage->InScene())
                    {
                        Print("Debug: Paused the application as the window lost focus.");
                        stage->Pause(true);
                    }
                }
            }
        }
        else if (e.id == EVENT_KEYDOWN || e.id == EVENT_KEYUP || e.id == EVENT_MOUSEDOWN || e.id == EVENT_MOUSEUP || e.id == EVENT_MOUSEWHEEL || e.id == EVENT_WINDOWDPICHANGED)
        { 
            // Re-emit window events with us being the source.
            // This is because the graphic window can get deleted and rebuilt at anytime and some custom widgets rely on these.
            if (e.source != Self()) EmitEvent(e.id, Self(), e.data, e.position.x, e.position.y, e.size.x, e.size.y, e.extra, e.text);
        }

        // Process Events
        bool ret = false;
        if (mainapp != NULL)
        {
            ret = mainapp->ProcessEvent(e);
        }
        return ret;
    }

    const shared_ptr<UltraEngine::Window> Program::ActiveWindow()
    {
        if (mainapp != NULL)
        {
            auto game = mainapp->As<GraphicsWindow>();
            if (game) return game->window;
        }
        return UltraEngine::ActiveWindow();
    }

    const iVec2 Program::GetFramebufferSize()
    {
        auto ret = iVec2(0);
        if (mainapp != NULL)
        {
            auto game = mainapp->As<GraphicsWindow>();
            if (game) ret = game->GetFramebufferSize();
        }
        return ret;
    }

    void Program::Render(shared_ptr<World> world)
    {
        if (mainapp != NULL)
        {
            auto game = mainapp->As<GraphicsWindow>();
            if (game)
            {
                game->Render(world);
            }
        }
    }

    void Program::TakeScreenshot()
    {
        if (mainapp != NULL)
        {
            auto game = mainapp->As<GraphicsWindow>();
            if (game) game->framebuffer->Capture();
        }
    }

    //void Program::ToggleConsole(/*const bool show*/)
    //{
    //    // Console App
    //    if (gamecontroller->Hit("Console"))
    //    {
    //        if (gameconsole.lock() != NULL)
    //        {
    //            auto hiddenstate = gameconsole.lock()->GetHidden();
    //            gameconsole.lock()->SetHidden(!hiddenstate);
    //        }
    //        else
    //        {
    //            // Show the console window.
    //            if (console) console->SetHidden(false);
    //        }
    //    }
    //}

    void Program::ResizeApp(const int width, const int height, const int style)
    {
        EmitEvent(EVENT_WINDOWSIZE, Self(), style, 0, 0, width, height, maindisplay);
    }

    bool Program::VREnabled()
    {
        return vrenabled;
    }

    WString Program::GetGUITheme()
    {
        WString theme = "UI/Color Schemes/dark.json";
       
        bool lightmode = false;
        const TitlebarTheme themesetting = (TitlebarTheme)GetProgram()->GetSetting(SETTING_WINDOWTHEME, (int)TITLEBAR_SYSTEM);
        if (themesetting == TITLEBAR_DEFAULT)
            lightmode = true;
        else if (themesetting == TITLEBAR_SYSTEM)
            lightmode = LightThemeEnabled();

        if (lightmode == false)
        {
            if (config["colorSchemesPath"].is_string() && config["defaultSettings"]["darkColorScheme"].is_string())
            {
                theme = String(config["colorSchemesPath"]) + "/" + String(config["defaultSettings"]["darkColorScheme"]);
            }
        }
        else
        {
            if (config["colorSchemesPath"].is_string() && config["defaultSettings"]["lightColorScheme"].is_string())
            {
                theme = String(config["colorSchemesPath"]) + "/" + String(config["defaultSettings"]["lightColorScheme"]);   
            }
        }

        theme = FixPath(theme);
        return theme;
    }

    shared_ptr<GameController> Program::GetGameController()
    {
        return gamecontroller;
    }

    shared_ptr<Stage> Program::GetStage()
    {
        return stage;
    }

    shared_ptr<Program> GetProgram()
    {
        static shared_ptr<UltraEngine::Game::Program> game = std::make_shared<Program>();
        return game;
    }
}