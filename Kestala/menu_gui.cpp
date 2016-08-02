#include "menu_gui.hpp"

void MenuGui::init(int height, int width) {
	titleFont.loadFromFile("assets/asst/kestala.ttf");
	font.loadFromFile("assets/asst/KF.ttf");
	title.setString("Kestala");
	start.setString("Start");
	info.setString("How to Play");
	credits.setString("Credits");

	title.setColor(sf::Color::Color(102, 255, 255, 255));
	title.setCharacterSize(50);
	title.setStyle(sf::Text::Italic);

	this->menuText.push_back(title);
	this->menuText.push_back(start);
	this->menuText.push_back(info);
	this->menuText.push_back(credits);
	int i = 1;
	for (auto &text : this->menuText) {
		
		if (i == 1) {
			text.setPosition(sf::Vector2f(width / 4, height / 100));
			text.setFont(titleFont);
			text.setColor(sf::Color::Color(102, 255, 255, 255));
			text.setCharacterSize(150);
			text.setStyle(sf::Text::Italic);
			
		}
		else {
			text.setPosition(sf::Vector2f(width / 3, ((height / (menuText.size() + 1) * i))));
			text.setFont(font);
			text.setColor(sf::Color::White);
			text.setCharacterSize(30);
		}
		i++;
		
	}
	this->selectedOption = 1;
	menuText[selectedOption].setColor(sf::Color::Red);
	menuText[selectedOption].setCharacterSize(40);
};

void MenuGui::draw(sf::RenderWindow& window) {
	for (auto &text : this->menuText) {
			window.draw(text);
	}	
}

void MenuGui::MoveUp() {
	if (selectedOption - 1 > 0) {
		menuText[selectedOption].setColor(sf::Color::White);
		menuText[selectedOption].setCharacterSize(30);
		selectedOption--;
		menuText[selectedOption].setColor(sf::Color::Red);
		menuText[selectedOption].setCharacterSize(40);
	}
}
void MenuGui::MoveDown() {
	if (selectedOption + 1 < menuText.size()) {
		menuText[selectedOption].setColor(sf::Color::White);
		menuText[selectedOption].setCharacterSize(30);
		selectedOption++;
		menuText[selectedOption].setColor(sf::Color::Red);
		menuText[selectedOption].setCharacterSize(40);
	}
}
int MenuGui::getSelection() {
	return this->selectedOption;
}