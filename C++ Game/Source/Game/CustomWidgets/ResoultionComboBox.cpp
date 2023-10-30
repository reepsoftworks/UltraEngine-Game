//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "../Game.h"

namespace UltraEngine::Game
{
	shared_ptr<ResoultionComboBox> CreateResoultionComboBox(const int x, const int y, const int width, const int height, shared_ptr<Widget> parent)
	{
		auto widget = std::make_shared<ResoultionComboBox>();
		
		// Get the list of the displays.
		auto displays = GetDisplays();
		auto display_count = displays.size();
		for (size_t i = 0; i < display_count; i++)
		{
			auto temp_combo = CreateComboBox(x, y, width, height, parent);

			// This is awful. We cycle through the avaiable resoultions and then remove any duplicates.
			// Duplicates are caused because the engine detects the same resoultion at diffrent refresh rates.
			std::list<String> resoultions;
			auto gfxmodes = displays[i]->graphicsmodes;
			for (int n = 0; n < gfxmodes.size(); ++n)
			{
				auto entry = String(gfxmodes[n].x) + " x " + String(gfxmodes[n].y);
				resoultions.push_back(entry);
			}
			resoultions.unique();

			auto x = Game::GetProgram()->GetSetting(SETTING_SCREENWIDTH, 1280);
			auto y = Game::GetProgram()->GetSetting(SETTING_SCREENHEIGHT, 768);
			auto default_res = String(x) + " x " + String(y);
			for (const auto& p : resoultions)
			{
				temp_combo->AddItem(p, p == default_res);
			}

			temp_combo->SetHidden(true);
			widget->combobox.push_back(temp_combo);
		}

		//ListenEvent(EVENT_NONE, NULL, ResoultionComboBox::EventCallback, widget);
		return widget;
	}

	void ResoultionComboBox::Enable()
	{
		for (int n = 0; n < combobox.size(); ++n)
		{
			combobox[n]->Unlock();
			combobox[n]->Enable();
		}
		disabled = false;
	}

	void ResoultionComboBox::Disable()
	{
		for (int n = 0; n < combobox.size(); ++n)
		{
			combobox[n]->Lock();
			combobox[n]->Disable();
		}
		disabled = true;
	}

	bool ResoultionComboBox::GetDisabled()
	{
		return disabled;
	}

	void ResoultionComboBox::SetLayout(const int left, const int right, const int top, const int bottom)
	{
		for (int n = 0; n < combobox.size(); ++n)
		{
			combobox[n]->SetLayout(left, right, top, bottom);
		}
	}

	void ResoultionComboBox::ShowDisplayBox(const int displayindex)
	{
		for (int n = 0; n < combobox.size(); ++n)
		{
			combobox[n]->SetHidden(true);
		}

		combobox[displayindex]->SetHidden(false);
	}

	iVec2 ResoultionComboBox::GetResoultion(const int displayindex)
	{
		auto text = combobox[displayindex]->GetItemText(combobox[displayindex]->GetSelectedItem());
		auto split = text.Split(" x ");
		return iVec2(static_cast<int>(split[0].ToInt()), static_cast<int>(split[1].ToInt()));
	}

	const bool ResoultionComboBox::ForceSelection(const int displayindex, const UltraEngine::WString& item)
	{
		auto box = combobox[displayindex];
		int i = 0;
		for (i = 0; i < box->CountItems(); i++)
		{
			auto itemtext = box->GetItemText(i);
			if (itemtext == item)
			{
				box->SelectItem(i);
				break;
			}
		}

		return i != 0;
	}

	bool ResoultionComboBox::EventCallback(const Event& e, shared_ptr<Object> extra)
	{
		auto elem = extra->As<ResoultionComboBox>();
		return elem->ProcessEvent(e);
	}

	bool ResoultionComboBox::ProcessEvent(const Event& e)
	{
		return true;
	}
}