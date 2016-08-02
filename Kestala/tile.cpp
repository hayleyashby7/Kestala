#include <SFML\Graphics.hpp>

#include "tile.hpp"

void Tile::draw(sf::RenderWindow& window) {
	window.draw(this->sprite);
	return;
}

