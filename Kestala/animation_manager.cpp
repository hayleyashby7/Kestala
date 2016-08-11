#include "animation_manager.hpp"

//provides first frame to sprite
sf::IntRect AnimationManager::firstFrame(std::string name) {	
	return this->startingFrames[name];
}

//update sprites
void AnimationManager::update(sf::Sprite& sprite) {
		if (sprite.getTextureRect().left == 32) {
			sf::IntRect newRect = sprite.getTextureRect();
			newRect.left = 0;
			sprite.setTextureRect(newRect);
		}
		else {
			sf::IntRect newRect = sprite.getTextureRect();
			newRect.left += 32;
			sprite.setTextureRect(newRect);
		}		
	return;
}

//called if player changes direction - update sprite
void AnimationManager::changeDirection(sf::Keyboard::Key& direction, sf::Sprite& sprite) {
	sf::IntRect rectSource = sprite.getTextureRect();
	switch (direction)
	{
	case sf::Keyboard::W:
		rectSource.top = 0;
		break;
	case sf::Keyboard::S:
		rectSource.top = 32;
		break;
	case sf::Keyboard::A:
		rectSource.top = 64;
		break;
	case sf::Keyboard::D:
		rectSource.top = 96;
		break;
	default:
		break;
	} 
	sprite.setTextureRect(rectSource);	
}

AnimationManager::AnimationManager() {
	this->startingFrames["player"] = sf::IntRect(0, 32, 32, 32);
	this->startingFrames["enemy"] = sf::IntRect(0, 128, 32, 32);
	this->startingFrames["blockedExit"] = sf::IntRect(0, 160, 32, 32);
	this->startingFrames["exit"] = sf::IntRect(32, 160, 32, 32);
	this->startingFrames["wall"] = sf::IntRect(0, 192, 32, 32);
	this->startingFrames["floor"] = sf::IntRect(32, 192, 32, 32);
	this->startingFrames["start"] = sf::IntRect(0, 224, 32, 32);
	this->startingFrames["gem"] = sf::IntRect(32, 224, 32, 32);
}