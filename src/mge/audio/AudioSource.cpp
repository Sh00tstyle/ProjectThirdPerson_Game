#include <iostream>

#include "AudioSource.h"
#include "mge/config.hpp"

AudioSource::AudioSource(float pMinPitch, float pMaxPitch, float pVolume, bool pIsLooping) {
	_minPitch = pMinPitch;
	_maxPitch = pMaxPitch;
	_volume = pVolume;
	_isLooping = pIsLooping;
}

AudioSource::~AudioSource() {
	for(unsigned i = 0; i < _buffers.size(); i++) {
		delete _buffers[i];
	}

	for(unsigned i = 0; i < _sounds.size(); i++) {
		delete _sounds[i];
	}
}

void AudioSource::AddSound(std::string filename) {
	sf::SoundBuffer* newBuffer = new sf::SoundBuffer();
	if(newBuffer->loadFromFile(config::MGE_AUDIO_PATH + filename)) {
		std::cout << "Loading audio file " + filename << std::endl;

		sf::Sound* newSound = new sf::Sound();
		newSound->setBuffer(*newBuffer);
		newSound->setVolume(_volume);
		newSound->setLoop(_isLooping);

		_buffers.push_back(newBuffer);
		_sounds.push_back(newSound);
	}
}

void AudioSource::Play() {
	if(_sounds.size() == 0) return;

	sf::Sound* mySound = _sounds[rand() % _sounds.size()];

	//dont play if its playing
	if(mySound->getStatus() == sf::SoundSource::Status::Playing && _isLooping) return;
	
	mySound->setPitch(_minPitch + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_maxPitch - _minPitch))));
	mySound->play();
}

void AudioSource::Stop() {
	for(unsigned i = 0; i < _sounds.size(); i++) {
		_sounds[i]->stop();
	}
}
