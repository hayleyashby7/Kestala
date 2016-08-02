#include "gui.hpp"
Gui::Gui() {
	font.loadFromFile("assets/asst/KF.ttf");



	this->guiText["code"] = gameCode;
	this->guiText["level"] = levelNumber;
	this->guiText["gem"] = numOfGems;
	this->guiText["player"] = playerStatus;
	this->guiText["spells"] = spells;

	guiText["code"].setString("Code: ");
	guiText["level"].setString("Level: ");
	guiText["gem"].setString("Gems Gathered: 0");
	guiText["player"].setString("Health: 100");
	guiText["spells"].setString("Spells: ");

	codeDisplay.setString(" ");

	codeDisplay.setPosition(sf::Vector2f(50, 150));
	codeDisplay.setFont(font);
	codeDisplay.setColor(sf::Color::White);
	codeDisplay.setCharacterSize(20);

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
	window.draw(codeDisplay);
	for (auto &text : this->guiText) {
		window.draw(text.second);
	}
}

void Gui::update(std::string key, std::string newValue) {
	guiText[key].setString(newValue);
	if (key.compare("code") == 0) {
		codeDisplay.setString("Your code for this game is " + newValue +
			"\n\nPlease make a note of this as you will need it \n\nfor the questionnaire after the game."
			" \n\nWhen you have made a note of the code,\n\n press Y to start the game");
	}
}