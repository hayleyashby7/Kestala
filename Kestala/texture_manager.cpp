#include <SFML\Graphics.hpp>
#include <string>
#include <map>

#include "texture_manager.hpp"

void TextureManager::loadTexture(const std::string& name, const std::string& filename) {
	//Load Texture
	sf::Texture tex;
	tex.loadFromFile(filename);

	//Add to list of Textures
	this->textures[name] = tex;

	return;
}

sf::Texture& TextureManager::getRef(const std::string& texture) {
	return this->textures.at(texture);
}