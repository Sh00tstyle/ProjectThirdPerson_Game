#ifndef AUDIOCONTAINER_H
#define AUDIOCONTAINER_H

#include <map>

class lua_State;
class AudioSource;

class AudioContainer {
	public:
		AudioContainer();
		~AudioContainer();

		static void PlaySound(std::string eventName);
		static void StopSound(std::string eventName);

	private:
		static int _createEvent(lua_State* state);
		static int _addSound(lua_State* state);

		static std::map<std::string, AudioSource*> _audioEvents;

		void _initAudio();
};

#endif