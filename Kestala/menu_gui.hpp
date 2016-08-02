#ifndef MENU_GUI_HPP
#define MENU_GUI_HPP

#include <SFML\Graphics.hpp>
#include <string>
#include "game.hpp"


class MenuGui {
public:

	void init(int height, int width);
	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int getSelection();

	MenuGui() {};

private:
	int selectedOption;
	sf::Text title;
	sf::Text start;
	sf::Text info;
	sf::Text options;
	sf::Text credits;

	sf::Font font;
	sf::Font titleFont;
	std::vector<sf::Text> menuText;




};

#endif // !MENU_GUI_HPP