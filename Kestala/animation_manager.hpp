#ifndef ANIMATION_MANAGER_HPP
#define ANIMATION_MANAGER_HPP
#include <map>
#include <string>
#include <SFML\Graphics.hpp>

class AnimationManager {

private:
	
	std::map<std::string, sf::IntRect> startingFrames;

public:
	//provides first frame to sprite
	sf::IntRect firstFrame(std::string name);

	//update sprites
	void update(sf::Sprite& sprite, sf::Vector2f origin);

	//called if player changes direction - update sprite
	void changeDirection(sf::Keyboard::Key& direction, sf::Sprite& sprite, sf::Vector2f& origin);

	//Constructor
	AnimationManager();
};

#endif // !ANIMATION_MANAGER_HPP

