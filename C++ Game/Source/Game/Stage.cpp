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
	static bool StageEventCallback(const Event& e, shared_ptr<Object> extra)
	{
		return extra->As<Stage>()->ProcessEvent(e);
	}

	Stage::Stage()
	{
		// Menu world and camera.
		auto sz = GetProgram()->GetFramebufferSize();
		menuworld = CreateWorld();

		// Set the loading screen to null at start.
		loadingscreen = NULL;

		// Create the game world.
		gameworld = CreateWorld();
		frequency = 60;
		threads = 0;
		iterations = 0;
		substeps = 1;

		// Scene
		gamescene = NULL;
		pausestate = false;
		startmaptimer = NULL;

		// Clear all canvases
		canvases.clear();

#ifdef OPTION_USE_VR
		// Init VR
		if (GetProgram()->VREnabled())
		{
			hmd[STAGEHMD_MENU] = UltraEngine::GetHmd(menuworld);
			hmd[STAGEHMD_GAME] = UltraEngine::GetHmd(gameworld);
		}
#endif
		// Set the menu world as the active world.
		activeworld = menuworld;
	}

	void Stage::LoadScene(const WString& filepath, const int screen)
	{
		// Swap to render the loading screen.
		if (loadingscreen) loadingscreen->ShowLoadScreen(screen);
		GetProgram()->Render(menuworld);
		activeworld = menuworld;

		// Pause the game world.
		gameworld->Pause();
		pausestate = true;

		// Sleep for a bit.
		Sleep(500);

		// Clear all canvases
		canvases.clear();

		if (gamescene != NULL)
		{
			gameworld->SetEnvironmentMap(NULL);
			gameworld->SetEnvironmentMap(NULL, UltraEngine::ENVIRONMENTMAP_DIFFUSE);
			gameworld->SetEnvironmentMap(NULL, UltraEngine::ENVIRONMENTMAP_SPECULAR);
			gamescene = NULL;
		}

		gamescene = UltraEngine::LoadMap(gameworld, filepath);
		if (gamescene != NULL)
		{
			FlushEvents();

			// Check if the scene has a camera and assign it to the pointer.
			for (const auto& p : gamescene->entities)
			{
				auto camera = p->As<Camera>();
				if (camera)
				{
					gamecamera = camera;
					break;
				}
			}

#if 0
			// If not, create one with a settings component.
			if (gamecamera.lock() == NULL)
			{
				Print("Warning: Created a camera as none exist in the scene!");
				auto camera = CreateCamera(gameworld);
				camera->SetPosition(0, 0, 0);

				// Apply the settings listener component so the game remains consistant.
				camera->AddComponent<GamePlayer>();

				gamecamera = camera;
			}
#endif

			// Assign the game world as the active world.
			activeworld = gameworld;

			// Save the path
			mappath = filepath;

			// Emit the event last.
			EmitEvent(EVENT_STAGELOADED, GetProgram());
		}
		else
		{
			if (loadingscreen) loadingscreen->ShowClearScreen();
			Print("Error: Failed to load " + QuoteWString(filepath));
			gameworld->Resume();
			pausestate = false;
			mappath.clear();
		}
	}

	void Stage::LoadStartMap()
	{
		String map = String(GetProgram()->config["startMap"]);
		if (!map.empty())
		{
			Print("Loading start map...");
			if (FileType(map) != 0)
			{
				LoadScene(map, -1);
			}
			else
			{
				WString map = "Maps/" + map + ".ultra";
				if (FileType(map) == 0) map = "Maps/" + map + ".map"; // Try loading legacy map.
				LoadScene(map, -1);
			}
		}
	}

	void Stage::Clear()
	{
		if (gamescene != NULL)
		{
			// Pause the game world.
			gameworld->Pause();
			pausestate = true;

			if (loadingscreen) loadingscreen->ShowClearScreen();
			GetProgram()->Render(menuworld);
			activeworld = menuworld;

			// Clear all canvases
			canvases.clear();

			gameworld->SetEnvironmentMap(NULL);
			gameworld->SetEnvironmentMap(NULL, UltraEngine::ENVIRONMENTMAP_DIFFUSE);
			gameworld->SetEnvironmentMap(NULL, UltraEngine::ENVIRONMENTMAP_SPECULAR);
			gamescene = NULL;

			// Flush all events
			FlushEvents();

			// Resume the game world although it's not active.
			gameworld->Resume();
			pausestate = false;

			// Emit the event last.
			EmitEvent(EVENT_STAGECLEARED, GetProgram());
		}
	}

	void Stage::Pause(const bool bState)
	{
		if (activeworld.lock() != gameworld) return;

		if (bState)
		{
			Print("Debug: Game Paused");
			gameworld->Pause();
			pausestate = true;
		}
		else
		{
			Print("Debug: Game Unpaused");
			gameworld->Resume();
			pausestate = false;
		}

		EmitEvent(EVENT_PAUSESTATE, GetProgram(), (int)bState);
	}

	bool Stage::GetPaused()
	{
		return pausestate;
	}

	bool Stage::InScene()
	{
		return gamescene != NULL;
	}

	void Stage::Update()
	{
		Assert(activeworld.lock() != NULL);

		// If the scene is null but the active world isn't the menu world, force it to be!
		if (gamescene == NULL && activeworld.lock() != menuworld)
		{
			if (loadingscreen) loadingscreen->ShowClearScreen();
			activeworld = menuworld;
		}

		// Update the world only if the window is not hidden.
		auto world = activeworld.lock();
		if (!GetProgram()->ActiveWindow()->GetHidden()) world->Update(frequency, threads, iterations, substeps);

		GetProgram()->Render(world);
	}

	bool Stage::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_PROGRAMSTART)
		{
			// Load the map if one is provided as an argument.
			auto map = CheckArgumentString(GetProgram()->commandline, "map");
			if (!map.empty())
			{
				LoadScene(map);
			}
			else
			{
				// Load start map if one is provided in the config.
				startmaptimer = CreateTimer(1500);
				ListenEvent(EVENT_TIMERTICK, startmaptimer, StageEventCallback, Self());
			}
		}
		if (e.id == EVENT_TIMERTICK)
		{
			if (e.source == startmaptimer && startmaptimer != NULL)
			{
				LoadStartMap();
				startmaptimer = NULL;
			}
		}
		if (e.id == EVENT_GRAPHICSWINDOW)
		{
		}
		else if (e.id == EVENT_STAGELOADED)
		{
			gameworld->Resume();
			pausestate = false;
			Print("Debug: Scene successfully loaded.");
		}
		else if (e.id == EVENT_STAGECLEARED)
		{
			menuworld->Resume();

			// Force show cursor when we cleared the scene.
			auto window = GetProgram()->ActiveWindow();
			if (window)
			{
				currentcursor = CURSOR_DEFAULT;
				window->SetCursor(currentcursor);
			}

			Print("Debug: Scene successfully cleared.");
		}
		else if (e.id == EVENT_CONSOLEEXECUTE)
		{
			auto line = e.text.Split(" ");
			auto cmd = line[0].ToString();
			if (line.size() > 1 && !line[1].empty())
			{
				if (cmd == COMMAND_MAP)
				{
					if (FileType(line[1]) != 0)
					{
						LoadScene(line[1]);
					}
					else
					{
						WString map = "Maps/" + line[1] + ".ultra";
						if (FileType(map) == 0) map = "Maps/" + line[1] + ".map"; // Try loading legacy map.

						// Load the scene
						if (FileType(map) == 1)
						{
							LoadScene(map);
						}
						else
							Print("Error: Failed to load " + QuoteWString(map) + " as file is missing!");
					}
				}
				else if (cmd == COMMAND_LOAD)
				{
					auto file = line[1];
					auto savedir = RealPath(GetPath(PATH_PROGRAMDATA) + "/" + GetProgram()->title) + "/Save";
					if (ExtractExt(file).empty()) file = file + ".sav";
					if (FileType(savedir) != 0) Load(savedir + "/" + file);
				}
				else if (cmd == COMMAND_SAVE)
				{
					auto file = line[1];
					auto savedir = RealPath(GetPath(PATH_PROGRAMDATA) + "/" + GetProgram()->title) + "/Save";
					if (ExtractExt(file).empty()) file = file + ".sav";
					if (FileType(savedir) != 0) Save(savedir + "/" + file);
				}
			}
			else if (cmd == COMMAND_CLEAR)
			{
				Clear();
			}
			else if (cmd == COMMAND_RESTART)
			{
				LoadScene(mappath);
			}
		}
		return true;
	}

	bool Stage::Save(const WString& path)
	{
		bool ret = false;
		if (gamescene != NULL)
		{
			ret = gamescene->Save(path);
		}
		return ret;

	}

	bool Stage::Load(const WString& path)
	{
		bool ret = false;
		if (FileType(path) != 1) return ret;
		if (gamescene != NULL)
		{
			// Pause the game world.
			gameworld->Pause();
			pausestate = true;

			// TODO: Move the below to an event calls to allow loading screens between saves.
			ret = gamescene->Reload(path);

			// Resume the game world although it's not active.
			gameworld->Resume();
			pausestate = false;
		}
		else
		{
			LoadScene(path);
		}
		return ret;
	}

	shared_ptr<World> Stage::GameWorld()
	{
		return gameworld;
	}

	shared_ptr<World> Stage::MenuWorld()
	{
		return menuworld;
	}

	shared_ptr<Camera> Stage::GameCamera()
	{
		return gamecamera.lock();
	}

	void Stage::AddEntity(shared_ptr<Entity> entity)
	{
		if (gamescene)
		{
			const bool exist = std::find(gamescene->entities.begin(), gamescene->entities.end(), entity) != gamescene->entities.end();
			if (!exist)
			{
				gamescene->entities.push_back(entity);
			}
		}
	}

	shared_ptr<Entity> Stage::FindEntity(shared_ptr<Entity> entity)
	{
		if (entity == NULL) return NULL;
		shared_ptr<Entity> rtn = NULL;
		if (gamescene)
		{
			for (auto const& p : gamescene->entities)
			{
				if (p == entity)
				{
					rtn = entity;
					break;
				}
			}
		}

		return rtn;
	}

	shared_ptr<Entity> Stage::FindEntity(const WString& name)
	{
		if (name.empty()) return NULL;
		shared_ptr<Entity> rtn = NULL;
		if (gamescene)
		{
			for (auto const& p : gamescene->entities)
			{
				if (p->name == name)
				{
					rtn = p;
					break;
				}
			}
		}

		return rtn;
	}

	shared_ptr<Entity> Stage::FindEntity(const String& uuid)
	{
		if (uuid.empty()) return NULL;
		shared_ptr<Entity> rtn = NULL;
		if (gamescene)
		{
			for (auto const& p : gamescene->entities)
			{
				if (p->GetUuid() == uuid)
				{
					rtn = p;
					break;
				}
			}
		}

		return rtn;
	}

	shared_ptr<Hmd> Stage::GetHmd(const StageHmd stagehmd)
	{
#ifdef OPTION_USE_VR
		if (!GetProgram()->VREnabled())
			return NULL;

		if (stagehmd == STAGEHMD_ACTIVE)
		{
			if (activeworld.lock() == gameworld)
				return hmd[STAGEHMD_GAME];
			else
				return hmd[STAGEHMD_MENU];
		}
		else
		{
			Assert(hmd[stagehmd] != NULL, "Stage HMD is NULL!");
			return hmd[stagehmd];
		}
#else
		return NULL;
#endif
	}

	shared_ptr<Stage> CreateStage()
	{
		auto stage = std::make_shared<Stage>();

		// Enable callbacks
		ListenEvent(EVENT_PROGRAMSTART, GetProgram(), StageEventCallback, stage);
		ListenEvent(EVENT_SETTINGCHANGED, GetProgram(), StageEventCallback, stage);
		ListenEvent(EVENT_STAGELOADED, GetProgram(), StageEventCallback, stage);
		ListenEvent(EVENT_STAGECLEARED, GetProgram(), StageEventCallback, stage);
		ListenEvent(EVENT_PAUSESTATE, GetProgram(), StageEventCallback, stage);
		ListenEvent(EVENT_CONSOLEEXECUTE, GetProgram(), StageEventCallback, stage);
		ListenEvent(EVENT_GRAPHICSWINDOW, GetProgram(), StageEventCallback, stage);

		return stage;
	}
}