//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "SettingsUI.h"

namespace UltraEngine::Game
{
	SettingsUI::SettingsUI()
	{
		listbox = NULL;
		mainpanel = NULL;
		panel.fill(NULL);
		button_ok = NULL;
		button_cancel = NULL;
		button_apply = NULL;
	}

	SettingsUI::~SettingsUI()
	{
		listbox = NULL;
		mainpanel = NULL;
		panel.fill(NULL);
		button_ok = NULL;
		button_cancel = NULL;
		button_apply = NULL;
	}

	void SettingsUI::BuildWidgets(const iVec2& size, const shared_ptr<Widget> parent)
	{
		root = CreatePanel(0, 0, size.x, size.y, parent);
		root->SetLayout(1, 1, 1, 1);
		auto ui_size = root->GetSize();

		listbox = CreateListBox(8, 8, 150, ui_size.y - 8 - 50, root);
		listbox->SetLayout(1, 0, 1, 1);
		listbox->AddItem("Window", true);
		listbox->AddItem("Graphics");
		listbox->AddItem("Controls");
		listbox->AddItem("Sound");

		mainpanel = CreatePanel(listbox->GetPosition().x + listbox->GetSize().x + 8, 8, ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, ui_size.y - 8 - 50, root, PANEL_BORDER);
		mainpanel->SetLayout(1, 1, 1, 1);

		//scrollpanel[0] = CreateScrollPanel(0, 0, ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, ui_size.y - 8 - 50, mainpanel);
		//scrollpanel[0]->SetLayout(1, 1, 1, 1);
		//scrollpanel[0]->SetHidden(false);
		//scrollpanel[0]->SetArea(ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, 400);

		panel[0] = CreateScrollPanel(0, 0, ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, ui_size.y - 8 - 50, mainpanel);
		panel[0]->SetLayout(1, 1, 1, 1);
		panel[0]->SetHidden(false);
		panel[0]->SetArea(ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, 350);

		panel[1] = CreateScrollPanel(0, 0, ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, ui_size.y - 8 - 50, mainpanel);
		panel[1]->SetLayout(1, 1, 1, 1);
		panel[1]->SetHidden(true);
		panel[1]->SetArea(ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, 350);

		panel[2] = CreateScrollPanel(0, 0, ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, ui_size.y - 8 - 50, mainpanel);
		panel[2]->SetLayout(1, 1, 1, 1);
		panel[2]->SetHidden(true);
		panel[2]->SetArea(ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, 350);

		panel[3] = CreateScrollPanel(0, 0, ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, ui_size.y - 8 - 50, mainpanel);
		panel[3]->SetLayout(1, 1, 1, 1);
		panel[3]->SetHidden(true);
		panel[3]->SetArea(ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, 350);

		button_apply = CreateButton("Apply", ui_size.x - (72 + 8), mainpanel->GetPosition().y + mainpanel->GetSize().y + 8, 72, 32, root);
		button_apply->SetLayout(0, 1, 0, 1);
		button_cancel = CreateButton("Cancel", button_apply->GetPosition().x - (button_apply->GetSize().x + 8), button_apply->GetPosition().y, button_apply->GetSize().x, button_apply->GetSize().y,root);
		button_cancel->SetLayout(0, 1, 0, 1);
		button_ok = CreateButton("OK", button_cancel->GetPosition().x - (button_cancel->GetSize().x + 8), button_cancel->GetPosition().y, button_cancel->GetSize().x, button_cancel->GetSize().y, root, BUTTON_OK);
		button_ok->SetLayout(0, 1, 0, 1);

		// Get the subpanel pointer
		auto activepanel = panel[0]->subpanel; //->As<ScrollPanel>()->subpanel;
		Assert(activepanel);

		// Display + Resoultion
		label_display = CreateLabel("Target Display Index", 8, 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		label_display->SetLayout(1, 0, 0, 0);
		label_resoultion = CreateLabel("Resoultion", label_display->GetPosition().x, label_display->GetPosition().y + label_display->GetSize().y + 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);

		combo_display = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_display->GetPosition().y, 180, 28, activepanel);
		combo_display->SetLayout(0, 1, 1, 0);

		auto displays = UltraEngine::GetDisplays();
		auto currentdisplay = GetProgram()->GetSetting(SETTING_DISPLAY, 0);
		for (int k = 0; k < displays.size(); ++k)
		{
			combo_display->AddItem("Display " + String(k), k == currentdisplay);
		}
		displays.clear();

		combo_resoultion = CreateResoultionComboBox(activepanel->GetSize().x - (180 + 8), label_resoultion->GetPosition().y, 180, 28, activepanel);
		auto res_combo_box_class = combo_resoultion->As<ResoultionComboBox>();
		if (res_combo_box_class)
		{
			res_combo_box_class->ShowDisplayBox(currentdisplay);
		}
		combo_resoultion->SetLayout(0, 1, 1, 0);

		// Window mode
		auto setting = GetProgram()->GetSetting(SETTING_WINDOWMODE, 0);
		label_windowmode = CreateLabel("Window Mode", label_resoultion->GetPosition().x, label_resoultion->GetPosition().y + label_resoultion->GetSize().y + 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		combo_windowmode = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_windowmode->GetPosition().y, 180, 28, activepanel);
		combo_windowmode->AddItem("Windowed", setting == 0);
		combo_windowmode->AddItem("Borderless Window", setting == 1);
		combo_windowmode->AddItem("Fullscreen", setting == 2);
		combo_windowmode->AddItem("Fullscreen (Native)", setting == 3);
		combo_windowmode->SetLayout(0, 1, 1, 0);

		// Window theme
		setting = GetProgram()->GetSetting(SETTING_WINDOWTHEME, (int)TITLEBAR_SYSTEM);
		label_windowtheme = CreateLabel("Window Theme", label_windowmode->GetPosition().x, label_windowmode->GetPosition().y + label_windowmode->GetSize().y + 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		combo_windowtheme = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_windowtheme->GetPosition().y, 180, 28, activepanel);
		combo_windowtheme->AddItem("Light", setting == 0);
		combo_windowtheme->AddItem("Dark", setting == 1);
		combo_windowtheme->AddItem("System", setting == 2);
		combo_windowtheme->SetLayout(0, 1, 1, 0);

		// Next panel....
		activepanel = panel[1]->subpanel;
		Assert(activepanel);

		// Light Quality
		setting = GetProgram()->GetSetting(SETTING_LIGHTQUALITY, 1);
		label_lightquality = CreateLabel("Light Quality", 8, 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		combo_lightquality = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_lightquality->GetPosition().y, 180, 28, activepanel);
		combo_lightquality->AddItem("Low", setting == 0);
		combo_lightquality->AddItem("Medium", setting == 1);
		combo_lightquality->AddItem("High", setting == 2);
		combo_lightquality->AddItem("Ultra", setting == 3);
		combo_lightquality->SetLayout(0, 1, 1, 0);

		// Tessellation
		setting = GetProgram()->GetSetting(SETTING_TESSELLATION, 2);
		label_tessellation = CreateLabel("Tessellation", label_lightquality->GetPosition().x, label_lightquality->GetPosition().y + label_lightquality->GetSize().y + 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		combo_tessellation = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_tessellation->GetPosition().y, 180, 28, activepanel);
		combo_tessellation->AddItem("Disabled", setting == 0);
		combo_tessellation->AddItem("Low", setting == 1);
		combo_tessellation->AddItem("Medium", setting == 2);
		combo_tessellation->AddItem("High", setting == 3);
		combo_tessellation->AddItem("Ultra", setting == 4);
		combo_tessellation->SetLayout(0, 1, 1, 0);
#if 0
		// Post Effects
		setting = GetProgram()->GetSetting(SETTING_POSTFX, 0);
		label_postfx = CreateLabel("Post Effects", label_tessellation->GetPosition().x, label_tessellation->GetPosition().y + label_tessellation->GetSize().y + 8, 150, 28, panel[1], LABEL_MIDDLE | LABEL_LEFT);
		combo_postfx = CreateComboBox(panel[1]->GetSize().x - (180 + 8), label_postfx->GetPosition().y, 180, 28, panel[1]);
		combo_postfx->AddItem("Disabled", (appsettings->GetSSAO() == false && appsettings->GetFXAA() == false && appsettings->GetBloom() == false));
		combo_postfx->AddItem("SSAO", (appsettings->GetSSAO() == true && appsettings->GetFXAA() == false && appsettings->GetBloom() == false));
		combo_postfx->AddItem("SSAO + FXAA", (appsettings->GetSSAO() == true && appsettings->GetFXAA() == true && appsettings->GetBloom() == false));
		combo_postfx->AddItem("SSAO + FXAA + Bloom", (appsettings->GetSSAO() == true && appsettings->GetFXAA() == true && appsettings->GetBloom() == true));
#endif

		// Screen-space reflections
		setting = GetProgram()->GetSetting(SETTING_SSR, 0);
		label_ssr = CreateLabel("Screen-space reflections", label_tessellation->GetPosition().x, label_tessellation->GetPosition().y + label_tessellation->GetSize().y + 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		combo_ssr = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_ssr->GetPosition().y, 180, 28, activepanel);
		combo_ssr->AddItem("Disabled", (bool)setting == false);
		combo_ssr->AddItem("Enabled", (bool)setting == true);
		combo_ssr->SetLayout(0, 1, 1, 0);

		setting = GetProgram()->GetSetting(SETTING_FOV, 70);
		label_fov = CreateLabel("Field Of View (FOV)", label_ssr->GetPosition().x, label_ssr->GetPosition().y + label_ssr->GetSize().y + 8, 150, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		slider_fov = CreateNumberSlider(54, 90, setting, activepanel->GetSize().x - (180 + 8), label_fov->GetPosition().y, 180, 28, activepanel);
		slider_fov->SetLayout(0, 1, 1, 0);

		// Next panel....
		activepanel = panel[2]->subpanel;
		Assert(activepanel);

		auto gamecontroller = GetProgram()->gamecontroller;
		tabber_actionsets = CreateTabber(0, 0, activepanel->GetSize().x, activepanel->GetSize().y, activepanel, TABBER_MINIMAL);

		int panelheight = 40;
		// Create a tab for every action set.
		{
			for (const auto& p : gamecontroller->GetActionSets())
			{
				tabber_actionsets->AddItem(p);
				auto panel = CreatePanel(0, 0, tabber_actionsets->GetSize().x - 8, tabber_actionsets->GetSize().y, tabber_actionsets);
				int space = 4;
				if (!gamecontroller->config[p].empty())
				{
					for (auto a : gamecontroller->config[p])
					{
						std::string key = a.first;
						if (key.empty()) continue;
						if (key == "axes")
						{
							for (auto b : gamecontroller->config[p]["axes"])
							{
								std::string axisname = b.first;

								// HACK: Skip anything with "Camera". Binding vector axes is unsupported!!
								if (!axisname.empty() && axisname != "Camera")
								{
									auto bindpanel = CreateInputBindPanel(axisname, gamecontroller->config[p]["axes"], 4, space, panel->GetSize().x - 8, panelheight, panel, p);
									auto cast = bindpanel->As<InputBindPanel>();
									if (cast) cast->savetoaxis = true;
									space = bindpanel->GetPosition().y + bindpanel->GetSize().y + 4;
									button_binds.push_back(bindpanel);
								}
							}
							continue;
						}

						// HACK: Skip "mousecursor"
						if (key != "mousecursor")
						{
							auto bindpanel = CreateInputBindPanel(key, gamecontroller->config[p], 4, space, panel->GetSize().x - 8, panelheight, panel, p);
							space = bindpanel->GetPosition().y + bindpanel->GetSize().y + 4;
							button_binds.push_back(bindpanel);
						}
					}
				}
				tabber_panels.push_back(panel);
			}
		}

		// Global Controls
		{
			tabber_actionsets->AddItem("Global");
			auto panel = CreatePanel(0, 0, tabber_actionsets->GetSize().x - 8, tabber_actionsets->GetSize().y, tabber_actionsets);
			int space = 4;
			if (!gamecontroller->config.empty())
			{
				for (auto a : gamecontroller->config)
				{
					std::string key = a.first;
					if (key.empty()) continue;
					if (gamecontroller->IsRegisteredActionSet(key)) continue;
					if (key == "settings") continue;
					if (key == "axes")
					{
						for (auto b : gamecontroller->config["axes"])
						{
							std::string axisname = b.first;

							// HACK: Skip anything with "Camera". Binding vector axes is unsupported!!
							if (!axisname.empty() && axisname != "Camera")
							{
								auto bindpanel = CreateInputBindPanel(axisname, gamecontroller->config["axes"], 4, space, panel->GetSize().x - 8, panelheight, panel);
								auto cast = bindpanel->As<InputBindPanel>();
								if (cast) cast->savetoaxis = true;
								space = bindpanel->GetPosition().y + bindpanel->GetSize().y + 4;
								button_binds.push_back(bindpanel);
							}
						}
						continue;
					}

					auto bindpanel = CreateInputBindPanel(key, gamecontroller->config, 4, space, panel->GetSize().x - 8, panelheight, panel);
					space = bindpanel->GetPosition().y + bindpanel->GetSize().y + 4;
					button_binds.push_back(bindpanel);
				}
			}

			// Whatever is the first tab, make it the focused tab!
			tabber_panels.push_back(panel);
			EmitEvent(EVENT_WIDGETSELECT, tabber_actionsets);
		}

		// Settings
		{
			tabber_actionsets->AddItem("Settings");
			auto panel = CreatePanel(0, 0, tabber_actionsets->GetSize().x - 8, tabber_actionsets->GetSize().y, tabber_actionsets);
			int space = 4;
			if (!gamecontroller->config["settings"].empty())
			{
				for (auto a : gamecontroller->config["settings"])
				{
					std::string name = a.first;
					auto settingspanel = CreateInputSettingsPanel(name, gamecontroller->config, 4, space, panel->GetSize().x - 8, 40, panel);
					space = settingspanel->GetPosition().y + settingspanel->GetSize().y + 4;
					settings_panels.push_back(settingspanel);
				}
			}


			tabber_panels.push_back(panel);
		}

		// Set the first item as selected.
		tabber_actionsets->SelectItem(0);

		// Resize the scroll subpanel based on the total number of button_binds. 
		// It's not perfect,but it's good enough to ensure everything is being drawn since the orginal code didn't
		// consider scrollpanels.
		tabber_actionsets->SetShape(iVec2(0), iVec2(tabber_actionsets->GetSize().x, (int)button_binds.size() * panelheight));
		panel[2]->SetArea(tabber_actionsets->GetSize().x, tabber_actionsets->GetSize().y);

		// Next panel....
		activepanel = panel[3]->subpanel;
		Assert(activepanel);

		label_hrtf = CreateLabel("Head-Related Transfer Function (HRTF)", 8, 8, 220, 28, activepanel, LABEL_MIDDLE | LABEL_LEFT);
		label_hrtf->SetLayout(1, 0, 0, 0);

		setting = GetProgram()->GetSetting(SETTING_HRTF, 0);
		combo_hrtf = CreateComboBox(activepanel->GetSize().x - (180 + 8), label_hrtf->GetPosition().y, 180, 28, activepanel);
		combo_hrtf->SetLayout(0, 1, 1, 0);
		combo_hrtf->AddItem("Disabled", (bool)setting == false);
		combo_hrtf->AddItem("Enabled", (bool)setting == true);

		Listen(EVENT_NONE);
	}

	bool SettingsUI::ProcessEvent(const Event& e)
	{
		if (e.id == EVENT_WINDOWSIZE)
		{
			auto ui_size = root->GetSize();
			for (int i = 0; i < panel.size() -1; i++)
			{
				panel[i]->SetArea(ui_size.x - (listbox->GetPosition().x + listbox->GetSize().x) - 16, 350);
			}
		}
		else if (e.id == EVENT_WIDGETACTION)
		{
			if (e.source == button_ok)
			{
				// Apply and quit
				ApplySettings();
				EmitEvent(EVENT_QUIT);
			}
			else if (e.source == button_cancel)
			{
				// Quit
				EmitEvent(EVENT_QUIT);
			}
			else if (e.source == button_apply)
			{
				// Apply but don't quit.
				ApplySettings();
			}
		}
		else if (e.id == EVENT_WIDGETSELECT)
		{
			if (e.source == listbox)
			{
				for (std::size_t n = 0; n < panel.size(); ++n)
				{
					if (n == e.data)
					{
						panel[n]->SetHidden(false);
					}
					else
					{
						panel[n]->SetHidden(true);
					}
				}
			}
			else if (e.source == combo_display)
			{
				auto res_combo_box_class = combo_resoultion->As<ResoultionComboBox>();
				if (res_combo_box_class)
				{
					res_combo_box_class->ShowDisplayBox(e.data);
				}
			}
			else if (e.source == combo_windowmode)
			{
				// Force the res combo box to the user's native resoultion.
				if (combo_windowmode->GetSelectedItem() == 3)
				{
					auto res_combo_box_class = combo_resoultion->As<ResoultionComboBox>();
					if (res_combo_box_class)
					{
						const int index = combo_display->GetSelectedItem();
						auto display = GetDisplays()[index];
						String str = String(display->size.x) + " x " + String(display->size.y);
						res_combo_box_class->ShowDisplayBox(index);
						res_combo_box_class->ForceSelection(index, str);

						res_combo_box_class->Disable();
					}
				}
				else
				{
					auto res_combo_box_class = combo_resoultion->As<ResoultionComboBox>();
					if (res_combo_box_class)
					{
						res_combo_box_class->Enable();
					}
				}
			}
			else if (e.source == combo_windowtheme)
			{
				auto config = GetProgram()->config;
				WString theme = "UI/Color Schemes/dark.json";

				bool lightmode = false;
				const TitlebarTheme themesetting = (TitlebarTheme)e.data;
				if (themesetting == TITLEBAR_DEFAULT)
					lightmode = true;
				else if (themesetting == TITLEBAR_SYSTEM)
					lightmode = LightThemeEnabled();

				if (lightmode == false)
				{
					if (config["colorSchemesPath"].is_string() && config["defaultSettings"]["darkColorScheme"].is_string())
					{
						theme = String(config["colorSchemesPath"]) + "/" + String(config["defaultSettings"]["darkColorScheme"]);
					}
				}
				else
				{
					if (config["colorSchemesPath"].is_string() && config["defaultSettings"]["lightColorScheme"].is_string())
					{
						theme = String(config["colorSchemesPath"]) + "/" + String(config["defaultSettings"]["lightColorScheme"]);
					}
				}

				//SetWindowTitlebarTheme(window, themesetting);
				//ui->LoadColorScheme(theme);
			}
			else if (e.source == tabber_actionsets)
			{
				for (int n = 0; n < tabber_actionsets->items.size(); ++n)
				{
					if (n == e.data)
					{
						tabber_panels[n]->SetHidden(false);
					}
					else
					{
						tabber_panels[n]->SetHidden(true);
					}
				}
			}

		}

		return true;
	}

	void SettingsUI::ApplySettings()
	{
		GetProgram()->SetSetting(SETTING_DISPLAY, combo_display->GetSelectedItem());
		auto res_combo_box_class = combo_resoultion->As<ResoultionComboBox>();
		if (res_combo_box_class)
		{
			auto res = res_combo_box_class->GetResoultion(combo_display->GetSelectedItem());
			GetProgram()->SetSetting(SETTING_SCREENWIDTH, res.x);
			GetProgram()->SetSetting(SETTING_SCREENHEIGHT, res.y);
		}
		GetProgram()->SetSetting(SETTING_WINDOWMODE, combo_windowmode->GetSelectedItem());
		GetProgram()->SetSetting(SETTING_WINDOWTHEME, combo_windowtheme->GetSelectedItem());

		auto fov_slider = slider_fov->As<NumberSlider>();
		if (fov_slider)
		{
			GetProgram()->SetSetting(SETTING_FOV, (int)fov_slider->GetValue());
		}

#if 0
		auto gamma_slider = slider_gamma->As<NumberSlider>();
		if (gamma_slider)
		{
			GetProgram()->SetSetting(SETTING_GAMMA, (int)gamma_slider->GetValue());
		}
#endif

		// Graphics
		GetProgram()->SetSetting(SETTING_LIGHTQUALITY, combo_lightquality->GetSelectedItem());
		GetProgram()->SetSetting(SETTING_TESSELLATION, combo_tessellation->GetSelectedItem());
		GetProgram()->SetSetting(SETTING_SSR, combo_ssr->GetSelectedItem());

		// Sound
		GetProgram()->SetSetting(SETTING_HRTF, combo_hrtf->GetSelectedItem());

		// Controls!
		for (const auto& p : button_binds)
		{
			auto cast = p->As<InputBindPanel>();
			if (cast)
			{
				cast->WriteBinding();
			}
		}

		for (const auto& s : settings_panels)
		{
			auto cast = s->As<InputSettingsPanel>();
			if (cast)
			{
				cast->WriteSetting();
			}
		}

		// Save the controller config.
		if (!GetProgram()->SaveControllerBindings())
		{
			Print("Error: Failed to save controller bindings.");
		}
	}
}