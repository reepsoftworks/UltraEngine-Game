//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#pragma once
#include "UltraEngine.h"

namespace UltraEngine::Game
{
	class Stage;
	class LoadingScreen : public Object
	{
		std::weak_ptr<Stage> stage;
		shared_ptr<Camera> camera;
		shared_ptr<Interface> ui;
		shared_ptr<Widget> logo;
		shared_ptr<Widget> loadingpanel;
		shared_ptr<Widget> loadinglabel;
		std::vector<shared_ptr<Pixmap>> loadingimage;

		//shared_ptr<Pivot> console;

		static bool EventCallback(const Event& e, shared_ptr<Object> extra);

	protected:
		void BuildGUI();
		virtual void Start();
		virtual void BuildClearScreen();
		virtual void ResizeClearScreen(const iVec2 size);
		virtual void BuildLoadScreen();
		virtual void ResizeLoadScreen(const iVec2 size);

		void Listen(const EventId eventid, shared_ptr<Object> source);
		virtual bool ProcessEvent(const Event& e);

		shared_ptr<Stage> GetStage();

	public:
		LoadingScreen();

		virtual void ShowClearScreen();
		virtual void ShowLoadScreen(const int index);
		
		friend class Stage;
	};
}