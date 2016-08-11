#include "game_state.hpp"
#include "game_level_state.hpp"
#include "main_menu_state.hpp"
#include "info_state.hpp"


void MainMenu::init() {
	this->game->window.clear(sf::Color::Black);
	this->game->background.setTexture(this->game->texmgr.getRef("background"));
	this->game->window.draw(this->game->background);
	this->game->bgMusic.openFromFile("assets/sounds/mainmenu.wav");
	this->game->bgMusic.setVolume(50);
	this->game->bgMusic.setLoop(true);
	this->game->bgMusic.play();	
}


void MainMenu::draw(const float dt) {
	this->game->window.setView(this->menuView);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	gui.draw(this->game->window);

	return;
}

void MainMenu::update(sf::Clock& clock) {

}

void MainMenu::eventHandler() {
	sf::Event event;

	while (this->game->window.pollEvent(event)) {
		switch (event.type){
		
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

			if (event.key.code == sf::Keyboard::W) {
				this->gui.MoveUp();
			}
			if (event.key.code == sf::Keyboard::S) {
				this->gui.MoveDown();
			}

			if (event.key.code == sf::Keyboard::Return) {
				switch (this->gui.getSelection())
				{
				case 1:
					this->game->changeState(new GameLevel(this->game));
					break;
				case 2:
					this->game->changeState(new Info(this->game, "info"));
					break;
				case 3:
					this->game->changeState(new Info(this->game, "credits"));
					break;
				default:
					break;
				}				
			}
		}

		default:
			break;
		}

	}
}

MainMenu::MainMenu(Game* game) {
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());	
	this->gui.init(pos.y, pos.x);
	this->menuView.setSize(pos);
	pos *= 0.5f;
	this->menuView.setCenter(pos);
	this->game->bgMusic.play();
}