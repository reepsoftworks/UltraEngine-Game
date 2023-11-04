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
#include "LoadingScreen.h"

namespace UltraEngine::Game
{
	class Program;
	class LoadingScreen;
}

namespace UltraEngine::Game
{
	class Stage : public Object
	{
		// Current world pointer.
		std::weak_ptr<World> activeworld;

		// World pointer for non-game menu.
		shared_ptr<World> menuworld;

		// Loading screen.
		shared_ptr<LoadingScreen> loadingscreen;

		// World pointer for in-game.
		shared_ptr<World> gameworld;
		std::weak_ptr<Camera> gamecamera;

		// Scene
		shared_ptr<Map> gamescene;
		WString mappath;
		bool pausestate;
		shared_ptr<Timer> startmaptimer;

	public:
		int frequency;
		int threads;
		int iterations;
		int substeps;

		Stage();

		void LoadScene(const WString& filepath, const int screen = 0);
		void LoadStartMap();
		void Clear();
		void Pause(const bool bState);
		bool GetPaused();
		bool InScene();

		virtual void Update();
		virtual bool ProcessEvent(const Event& e);

		shared_ptr<World> GameWorld();
		shared_ptr<World> MenuWorld();
		shared_ptr<Camera> GameCamera();

		void AddEntity(shared_ptr<Entity> entity);
		virtual shared_ptr<Entity> FindEntity(shared_ptr<Entity> entity);
		virtual shared_ptr<Entity> FindEntity(const WString& name);
		virtual shared_ptr<Entity> FindEntity(const String& uuid);

		bool Save(const WString& path);
		bool Load(const WString& path);

		friend class LoadingScreen;

		template <class T>
		shared_ptr<T> SetLoadingScreen()
		{
			auto o = loadingscreen;
			if (o) return o;
			o = std::make_shared<T>();
			std::shared_ptr<LoadingScreen> c = std::dynamic_pointer_cast<LoadingScreen>(o);
			if (c == NULL) RuntimeError("Type must be a LoadingScreen.");
			c->stage = As<Stage>();
			c->Start();
			loadingscreen = c;
			return c;
		}

		friend class GamePlayer;
		friend UltraEngine::Game::Program;
		friend shared_ptr<Stage> CreateStage();
	};

	extern shared_ptr<Stage> CreateStage();
}
