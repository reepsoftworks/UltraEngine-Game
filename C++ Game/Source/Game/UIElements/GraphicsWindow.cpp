//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "GraphicsWindow.h"

namespace UltraEngine::Game
{
	GraphicsWindow::GraphicsWindow()
	{
		window = NULL;
		framebuffersize = iVec2(0);
	}

	bool GraphicsWindow::Initialize(const WString& title, const int width, const int height, shared_ptr<Display> display, const GraphicWindowStyles style)
	{
		// Release existing pointers so we can rebuild!
		static bool firstwindow = true;

		// Set the cursor to default before we delete the window.
		if (window)
		{
			// I need this function to exist to reapply the cursor after the new window.
			// Right now, I'm using a global member "currentcursor".
			// https://www.ultraengine.com/community/topic/62164-windowgetcursor/#comment-301820
			//current_cursor = window->GetCursor();
			window->SetCursor(CURSOR_DEFAULT);
		}

		window = NULL;
		framebuffer = NULL;

		int w = width;
		int h = height;

		WindowStyles window_style = WINDOW_TITLEBAR | WINDOW_CENTER | WINDOW_HIDDEN;
		switch (style)
		{
		case GRAPHICSWINDOW_TITLEBAR:
			window_style = WINDOW_TITLEBAR | WINDOW_CENTER | WINDOW_HIDDEN;
			break;

		case GRAPHICSWINDOW_BORDERLESS:
			window_style = WINDOW_CENTER | WINDOW_HIDDEN;
			break;

		case GRAPHICSWINDOW_FULLSCREEN:
			window_style = WINDOW_FULLSCREEN | WINDOW_CENTER | WINDOW_HIDDEN;
			break;

		case GRAPHICSWINDOW_FULLSCREENNATIVE:
			w = display->size.x;
			h = display->size.y;
			window_style = WINDOW_FULLSCREEN | WINDOW_CENTER | WINDOW_HIDDEN;
			break;

		default:
			break;
		}

		this->style = style;
		if (IsDebug())
		{
			WString debugtag = " - [Debug]"; // 10;
			if (title.Right((int)debugtag.length()) != debugtag)
			{
				windowtitle = title + debugtag;
			}
			else
			{
				windowtitle = title;
			}
		}
		else
		{
			windowtitle = title;
		}
			
		window = CreateWindow(windowtitle, 0, 0, w, h, display, window_style);

		const TitlebarTheme themesetting = (TitlebarTheme)GetProgram()->GetSetting(SETTING_WINDOWTHEME, (int)TITLEBAR_SYSTEM);
		SetWindowTitlebarTheme(window, themesetting);
		framebuffer = CreateFramebuffer(window);

		if (firstwindow)
		{
			splashscreen = std::make_shared<SplashScreen>();
			if (!splashscreen->Initialize(window, 1500)) splashscreen = NULL;
			firstwindow = false;
		}
		else
		{
			window->SetHidden(false);
		}

		// Set this to focus!
		window->SetCursor(currentcursor);
		window->Activate();

		// Send this event with the size of the framebuffer.
		EmitEvent(EVENT_GRAPHICSWINDOW, GetProgram(), 0, window->GetPosition().x, window->GetPosition().y, framebuffer->GetSize().x, framebuffer->GetSize().y, window);
		return true;
	}

	bool GraphicsWindow::ProcessEvent(const Event& e)
	{	
		//if (splashscreen != NULL) splashscreen->ProcessEvent(e);
		if (splashscreen)
		{
			if (e.id == EVENT_TIMERTICK && e.source == splashscreen->timer)
			{
				splashscreen->SetHidden(true);
				window->SetHidden(false);
				window->Activate();
				splashscreen->timer = NULL;
			}
		}

		if (e.id == EVENT_STARTRENDERER)
		{
			if (e.data == 0)
			{
				MessageError(L"Renderer failed to intialize.\n\n" + e.text, "Error");
				return false;
			}

			if (splashscreen == NULL)
			{
				window->SetHidden(false);
				window->Activate();
			}
		}
		else if (e.id == EVENT_WINDOWSIZE)
		{
			if (e.extra != NULL)
			{
				auto display = e.extra->As<Display>();
				Resize(display, e.size.x, e.size.y, (GraphicWindowStyles)e.data);
			}
		}
		else if (e.id == EVENT_WINDOWDPICHANGED)
		{
			// This isn't supported due to the program tweaking out when this event is called...
			//auto display = GetProgram()->maindisplay;
			//Resize(display, e.size.x, e.size.y, style);
		}
		else if (e.id == EVENT_WINDOWCLOSE && e.source == window)
		{
			EmitEvent(EVENT_QUIT);
		}
		else if (e.id == EVENT_STAGELOADED || e.id == EVENT_STAGECLEARED)
		{
			// Make sure we auto activate upon map load and clearing.
			window->Activate();
		}
		return true;
	}

	void GraphicsWindow::Resize(shared_ptr<Display> display, const int width, const int height, const GraphicWindowStyles style)
	{
		if (display != NULL)
		{
			if (!Initialize(windowtitle, width, height, display, style))
			{
				MessageError("Failed to rebuild window.", "Error");
			}
		}
	}

	void GraphicsWindow::Render(shared_ptr<World> world)
	{
		Assert(framebuffer != NULL, "Assert: Failed to render as framebuffer is NULL!");
		Assert(world != NULL, "Assert: Failed to render as world is NULL!");
		world->Render(framebuffer);
	}

	const iVec2 GraphicsWindow::GetFramebufferSize()
	{
		auto ret = iVec2(0);
		if (framebuffer != NULL)
		{
			ret = framebuffer->GetSize();
		}
		return ret;
	}

	extern shared_ptr<GraphicsWindow> CreateGraphicsWindow(const WString& title, const int width, const int height, shared_ptr<Display> display, const GraphicWindowStyles style)
	{
		auto app = std::make_shared<GraphicsWindow>();
		app->Initialize(title, width, height, display, style);
		//app->Listen(EVENT_NONE, NULL); 	// << Listen to all events!
		return app;
	}
}