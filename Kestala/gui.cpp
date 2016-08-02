#include "gui.hpp"
Gui::Gui() {
	font.loadFromFile("assets/asst/KF.ttf");
	
	this->guiText["level"] = levelNumber;
	this->guiText["gem"] = numOfGems;
	this->guiText["player"] = playerStatus;
	this->guiText["spells"] = spells;

	guiText["level"].setString("Level: ");
	guiText["gem"].setString("Gems Gathered: 0");
	guiText["player"].setString("Health: 100");
	guiText["spells"].setString("Spells: ");

	guiStart.x = 557;
	guiStart.y = 25;

	int i = 0;
	for (auto &text : this->guiText) {
		float x = guiStart.x;
		float y = guiStart.y + (i * 100);
		text.second.setPosition(x, y);
		text.second.setFont(font);
		text.second.setColor(sf::Color::White);
		text.second.setCharacterSize(20);
		i++;
	}
};
void Gui::draw(sf::RenderWindow& window) {
	for (auto &text : this->guiText) {
		window.draw(text.second);
	}
}

void Gui::update(std::string key, std::string newValue) {
	guiText[key].setString(newValue);
}