#ifndef PLAYER_HPP
#define PLAYER_HPP

//#include <random>

#include "entity.hpp"


class Player : public Entity {
public:
	//spell sprite
	sf::Vector2f spellOrigin;

	//player movement
	float moveSpeed = 0.125;
	sf::Vector2f movePosition(sf::Keyboard::Key& dirKey);
	void setPosition(sf::Vector2f position);

	bool beenHit;
	int health;
	int gems;
	int spells;
	int gold;
	int key;
	bool grimoire;
	std::string getHealth();
	std::string getGems();
	std::string getSpells();
	std::string getGold();
	std::string getKeys();
	void takeDamage();
	bool isDead();
	void setSpell(sf::Texture& texture, sf::IntRect texturePosition);
	std::map <std::string, bool> itemCollected;
	

	Player(){}

	Player(sf::Vector2f& initialPos, sf::Texture& texture, sf::IntRect texturePosition) {
		this->isSolid = true;		
		this->type = entityType::PLAYER;
		this->health = 100;
		this->gems = 0;
		this->gold = 0;
		this->spells = 3;
		this->key = 0;
		this->grimoire = false;
		this->beenHit = false;
		this->sprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
		this->sprite.setPosition(initialPos);
		this->sprite.setTexture(texture);
		this->sprite.setTextureRect(texturePosition);
		this->spriteOrigin.x = texturePosition.left;
		this->spriteOrigin.y = texturePosition.top;
		itemCollected["fireGem"] = false;
		itemCollected["waterGem"] = false;
		itemCollected["airGem"] = false;
		itemCollected["earthGem"] = false;


	}

};

#endif // !PLAYER_HPP
