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
	class Canvas : public Object
	{
		shared_ptr<Camera> camera;	

	public:
		Canvas();
		virtual ~Canvas();

		void SetHidden(const bool hide);
		bool GetHidden();

		void Reposition(const iVec2& framebuffersize);
		unsigned int GetRenderLayers();

		friend shared_ptr<Canvas> GetCanvas(shared_ptr<World> world, const int renderlayer);
		friend void RenderToCanvas(shared_ptr<Entity> entity, shared_ptr<Canvas> canvas);
	};

	extern std::map<int, shared_ptr<Canvas>> canvases;
	extern shared_ptr<Canvas> GetCanvas(shared_ptr<World> world, const int renderlayer);
	extern void RenderToCanvas(shared_ptr<Entity> entity, shared_ptr<Canvas> canvas);
}