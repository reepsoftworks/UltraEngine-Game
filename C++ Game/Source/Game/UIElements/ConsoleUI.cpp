//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "ConsoleUI.h"

namespace UltraEngine::Game
{
	ConsoleUI::ConsoleUI()
	{
		root = NULL;
	}

	ConsoleUI::~ConsoleUI()
	{
		root = NULL;
	}

	void ConsoleUI::BuildWidgets(const iVec2& size, const shared_ptr<Widget> parent)
	{
		root = CreatePanel(0, 0, size.x, size.y, parent);
		root->SetLayout(1, 1, 1, 1);

		auto ui_size = root->GetSize();
		textarea_log = CreateTextArea(8, 8, ui_size.x - 16, ui_size.y - 8 - 50, root, TEXTAREA_SCROLLDOWN);
		textarea_log->SetLayout(1, 1, 1, 1);

		panel_entry = CreatePanel(8, textarea_log->GetPosition().y + textarea_log->GetSize().y + 8, textarea_log->GetSize().x, 32, root);
		panel_entry->SetLayout(1, 1, 0, 1);
		button_send = CreateButton("Send", panel_entry->GetSize().x - 72, 0, 72, 32, panel_entry);
		button_send->SetLayout(0, 1, 0, 1);
		textfield_entry = CreateTextField(0, 0, panel_entry->GetSize().x - 72 - 16, 32, panel_entry);
		textfield_entry->SetLayout(1, 1, 0, 1);
		textfield_entry->Activate();

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

		Listen(EVENT_NONE);
	}

	bool ConsoleUI::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_PRINT)
		{
			textarea_log->AddText(e.text.ToString() + "\n");
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

					// Call the event!
					EmitEvent(EVENT_CONSOLEEXECUTE, GetProgram(), 0, 0, 0, 0, 0, NULL, cmd);
				}
			}
		}
		return true;
	}


	void ConsoleUI::OnActivate()
	{
		textfield_entry->Activate();
	}
}