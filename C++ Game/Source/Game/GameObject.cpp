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
	// Deprecated
#if 0
	GameObject::GameObject()
	{
	}

	GameObject::~GameObject()
	{
	}

	shared_ptr<Stage> GameObject::GetStage()
	{
		return GetProgram()->stage;
	}

	shared_ptr<GameController> GameObject::GetInput()
	{
		return GetProgram()->gamecontroller;
	}

	// GamePlayer
	GamePlayer::~GamePlayer()
	{
		GetProgram()->pausewhenunselected = false;
	}

	shared_ptr<Camera> GamePlayer::InitCamera()
	{
		auto camera = CreateCamera(GetEntity()->GetWorld());
		ApplyCameraSettings(camera);

		// Tell the application we want to pause if the window ever loses focus!
		GetProgram()->pausewhenunselected = true;

		return camera;
	}

	void GamePlayer::ApplyCameraSettings(shared_ptr<Camera> camera)
	{
		if (camera != NULL)
		{
			// Tessellation
			float v = static_cast<float>(GetProgram()->GetSetting(SETTING_TESSELLATION, 2));
			camera->SetTessellation(v);

			// Screen Space Reflections
			camera->SetSSR((bool)GetProgram()->GetSetting(SETTING_SSR, 0));

			// FOV
			v = static_cast<float>(GetProgram()->GetSetting(SETTING_FOV, 70));
			camera->SetFov(v);

			// Tell the camera to listen.
			camera->Listen();

			// Set the camera tag.
			camera->AddTag("GamePlayerCamera");
		}
	}

	void GamePlayer::ProcessCameraEvents(const Event& e, shared_ptr<Camera> camera)
	{
		if (e.id == EVENT_SETTINGCHANGED)
		{
			if (e.text == SETTING_LIGHTQUALITY)
			{
				if (camera) camera->GetWorld()->SetLightQuality(e.data);
			}
			else if (e.text == SETTING_TESSELLATION)
			{
				const float v = static_cast<float>(e.data);
				if (camera) camera->SetTessellation(v);
			}
			else if (e.text == SETTING_SSR)
			{
				if (camera) camera->SetSSR((bool)e.data);
			}
			else if (e.text == SETTING_FOV)
			{
				const float v = static_cast<float>(e.data);
				if (camera) camera->SetFov(v);
			}
			else if (e.text == SETTING_HRTF)
			{
				SetHrtf((bool)e.data);
			}
		}
		if (e.id == EVENT_CONSOLEEXECUTE)
		{
			auto line = e.text.Split(" ");
			auto cmd = line[0].ToString();
			if (line.size() > 1 && !line[1].empty())
			{
				if (cmd == SETTING_LIGHTQUALITY || cmd == SETTING_TESSELLATION || cmd == SETTING_SSR ||
					cmd == SETTING_FOV || cmd == SETTING_HRTF)
				{
					GetProgram()->SetSetting(cmd, (int)line[1].ToInt());
					Print("Applied Setting " + QuoteString(cmd) + " to: " + line[1]);
				}
				else if (cmd == COMMAND_SHOWPHYSICS)
				{
					if (camera)
					{
						const bool v = (int)line[1].ToInt();
						camera->SetDebugPhysicsMode(v);
						Print("Applied " + QuoteString(cmd) + " to: " + line[1]);
					}
				}
				else if (cmd == COMMAND_WIREFRAME)
				{
					if (camera)
					{
						const bool v = (int)line[1].ToInt();
						camera->SetWireframe(v);
						Print("Applied " + QuoteString(cmd) + " to: " + line[1]);
					}
				}
			}
		}
	}
#endif
}