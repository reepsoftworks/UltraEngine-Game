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

#ifdef _WIN32
#define DARK_MODE
#endif

using namespace UltraEngine;

#include "CustomEvents.h"

//Input
#include "Input/ButtonCode.h"
#include "Input/Input.h"
#include "Input/GameController.h"

//Sound
#include "Sound/SoundProfile.h"
#include "Sound/SoundFunctions.h"
#include "Sound/GameSpeaker.h"

//Custom widgets
#include "CustomWidgets/InputBindPanel.h"
#include "CustomWidgets/KeyBindButton.h"
#include "CustomWidgets/NumberSlider.h"
#include "CustomWidgets/ResoultionComboBox.h"
#include "CustomWidgets/ScrollPanel.h"
#include "CustomWidgets/Spinner.h"

//Utilities
#include "Utilities/CheckArgument.h"
#include "Utilities/MessageBox.h"
#include "Utilities/RandomNumber.h"
#include "Utilities/StringFunctions.h"
#include "Utilities/SystemTime.h"
#include "Utilities/Win32System.h"

//Game
#include "Stage.h"
#include "GameLuaBindings.h"
#include "GameObject.h"
#include "LoadingScreen.h"
#include "Canvas.h"

//Game Elements
#include "UIElement.h"
#include "UIElements/SplashScreen.h"
#include "UIElements/GraphicsWindow.h"
#include "UIElements/UIPanel.h"
#include "UIElements/UIWindow.h"

//CFLAGS
#define CFLAG_CMD "cmd"
#define CFLAG_CONSOLEWINDOW "console"
#define CFLAG_DEBUG "debug"
#define CFLAG_EDITOR "editor"
#define CFLAG_LUASCRIPT "luamain"
#define CFLAG_NOLOG "nolog"
#define CFLAG_NOSPLASH "nosplash"
#define CFLAG_SETTINGSAPP "settings"
#define CFLAG_VR "vr"
#define CFLAG_NOVR "novr"

//Commands
#define COMMAND_MAP "map"
#define COMMAND_CLEAR "clear"
#define COMMAND_LOAD "load"
#define COMMAND_SAVE "save"
#define COMMAND_RESTART "restart"
#define COMMAND_QUIT "quit"
#define COMMAND_SHOWDEBUG "d_show"
#define COMMAND_SHOWPHYSICS "d_physics"
#define COMMAND_WIREFRAME "d_wireframe"

//Settings (Also Commands)
#define SETTING_SCREENWIDTH "screenwidth"
#define SETTING_SCREENHEIGHT "screenheight"
#define SETTING_WINDOWMODE "windowmode"
#define SETTING_DISPLAY "display"
#define SETTING_WINDOWTHEME "windowtheme"
#define SETTING_LIGHTQUALITY "lightquality"
#define SETTING_TESSELLATION "tessellation"
#define SETTING_SSR "ssr"
#define SETTING_FOV "fov"
#define SETTING_HRTF "hrtf"

//Fallback Assets
#define ASSETFALLBACK_SOUND "Sound/error.wav"

//Render Layers
#define RENDERLAYER_NULL 0
#define RENDERLAYER_DEFAULT 1
#define RENDERLAYER_HIGHLIGHT 2
#define RENDERLAYER_HIGHLIGHT2D 4
#define RENDERLAYER_DEBUG 8
#define RENDERLAYER_VIEWMODEL 16
#define RENDERLAYER_HUD 1

//Options
#define OPTION_USE_CONSOLEWINDOW 1
#define OPTION_USE_VR 1

#ifdef DEBUG
#define IsDebug() true
#else
#define IsDebug() false
#endif

class InGameConsole;
namespace UltraEngine::Game
{
	class SplashScreen;
	class GraphicsWindow;
	class UIWindow;

	enum ProgramApp
	{
		PROGRAMAPP_GAME,
		PROGRAMAPP_SETTINGS,
		PROGRAMAPP_LUASCRIPT
	};

	class Program : public Object
	{
		bool vrenabled;
		bool InitializeEngineSettings();
	public:
		table config;
		shared_ptr<Stream> log;
		shared_ptr<BufferStream> consolelog;
		table commandline;
		WString title;
		WString author;
		WString version;
		std::vector<shared_ptr<Plugin>> plugins;
		std::vector<shared_ptr<Package>> packages;
		table settings;
		shared_ptr<Timer> debugtimer;
		shared_ptr<GameController> gamecontroller;
		WString controllerconfigpath;
		WString screenshotpath;

		shared_ptr<Display> maindisplay;
		shared_ptr<UIElement> mainapp;
		shared_ptr<Stage> stage;
		shared_ptr<UIWindow> console;

		bool luaapp;
		bool pausewhenunselected;
		bool consolemode;

		Program();

		bool Initialize(const bool fullload = false);
		bool LoadPlugins();
		bool LoadSettings();
		bool SaveSettings();
		bool SetSetting(const String& name, const int value);
		const int GetSetting(const String& name, const int defaultvalue = 0);
		bool LoadControllerBindings();
		bool SaveControllerBindings();

		bool LoadPackages();
		bool Launch();
		void Update();

		bool Shutdown();
		void Terminate(const bool force = true);
		bool ProcessEvent(const Event& e);
		static bool EventCallback(const Event& e, shared_ptr<Object> extra);
		bool Initialized;

		const shared_ptr<UltraEngine::Window> ActiveWindow();
		const iVec2 GetFramebufferSize();
		void Render(shared_ptr<World> world);
		void TakeScreenshot();
		void ResizeApp(const int width, const int height, const int style);
		bool VREnabled();

		WString GetGUITheme();
		shared_ptr<GameController> GetGameController();
		shared_ptr<Stage> GetStage();
	};

	extern MouseCursor currentcursor;
	extern shared_ptr<Program> GetProgram();
}