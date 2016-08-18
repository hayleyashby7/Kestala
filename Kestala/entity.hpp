#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>


class Entity {

public:
	sf::IntRect rectSource;
	sf::Sprite sprite;
	bool active = true;
	int tileSize = 32;
	char id;
	sf::Vector2f spriteOrigin;

	enum class entityType {FLOOR = 0, WALL, ENEMY, PLAYER, PICKUP, EXIT, START, DOOR, INTERACTIVE} type;
	bool isSolid;

	virtual void draw(sf::RenderWindow& window) {
		window.draw(this->sprite);
	}
	virtual void updatePos(sf::Vector2f& position) {
		this->sprite.setPosition(position);
		return;
	}
	virtual sf::Vector2f getPosition() {
		return this->sprite.getPosition();
	}
	virtual void takeDamage() {};

protected:
	Entity() {};

};

#endif // !ENTITY_HPP

