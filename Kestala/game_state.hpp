#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "game.hpp"

#include <string>


class GameState {
public:
	Game* game;
	int type = 0;

	virtual void init() = 0;
	virtual void cleanUp() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void draw(const float dt) = 0;
	virtual void update(sf::Clock& clock) = 0;
	virtual void eventHandler() = 0;

	void changeState(Game* game, GameState* state) {
		game->changeState(state);
	}
	
	

	GameState(){}
};
#endif // !GAME_STATE_HPP