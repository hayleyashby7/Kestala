#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>


class Entity {

public:
	sf::FloatRect entRect;
	sf::Sprite sprite;
	bool active = true;

	int tileSize = 32;
	

	enum class entityType {FLOOR = 0, WALL, ENEMY, PLAYER, PICKUP, EXIT, START } type;

	bool isSolid;

	virtual void draw(sf::RenderWindow& window) {
		window.draw(this->sprite);
	}
	virtual void updatePos(sf::Vector2f& position) {
		this->sprite.setPosition(position);
		return;
	}

	virtual sf::Vector2f getPosition() {
		sf::Vector2f position = this->sprite.getPosition();
		return position;
	}
	virtual void takeDamage() {};

protected:
	Entity() {};

};

#endif // !ENTITY_HPP

