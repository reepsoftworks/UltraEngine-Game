//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "KeyBindButton.h"
#include "InputBindPanel.h"
#include "FloatSlider.h"
#include "NumberSlider.h"
#include "../Game.h"

namespace UltraEngine::Game
{
	bool InputBindPanel::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<InputBindPanel>();
		return elem->ProcessEvent(e);
	}

	bool InputBindPanel::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_WIDGETACTION && e.source == clearbutton)
		{
			for (const auto& p : bindbutton)
			{
				auto kb = p->As<KeyBindButton>();
				if (kb)
				{
					kb->Clear();
				}
			}
		}
		return true;
	}

	void InputBindPanel::WriteBinding()
	{
		auto controller = Game::GetProgram()->gamecontroller;
		
		if (bindbutton.size() == 1)
		{
			auto kb = bindbutton[0]->As<KeyBindButton>();
			if (kb)
			{
				controller->SetAction(action, (Game::ButtonCode)kb->buttonassigned, actionset);
			}
		}
		else
		{
			// Collect the buttons.
			std::vector<Game::ButtonCode> btns;
			for (const auto& p : bindbutton)
			{
				auto kb = p->As<KeyBindButton>();
				if (kb)
				{
					btns.push_back((ButtonCode)kb->buttonassigned);
				}
			}

			// Reverse the order!
			std::reverse(btns.begin(), btns.end());

			// Save it correctly.
			if (savetoaxis)
			{
				ButtonAxis moveaxis =
				{
					btns[0],
					btns[1],
					btns[2],
					btns[3]
				};

				controller->SetAction(action, moveaxis, actionset);
			}
			else
			{
				controller->SetAction(action, btns, actionset);
			}
		}
	}

	//void InputBindPanel::SetLayout(const int left, const int right, const int top, const int bottom)
	//{
	//	if (panel) panel->SetLayout(left, right, top, bottom);
	//}

	shared_ptr<InputBindPanel> CreateInputBindPanel(const String& label, table table, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent, const String& set)
	{
		auto widget = std::make_shared<InputBindPanel>();
		widget->Initialize("", x, y, width, height, parent, 0);
		widget->panel = CreatePanel(x, y, width, height, parent, PANEL_BORDER);
		widget->label = CreateLabel(label, 4, 0, width / 4, height, widget->panel, LABEL_LEFT | LABEL_MIDDLE);

		auto button_sz = iVec2(128, 32);
		widget->clearbutton = CreateButton("x", widget->panel->GetSize().x - 32 - 8, widget->panel->GetSize().y / 2 - 18, 32, 32, widget->panel, BUTTON_DEFAULT);
		ListenEvent(EVENT_WIDGETACTION, widget->clearbutton, InputBindPanel::EventCallback, widget);

		int key = -1;
		if (!table.empty())
		{
			const char* action = label.c_str();
			if (table[action].is_integer())
			{
				key = table[action];
				auto btn = CreateKeyBindButton(widget->clearbutton->GetPosition().x - button_sz.x - 8, widget->clearbutton->GetPosition().y, button_sz.x, button_sz.y, widget->panel, key);
				widget->bindbutton.push_back(btn);
			}
			else if (table[action].is_array())
			{
				std::vector<int> btns;
				for (auto a : table[action])
				{
					String s = a.second;
					btns.push_back((int)s.ToInt());
				}

				button_sz = iVec2(62, 32);
				static iVec2 pos = iVec2(widget->clearbutton->GetPosition().x - button_sz.x - 8, widget->clearbutton->GetPosition().y);

				// Create the buttons backwards...
				size_t index = btns.size() - 1;
				for (std::vector<int>::reverse_iterator rit = btns.rbegin();
					rit != btns.rend(); ++rit, --index)
				{
					key = btns[index];

					WString prefix;
					if (btns.size() == 4)
					{
						switch (index)
						{
						case 3:
							//prefix = L'\u2192';
							prefix = L"→ ";
							break;
						case 2:
							//prefix = L'\u2190';
							prefix = L"← ";
							break;
						case 1:
							//prefix = L'\u2193';
							prefix = L"↓ ";
							break;
						case 0:
							//prefix = L'\u2191';
							prefix = L"↑ ";
							break;
						}
					}

					auto btn = CreateKeyBindButton(pos.x, pos.y, button_sz.x, button_sz.y, widget->panel, key, prefix);

					auto cast = btn->As<KeyBindButton>();
					if (cast) cast->pressmessage = "...";
					pos = iVec2(btn->GetPosition().x - button_sz.x - 4, btn->GetPosition().y);
					widget->bindbutton.push_back(btn);
				}
			}
		}

		widget->action = label;
		widget->actionset = set;
		widget->savetoaxis = false;
		return widget;
	}

	bool InputSettingsPanel::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<InputSettingsPanel>();
		return elem->ProcessEvent(e);
	}

	bool InputSettingsPanel::ProcessEvent(const Event& e)
	{
		//if (e.id == EVENT_WIDGETACTION && e.source == valuewidget)
		//{
		//	valuewidget->SetState((bool)e.data);
		//}
		return true;
	}

	void InputSettingsPanel::WriteSetting()
	{
		auto controller = Game::GetProgram()->gamecontroller;
		if (type == 0)
		{
			const bool state = (bool)valuewidget->GetSelectedItem();
			controller->SetSetting(setting, state);
		}
		else if (type == 1)
		{
			auto cast = valuewidget->As<FloatSlider>();
			if (cast)
			{
				controller->SetSetting(setting, (double)cast->GetValue());
			}
			
		}
		//else if (type == 2)
		//{
		//	auto cast = valuewidget->As<NumberSlider>();
		//	if (cast)
		//	{
		//		controller->SetSetting(setting, (int)Ceil(cast->GetValue()));
		//	}
		//}
	}

	shared_ptr<InputSettingsPanel> CreateInputSettingsPanel(const String& label, table config, const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto widget = std::make_shared<InputSettingsPanel>();
		widget->Initialize("", x, y, width, height, parent, 0);
		widget->panel = CreatePanel(x, y, width, height, parent, PANEL_BORDER);
		widget->label = CreateLabel(label, 4, 0, width / 4, height, widget->panel, LABEL_LEFT | LABEL_MIDDLE);

		auto setting = config["settings"][label];
		if (setting.is_boolean())
		{
			widget->valuewidget = CreateComboBox(widget->panel->GetSize().x - (180 + 8), widget->panel->GetSize().y / 2 - 14, 180, 28, widget->panel);
			widget->valuewidget->AddItem("Disabled", (bool)setting == false);
			widget->valuewidget->AddItem("Enabled", (bool)setting == true);

			widget->type = 0;
		}
		else //if (setting.is_float())
		{
			widget->valuewidget = CreateFloatSlider(0.0f, 100.0f, config["settings"][label], widget->panel->GetSize().x - (180 + 8), widget->panel->GetSize().y / 2 - 14, 180, 28, widget->panel);
			widget->type = 1;
		}
		//else if (setting.is_integer())
		//{
		//	widget->valuewidget = CreateNumberSlider(0, 100, config["settings"][label], widget->panel->GetSize().x - (180 + 8), widget->panel->GetSize().y / 2 - 14, 180, 28, widget->panel);
		//	widget->type = 2;
		//}

		//ListenEvent(EVENT_WIDGETACTION, widget->valuewidget, InputSettingsPanel::EventCallback, widget);
		widget->setting = label;
		return widget;
	}
}