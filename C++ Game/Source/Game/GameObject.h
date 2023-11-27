#pragma once
#include "UltraEngine.h"

namespace UltraEngine::Game
{
	class Stage;
	class GameController;
	class GameObject : public Component
	{
		table properties;
		std::map<String, String> keyvalues;
		std::map<int, shared_ptr<GameSpeaker>> speaker;

		shared_ptr<Stage> GetStage();
		shared_ptr<GameController> GetInput();
	public:
		GameObject();
		virtual ~GameObject();

		// Virtual Data
		void SetKeyValue(const String& key, const String& value);
		const String GetKeyValue(const String& key, const String& defaultvalue = "");

		// Sound Managment
		virtual void EmitSound(const WString& path, const LoadFlags flags);
		virtual void EmitSound(shared_ptr<SoundProfile> profile);
		virtual void StopAllSounds();

		// Save/Load
		virtual bool Load(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const LoadFlags flags);
		virtual bool Save(table& properties, shared_ptr<Stream> binstream, shared_ptr<Map> scene, const SaveFlags flags);

		friend Stage;
	};
}