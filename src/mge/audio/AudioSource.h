#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <string>
#include <SFML\Audio.hpp>

class AudioSource {
	public:
		AudioSource(float pMinPitch, float pMaxPitch, float pVolume, bool pIsLooping);
		~AudioSource();

		void AddSound(std::string filename);

		void Play();
		void Stop();

	private:

		float _minPitch;
		float _maxPitch;
		float _volume;
		bool _isLooping;

		std::vector<sf::SoundBuffer*> _buffers;
		std::vector<sf::Sound*> _sounds;
};

#endif