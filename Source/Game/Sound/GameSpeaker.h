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
	class SoundVolume;
	class GameSpeaker : public Object
	{
		shared_ptr<SoundProfile> profile;
		shared_ptr<Speaker> speaker;

		Vec3 position;
		float length;
		bool pausewithstage;
		float pausetime;
		int filtersize;

	public:
		GameSpeaker();
		~GameSpeaker();

		shared_ptr<SoundProfile> GetProfile();
		void AddFilters(const std::vector<shared_ptr<AudioFilter>>& filters);
		void ClearFilters();
		virtual shared_ptr<GameSpeaker> GameSpeaker::Copy();
		void SetTime(const float time);
		float GetTime();
		float GetLength();
		void Play(const int index = 0);
		void PlayRandom();
		void Cue();
		void Pause();
		void Resume();
		void Stop();
		void SetDirection(const Vec3& direction);
		void SetConeAngles(const float inner, const float outer);
		const SpeakerState GetState();
		void SetPosition(const xVec3& position);
		Vec3 GetPosition();
		void OverrideProfile(const float volume, const float pitch = 0.0f, const float range = 0.0f);
		void PauseWithStage(const bool state);

		const bool Load(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const LoadFlags flags);
		const bool Save(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const SaveFlags flags);
		virtual bool ProcessEvent(const Event& e);

		friend class SoundVolume;
		friend shared_ptr<GameSpeaker> CreateGameSpeaker(const WString& path, const xVec3& position);
	};

	extern std::vector<std::shared_ptr<GameSpeaker>> GameSpeakers;
	extern shared_ptr<GameSpeaker> CreateGameSpeaker(const WString& path, const xVec3& position = xVec3(0));
}