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
	extern shared_ptr<Sound> LoadSoundSafe(const WString& path, const LoadFlags flags);
	extern shared_ptr<Speaker> LoadSpeaker(shared_ptr<SoundProfile> profile);
	extern shared_ptr<Speaker> LoadSpeaker(const WString& path, const LoadFlags flags);
	extern void EmitSoundFromEntity(shared_ptr<Entity> entity, shared_ptr<SoundProfile> profile);
}