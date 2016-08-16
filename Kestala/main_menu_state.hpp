#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include <SFML\Graphics.hpp>

#include "game_state.hpp"
#include "menu_gui.hpp"

class MainMenu : public GameState {
private:
	sf::View menuView;
	MenuGui gui;

public:
	virtual void init();
	virtual void draw(const float dt);
	virtual void update(sf::Clock& clock);
	virtual void eventHandler();

	MainMenu(Game* game);
};

#endif // !MAIN_MENU_STATE_HPP