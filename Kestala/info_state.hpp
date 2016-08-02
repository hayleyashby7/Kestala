#ifndef  INFO_STATE_HPP
#define INFO_STATE_HPP

#include <SFML\Graphics.hpp>
#include <string>
#include "game_state.hpp"
#include "main_menu_state.hpp"
#include "game.hpp"
#include "info_gui.hpp"

class Info : public GameState {
private:
	sf::View infoView;
	std::string filename;
	InfoGui infoGui;
	std::string type;

public:	
	virtual void init();
	virtual void cleanUp();

	virtual void pause();
	virtual void resume();

	virtual void draw(const float dt);
	virtual void update(sf::Clock& clock);
	virtual void eventHandler();
	Info(Game* game, std::string type);

};

#endif // ! INFO_STATE_HPP
