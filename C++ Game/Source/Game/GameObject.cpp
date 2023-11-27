#include "UltraEngine.h"
#include "Game.h"

namespace UltraEngine::Game
{
	GameObject::GameObject()
	{
	}

	GameObject::~GameObject()
	{
	}

	shared_ptr<Stage> GameObject::GetStage()
	{
		return GetProgram()->stage;
	}

	shared_ptr<GameController> GameObject::GetInput()
	{
		return GetProgram()->gamecontroller;
	}

	void GameObject::SetKeyValue(const String& key, const String& value)
	{
		keyvalues[key] = value;
	}

	const String GameObject::GetKeyValue(const String& key, const String& defaultvalue)
	{
		return keyvalues[key];
	}

	void GameObject::EmitSound(const WString& path, const LoadFlags flags)
	{
		shared_ptr<SoundProfile> profile = LoadSoundProfileSafe(path, flags);
		EmitSoundFromEntity(GetEntity(), profile);
		profile = NULL;
	}

	void GameObject::EmitSound(shared_ptr<SoundProfile> profile)
	{
		EmitSoundFromEntity(GetEntity(), profile);
	}

	void GameObject::StopAllSounds()
	{
		if (!speaker.empty())
		{
			for (const auto& p : speaker)
			{
				p.second->Stop();
			}
		}
	}

	bool GameObject::Load(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const LoadFlags flags)
	{
		this->properties = properties;

		if (properties["keyvalues"].is_array())
		{
			for (const auto& p : properties["keyvalues"])
			{
				keyvalues[p.first] = p.second;
			}
		}

		if (!properties["speakercount"].is_null())
		{
			int max = properties["speakercount"];
			for (int i = 0; i < max; i++)
			{
				String entry = "speaker" + String(i);
				if (!properties[entry].is_null())
				{
					if (speaker[i])
					{
						speaker[i]->Load(properties[entry], binstream, scene, flags);
					}
					else
					{
						speaker[i] = CreateGameSpeaker("", GetEntity()->GetPosition());
						speaker[i]->Load(properties[entry], binstream, scene, flags);
					}
				}
			}
		}

		return true;
	}

	bool GameObject::Save(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const SaveFlags flags)
	{
		properties = this->properties;

		if (!keyvalues.empty())
		{
			properties["keyvalues"] = {};
			for (const auto& p : keyvalues)
			{
				properties["keyvalues"][p.first] = p.second;
			}
		}

		if (!speaker.empty())
		{
			properties["speakercount"] = (int)speaker.size();
			for (const auto& p : speaker)
			{
				String entry = "speaker" + String(p.first);
				properties[entry] = {};
				p.second->Save(properties[entry], binstream, scene, flags);
			}
		}

		return true;
	}
}