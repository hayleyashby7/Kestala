#ifndef GAME_LEVEL_STATE_HPP
#define GAME_LEVEL_STATE_HPP

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <map>
#include <random>

#include "game_state.hpp"
#include "audio_manager.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "player.hpp"


class GameLevel : public GameState {
private:
	
	Map map;
	Gui gui;
	sf::Clock startClock;
	
	std::map<std::string, std::string> mapFiles;
	std::map<std::string, Map> mapList;
	std::vector<Map>prevLevels;

	bool start;
	int currentLevel;
	int maxLevel = 4;
	int altarsUnlocked;
	std::vector <bool> itemCollected;
	sf::Clock keyPress;
	sf::Keyboard::Key lastPressed;


public:
	Player player;
	bool gameOver;
	bool gameWon;

	void playerMove(sf::Keyboard::Key& dirKey);
	
	virtual void init();
	virtual void draw(const float dt);
	virtual void update(sf::Clock& clock);
	virtual void eventHandler();

	GameLevel(Game* game);

	
};

#endif // !GAME_LEVEL_STATE_HPP
