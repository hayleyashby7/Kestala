#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>


#include "texture_manager.hpp"
#include "audio_manager.hpp"

#include "tile.hpp"
#include "entity.hpp"

class GameState;

class Game {
private:
	void loadTextures();
	void loadTiles();
	void loadAudio();

	std::vector<GameState*> states;

public:	

	const static int halfWidth = 16;
	const static int tileHeight = 32;
	const float gameSpeed = 0.25;

	sf::RenderWindow window;
	TextureManager texmgr;
	sf::Sprite background;

	AudioManager audmgr;
	sf::Music bgMusic;
	sf::Sound SFX;

	std::map<std::string, Tile> tileAtlas;


	void pushBackState(GameState* state);
	void popBackState();
	void changeState(GameState* state);
	void goBackState();

	void draw(const float dt);
	void update(sf::Clock& clock);
	void eventHandler();

	void cleanUp();
	void newGame(GameState* state);

	GameState* checkState();

	void gameLoop();

	Game();
	~Game();
};

#endif // !GAME_HPP
