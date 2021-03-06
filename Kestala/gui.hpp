#ifndef GUI_HPP
#define GUI_HPP

#include <SFML\Graphics.hpp>
#include <string>
#include <map>

#include "game.hpp"


class Gui {
public:
	sf::Text intro;
	sf::Text playerStatus;
	sf::Text numOfGems;
	sf::Text gold;
	sf::Text spells;
	sf::Text keys;
	sf::Text clue;
	sf::Font font;


	std::map<std::string, sf::Text> guiText;
	
	sf::Vector2f guiStart;

	void draw(sf::RenderWindow& window);
	void update(std::string key, std::string newValue);

	Gui();


};

#endif // !GUI_HPP