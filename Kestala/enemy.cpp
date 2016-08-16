#include <SFML\Graphics.hpp>

#include "enemy.hpp"

sf::Vector2f Enemy::changeDirection() {
	sf::Vector2f newPos = this->getPosition();
	switch (this->enemyDirection) {
	case up:
		enemyDirection = down;
		newPos.y += tileSize;
		break;
	case left:
		enemyDirection = right;
		newPos.x += tileSize;
		break;
	case down:
		enemyDirection = up;
		newPos.y -= tileSize;
		break;
	case right:
		enemyDirection = left;
		newPos.x -= tileSize;
		break;
	}
	return newPos;
}

void Enemy::setX(int x) {
	this->x = x;
}
void Enemy::setY(int y) {
	this->y = y;
}

void Enemy::randomDirection() {
	std::random_device seed;
	std::mt19937_64 generator{ seed() };
	std::uniform_int_distribution<> dist{ 1, 3};
	int random = dist(generator);
	this->enemyDirection = direction(random);
}