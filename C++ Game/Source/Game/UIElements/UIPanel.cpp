//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "UIPanel.h"

namespace UltraEngine::Game
{
	UIPanel::UIPanel()
	{
		root = NULL;
	}

	UIPanel::~UIPanel()
	{
		root = NULL;
	}
}