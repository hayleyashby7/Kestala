#include "animation_manager.hpp"

//provides first frame to sprite
sf::IntRect AnimationManager::firstFrame(std::string name) {	
	return this->startingFrames[name];
}

//update sprites
void AnimationManager::update(sf::Sprite& sprite, sf::Vector2f spriteOrigin) {
		if (sprite.getTextureRect().left == (spriteOrigin.x + 32)) {
			sf::IntRect newRect = sprite.getTextureRect();
			newRect.left = spriteOrigin.x;
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
void AnimationManager::changeDirection(sf::Keyboard::Key& direction, sf::Sprite& sprite, sf::Vector2f& spriteOrigin) {
	sf::IntRect rectSource = sprite.getTextureRect();
	switch (direction)
	{
	case sf::Keyboard::W:
		rectSource.top = 0;
		rectSource.left = 0;
		break;
	case sf::Keyboard::S:
		rectSource.top = 0;
		rectSource.left = 64;
		break;
	case sf::Keyboard::A:
		rectSource.top = 32;
		rectSource.left = 0;
		break;
	case sf::Keyboard::D:
		rectSource.top = 32;
		rectSource.left = 64;
		break;
	default:
		break;
	} 
	sprite.setTextureRect(rectSource);
	spriteOrigin.x = rectSource.left;
	spriteOrigin.y = rectSource.top;
}

AnimationManager::AnimationManager() {
	this->startingFrames["player"] = sf::IntRect(64, 0, 32, 32);
	this->startingFrames["enemy"] = sf::IntRect(0, 64, 32, 32);
	this->startingFrames["specialenemy"] = sf::IntRect(0, 256, 32, 32);
	this->startingFrames["blockeddoor"] = sf::IntRect(64, 64, 32, 32);
	this->startingFrames["opendoor"] = sf::IntRect(96, 64, 32, 32);
	this->startingFrames["wall"] = sf::IntRect(0, 96, 32, 32);
	this->startingFrames["floor"] = sf::IntRect(32, 96, 32, 32);
	this->startingFrames["firegem"] = sf::IntRect(64, 96, 32, 32);
	this->startingFrames["earthgem"] = sf::IntRect(96, 96, 32, 32);
	this->startingFrames["airgem"] = sf::IntRect(0, 128, 32, 32);
	this->startingFrames["watergem"] = sf::IntRect(32, 128, 32, 32);
	this->startingFrames["firealtar"] = sf::IntRect(64, 128, 32, 32);
	this->startingFrames["earthaltar"] = sf::IntRect(0, 160, 32, 32);
	this->startingFrames["airaltar"] = sf::IntRect(64, 160, 32, 32);
	this->startingFrames["wateraltar"] = sf::IntRect(0, 192, 32, 32);
	this->startingFrames["hiddentreasure"] = sf::IntRect(64, 192, 32, 32);
	this->startingFrames["treasure"] = sf::IntRect(96, 192, 32, 32);
	this->startingFrames["clue"] = sf::IntRect(0, 224, 32, 32);
	this->startingFrames["minortreasure"] = sf::IntRect(32, 224, 32, 32);
	this->startingFrames["key"] = sf::IntRect(64, 224, 32, 32);

}