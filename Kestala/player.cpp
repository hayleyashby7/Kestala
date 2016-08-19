#include <SFML\Graphics.hpp>

#include "player.hpp"


sf::Vector2f Player::movePosition(sf::Keyboard::Key& dirKey) {
	sf::Vector2f newPos = this->getPosition();

	switch (dirKey) {
	case sf::Keyboard::W :
		newPos.y -= tileSize;
		break;
	case sf::Keyboard::A:
		newPos.x -= tileSize;
		break;
	case sf::Keyboard::S:
		newPos.y += tileSize;
		break;
	case sf::Keyboard::D:
		newPos.x += tileSize;
		break;
	}	

	return newPos;
}

void Player::setPosition(sf::Vector2f position) {
	this->sprite.setPosition(position);
	return;
}


void Player::takeDamage() {
	this->health -= 10;	
	beenHit = true;
}

std::string Player::getHealth() {
	std::string healthNum = std::to_string(this->health);
	return healthNum;
}

std::string Player::getGems() {
	std::string gemNum = std::to_string(this->gems);
	return gemNum;
}

std::string Player::getSpells() {
	std::string spellNum = std::to_string(this->spells);
	return spellNum;
}

std::string Player::getGold() {
	std::string goldNum = std::to_string(this->gold);
	return goldNum;
}

std::string Player::getKeys() {
	std::string keyNum = std::to_string(this->key);
	return keyNum;
}

bool Player::isDead() {
	if (this->health <= 0) {
		return true;
	}
	return false;
}