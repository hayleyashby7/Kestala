#ifndef PLAYER_HPP
#define PLAYER_HPP

//#include <random>

#include "entity.hpp"


class Player : public Entity {
public:
	int x, y;
	//player movement
	float moveSpeed;
	float lastMoveTime;
	sf::Vector2f movePosition(sf::Keyboard::Key& dirKey);
	void setPosition(sf::Vector2f position);
	void setX(int x);
	void setY(int y);

	bool beenHit;
	int health;
	int gems;
	int spells;
	std::string getHealth();
	std::string getGems();
	void takeDamage();
	bool isDead();
	

	Player(){}

	Player(sf::Vector2f& initialPos, sf::Texture& texture, sf::IntRect texturePosition) {
		this->isSolid = true;		
		this->type = entityType::PLAYER;
		this->health = 100;
		this->gems = 0;
		this->spells = 3;
		this->moveSpeed = 0.25;
		this->beenHit = false;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setPosition(initialPos);
		this->sprite.setTexture(texture);
		this->sprite.setTextureRect(texturePosition);
	}

};

#endif // !PLAYER_HPP
