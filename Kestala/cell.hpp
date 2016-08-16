#ifndef CELL_HPP
#define CELL_HPP

#include <vector>

#include "entity.hpp"

class Cell {
public:
	std::vector<Entity> cellContents;
	int distance;
	int cost;
	int fScore = 0;
	int positionScore;
	Cell* parent;

	float cellX;
	float cellY;
	int tileSize = 32;	

	bool closedList = false;
	bool openList = false;

	sf::Vector2f getPosition() {
		sf::Vector2f position;
		position.x = cellX * tileSize;
		position.y = cellY * tileSize;
		return position;
	}

	//Scores for pathfinding algorithm

	void setDistance(int newDistance) {
		this->distance = newDistance;
	}
	void setCost(int newCost) {
		this->cost = newCost;
	}
	void setfScore(int newFScore) {
		this->fScore = newFScore;
	}
	void setParent(Cell* parent) {
		this->parent = parent;
	}

	Cell(const float& x, const float& y, int i) {
		this->cellX = x;
		this->cellY = y;
		this->positionScore = i;
	}
	

};

#endif // !CELL_HPP