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
			cell.cellContents.push_back(entityAtlas.at("floor"));
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
				enemy.id = 0;
				enemies.push_back(enemy);
				break;
			case 's':
				char eType;
				input.get(eType);
				switch (eType)
				{
				case '1':
					//air enemy
					enemy.sprite.setColor(airColour);
					break;
				case '2':
					//earth enemy
					enemy.sprite.setColor(earthColour);
					break;
				case '3':
					//fire enemy
					enemy.sprite.setColor(fireColour);
					break;
				case '4':
					//water enemy
					enemy.sprite.setColor(waterColour);
					break;
				default:
					break;
				}
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
			case 'y':
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
			case 'Y':
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
	distanceCalculation(player);
	for (auto &enemy : this->enemies) {
		if (enemy.active) {
			if (!enemy.isSolid) {
				sf::Vector2f newEnemyPos = enemyPathFinder(enemy, player);
				if (!enemyCollision(newEnemyPos, enemy, player)) {
					enemy.updatePos(newEnemyPos);
				}
			}
			if (enemy.getPosition() == player.getPosition()) {
				player.takeDamage();
			}
			game->animgr.update(enemy.sprite, enemy.spriteOrigin);
		}
	}
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

bool Map::playerCollision(sf::Vector2f position, Player& player) {
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
				if (content.type == Entity::entityType::EXIT) {
					leaveMap(this->nextLevel);
				}

				//player reaches door
				if (content.type == Entity::entityType::DOOR) {
					if (!content.isSolid) {
						return false;
					}
					else {
						return true;
					}
				}

				//player goes back level
				if (content.type == Entity::entityType::START
					&& this->firstLevel == false) {
					leaveMap(this->prevLevel);
				}

				//pickup item
				if (content.type == Entity::entityType::PICKUP) {
					if (content.active && !content.isSolid) {
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
							while (player.spells < 3) {
								player.spells++;
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
							while (player.spells < 3) {
								player.spells++;
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
							while (player.spells < 3) {
								player.spells++;
							}
							cell.cellContents.pop_back();
							return false;
							break;
						case 'w':
							//water gem
							player.itemCollected["waterGem"] = true;
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
							player.grimoire = true;
							cell.cellContents.pop_back();
							return false;
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
							player.key++;
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
	for (auto &enemy : this->enemies) {
		if (enemy.getPosition() == player.getPosition() && enemy.active) {
			player.takeDamage();
			return false;
		}
		if (enemy.isSolid && enemy.getPosition() == position) {
			return true;
		}
	}
	return false;
}

bool Map::enemyCollision(sf::Vector2f position, Enemy& enemy, Player& player){
	position.x = position.x / tileSize;
	position.y = position.y / tileSize;

	//check out of bounds
	if (position.x < 0 || position.x >= 15
		|| position.y < 0 || position.y >= 15) {
		return true;
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

void Map::spell(Player& player, Game* game) {
	player.spells--;
	spellTicks = 0;
	spellCast = true;
	player.setSpell(game->texmgr.getRef("spritesheet"), game->animgr.firstFrame("spell"));
	std::vector<sf::Vector2f> nearby = nearPlayer(player);
	for (auto& enemy : enemies) {
		for (auto& near : nearby) {
			if (enemy.getPosition() == near) {
				switch (enemy.id)
				{
				case 0:
					//normal enemy
					enemy.active = false;
					break;
				case 1:
					//fire enemy
					if (player.itemCollected["fireGem"]) {
						enemy.active = false;
					}					
					break;
				case 2:
					//air enemy
					if (player.itemCollected["airGem"]) {
						enemy.active = false;
					}
					break;
				case 3:
					//earth enemy
					if (player.itemCollected["earthGem"]) {
						enemy.active = false;
					}
					break;
				case 4:
					//water enemy
					if (player.itemCollected["waterGem"]) {
						enemy.active = false;
					}
					break;
				default:
					break;
				}				
			}
		}
	}
}


bool Map::interact(Player& player) {
	sf::Vector2f position = player.getPosition();
	std::vector<sf::Vector2f> nearby = nearPlayer(player);
	for (auto &cell : this->mapCells) {
		if (cell.cellX == (position.x/tileSize) && cell.cellY == (position.y/tileSize)) {
			for (auto &content : cell.cellContents) {
				//interactive item
				if (content.type == Entity::entityType::INTERACTIVE) {
					switch (content.id)
					{
					case 'r':
						if (player.itemCollected["fireGem"]) {
							Entity gem = this->tileAtlas.at("fireGem");
							gem.isSolid = true;
							cell.cellContents.push_back(gem);
							player.itemCollected["fireGem"] = false;
							return true;
						}
						break;
					case 'g':
						if (player.itemCollected["earthGem"]) {
							Entity gem = this->tileAtlas.at("earthGem");
							gem.isSolid = true;
							cell.cellContents.push_back(gem);
							player.itemCollected["earthGem"] = false;
							return true;
						}
						break;
					case 'y':
						if (player.itemCollected["airGem"]) {
							Entity gem = this->tileAtlas.at("airGem");
							gem.isSolid = true;
							cell.cellContents.push_back(gem);
							player.itemCollected["airGem"] = false;
							return true;
						}
						break;						
					case 'b':
						if (player.itemCollected["waterGem"]) {
							Entity gem = this->tileAtlas.at("waterGem");
							gem.isSolid = true;
							cell.cellContents.push_back(gem);
							player.itemCollected["waterGem"] = false;
							return true;
						}
						break;
					default:
						return false;
					}
				}

			}
		}
		
		for (auto& near : nearby) {
			if (cell.getPosition() == near) {
				for (auto& content : cell.cellContents) {
					if (content.type == Entity::entityType::DOOR) {
						if (player.key > 0) {
							cell.cellContents.pop_back();
							Entity door = this->tileAtlas.at("openDoor");
							door.isSolid = false;
							cell.cellContents.push_back(door);
							this->unlocked = true;
							player.key--;
							return false;
						}
					}
				}
			}
		}
	}
	return false;
}

void Map::unlockTreasure() {
	for (auto &cell : this->mapCells) {
		for (auto &content : cell.cellContents) {
			if (content.id == 'H') {
				cell.cellContents.pop_back();
				cell.cellContents.push_back(this->tileAtlas.at("treasure"));
				return;
			}
		
		}
	}
}

std::vector<sf::Vector2f> Map::nearPlayer(Player& player) {
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
	return nearby;
}

std::string Map::clueText() {
	std::string clue = "Misplaced Note:\nTrue Arcane Power \ncomes from the \n joining  of all \n elements of creation";
	return clue;
}

void Map::updateSpell(Player& player, Game* game) {
	if (spellTicks >= 8) {
			spellCast = false;
			spellTicks = 0;
			player.setSpell(game->texmgr.getRef("spritesheet"), game->animgr.firstFrame("spell"));
		}
		else if (spellCast) {
			game->animgr.update(player.spell, player.spellOrigin, spellTicks);
			spellTicks++;
		}
}


void Map::draw(sf::RenderWindow& window, Player &player, Game* game, const float dt) {
	if (dt > 0.25) {
		updateSpell(player, game);
	}
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