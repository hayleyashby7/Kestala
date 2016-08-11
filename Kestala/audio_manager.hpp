#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <SFML\Audio.hpp>
#include <string>
#include <map>
#include <queue>

class AudioManager {
private:

	//Array of sounds used
	std::map<std::string, sf::SoundBuffer> soundbuffs;
	std::queue<std::string> soundQueue;

public:
	//SoundBuffer from file
	void loadSoundBuffer(const std::string& name, const std::string& filename);

	//id into a reference
	sf::SoundBuffer& getRef(const std::string& soundbuffer);

	//add sounds to queue
	void addBufferToQueue(std::string bufferName);

	//play sounds in queue
	void playSound(sf::Sound& sound);

	//Constructor
	AudioManager() {

	}
};

#endif // !AUDIO_MANAGER_HPP
