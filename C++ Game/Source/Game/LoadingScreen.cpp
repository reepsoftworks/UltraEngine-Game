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
	LoadingScreen::LoadingScreen()
	{
		ui = NULL;
		camera = NULL;
		logo = NULL;
		loadingpanel = NULL;
		loadinglabel = NULL;
		loadingimage.clear();
	}

	bool LoadingScreen::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<LoadingScreen>();
		return elem->ProcessEvent(e);
	}

	void LoadingScreen::Listen(const EventId eventid, shared_ptr<Object> source)
	{
		ListenEvent(eventid, source, LoadingScreen::EventCallback, Self());
	}

	bool LoadingScreen::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_GRAPHICSWINDOW)
		{
			if (ui != NULL)
			{
				// Base
				camera->SetPosition(float(e.size.x) * 0.5f, float(e.size.y) * 0.5f);
				ui->SetSize(e.size);

				ResizeClearScreen(e.size);
				ResizeLoadScreen(e.size);
			}
		}
		return true;
	}

	void LoadingScreen::BuildGUI()
	{
		if (ui == NULL)
		{
			auto world = GetStage()->menuworld;
			Assert(world != NULL);

			// Camera
			auto sz = GetProgram()->GetFramebufferSize();
			camera = CreateCamera(world, PROJECTION_ORTHOGRAPHIC);
			camera->SetPosition(float(sz.x) * 0.5f, float(sz.y) * 0.5f);
			camera->SetHidden(false);

			// GUI
			auto default_font = LoadFont("Fonts/arial.ttf");
			ui = CreateInterface(world, default_font, sz);
		}
	}

	void LoadingScreen::BuildClearScreen()
	{
		// Check if UI exists.
		Assert(ui != NULL, "Clear screen couldn't be built as UI is NULL!");

		// Config
		auto config = GetProgram()->config;

		// Clear Screen Color
		auto color = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		if (config["defaultSettings"]["clearScreen"]["backgroundColor"].is_array() && config["defaultSettings"]["clearScreen"]["backgroundColor"].size() == 4)
		{
			auto v = config["defaultSettings"]["clearScreen"]["backgroundColor"];
			color = Vec4(v[0], v[1], v[2], v[3]);
		}
		ui->root->SetColor(color);

		auto sz = ui->root->size;
		float logoscale = 1.0f;
		if (GetProgram()->config["defaultSettings"]["clearScreen"]["logoScale"].is_float())
		{
			logoscale = GetProgram()->config["defaultSettings"]["clearScreen"]["logoScale"];
		}

		auto logosize = iVec2(256, 256);
		logo = CreatePanel(sz.x / 2 - (logosize.x / 2), sz.y / 2 - (logosize.y / 2), logosize.x, logosize.y, ui->root);
		logo->SetColor(0.0f, 0.0f, 0.0f, 0.0f);
		if (GetProgram()->config["defaultSettings"]["clearScreen"]["logo"].is_string())
		{
			String config_entry = GetProgram()->config["defaultSettings"]["clearScreen"]["logo"];
			if (!config_entry.empty())
			{
				if (ExtractExt(config_entry) == "svg")
				{
					auto icon = LoadIcon(config_entry);
					if (icon != NULL)
					{
						logo->SetIcon(icon, PIXMAP_CENTER, logoscale);
						logo->SetShape(sz.x / 2 - (icon->size.x / 2) * Ceil(logoscale), sz.y / 2 - (icon->size.y / 2) * Ceil(logoscale), icon->size.x * Ceil(logoscale), icon->size.y * Ceil(logoscale));
						icon = NULL;
					}
				}
				else
				{
					auto icon = LoadPixmap(config_entry);
					if (icon != NULL)
					{
						logo->SetPixmap(icon, PIXMAP_CENTER);
						logo->SetShape(sz.x / 2 - (icon->size.x / 2) * Ceil(logoscale), sz.y / 2 - (icon->size.y / 2) * Ceil(logoscale), icon->size.x * Ceil(logoscale), icon->size.y * Ceil(logoscale));
						icon = NULL;
					}
				}
			}
		}
	}

	void LoadingScreen::ResizeClearScreen(const iVec2 size)
	{
		// Reposition the logo
		logo->SetShape(size.x / 2 - (logo->size.x / 2), size.y / 2 - (logo->size.y / 2), logo->size.x, logo->size.y);
	}

	void LoadingScreen::BuildLoadScreen()
	{
		// Check if UI exists.
		Assert(ui != NULL, "Loading screen couldn't be built as UI is NULL!");

		// Config
		auto config = GetProgram()->config;
		auto sz = ui->root->size;

		// Loading screen
		loadingpanel = CreatePanel(0, 0, sz.x, sz.y, ui->root);
		loadingpanel->SetLayout(1, 1, 1, 1);
		loadingpanel->SetHidden(true);

		if (config["defaultSettings"]["loadingScreen"]["backgroundImage"].is_string())
		{
			String imagepath = String(config["defaultSettings"]["loadingScreen"]["backgroundImage"]);
			auto image = LoadPixmap(imagepath);
			if (image != NULL)
			{
				loadingpanel->SetPixmap(image, PIXMAP_STRETCH);
				loadingimage.push_back(image);
			}
		}
		else if (config["defaultSettings"]["loadingScreen"]["backgroundImage"].is_array())
		{
			for (int n = 0; n < config["defaultSettings"]["loadingScreen"]["backgroundImage"].size(); ++n)
			{
				String imagepath = String(config["defaultSettings"]["loadingScreen"]["backgroundImage"][n]);
				auto image = LoadPixmap(imagepath);
				if (image != NULL)
				{
					loadingpanel->SetPixmap(image, PIXMAP_STRETCH);
					loadingimage.push_back(image);
				}
			}
		}

		auto labelsize = iVec2(256, 256);
		loadinglabel = CreatePanel(sz.x / 2 - (labelsize.x / 2), sz.y / 2 - (labelsize.y / 2), labelsize.x, labelsize.y, loadingpanel);
		loadinglabel->SetColor(0.0f, 0.0f, 0.0f, 0.0f);

		float labelscale = 1.0f;
		if (GetProgram()->config["defaultSettings"]["loadingScreen"]["iconScale"].is_float())
		{
			labelscale = GetProgram()->config["defaultSettings"]["loadingScreen"]["iconScale"];
		}

		if (GetProgram()->config["defaultSettings"]["loadingScreen"]["loadingIcon"].is_string())
		{
			String config_entry = GetProgram()->config["defaultSettings"]["loadingScreen"]["loadingIcon"];
			if (!config_entry.empty())
			{
				if (ExtractExt(config_entry) == "svg")
				{
					auto icon = LoadIcon(config_entry);
					if (icon != NULL)
					{
						loadinglabel->SetIcon(icon, PIXMAP_CENTER, labelscale);
						loadinglabel->SetShape(sz.x / 2 - (icon->size.x / 2) * Ceil(labelscale), sz.y / 2 - (icon->size.y / 2) * Ceil(labelscale), icon->size.x * Ceil(labelscale), icon->size.y * Ceil(labelscale));
						icon = NULL;
					}
				}
				else
				{
					auto icon = LoadPixmap(config_entry);
					if (icon != NULL)
					{
						loadinglabel->SetPixmap(icon, PIXMAP_CENTER);
						loadinglabel->SetShape(sz.x / 2 - (icon->size.x / 2) * Ceil(labelscale), sz.y / 2 - (icon->size.y / 2) * Ceil(labelscale), icon->size.x * Ceil(labelscale), icon->size.y * Ceil(labelscale));
						icon = NULL;
					}
				}
			}
		}
	}


	void LoadingScreen::ResizeLoadScreen(const iVec2 size)
	{
		// Loading screen
		loadingpanel->SetShape(0, 0, size.x, size.y);
		loadinglabel->SetShape(0, 0, loadingpanel->size.x, loadingpanel->size.y);
	}

	void LoadingScreen::Start()
	{
		// Bind Events
		Listen(EVENT_GRAPHICSWINDOW, GetProgram());
		Listen(EVENT_WINDOWDPICHANGED, GetProgram());

		// Build GUI
		BuildGUI();

		// Build clear sceeen.
		BuildClearScreen();

		// Build loading screen.
		BuildLoadScreen();

		// If we're in VR, tell the camera to hide as ortho-cameras don't work well in VR.
		if (GetProgram()->VREnabled()) camera->SetHidden(true);
	}

	void LoadingScreen::ShowClearScreen()
	{
		logo->SetHidden(false);
		loadingpanel->SetHidden(true);
		ui->Draw();
	}

	void LoadingScreen::ShowLoadScreen(const int index)
	{
		// HACK: If the int is less than 0, show the clear screen.
		if (index <= -1)
		{
			ShowClearScreen();
		}
		else
		{
			auto final_index = index;
			if (index > loadingimage.size()) final_index = 0;
			if (loadingpanel->pixmap != loadingimage[final_index])
			{
				loadingpanel->SetPixmap(loadingimage[final_index], PIXMAP_FIT);
			}

			//if (console)
			//{
			//	auto component = console->GetComponent<InGameConsole>();
			//	if (component) component->SetHidden(true);
			//}

			logo->SetHidden(true);
			loadingpanel->SetHidden(false);

			ui->Draw();
		}
	}

	shared_ptr<Stage> LoadingScreen::GetStage()
	{
		return stage.lock();
	}
}