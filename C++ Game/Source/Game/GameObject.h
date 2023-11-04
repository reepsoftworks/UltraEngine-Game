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

namespace UltraEngine::Game
{
	// Deprecated
#if 0
	class Stage;
	class GameController;

	class GameObject : public Component
	{
	protected:
		shared_ptr<Stage> GetStage();
		shared_ptr<GameController> GetInput();

		//virtual void OnPerTick() {};
		//virtual void UpdateStage() {};

	public:
		GameObject();
		virtual ~GameObject();

		virtual bool OnInteraction(shared_ptr<Entity> caller) { return false; }
		virtual void OnLookAt(shared_ptr<Entity> caller) {}
	};

	// GamePlayer:
	// A dedicated component class for the entity that's defined as the player.
	class GamePlayer : public GameObject
	{
	protected:
		virtual ~GamePlayer();

		shared_ptr<Camera> InitCamera();
		void ApplyCameraSettings(shared_ptr<Camera> camera);
		void ProcessCameraEvents(const Event& e, shared_ptr<Camera> camera);

		friend class Stage;
	};
#endif
}