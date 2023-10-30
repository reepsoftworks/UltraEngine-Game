//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "SplashScreen.h"

namespace UltraEngine::Game
{
	SplashScreen::SplashScreen()
	{
		splashimagefile = "UI/Splash/samurai.dds";
		timer = NULL;
		window = NULL;
		ui = NULL;
		mainpanel = NULL;
		label = NULL;
	}

	bool SplashScreen::Initialize(shared_ptr<Window> window, const int time)
	{
		auto config = GetProgram()->config;
		if (config.empty()) return false;

		// Skip the splash screen if we're told to do so by the config or commandline.
		if (config["defaultSettings"]["splashScreen"]["noSplash"].is_boolean())
		{
			if (config["defaultSettings"]["splashScreen"]["noSplash"] == true || CheckArgument(GetProgram()->commandline, CFLAG_NOSPLASH) 
				|| CheckArgument(GetProgram()->commandline, CFLAG_EDITOR))
			{
				// Return false telling the graphics window that no splash window was created!
				return false;
			}
		}

		// Build the splash window.
		this->window = CreateWindow("", 0, 0, 750, 460, window, WINDOW_CENTER);
		ui = CreateInterface(this->window);

		// Main Panel
		if (config["defaultSettings"]["splashScreen"]["background"].is_string())
		{
			splashimagefile = config["defaultSettings"]["splashScreen"]["background"];
			auto splashimage = LoadPixmap(splashimagefile);
			if (splashimage)
			{
				ui->root->SetPixmap(splashimage, PIXMAP_COVER);
				splashimage = NULL;
			}
		}
		else if (config["defaultSettings"]["splashScreen"]["background"].is_array())
		{
			auto rnd = RandomNumber(0, (int)config["defaultSettings"]["splashScreen"]["background"].size() - 1);
			splashimagefile = config["defaultSettings"]["splashScreen"]["background"][rnd];
			auto splashimage = LoadPixmap(splashimagefile);
			if (splashimage)
			{
				ui->root->SetPixmap(splashimage, PIXMAP_COVER);
				splashimage = NULL;
			}
		}

		mainpanel = CreatePanel(0, ui->root->GetSize().y - 80, ui->root->size.x, 80, ui->root, PANEL_DEFAULT);
		mainpanel->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Color bar
		auto splashcolorbar = CreatePanel(0, 0, ui->root->size.x, 4, mainpanel, PANEL_DEFAULT);
		auto color = Vec4(0.95f, 0.59f, 0.18f, 1.0f);
		if (config["defaultSettings"]["splashScreen"]["stripColor"].is_array() && config["defaultSettings"]["splashScreen"]["stripColor"].size() == 4)
		{
			auto v = config["defaultSettings"]["splashScreen"]["stripColor"];
			color = Vec4(v[0], v[1], v[2], v[3]);
		}
		splashcolorbar->SetColor(color);

		// Logo
		WString logoimagefile = "";
		if (config["defaultSettings"]["splashScreen"]["logo"].is_string())
		{
			logoimagefile = config["defaultSettings"]["splashScreen"]["logo"];
		}
		auto logo = LoadPixmap(logoimagefile);
		if (logo != NULL)
		{
			auto logopanel = CreatePanel(8, mainpanel->GetSize().y - (logo->size.y + 8), logo->size.x, logo->size.y, mainpanel);
			logopanel->SetColor(0.0f, 0.0f, 0.0f, 0.0f);
			logopanel->SetPixmap(logo);
			logo = NULL;
		}

		// Copyright label
		if (config["defaultSettings"]["splashScreen"]["copyright"].is_string())
		{
			String author = "Unknown Developer";
			if (config["appAuthor"].is_string()) author = config["appAuthor"];
			author = author.Trim();

			String copyright = "20XX";
			if (config["defaultSettings"]["splashScreen"]["copyright"].is_string()) copyright = config["defaultSettings"]["splashScreen"]["copyright"];
			copyright = copyright.Trim();

			String finalstring = copyright + " " + author;
			label = CreateLabel(L"Copyright © " + WString(finalstring + "\nAll rights reserved."), 0, 0, mainpanel->GetSize().x - 8, mainpanel->GetSize().y, mainpanel, LABEL_RIGHT | LABEL_MIDDLE);
		}

		// Delay
		unsigned int delay = 0;
		if (config["defaultSettings"]["splashScreen"]["delay"].is_integer()) delay = config["defaultSettings"]["splashScreen"]["delay"];
		delay = delay + time;

		// Timer
		timer = CreateTimer(delay);

		return true;
	}

	void SplashScreen::SetHidden(const bool hide)
	{
		if (window) window->SetHidden(hide);
	}

	bool SplashScreen::ProcessEvent(const Event& e)
	{
		//if (e.id == EVENT_TIMERTICK && e.source == timer)
		//{
		//	SetHidden(true);
		//	timer = NULL;
		//}

		return true;
	}
}