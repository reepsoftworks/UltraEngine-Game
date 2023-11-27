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
	std::vector<std::shared_ptr<GameSpeaker>> GameSpeakers;
	static bool GameSpeakerEventCallback(const Event& e, shared_ptr<Object> extra)
	{
		return extra->As<GameSpeaker>()->ProcessEvent(e);
	}

	GameSpeaker::GameSpeaker()
	{
		profile = NULL;
		speaker = NULL;
		pausewithstage = true;
	}

	GameSpeaker::~GameSpeaker()
	{
		profile = NULL;
		speaker = NULL;
		pausewithstage = false;
	}

	shared_ptr<SoundProfile> GameSpeaker::GetProfile()
	{
		return profile;
	}

	void GameSpeaker::AddFilters(const std::vector<shared_ptr<AudioFilter>>& filters)
	{
		if (speaker)
		{
			for (const auto& p : filters)
			{
				for (int i = 0; i < filters.size(); i++)
				{
					speaker->SetFilter(filters[i], i);
					filtersize++;
				}
			}
		}
	}

	void GameSpeaker::ClearFilters()
	{
		// Clear filters if the size is non-zero.
		if (filtersize > 0)
		{
			for (int i = 0; i < filtersize; i++)
			{
				speaker->SetFilter(NULL, i);
			}
			filtersize = 0;
		}
	}

	shared_ptr<GameSpeaker> GameSpeaker::Copy()
	{
		return std::make_shared<GameSpeaker>(*this);
	}

	void GameSpeaker::SetTime(const float time)
	{
		if (speaker)
		{
			speaker->SetTime(time);
		}
	}

	float GameSpeaker::GetTime()
	{
		if (speaker)
		{
			return speaker->GetTime();
		}

		return 0.0f;
	}

	float GameSpeaker::GetLength()
	{
		return length;
	}

	void GameSpeaker::Play(const int index)
	{
		auto sound = profile->GetSounds();
		if (!sound.empty())
		{
			if (speaker)
			{	
				length = profile->GetSounds()[0]->GetLength();
				speaker->SetSound(sound[0]);
				speaker->SetTime(0.0f);
				speaker->Play();
				EmitEvent(EVENT_GAMESPEAKER, GetProgram(), 0, 0, 0, 0, 0, Self(), profile->GetCloseCaption());
			}
		}
	}

	void GameSpeaker::PlayRandom()
	{
		auto sound = profile->GetSounds();
		if (!sound.empty())
		{
			if (sound.size() > 1)
			{
				int index = RandomNumber(0, (int)sound.size() - 1);
				if (sound[index] == NULL)
				{
					Print("Error: Failed to play game speaker as sound file is NULL.");
				}
				else
				{
					Play(index);
				}
			}
			else
			{
				Play(0);
			}
		}
	}

	void GameSpeaker::Cue()
	{
		if (speaker) speaker->Cue();
	}

	void GameSpeaker::Pause()
	{
		if (speaker)
		{
			//if (speaker->GetState() == SPEAKER_PLAYING) speaker->Pause();

			if (speaker->GetState() == SPEAKER_PLAYING)
			{
				pausetime = speaker->GetTime();
				speaker->Pause();
			}
		}
	}

	void GameSpeaker::Resume()
	{
		if (speaker)
		{
			//if (speaker->GetState() == SPEAKER_PAUSED) speaker->Resume();

			//https://www.ultraengine.com/community/topic/62290-speakerpause-sets-its-state-to-speaker_stopped
			if (speaker->GetState() == SPEAKER_PAUSED || pausetime > 0)
			{
				speaker->SetTime(pausetime);
				speaker->Play();
				pausetime = 0;
			}
		}		
	}

	void GameSpeaker::Stop()
	{
		if (speaker) speaker->Stop();
	}

	void GameSpeaker::SetDirection(const Vec3& direction)
	{
		if (speaker) speaker->SetDirection(direction);
	}
	void GameSpeaker::SetConeAngles(const float inner, const float outer)
	{
		if (speaker) speaker->SetConeAngles(inner, outer);
	}

	const SpeakerState GameSpeaker::GetState()
	{
		if (speaker) return (SpeakerState)speaker->GetState();
		return SPEAKER_STOPPED;
	}

	void GameSpeaker::SetPosition(const xVec3& position)
	{
		this->position = position;
		if (speaker)
		{
			speaker->SetPosition(position);
		}
	}

	Vec3 GameSpeaker::GetPosition()
	{
		return position;
	}

	void GameSpeaker::OverrideProfile(const float volume, const float pitch, const float range)
	{
		if (speaker)
		{
			if (volume > 0.0f) speaker->SetVolume(volume);
			if (pitch > 0.0f) speaker->SetPitch(pitch);
			if (range > 0.0f) speaker->SetRange(range);
		}
	}

	void GameSpeaker::PauseWithStage(const bool state)
	{
		pausewithstage = state;
	}

	const bool GameSpeaker::Load(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const LoadFlags flags)
	{
		if (properties.empty()) return false;
		if (properties["position"].is_array() && properties["position"].size() == 3)
		{
			Vec3 pos = Vec3(0);
			pos.x = properties["position"][0];
			pos.y = properties["position"][1];
			pos.z = properties["position"][2];
			SetPosition(pos);
		}

		if (speaker)
		{
			// Load the profile
			if (!properties["profile"].is_null())
			{
				String path = properties["profile"]["path"];
				if (profile == NULL && !path.empty())
				{
					profile = LoadSoundProfile(path);
				}
				
				if (properties["profile"]["volume"].is_float())
				{
					float v = properties["profile"]["volume"];
					if (v > 0.0f) speaker->SetVolume(v);
				}

				if (properties["profile"]["pitch"].is_float())
				{
					float p = properties["profile"]["pitch"];
					if (p > 0.0f)speaker->SetPitch(properties["profile"]["pitch"]);
				}
			}

			// Change the time
			if (properties["time"].is_float())
			{
				float t = properties["time"];
				speaker->SetTime(t);
			}

			// Resume if needed.
			if (properties["state"].is_number())
			{
				int i = properties["state"];
				SpeakerState state = (SpeakerState)i;
				int actualstate = speaker->GetState();
				if (state == SPEAKER_PLAYING && actualstate != SPEAKER_PLAYING)
				{
					speaker->Play();
				}
				else if (state == SPEAKER_STOPPED && actualstate != SPEAKER_STOPPED)
				{
					speaker->Stop();
				}
				else if (state == SPEAKER_PAUSED && actualstate != SPEAKER_PAUSED)
				{
					speaker->Pause();
				}
			}
		}

		return true;
	}

	const bool GameSpeaker::Save(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const SaveFlags flags)
	{
		properties["position"] = {};
		properties["position"][0] = position.x;
		properties["position"][1] = position.y;
		properties["position"][2] = position.z;

		properties["profile"] = {};
		properties["profile"]["path"] = String(profile->GetFilePath());
		properties["profile"]["volume"] = profile->GetVolume();
		properties["profile"]["pitch"] = profile->GetPitch();
		properties["profile"]["range"] = profile->GetRange();

		if (speaker)
		{
			properties["time"] = speaker->GetTime();
			properties["state"] = speaker->GetState();
		}

		return true;
	}

	bool GameSpeaker::ProcessEvent(const Event& e)
	{
		if (pausewithstage && speaker != NULL)
		{
			if (e.id == EVENT_PAUSESTATE)
			{
				const bool state = (bool)e.data;
				if (state)
					Pause();
				else
					Resume();
			}
		}

		return true;
	}

	shared_ptr<GameSpeaker> CreateGameSpeaker(const WString& path, const xVec3& position)
	{
		auto gamespeaker = std::make_shared<GameSpeaker>();

		if (!path.empty())
		{
			gamespeaker->profile = LoadSoundProfile(path);
			if (!gamespeaker->profile)
			{
				gamespeaker = NULL;
				return NULL;
			}
			gamespeaker->speaker = LoadSpeaker(gamespeaker->profile);
		}
		else
		{
			gamespeaker->speaker = CreateSpeaker(NULL);
		}

		gamespeaker->position = position;
		gamespeaker->speaker->SetPosition(gamespeaker->position);
		ListenEvent(EVENT_PAUSESTATE, GetProgram(), GameSpeakerEventCallback, gamespeaker);

		return gamespeaker;
	}
}