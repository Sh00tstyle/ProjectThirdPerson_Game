#include "AudioContainer.h"

#include <string>
#include <iostream>

#include "mge/config.hpp"
#include "mge/audio/AudioSource.h"

#include <lua.hpp>

std::map<std::string, AudioSource*> AudioContainer::_audioEvents = std::map<std::string, AudioSource*>();
AudioContainer::AudioContainer() {
	_initAudio();
}

AudioContainer::~AudioContainer() {
	for(auto sysListener : _audioEvents) {
		delete sysListener.second;
	}
}

void AudioContainer::PlaySound(std::string eventName) {
	AudioSource* mySource = _audioEvents[eventName];

	if(mySource != nullptr) mySource->Play();
}

void AudioContainer::StopSound(std::string eventName) {
	AudioSource* mySource = _audioEvents[eventName];

	if(mySource != nullptr) mySource->Stop();
}

int AudioContainer::_createEvent(lua_State * state) {
	std::string eventName = lua_tostring(state, 1);
	float minPitch = (float)lua_tonumber(state, 2);
	float maxPitch = (float)lua_tonumber(state, 3);
	float volume = (float)lua_tonumber(state, 4);
	bool looping = lua_toboolean(state, 5);

	lua_settop(state, 0); //clear the stack

	AudioSource* newSource = new AudioSource(minPitch, maxPitch, volume, looping);
	_audioEvents[eventName] = newSource;

	return 0;
}

int AudioContainer::_addSound(lua_State * state) {
	std::string eventName = lua_tostring(state, 1);
	std::string filename = lua_tostring(state, 2);

	lua_settop(state, 0); //clear the stack

	AudioSource* mySource = _audioEvents[eventName];
	mySource->AddSound(filename);

	return 0;
}

void AudioContainer::_initAudio() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state); // get all libs in state (math, os, io)

	//register functions as global lua functions
	lua_register(state, "CreateEvent", _createEvent);
	lua_register(state, "AddSound", _addSound);

	std::string filename = config::MGE_LUA_PATH + "audio.lua";
	luaL_dofile(state, filename.c_str()); //execute lua file

    //calling Audio.init() in lua
	lua_getglobal(state, "Audio"); // Audio table on stack
	if(lua_istable(state, -1)) {
		lua_getfield(state, -1, "init"); //Audio.init on stack
		if(lua_isfunction(state, -1)) {
			if(lua_pcall(state, 0, 0, 0) != 0) { // call Audio.init() from lua, pcall(state, argument amt, result amt, ?)
				printf("Error : %s", lua_tostring(state, -1));
				getchar();
				exit(-3);
			}
		}
	}
}
