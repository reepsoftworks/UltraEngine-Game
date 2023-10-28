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
#include "Game.h"

namespace UltraEngine::Game
{
	Canvas::Canvas()
	{
		camera = NULL;
	}

	Canvas::~Canvas()
	{
		camera = NULL;
	}

	void Canvas::SetHidden(const bool hide)
	{
		if (camera)
		{
			camera->SetHidden(hide);
		}
	}

	bool Canvas::GetHidden()
	{
		return camera->GetHidden();
	}

	void Canvas::Reposition(const iVec2& framebuffersize)
	{
		camera->SetPosition(float(framebuffersize.x) * 0.5f, float(framebuffersize.y) * 0.5f);
	}

	unsigned int Canvas::GetRenderLayers()
	{
		return camera->GetRenderLayers();
	}

	std::map<int, shared_ptr<Canvas>> canvases;
	shared_ptr<Canvas> GetCanvas(shared_ptr<World> world, const int renderlayer)
	{
		if (canvases[renderlayer] == NULL)
		{
			canvases[renderlayer] = std::make_shared<Canvas>();

			auto sz = GetProgram()->GetFramebufferSize();
			canvases[renderlayer]->camera = CreateCamera(world, PROJECTION_ORTHOGRAPHIC);
			canvases[renderlayer]->camera->SetRenderLayers(renderlayer);
			canvases[renderlayer]->camera->SetClearMode(CLEAR_DEPTH);
			canvases[renderlayer]->camera->SetPosition(float(sz.x) * 0.5f, float(sz.y) * 0.5f);
		}

		return canvases[renderlayer];
	}

	void RenderToCanvas(shared_ptr<Entity> entity, shared_ptr<Canvas> canvas)
	{
		if (canvas == NULL)
			return;

		if (entity)
		{
			entity->SetRenderLayers(canvas->GetRenderLayers());
		}
	}
}