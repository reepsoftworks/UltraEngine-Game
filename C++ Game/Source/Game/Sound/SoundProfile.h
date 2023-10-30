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
	class SoundProfile
	{
	protected:
		WString filepath;
		float volume;
		float pitch;
		float range;
		bool loopmode;
		bool is3D;
		WString closecaption;
		WString whospeaking;

		std::vector<shared_ptr<Sound>> sounds;

	public:
		SoundProfile();
		~SoundProfile();

		const float GetVolume();
		const float GetPitch();
		const float GetRange();
		const bool GetLoopMode();
		const bool Is3D();
		WString GetCloseCaption();
		WString GetWhosSpeaking();
		WString GetFilePath();

		const std::vector<shared_ptr<Sound>> GetSounds();

		friend shared_ptr<SoundProfile> LoadSoundProfile(const WString& path, const LoadFlags flags);
		friend shared_ptr<SoundProfile> LoadSoundProfileSafe(const WString& path, const LoadFlags flags);
	};

	extern shared_ptr<SoundProfile> LoadSoundProfile(const WString& path, const LoadFlags flags = LOAD_DEFAULT);
	extern shared_ptr<SoundProfile> LoadSoundProfileSafe(const WString& path, const LoadFlags flags = LOAD_DEFAULT);
}