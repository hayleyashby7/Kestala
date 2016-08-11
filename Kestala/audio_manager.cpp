#include <SFML\Audio.hpp>
#include <string>
#include <map>

#include "audio_manager.hpp"

void AudioManager::loadSoundBuffer(const std::string& name, const std::string& filename) {
	//Load SoundBuffer
	sf::SoundBuffer buf;
	buf.loadFromFile(filename);

	//Add to list of SoundBuffers
	this->soundbuffs[name] = buf;

	return;
}

sf::SoundBuffer& AudioManager::getRef(const std::string& soundbuffer) {
	return this->soundbuffs.at(soundbuffer);
}

void AudioManager::addBufferToQueue(std::string bufferName) {
	this->soundQueue.push(bufferName);
}

void AudioManager::playSound(sf::Sound& sound) {
	while(!soundQueue.empty() || sound.Playing != 2) {
		std::string bufferName = soundQueue.front();
		sound.setBuffer(getRef(bufferName));
		sound.play();
		soundQueue.pop();
	}	
}