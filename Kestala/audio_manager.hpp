#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <SFML\Audio.hpp>
#include <string>
#include <map>

class AudioManager {
private:

	//Array of sounds used
	std::map<std::string, sf::SoundBuffer> soundbuffs;

public:
	//SoundBuffer from file
	void loadSoundBuffer(const std::string& name, const std::string& filename);

	//id into a reference
	sf::SoundBuffer& getRef(const std::string& soundbuffer);

	void playSound(std::string bufferName, sf::Sound& sound);

	//Constructor
	AudioManager() {

	}
};

#endif // !AUDIO_MANAGER_HPP
