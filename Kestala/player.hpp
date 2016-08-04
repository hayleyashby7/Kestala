#ifndef PLAYER_HPP
#define PLAYER_HPP

//#include <random>

#include "entity.hpp"


class Player : public Entity {

public:
	//player movement
	int moveSpeed;
	sf::Vector2f movePosition(sf::Keyboard::Key& dirKey);
	void setPosition(sf::Vector2f position);

	bool beenHit;
	int health;
	int gems;
	int spells;
	std::string getHealth();
	std::string getGems();
	void takeDamage();
	bool isDead();
	

	Player(){}

	Player(sf::Vector2f& initialPos, sf::Texture& texture) {
		this->isSolid = true;		
		this->type = entityType::PLAYER;
		this->health = 100;
		this->gems = 0;
		this->spells = 3;
		this->beenHit = false;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setPosition(initialPos);
		this->sprite.setTexture(texture);
	}

};

#endif // !PLAYER_HPP
