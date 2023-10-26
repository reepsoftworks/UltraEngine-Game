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

namespace UltraEngine
{
	const EventId EVENT_PROGRAMSTART = EventId(11001); // Used when the correct applet is into memory.
	const EventId EVENT_GRAPHICSWINDOW = EventId(11002); // Emitted when a graphics window is created. Useful when rescaling the UI.
	const EventId EVENT_CONSOLEEXECUTE = EventId(11003); // Emitted when a console command is executed.
	const EventId EVENT_SETTINGCHANGED = EventId(11004); // Emitted when a setting has been changed.

	// Stage
	const EventId EVENT_STAGELOADED = EventId(11005); // Emitted when a scene has successfully loaded.
	const EventId EVENT_STAGECLEARED = EventId(11006); // Emitted when a scene has successfully been cleared.
	const EventId EVENT_PAUSESTATE = EventId(11007); // Emitted when the stage is paused. Poll it to change states.

	// Input
	const EventId EVENT_INPUTDEVICE = EventId(11008); // Emitted when a new form of input is activated.

	// Sound
	const EventId EVENT_GAMESPEAKER = EventId(11009); // Emitted when a GameSpeaker emits a sound.
}