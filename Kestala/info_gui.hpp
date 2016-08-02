#ifndef INFO_GUI_HPP
#define INFO_GUI_HPP

#include <SFML\Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include "game.hpp"

class InfoGui {
public:
	void init(int height, int width, std::string type);
	void draw(sf::RenderWindow& window);
	InfoGui() {};

private:
	std::vector<sf::Text> infoText;
	sf::Font font;
};

#endif // !INFO_GUI_HPP