#include "gui.hpp"
Gui::Gui() {
	font.loadFromFile("assets/asst/KF.ttf");

	this->guiText["t_clue"] = clue;
	this->guiText["gold"] = gold;
	this->guiText["gem"] = numOfGems;
	this->guiText["player"] = playerStatus;
	this->guiText["spells"] = spells;


	guiText["gold"].setString("Gold Collected: 0");
	guiText["gem"].setString("Gems Gathered: 0");
	guiText["player"].setString("Health: 100");
	guiText["spells"].setString("Spells: ");
	guiText["t_clue"].setString("");

	intro.setString(" ");
	intro.setPosition(sf::Vector2f(20, 50));
	intro.setFont(font);
	intro.setColor(sf::Color::White);
	intro.setCharacterSize(20);

	guiStart.x = 500;
	guiStart.y = 10;

	int i = 0;
	for (auto &text : this->guiText) {
		float x = guiStart.x;
		float y = guiStart.y + (i * 75);
		text.second.setPosition(x, y);
		text.second.setFont(font);
		text.second.setColor(sf::Color::White);
		text.second.setCharacterSize(20);
		i++;
	}
	guiText["t_clue"].setColor(sf::Color::Color(102, 255, 255, 255));
};
void Gui::draw(sf::RenderWindow& window) {
	window.draw(intro);
	for (auto &text : this->guiText) {
		window.draw(text.second);
	}
}

void Gui::update(std::string key, std::string newValue) {
	if (key.compare("intro") == 0) {
		intro.setString("You are Wryn, a brave sorcerer-in-training.\nDesperate to prove yourself a master of the arcane,\nyou seek 'Kestala's Grimoire',\n a long-lost spellbook of great power.\n\n\nDetermined, you bravely explore the haunted caves\nwhere the Grimoire is rumoured to be hidden.\nready to face whatever challenges await you...\n...and hopefully find some extra treasure.\n\nPress Y to begin your adventure.");
	}
	else {
		guiText[key].setString(newValue);
	}
}