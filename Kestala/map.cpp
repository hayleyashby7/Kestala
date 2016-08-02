#include "map.hpp"


void Map::loadMap(const std::string& filename,int id, unsigned int width, unsigned int height, 
	std::map<std::string, Tile>& entityAtlas, Game* game, Player& player) {
	this->id = id;
	int totalSize = width * height;	
	std::ifstream input;
	input.open(filename);
	float currentX = 0;
	float currentY = 0;

	for (int i = 0; i <totalSize; i++)	
	{
		currentX = float(i % width + 1);
		if (i % height == 0) currentY++;

		Cell cell(currentX, currentY);	

		char character;
		input.get(character);
		switch (character) {
		case '0':
			cell.cellContents.push_back(entityAtlas.at("floor"));
			break;
		case '1':
			cell.cellContents.push_back(entityAtlas.at("wall"));
			break;
		case '2': {
			Enemy enemy = Enemy(sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize), game->texmgr.getRef("enemy"));
			enemies.push_back(enemy);
			cell.cellContents.push_back(entityAtlas.at("floor"));
		}
		case '3':
			//pickup
			this->gems++;
			cell.cellContents.push_back(entityAtlas.at("floor"));
			cell.cellContents.push_back(entityAtlas.at("gem"));
			
			break;

		case '4': 
			//player
			player.setPosition(sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize));
			startPos = sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize);
			cell.cellContents.push_back(entityAtlas.at("start"));
			break;
		case '5':
			//exit
			exitPos = sf::Vector2f(currentX * this->tileSize, currentY* this->tileSize);
			cell.cellContents.push_back(entityAtlas.at("blockedexit"));
			
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

void Map::enemyMove(Player &player) {
	for (auto &enemy : this->enemies) {
		sf::Vector2f newEnemyPos = enemy.movePosition(enemy.enemyDirection);
		if (!checkCollision(newEnemyPos, enemy)) {
			enemy.updatePos(newEnemyPos);
			if (newEnemyPos == player.getPosition()) {
				player.takeDamage();
			}
		}
		else {
			enemy.changeDirection();
		}
	}
}

bool Map::checkCollision(sf::Vector2f position, Entity movingEntity) {
	position.x = position.x / tileSize;
	position.y = position.y / tileSize;

	//check out of bounds
	if (position.x <=0 || position.x > 15
		|| position.y <= 0 || position.y > 15) {
		return true;
	}
	
	//check collisions within map
	for (auto &cell : this->mapCells) {
		if (cell.cellX == position.x && cell.cellY == position.y) {		
			int currentGems = this->gems;
			for (auto &content : cell.cellContents) {					
				if (content.type == Entity::entityType::WALL) {						
					return true;
				}
				//player reaches exit
				if (movingEntity.type == Entity::entityType::PLAYER
					&& content.type == Entity::entityType::EXIT) {
					if ((this->totalGems - this->gems) >= 3) {
						leaveMap(this->nextLevel);
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
						this->gems--;
						content.active = false;
						if ((this->totalGems - this->gems) >= 3) {
							for (auto& cell : mapCells) {
								if (cell.cellX == (this->exitPos.x / tileSize) && cell.cellY == (this->exitPos.y / tileSize)) {
									cell.cellContents.pop_back();
									cell.cellContents.push_back(this->tileAtlas.at("exit"));
									this->unlocked = true;
								}
							}
						}						
					}
				}
			}
			if (currentGems != this->gems) {
				cell.cellContents.pop_back();
			}
		}
	}

	//check if player hits enemy
	if (movingEntity.type == Entity::entityType::PLAYER) {
		
		for (auto &enemy : this->enemies) {
			if (enemy.getPosition() == movingEntity.getPosition()) {
				movingEntity.takeDamage();
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
	nearby[0].x += tileSize;
	nearby[1].x -= tileSize;
	nearby[2].y += tileSize;
	nearby[3].y -= tileSize;

	for (auto& cell : mapCells) {
		for (auto& near : nearby) {
			if (cell.getPosition() == near) {
				bool changed = false;
				for (auto &content : cell.cellContents) {					
					switch (content.type){
					case Entity::entityType::WALL:
						content.type = Entity::entityType::FLOOR;
						changed = true;						
						break;
					default:
						break;
					}
				}
				if (changed) {
					cell.cellContents.pop_back();
					cell.cellContents.push_back(this->tileAtlas.at("floor"));
				}
			}
		}
	}


}

void Map::draw(sf::RenderWindow& window) {

	for (auto &cell : this->mapCells) {
		for (auto &content : cell.cellContents){
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
		enemy.draw(window);
	}	
	
}