//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "../Game.h"

namespace UltraEngine::Game
{
	SoundProfile::SoundProfile()
	{
		volume = 1.0f;
		pitch = 1.0f;
		range = 50.0f;
		loopmode = false;
		is3D = true;
		whospeaking.clear();
		closecaption.clear();
		sounds.clear();
		filepath.clear();
	}

	SoundProfile::~SoundProfile()
	{
		sounds.clear();
	}

	const float SoundProfile::GetVolume()
	{
		return volume;
	}

	const float SoundProfile::GetPitch()
	{
		return pitch;
	}

	const float SoundProfile::GetRange()
	{
		return range;
	}

	const bool SoundProfile::GetLoopMode()
	{
		return loopmode;
	}

	const bool SoundProfile::Is3D()
	{
		return is3D;
	}

	WString SoundProfile::GetCloseCaption()
	{
		return closecaption;
	}

	WString SoundProfile::GetWhosSpeaking()
	{
		return whospeaking;
	}

	WString SoundProfile::GetFilePath()
	{
		return filepath;
	}

	const std::vector<shared_ptr<Sound>> SoundProfile::GetSounds()
	{
		return sounds;
	}

	shared_ptr<SoundProfile> LoadSoundProfile(const WString& path, const LoadFlags flags)
	{
		if (path.empty()) return NULL;
		auto table = LoadTable(path);
		if (table.is_null())
		{
			Print("Error: Failed to load sound profile " + QuoteWString(path) + ".");
			return NULL;
		}

		if (table["audioProfile"].is_null())
		{
			Print("Error: Failed to load sound profile " + QuoteWString(path) + ".");
			return NULL;
		}

		auto profile = std::make_shared<SoundProfile>();
		profile->filepath = path;
		if (table["audioProfile"]["file"].is_array())
		{
			for (const auto& p : table["audioProfile"]["file"])
			{
				String file = p.second;
				profile->sounds.push_back(LoadSoundSafe(file, flags));
			}
		}
		else if (table["audioProfile"]["file"].is_string())
		{
			String file = table["audioProfile"]["file"];
			profile->sounds.push_back(LoadSoundSafe(file, flags));
		}

		// Volume
		if (table["audioProfile"]["volume"].is_number() || table["audioProfile"]["volume"].is_float())
		{
			profile->volume = table["audioProfile"]["volume"];
		}

		// Pitch
		if (table["audioProfile"]["pitch"].is_number() || table["audioProfile"]["pitch"].is_float())
		{
			profile->pitch = table["audioProfile"]["pitch"];
		}

		// Range
		if (table["audioProfile"]["range"].is_number() || table["audioProfile"]["range"].is_float())
		{
			profile->range = table["audioProfile"]["range"];
		}

		// Loop
		if (table["audioProfile"]["loop"].is_boolean())
		{
			profile->loopmode = table["audioProfile"]["loop"];
		}

		// Is this a 3D sound?
		if (table["audioProfile"]["is3D"].is_boolean())
		{
			profile->is3D = table["audioProfile"]["is3D"];
		}

		// Close Caption
		if (table["audioProfile"]["closecaption"].is_string())
		{
			profile->closecaption = table["audioProfile"]["closecaption"];
		}

		// Who's speaking?
		if (table["audioProfile"]["speaker"].is_string())
		{
			profile->whospeaking = table["audioProfile"]["speaker"];
		}

		return profile;
	}


	shared_ptr<SoundProfile> LoadSoundProfileSafe(const WString& path, const LoadFlags flags)
	{
		shared_ptr<SoundProfile> profile = NULL;
		if (FileType(path) == 0)
		{
			Print("Error: Failed to load sound profile " + QuoteWString(path) + " as file is missing.");

			// Create a profile.
			auto profile = std::make_shared<SoundProfile>();
			profile->filepath = "";
			profile->volume = 1.0f;
			profile->pitch = 1.0f;
			profile->range = 50.0f;
			profile->sounds.push_back(LoadSoundSafe(ASSETFALLBACK_SOUND, flags));
		}
		else
		{
			// Try to load the sound.
			// If it's invaild, release the pointer and load the fallback.
			profile = LoadSoundProfile(path, flags);
			if (!profile)
			{
				Print("Error: Failed to load sound profile " + QuoteWString(path) + " as file is invaild.");
				profile = NULL;

				// Create a profile.
				auto profile = std::make_shared<SoundProfile>();
				profile->filepath = "";
				profile->volume = 1.0f;
				profile->pitch = 1.0f;
				profile->range = 50.0f;
				profile->sounds.push_back(LoadSoundSafe(ASSETFALLBACK_SOUND, flags));
			}
		}

		if (profile) RuntimeError("Failed create fallback sound profile.");
		return profile;
	}
}