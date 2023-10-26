//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "GameLuaBindings.h"

namespace UltraEngine::Game
{
	void InitGameLuaBindings(sol::state* L)
	{
		// Set Event IDs
		L->set("EVENT_PROGRAMSTART", int(EVENT_PROGRAMSTART));
		L->set("EVENT_GRAPHICSWINDOW", int(EVENT_GRAPHICSWINDOW));
		L->set("EVENT_CONSOLEEXECUTE", int(EVENT_CONSOLEEXECUTE));
		L->set("EVENT_SETTINGCHANGED", int(EVENT_SETTINGCHANGED));
		L->set("EVENT_STAGELOADED", int(EVENT_STAGELOADED));
		L->set("EVENT_STAGECLEARED", int(EVENT_STAGECLEARED));
		L->set("EVENT_PAUSESTATE", int(EVENT_PAUSESTATE));
	}
}