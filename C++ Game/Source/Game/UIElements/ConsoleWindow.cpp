//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "ConsoleWindow.h"

namespace UltraEngine::Game
{
	ConsoleWindow::ConsoleWindow()
	{
		historyindex = 0;
		history.clear();
		previousactionset.clear();
	}

	void ConsoleWindow::ManageHistory(const int data)
	{
		if (data == KEY_UP)
		{
			if (textfield_entry)
			{
				if (historyindex > 0 && historyindex <= history.size())
				{
					historyindex--;
					textfield_entry->SetText(history[historyindex]);
				}

			}
		}
		else if (data == KEY_DOWN)
		{
			if (textfield_entry)
			{
				if (historyindex >= 0 && historyindex < history.size())
				{
					textfield_entry->SetText(history[historyindex]);
					historyindex++;
				}
			}
		}
	}

	void ConsoleWindow::BuildWindow(shared_ptr<UltraEngine::Window> parent)
	{
		// We need a parent window.
		if (parent == NULL) return;

		// If the parent window is the same, no need to rebuild.
		parent->Activate();
	
		// Delete window
		textfield_entry = NULL;
		button_send = NULL;
		panel_entry = NULL;
		textarea_log = NULL;
		ui = NULL;
		window = NULL;

		auto windowsize = iVec2(800, 600);
		auto display = GetProgram()->maindisplay;

		//window = CreateWindow("Console", 0, 0, windowsize.x, windowsize.y, parent, WINDOW_CENTER | WINDOW_DEFAULT | WINDOW_HIDDEN);
		window = CreateWindow("Console", 0, 0, windowsize.x, windowsize.y, GetProgram()->maindisplay, WINDOW_CENTER | WINDOW_DEFAULT | WINDOW_HIDDEN);
		window->SetMinSize(windowsize.x / 2, windowsize.y / 2);
		SetWindowTitlebarTheme(window, TITLEBAR_SYSTEM);
		parentwindow = parent;

		if (display->GetScale() != 1.0f)
		{
			auto window_size = window->GetSize();
			EmitEvent(EVENT_WINDOWDPICHANGED, window, Round(display->GetScale() * 100.0f), 0, 0, Round(window_size.x * display->GetScale()), Round(window_size.y * display->GetScale()));
		}

		Assert(window != NULL);
		ListenEvent(EVENT_WINDOWCLOSE, window, EventCallback, Self());
		ListenEvent(EVENT_KEYUP, window, EventCallback, Self());

		ui = CreateInterface(window);
		ui->LoadColorScheme(GetProgram()->GetGUITheme());

		// Build base UI panels
		auto ui_size = ui->root->GetSize();
		textarea_log = CreateTextArea(8, 8, ui_size.x - 16, ui_size.y - 8 - 50, ui->root, TEXTAREA_SCROLLDOWN);
		textarea_log->SetLayout(1, 1, 1, 1);
		
		ListenEvent(EVENT_PRINT, NULL, EventCallback, Self());

		panel_entry = CreatePanel(8, textarea_log->GetPosition().y + textarea_log->GetSize().y + 8, textarea_log->GetSize().x, 32, ui->root);
		panel_entry->SetLayout(1, 1, 0, 1);
		button_send = CreateButton("Send", panel_entry->GetSize().x - 72, 0, 72, 32, panel_entry);
		button_send->SetLayout(0, 1, 0, 1);
		textfield_entry = CreateTextField(0, 0, panel_entry->GetSize().x - 72 - 16, 32, panel_entry);
		textfield_entry->SetLayout(1, 1, 0, 1);
		textfield_entry->Activate();
		ListenEvent(EVENT_WIDGETACTION, textfield_entry, EventCallback, Self());

		// Write what's already in the console log buffer.
		auto log = GetProgram()->consolelog;
		if (log)
		{
			textarea_log->AddText(GetProgram()->author + " - Console (" + String(__DATE__) + ")\n");
			while (!log->Eof())
			{
				textarea_log->AddText(log->ReadWLine());
			}
		}
	}

	void ConsoleWindow::SetHidden(const bool hide)
	{
		if (window != NULL)
		{
			auto gamecontroller = GetProgram()->GetGameController();

			window->SetHidden(hide);
			if (!hide)
			{
				if (gamecontroller)
				{
					previousactionset = gamecontroller->GetActiveSet();
					gamecontroller->SetActiveSet("");
				}
				
				window->Activate();
				textfield_entry->Activate();
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

	bool ConsoleWindow::ProcessEvent(const Event& e)
	{
		if (window == NULL) return false;
		if (e.id == EVENT_PRINT)
		{
			textarea_log->AddText(e.text.ToString() + "\n");
		}
		//else if (e.id == EVENT_KEYUP)
		//{
		//	ManageHistory(e.data);
		//}
		else if (e.id == EVENT_WINDOWCLOSE)
		{
			if (e.source == window)
			{
				SetHidden(true);
			}
		}
		else if (e.id == EVENT_WIDGETSELECT)
		{
			if (e.source == window)
			{
				SetHidden(true);
			}
		}
		else if (e.id == EVENT_WINDOWDPICHANGED)
		{
			//Resize window if desired
			window->SetShape(e.position, e.size);

			//Get the user interface
			ui->SetScale(float(e.data) / 100.0f);
		}
		else if (e.id == EVENT_WIDGETACTION)
		{
			if (e.source == textfield_entry || e.source == button_send)
			{
				auto cmd = e.text.ToString();
				if (!cmd.empty())
				{
					textarea_log->AddText("] " + cmd + "\n");
					textfield_entry->SetText("");
					textfield_entry->Activate();

					// Add to History
					history.push_back(cmd);
					historyindex = history.size();

					// Call the event!
					EmitEvent(EVENT_CONSOLEEXECUTE, GetProgram(), 0, 0, 0, 0, 0, NULL, cmd);
				}
			}
		}
		return true;
	}

	shared_ptr<ConsoleWindow> ConsoleWindow::Create(shared_ptr<UltraEngine::Window> parent)
	{
		auto app = std::make_shared<ConsoleWindow>();
		app->BuildWindow(parent);
		return app;
	}

	shared_ptr<ConsoleWindow> CreateConsoleWindow(shared_ptr<UltraEngine::Window> parent)
	{
		return ConsoleWindow::Create(parent);
	}
}