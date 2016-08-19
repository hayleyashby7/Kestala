#ifndef MAP_HPP
#define MAP_HPP

#include <SFML\Graphics.hpp>
#include <boost\heap\fibonacci_heap.hpp>
#include <boost\graph\astar_search.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

#include "tile.hpp"
#include "game.hpp"
#include "cell.hpp"
#include "player.hpp"
#include "enemy.hpp"


class Map {

public:
	int height;
	int width;
	int tileSize;
	int gems;
	int totalGems;
	int id;

	
	bool firstLevel = false;
	bool nextLevel;
	bool prevLevel;
	bool prevVisited = false;
	bool unlocked = false;
	bool clueFound = false;


	sf::Vector2f exitPos;
	sf::Vector2f startPos;
	
	std::vector<Cell> mapCells;
	std::vector<Enemy> enemies;
	std::map<std::string, Tile> tileAtlas;

	//load map and find cells
	void loadMap(const std::string& filename, int id, unsigned int width, unsigned int height,
		std::map<std::string, Tile>& enemyAtlas, Game* game, Player& player);
	Cell* getCell(sf::Vector2f position);

	//enemy movement
	void distanceCalculation(Player& player);
	void resetCellScores();
	sf::Vector2f enemyPathFinder(Enemy& enemy, Player& player);
	void enemyMove(Player& player, Game* game);

	//entity interactions
	bool playerCollision(sf::Vector2f position, Player& player);
	bool enemyCollision(sf::Vector2f position,Enemy& enemy, Player& player);
	void spell(Player& player);
	bool interact(Player& player);
	std::vector<sf::Vector2f> nearPlayer(Player& player);
	void unlockTreasure();
	std::string clueText();
	
	//move between maps
	void leaveMap(bool& change);
	void returnMap(Player& player);
	void restartMap(Player& player);


	void draw(sf::RenderWindow& window);

	Map() {		
		this->width = 0;
		this->height = 0;
		this->tileSize = 0;
		this->gems = 0;
		this->nextLevel = false;
		this->prevLevel = false; 
	}

	Map(const std::string& filename, int id, unsigned int width, unsigned int height, unsigned int tileSize,
		std::map<std::string, Tile>& tileAtlas, Game* game, Player& player) {
		this->width = width;
		this->height = height;
		this->tileSize = tileSize;
		this->gems = 0;
		this->nextLevel = false;
		this->prevLevel = false;
		this->tileAtlas = tileAtlas;
		loadMap(filename, id, width, height, this->tileAtlas, game, player);
	}

	Map(const std::string& filename, int id, unsigned int width, unsigned int height, unsigned int tileSize,
		std::map<std::string, Tile>& tileAtlas, Game* game, Player& player, bool first) {
		this->width = width;
		this->height = height;
		this->tileSize = tileSize;
		this->gems = 0;
		this->nextLevel = false;
		this->prevLevel = false;
		this->firstLevel = first;
		this->tileAtlas = tileAtlas;
		loadMap(filename,id, width, height, this->tileAtlas, game, player);
	}
};

#endif // !MAP_HPP
