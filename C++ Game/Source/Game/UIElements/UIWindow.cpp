//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "UIWindow.h"

namespace UltraEngine::Game
{
	static String previousactionset = "";

	UIWindow::UIWindow()
	{
		ui = NULL;
		root = NULL;
		panel = NULL;
		callaction.clear();
	}

	UIWindow::~UIWindow()
	{
		ui = NULL;
		root = NULL;
		panel = NULL;
		callaction.clear();
	}

	const bool UIWindow::BuildWindow(const WString title, const int w, const int h, shared_ptr<UltraEngine::Window> parent)
	{
		// If the parent window is the same, no need to rebuild.
		 if (parent != NULL) parent->Activate();

		// Delete previous members.
		ui = NULL;
		root = NULL;

		auto windowsize = iVec2(w, h);
		auto display = GetProgram()->maindisplay;

		root = CreateWindow(title, 0, 0, windowsize.x, windowsize.y, GetProgram()->maindisplay, WINDOW_CENTER | WINDOW_DEFAULT | WINDOW_RESIZABLE | WINDOW_HIDDEN);
		root->SetMinSize(windowsize.x / 2, windowsize.y / 2);
		const TitlebarTheme themesetting = (TitlebarTheme)GetProgram()->GetSetting(SETTING_WINDOWTHEME, (int)TITLEBAR_SYSTEM);
		SetWindowTitlebarTheme(root, themesetting);
		parentwindow = parent;

		if (display->GetScale() != 1.0f)
		{
			auto window_size = root->GetSize();
			EmitEvent(EVENT_WINDOWDPICHANGED, root, Round(display->GetScale() * 100.0f), 0, 0, Round(window_size.x * display->GetScale()), Round(window_size.y * display->GetScale()));
		}

		// Build UI
		ui = CreateInterface(root);
		ui->LoadColorScheme(GetProgram()->GetGUITheme());

		// If we don't have a parent, assume it's a standard app window.
		if (parentwindow.lock() == NULL) root->SetHidden(false);

		Listen(EVENT_NONE, NULL);

		return true;
	}

	void UIWindow::SetHidden(const bool hide)
	{
		if (root != NULL)
		{
			auto gamecontroller = GetProgram()->GetGameController();

			root->SetHidden(hide);
			if (!hide)
			{
				if (gamecontroller)
				{
					previousactionset = gamecontroller->GetActiveSet();
					gamecontroller->SetActiveSet("");
				}

				root->Activate();
				if (panel) panel->OnActivate();
			}
			else
			{
				if (gamecontroller && !previousactionset.empty()) gamecontroller->SetActiveSet(previousactionset);
				if (parentwindow.lock() != NULL)
				{
					parentwindow.lock()->Activate();
					previousactionset.clear();
				}
			}
		}
	}

	bool UIWindow::ProcessEvent(const Event& e)
	{
		if (root == NULL) return false;
		if (e.id == EVENT_WINDOWCLOSE)
		{
			if (e.source == root)
			{
				if (parentwindow.lock() == NULL)
				{
					EmitEvent(EVENT_QUIT);
					return true;
				}

				SetHidden(true);
			}
		}
		else if (e.id == EVENT_WINDOWDPICHANGED)
		{
			if (ui)
			{
				//Resize window if desired
				root->SetShape(e.position, e.size);

				//Get the user interface
				ui->SetScale(float(e.data) / 100.0f);
			}
		}
		else if (e.id == EVENT_SETTINGCHANGED)
		{
			if (ui)
			{
				if (e.text == SETTING_WINDOWTHEME)
				{
					const TitlebarTheme themesetting = (TitlebarTheme)e.data;
					SetWindowTitlebarTheme(root, themesetting);

					ui->LoadColorScheme(GetProgram()->GetGUITheme());
				}
			}
		}
		else if (e.id == EVENT_KEYDOWN)
		{
			if (!callaction.empty())
			{
				if (GetProgram()->gamecontroller->Hit(callaction))
				{
					//Show the window.
					SetHidden(false);
				}
			}
		}

		return true;
	}
}