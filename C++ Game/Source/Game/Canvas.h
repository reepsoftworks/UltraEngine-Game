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
		iVec2 size;
		float depth;

	public:
		shared_ptr<Interface> ui;

		Canvas();
		virtual ~Canvas();

		void SetHidden(const bool hide);
		bool GetHidden();

		void Reposition(const iVec2& framebuffersize);
		unsigned int GetRenderLayers();
		iVec2 GetSize();
		shared_ptr<Camera> GetCamera();

		friend shared_ptr<Canvas> GetCanvas(shared_ptr<World> world, const int renderlayer, const float depth);
		friend void RenderToCanvas(shared_ptr<Entity> entity, shared_ptr<Canvas> canvas);
	};

	extern std::map<int, std::weak_ptr<Canvas>> canvases;
	extern shared_ptr<Canvas> GetCanvas(shared_ptr<World> world, const int renderlayer, const float depth = 0.0f);
	extern void RenderToCanvas(shared_ptr<Entity> entity, shared_ptr<Canvas> canvas);
}