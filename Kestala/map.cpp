#include "map.hpp"


void Map::loadMap(const std::string& filename, int id, unsigned int width, unsigned int height,
	std::map<std::string, Tile>& entityAtlas, Game* game, Player& player) {
	this->id = id;
	int totalSize = width * height;
	std::ifstream input;
	input.open(filename);
	float currentX = 0;
	float currentY = 0;

	for (int i = 0; i <totalSize; i++)
	{
		currentX = float(i % width);
		if (i >0 && i % height == 0) currentY++;

		Cell cell(currentX, currentY, i);
		
		char character;
		input.get(character);
		switch (character) {
		case 'F':
			//floor
			cell.cellContents.push_back(entityAtlas.at("floor"));
			break;
		case 'W':
			//wall
			cell.cellContents.push_back(entityAtlas.at("wall"));
			break;
		case 'E':
			//exit
			exitPos = sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize);
			cell.cellContents.push_back(entityAtlas.at("exit"));
			break;

		case 'L':
			//locked passage
			cell.cellContents.push_back(entityAtlas.at("blockedDoor"));
			break;
		case 'k':
			//key
			cell.cellContents.push_back(entityAtlas.at("floor"));
			cell.cellContents.push_back(entityAtlas.at("key"));
			break;

		case 'e': {
			//enemy
			Enemy enemy = Enemy(sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize), game->texmgr.getRef("spritesheet"), game->animgr.firstFrame("enemy"));
			char nxtE;
			input.get(nxtE);
			switch (nxtE)
			{
			case 'n':
				enemy.isSolid = false;
				enemies.push_back(enemy);
				break;
			case 's':
				enemy.isSolid = true;
				enemies.push_back(enemy);
				break;
			}
			cell.cellContents.push_back(entityAtlas.at("floor"));
			break;
		}
		case 'P':
			//player
			player.setPosition(sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize));
			startPos = sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize);
			cell.cellContents.push_back(entityAtlas.at("start"));
			break;

		case 't':
			//minor treasure
			cell.cellContents.push_back(entityAtlas.at("floor"));
			cell.cellContents.push_back(entityAtlas.at("minorTreasure"));

			break;
		case 'T':
			//main treasure
			cell.cellContents.push_back(entityAtlas.at("floor"));
			cell.cellContents.push_back(entityAtlas.at("hiddenTreasure"));
			break;

		case 'g':
			//spell gems
			this->gems++;
			cell.cellContents.push_back(entityAtlas.at("floor"));
			char nxtG;
			input.get(nxtG);
			switch (nxtG)
			{
			case 'r':
				cell.cellContents.push_back(entityAtlas.at("fireGem"));
				break;
			case 'g':
				cell.cellContents.push_back(entityAtlas.at("earthGem"));
				break;
			case 'b':
				cell.cellContents.push_back(entityAtlas.at("waterGem"));
				break;
			case 'd':
				cell.cellContents.push_back(entityAtlas.at("airGem"));
				break;
			}			
			break;
		case 'G':
			//Gem Altar
			cell.cellContents.push_back(entityAtlas.at("floor"));
			char nxtA;
			input.get(nxtA);
			switch (nxtA)
			{
			case 'R':
				cell.cellContents.push_back(entityAtlas.at("fireAltar"));
				break;
			case 'G':
				cell.cellContents.push_back(entityAtlas.at("earthAltar"));
				break;
			case 'B':
				cell.cellContents.push_back(entityAtlas.at("waterAltar"));
				break;
			case 'D':
				cell.cellContents.push_back(entityAtlas.at("airAltar"));
				break;
			}
			break;
		case 'c':
			//clue
			cell.cellContents.push_back(entityAtlas.at("floor"));
			cell.cellContents.push_back(entityAtlas.at("clue"));
			break;
		default:
			break;
		}
		mapCells.push_back(cell);
	}
	this->totalGems = this->gems;
	input.close();
	return;
}

Cell* Map::getCell(sf::Vector2f position) {
	int y = position.y / tileSize;
	int x = position.x / tileSize;
	int i = (y * 15) + x;
	return &this->mapCells[i];
}

void Map::distanceCalculation(Player& player) {
	for (auto& cell : this->mapCells) {
		int dist = 0;
		dist += ((abs(cell.getPosition().y - player.getPosition().y))/tileSize);
		dist += ((abs(cell.getPosition().x - player.getPosition().x))/tileSize);
		cell.setDistance(dist);
	}	
}

void Map::enemyMove(Player &player, Game* game) {
	//distanceCalculation(player);
	//for (auto &enemy : this->enemies) {
	//	if (enemy.active) {
	//		if (!enemy.isSolid) {
	//			sf::Vector2f newEnemyPos = enemyPathFinder(enemy, player);
	//			if (!checkCollision(newEnemyPos, enemy, player)) {
	//				enemy.updatePos(newEnemyPos);
	//			}
	//		}
	//		if (enemy.getPosition() == player.getPosition()) {
	//			player.takeDamage();
	//		}
	//		game->animgr.update(enemy.sprite, enemy.spriteOrigin);
	//	}

	//}
}

struct lowestFScore {
	bool operator()(const Cell* cell1, const Cell* cell2) const {
		if (cell1->fScore == cell2->fScore) {
			return cell1->positionScore > cell2->positionScore;
		}
		else {
			return cell1->fScore > cell2->fScore;
		}		
	}
};

void Map::resetCellScores() {
	for (auto& cell : this->mapCells) {
		cell.setCost(0);
		cell.setfScore(0);
		cell.setParent(NULL);
		cell.openList = false;
		cell.closedList = false;
	}
}

sf::Vector2f Map::enemyPathFinder(Enemy& enemy, Player& player) {
	boost::heap::fibonacci_heap<Cell*, boost::heap::compare<lowestFScore>> openList;
	std::vector<Cell*> closedList;
	std::vector <sf::Vector2f> path;

	//start
	resetCellScores();
	sf::Vector2f startPosition = enemy.getPosition();
	Cell* startCell = getCell(enemy.getPosition());
	Cell* endCell = getCell(player.getPosition());
	int f = startCell->distance;
	startCell->setCost(0);	
	startCell->setfScore(f);
	startCell->setParent(startCell);
	openList.push(startCell);
	startCell->openList = true;

	bool finished = false;
	while (!finished) {
		if (openList.empty()) {
			finished = true;
			return startPosition;
			break;
		}
		//check cell with lowest f score
		Cell* currentCell = openList.top();
		if (currentCell->getPosition() == endCell->getPosition()) {
			while (currentCell->parent != startCell) {
				path.push_back(currentCell->getPosition());
				currentCell = currentCell->parent;
			}
			finished = true;
			return currentCell->getPosition();
		}
		openList.pop();
		currentCell->openList = false;
		closedList.push_back(currentCell);
		currentCell->closedList = true;

		//add children if passable
		sf::Vector2f currentPos = currentCell->getPosition();
		std::vector<Cell*> children;

		if (((currentPos.y / tileSize) + 1) <= 14) {
			sf::Vector2f newPos = currentPos;
			newPos.y += tileSize;
			Cell* South = getCell(newPos);
			for (auto &content : South->cellContents) {
				if (content.type != Entity::entityType::WALL) {
					children.push_back(South);
				}
			}
		}
		if (((currentPos.y / tileSize) - 1) >= 0) {
			sf::Vector2f newPos = currentPos;
			newPos.y -= tileSize;
			Cell* North = getCell(newPos);
			for (auto &content : North->cellContents) {
				if (content.type != Entity::entityType::WALL) {
					children.push_back(North);
				}
			}
		}
		if (((currentPos.x / tileSize) + 1) <= 14) {
			sf::Vector2f newPos = currentPos;
			newPos.x += tileSize;
			Cell* East = getCell(newPos);
			for (auto &content : East->cellContents) {
				if (content.type != Entity::entityType::WALL) {
					children.push_back(East);
				}
			}
		}

		if (((currentPos.x / tileSize) - 1) >= 0) {
			sf::Vector2f newPos = currentPos;
			newPos.x -= tileSize;
			Cell* West = getCell(newPos);
			for (auto &content : West->cellContents) {
				if (content.type != Entity::entityType::WALL) {
					children.push_back(West);
				}
			}
		}
				
		for (auto& child : children) {
			if (!child->closedList) {
				child->setParent(currentCell);
				child->setCost(child->parent->cost + 1);
				child->setfScore(child->cost + child->distance);
				if (!child->openList) {
					openList.push(child);
					child->openList = true;
				}
				else if (child->openList) {
					if ((currentCell->cost + 1) < child->cost) {								
						child->setParent(currentCell);
						openList.push(child);
					}
				}			
			}
		}
	}
}

bool Map::checkCollision(sf::Vector2f position, Entity movingEntity, Player& player) {
	position.x = position.x / tileSize;
	position.y = position.y / tileSize;

	//check out of bounds
	if (position.x < 0 || position.x >=15
		|| position.y < 0 || position.y >=15) {
		return true;
	}

	//check collisions within map
	for (auto &cell : this->mapCells) {
		if (cell.cellX == position.x && cell.cellY == position.y) {
			for (auto &content : cell.cellContents) {
				if (content.type == Entity::entityType::WALL) {
					return true;
				}
				//player reaches exit
				if (movingEntity.type == Entity::entityType::PLAYER
					&& content.type == Entity::entityType::EXIT) {
					leaveMap(this->nextLevel);
				}

				//player reaches door
				if (movingEntity.type == Entity::entityType::PLAYER
					&& content.type == Entity::entityType::DOOR) {
					if (player.key) {
						cell.cellContents.pop_back();
						cell.cellContents.push_back(this->tileAtlas.at("openDoor"));
						this->unlocked = true;
						player.key = false;
						return false;
					}
					else {
						return true;
					}
				}

				//player goes back level
				if (movingEntity.type == Entity::entityType::PLAYER
					&& content.type == Entity::entityType::START
					&& this->firstLevel == false) {
					leaveMap(this->prevLevel);
				}

				//pickup item
				if (movingEntity.type == Entity::entityType::PLAYER
					&& content.type == Entity::entityType::PICKUP) {
					if (content.active) {
						content.active = false;
						int healthAdd = 25;
						switch (content.id)
						{
						case 'f':
							//fire gem
							player.itemCollected["fireGem"] = true;	
							player.gems++;
							while (player.health < 100 || healthAdd < 0) {
								player.health++;
								healthAdd--;
							}
							if (player.spells < 3) {
								player.spells += 1;
							}
							cell.cellContents.pop_back();
							return false;
							break;
						case 'e':
							//earth gem
							player.itemCollected["earthGem"] = true;
							player.gems++;
							while (player.health < 100 || healthAdd < 0) {
								player.health++;
								healthAdd--;
							}
							if (player.spells < 3) {
								player.spells += 1;
							}
							cell.cellContents.pop_back();
							return false;
							break;
						case 'a':
							//air gem
							player.itemCollected["airGem"] = true;
							player.gems++;
							while (player.health < 100 || healthAdd < 0) {
								player.health++;
								healthAdd--;
							}
							if (player.spells < 3) {
								player.spells += 1;
							}
							cell.cellContents.pop_back();
							return false;
							break;
						case 'w':
							//water gem
							player.itemCollected["fireGem"] = true;
							player.gems++;
							while (player.health < 100 || healthAdd < 0) {
								player.health++;
								healthAdd--;
							}
							if (player.spells < 3) {
								player.spells += 1;
							}
							cell.cellContents.pop_back();
							return false;
							break;
						case 'T':
							//final treasure
							break;
						case 'C':
							//clue
							clueFound = true;
							break;
						case 'M':
							//minor treasure
							player.gold += 20;
							break;
						case 'K':
							//key
							player.key = true;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
		//check if player hits enemy
		if (movingEntity.type == Entity::entityType::PLAYER) {
			for (auto &enemy : this->enemies) {
				if (enemy.getPosition() == movingEntity.getPosition() && enemy.active) {
					movingEntity.takeDamage();
					if (enemy.isSolid) {
						return true;
					}
				}
			}
		}	
	return false;
}

void Map::leaveMap(bool& change) {
	change = true;
}

void Map::returnMap(Player& player) {
	player.setPosition(exitPos);
	prevLevel = false;
	nextLevel = false;
}

void Map::restartMap(Player& player) {
	player.setPosition(startPos);
	prevLevel = false;
	nextLevel = false;
}

void Map::explode(Player& player) {
	player.spells--;
	sf::Vector2f playerPos = player.getPosition();
	std::vector<sf::Vector2f> nearby;
	nearby.push_back(playerPos);
	nearby.push_back(playerPos);
	nearby.push_back(playerPos);
	nearby.push_back(playerPos);
	nearby.push_back(playerPos);
	nearby[0].x += tileSize;
	nearby[1].x -= tileSize;
	nearby[2].y += tileSize;
	nearby[3].y -= tileSize;

	for (auto& enemy : enemies) {
		for (auto& near : nearby) {
			if (enemy.getPosition() == near) {
				enemy.active = false;
			}
		}
	}


}

std::string Map::clueText() {
	std::string clue = "True Arcane Power \ncomes from the \n joining  of all \n elements of creation";
	return clue;
}

void Map::draw(sf::RenderWindow& window) {

	for (auto &cell : this->mapCells) {
		for (auto &content : cell.cellContents) {
			if (content.active) {
				sf::Vector2f pos;
				pos.x = cell.cellX * content.tileSize;
				pos.y = cell.cellY * content.tileSize;
				content.sprite.setPosition(pos);
				content.draw(window);
			}
		}
	}
	for (auto &enemy : this->enemies) {
		if (enemy.active) {
			enemy.draw(window);
		}

	}

}