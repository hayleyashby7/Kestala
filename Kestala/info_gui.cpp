#include "info_gui.hpp"

void InfoGui::init(int height, int width, std::string filename) {
	std::string line;
	std::ifstream input(filename);
	if (!input.is_open())
		perror("Error opening file");
	while (std::getline(input, line)) {
		sf::Text nextLine;
		nextLine.setString(line);
		infoText.push_back(nextLine);
	}
	if (input.bad())
		perror("Error reading file");

	font.loadFromFile("assets/asst/KF.ttf");
	int i = 0;
	for (auto &text : this->infoText) {
		text.setPosition(sf::Vector2f(width / 25, ((height / (infoText.size() + 5) * i))));
		text.setFont(font);
		if (filename.compare("assets/files/win.txt") == 0) {
			text.setColor(sf::Color::Yellow);
		}
		if (filename.compare("assets/files/dead.txt") == 0) {
			text.setColor(sf::Color::Red);
		}
		else { text.setColor(sf::Color::White); }
		text.setCharacterSize(25);
		i++;
	}
};

void InfoGui::draw(sf::RenderWindow& window) {
	for (auto &text : this->infoText) {
		window.draw(text);
	}
}