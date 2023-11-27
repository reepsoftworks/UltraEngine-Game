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
		// Never reposition for VR.
		if (GetProgram()->VREnabled() == false)
		{
			size = framebuffersize;
			camera->SetPosition(float(size.x) * 0.5f, float(size.y) * 0.5f, depth);
		}
		else
		{
			camera->SetPosition(size.x, size.y, depth);
		}
	}

	unsigned int Canvas::GetRenderLayers()
	{
		return camera->GetRenderLayers();
	}

	iVec2 Canvas::GetSize()
	{
		return size;
	}

	shared_ptr<Camera> Canvas::GetCamera()
	{
		return camera;
	}

	std::map<int, std::weak_ptr<Canvas>> canvases;
	shared_ptr<Canvas> GetCanvas(shared_ptr<World> world, const int renderlayer, const float depth)
	{
		auto canvas = canvases[renderlayer].lock();
		if (canvas == NULL)
		{
			canvas = std::make_shared<Canvas>();

			const bool vr = GetProgram()->VREnabled();
			if (!vr)
			{
				auto sz = GetProgram()->GetFramebufferSize();
				canvas->camera = CreateCamera(world, PROJECTION_ORTHOGRAPHIC);
				canvas->camera->SetRenderLayers(renderlayer);
				canvas->camera->SetClearMode(CLEAR_DEPTH);
				canvas->camera->SetPosition(float(sz.x) * 0.5f, float(sz.y) * 0.5f, depth);
				canvas->size = sz;
				canvas->depth = depth;
			}
			else
			{
				canvas->camera = CreateCamera(world, PROJECTION_PERSPECTIVE);
				canvas->camera->SetRenderLayers(renderlayer);
				canvas->camera->SetClearMode(CLEAR_DEPTH);
				canvas->camera->SetPosition(0, 0, depth);
				canvas->size = 0;
				canvas->depth = depth;
			}
		}

		return canvas;
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