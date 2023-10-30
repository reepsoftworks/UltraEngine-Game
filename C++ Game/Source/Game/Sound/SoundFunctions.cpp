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
#include "../Game.h"

namespace UltraEngine::Game
{
	shared_ptr<Sound> LoadFallbackSound()
	{
		// Fallback sound file to load. 
		WString error_sound = ASSETFALLBACK_SOUND;

		shared_ptr<Sound> snd = NULL;
		if (FileType(error_sound) == 1)
		{
			snd = LoadSound(error_sound);
		}

		// Check if the sound is vaild.
		if (!snd) RuntimeError("Failed to load sound " + QuoteWString(error_sound) + " as file is missing.");
		return snd;
	}

	shared_ptr<Sound> LoadSoundSafe(const WString& path, const LoadFlags flags)
	{
		shared_ptr<Sound> snd = NULL;

		// If the file is invaild, load the fallback.
		if (FileType(path) == 0)
		{
			Print("Error: Failed to load sound file " + QuoteWString(path) + " as file is missing.");
			snd = LoadFallbackSound();
		}
		else
		{
			// Try to load the sound.
			// If it's invaild, release the pointer and load the fallback.
			snd = LoadSound(path, flags);
			if (!snd)
			{
				Print("Error: Failed to load sound file " + QuoteWString(path) + " as file is invaild.");
				snd = NULL;
				snd = LoadFallbackSound();
			}
		}

		return snd;
	}

	shared_ptr<Speaker> LoadSpeaker(shared_ptr<SoundProfile> profile)
	{
		shared_ptr<Speaker> speaker = CreateSpeaker(profile->GetSounds()[0]);
		speaker->SetVolume(profile->GetVolume());
		speaker->SetPitch(profile->GetPitch());
		speaker->SetRange(profile->GetRange());
		speaker->SetLooping(profile->GetLoopMode());
		return speaker;
	}

	shared_ptr<Speaker> LoadSpeaker(const WString& path, const LoadFlags flags)
	{
		shared_ptr<SoundProfile> profile = LoadSoundProfileSafe(path, flags);
		shared_ptr<Speaker> speaker = LoadSpeaker(profile);
		return speaker;
	}

	void EmitSoundFromEntity(shared_ptr<Entity> entity, shared_ptr<SoundProfile> profile)
	{
		auto sound = profile->GetSounds();
		if (!sound.empty())
		{
			if (sound.size() > 1)
			{
				int index = RandomNumber(0, (int)sound.size() - 1);
				if (sound[index] == NULL)
				{
					Print("Error: Failed to play as sound file is NULL.");
				}
				else
				{
					entity->EmitSound(sound[index], profile->GetRange(), profile->GetVolume(), profile->GetPitch(), profile->GetLoopMode(), profile->Is3D());
					EmitEvent(EVENT_GAMESPEAKER, GetProgram(), 0, 0, 0, 0, 0, entity, profile->GetCloseCaption());
				}
			}
			else
			{
				if (sound[0] == NULL)
				{
					Print("Error: Failed to play as sound file is NULL.");
				}
				else
				{
					entity->EmitSound(sound[0], profile->GetRange(), profile->GetVolume(), profile->GetPitch(), profile->GetLoopMode(), profile->Is3D());
					EmitEvent(EVENT_GAMESPEAKER, GetProgram(), 0, 0, 0, 0, 0, entity, profile->GetCloseCaption());
				}
			}
		}
	}
}