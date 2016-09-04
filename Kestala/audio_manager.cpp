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
	if (this->soundQueue.empty()) {
		this->soundQueue.push(bufferName);
	}

	else if (this->soundQueue.front().compare(bufferName) != 0) {
		this->soundQueue.push(bufferName);
	}
}

void AudioManager::clearQueue() {
	while (!this->soundQueue.empty()) {
		soundQueue.pop();
	}
}

void AudioManager::playSound(sf::Sound& sound1, sf::Sound& sound2) {
	if (sound1.getStatus() == sf::Sound::Status::Playing) {
		while (!soundQueue.empty() && sound2.getStatus() != sf::Sound::Status::Playing) {
			std::string bufferName = soundQueue.front();
			sound2.setBuffer(getRef(bufferName));
			sound2.play();
			soundQueue.pop();

		}
	}
	else {
		while (!soundQueue.empty() && sound1.getStatus() != sf::Sound::Status::Playing) {
			std::string bufferName = soundQueue.front();
			sound1.setBuffer(getRef(bufferName));
			sound1.play();
			soundQueue.pop();
		}	
	}	
}