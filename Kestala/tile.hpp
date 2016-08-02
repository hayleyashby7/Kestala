#ifndef TILE_HPP
#define TILE_HPP

#include <SFML\Graphics.hpp>
#include <vector>

#include "entity.hpp"


class Tile : public Entity {
public:
	
	void draw(sf::RenderWindow& window);
	int tileSize;


	Tile(){}

	Tile(const unsigned int tileSize, sf::Texture& texture, bool isSolid, entityType type){
		this->type = type;
		this->isSolid = isSolid;
		this->tileSize = tileSize;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setTexture(texture);
		
	}


};

#endif // !TILE_HPP