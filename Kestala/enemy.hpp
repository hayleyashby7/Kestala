#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <random>

#include "entity.hpp"


class Enemy : public Entity {
public:
	//enemy movement
	
	enum direction {up = 1, down, left, right} enemyDirection;
	int x, y;

	sf::Vector2f changeDirection();
	void randomDirection();
	void setX(int x);
	void setY(int y);

	Enemy() {}

	Enemy(sf::Vector2f& initialPos, sf::Texture& texture, sf::IntRect texturePosition) {
		this->isSolid = true;
		this->active = true;
		this->type = entityType::ENEMY;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setPosition(initialPos);
		this->sprite.setTexture(texture);
		this->sprite.setTextureRect(texturePosition);
		randomDirection();
	}

};

#endif // !ENEMY_HPP