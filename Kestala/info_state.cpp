#include "info_state.hpp"


void Info::init() {
	this->game->window.clear(sf::Color::Black);
	this->game->background.setTexture(this->game->texmgr.getRef("background"));
	this->game->window.draw(this->game->background);
}

void Info::cleanUp() {}
void Info::pause() {}
void Info::resume() {}

void Info::draw(const float dt) {
	this->game->window.setView(this->infoView);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	infoGui.draw(this->game->window);
	return;
}

void Info::update(sf::Clock& clock) {}
void Info::eventHandler() {
	sf::Event event;

	while (this->game->window.pollEvent(event)) {
		switch (event.type) {
		/*Window closed*/
		case sf::Event::Closed: {
			game->window.close();
			break;
		}

		/*Key Pressed*/
		case sf::Event::KeyPressed: {
			if (event.key.code == sf::Keyboard::Q) {
				game->window.close();
				break;
			}
			if (event.key.code == sf::Keyboard::B) {
				if (type.compare("win") == 0 || type.compare("dead") == 0) {
					this->game->newGame(new MainMenu(this->game));
					break;
				}
				else if (type.compare("info") == 0 || type.compare("credits") == 0) {
					this->game->goBackState();
					break;
				}
				
			}
		}
		}
	}
};

Info::Info(Game* game, std::string type) {
	this->game = game;
	this->type = type;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());		
	this->infoView.setSize(pos);
	int width = pos.x;
	int height = pos.y;
	pos *= 0.5f;
	this->infoView.setCenter(pos);

	
	if (type.compare("info")==0) {
		filename = "assets/files/info.txt";
	}
	else if (type.compare("credits")==0) {
		filename = "assets/files/credits.txt";
	}
	else if (type.compare("win") == 0) {
		filename = "assets/files/win.txt";
	}
	else if (type.compare("dead") == 0) {
		filename = "assets/files/dead.txt";
	}
	this->infoGui.init(width, height, filename);
};