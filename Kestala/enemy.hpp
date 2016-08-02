#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <random>

#include "entity.hpp"


class Enemy : public Entity {
public:
	//enemy movement
	
	enum direction {up = 1, down, left, right} enemyDirection;
	sf::Vector2f movePosition(direction dir);
	sf::Vector2f changeDirection();
	void randomDirection();

	Enemy() {}

	Enemy(sf::Vector2f& initialPos, sf::Texture& texture) {
		this->isSolid = true;
		this->type = entityType::ENEMY;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setPosition(initialPos);
		this->sprite.setTexture(texture);
		randomDirection();
	}

};

#endif // !ENEMY_HPP