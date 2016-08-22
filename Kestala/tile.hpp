#ifndef TILE_HPP
#define TILE_HPP

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>

#include "entity.hpp"


class Tile : public Entity {
public:
	
	void draw(sf::RenderWindow& window);
	int tileSize;
	sf::Vector2f spriteOrigin;

	Tile(){}

	Tile(const unsigned int tileSize, sf::Texture& texture, sf::IntRect texturePosition, bool isSolid, entityType type, char id){
		this->type = type;
		this->id = id;
		this->isSolid = isSolid;
		this->tileSize = tileSize;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setTexture(texture);
		this->sprite.setTextureRect(texturePosition);
		this->spriteOrigin.x = texturePosition.top;
		this->spriteOrigin.y = texturePosition.left;
	}
	Tile(const unsigned int tileSize, sf::Texture& texture, sf::IntRect texturePosition, sf::Color colour, bool isSolid, entityType type, char id) {
		this->type = type;
		this->id = id;
		this->isSolid = isSolid;
		this->tileSize = tileSize;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setTexture(texture);
		this->sprite.setTextureRect(texturePosition);
		this->sprite.setColor(colour);
		this->spriteOrigin.x = texturePosition.top;
		this->spriteOrigin.y = texturePosition.left;
	}


};

#endif // !TILE_HPP