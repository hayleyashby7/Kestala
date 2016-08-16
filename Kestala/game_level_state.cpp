#include "game_state.hpp"
#include "game_level_state.hpp"
#include "main_menu_state.hpp"
#include "info_state.hpp"
#include "map.hpp"

#include <SFML\Audio.hpp>


void GameLevel::init() {
	this->game->window.clear(sf::Color::Black);
	this->game->background.setTexture(this->game->texmgr.getRef("background"));
	this->game->window.draw(this->game->background);
}

void GameLevel::draw(const float dt) {
	this->game->window.clear(sf::Color::Black);	
	this->game->window.draw(this->game->background);
	gui.draw(this->game->window);
	map.draw(this->game->window);
	player.draw(this->game->window);
	return;
}

void GameLevel::update(sf::Clock& clock) {
		/*Check Game Over*/
		if (this->gameOver) {
			this->game->changeState(new Info(this->game, "dead"));
		}
		if (this->gameWon) {
			this->game->changeState(new Info(this->game, "win"));
		}

		/*Check if progressed level*/
		if (this->map.nextLevel) {
			this->map.prevVisited = true;
			if (prevLevels.size() >= this->map.id) {
				prevLevels[currentLevel - 1] = this->map;
			}
			else {
				prevLevels.push_back(this->map);
			}
			currentLevel++;
			if (currentLevel > maxLevel) {
				gameWon = true;
			}
			else {
				bool previous = false;
				for (auto& oldMap : prevLevels) {
					if (oldMap.prevVisited && oldMap.id == currentLevel) {
						this->map = oldMap;
						this->map.restartMap(player);
						previous = true;
						break;
					}
				}
				if (!previous) {
					std::string file = "map" + std::to_string(currentLevel);
					map = Map(mapFiles[file], currentLevel, 15, 15, 32, game->tileAtlas, game, player);
					mapList[file] = map;
					this->gui.update("level", "Level: " + std::to_string(currentLevel));
				}

			}
			map.nextLevel = false;
		}

		/*Check if regressed level*/
		if (this->map.prevLevel) {
			this->map.prevVisited = true;
			if (prevLevels.size() >= currentLevel) {
				prevLevels[currentLevel - 1] = this->map;
			}
			else {
				prevLevels.push_back(this->map);
			}
			if (currentLevel > 1) {
				currentLevel--;
			}
			map = prevLevels[currentLevel - 1];
			this->gui.update("level", "Level: " + std::to_string(currentLevel));
			map.returnMap(player);

		}

		/*Enemy movement*/
		float dt = clock.getElapsedTime().asSeconds();
		if (dt > this->game->gameSpeed) {
			this->map.enemyMove(player, this->game);			
			if (this->player.beenHit) {
				this->gui.update("player", "Health: " + this->player.getHealth());
				this->gui.update("gem", "Gems Gathered: " + this->player.getGems());
				this->game->audmgr.addBufferToQueue("playerhit");
			}
			gameOver = player.isDead();
			this->player.beenHit = false;
		}
		this->game->audmgr.playSound(this->game->SFX);
		return;
}

void GameLevel::eventHandler() {
	sf::Event event;

	while (this->game->window.pollEvent(event)) {
		
		switch (event.type) {
		/*Window closed*/
		case sf::Event::Closed: {
			game->window.close();
			break;
		}

		/*Key Pressed*/
		case sf::Event::KeyPressed: {
			if (event.key.code == sf::Keyboard::Q) {
				game->window.close();
				break;
			}
			if (event.key.code == sf::Keyboard::B) {
				this->game->goBackState();
				this->game->bgMusic.stop();
				break;
			}
			
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::A ||
				event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::D) {
				sf::Keyboard::Key k = event.key.code;
				if (keyPress.getElapsedTime().asSeconds() > (this->game->gameSpeed/2) || k != lastPressed) {					
					this->playerMove(k);
					this->game->animgr.update(player.sprite);
					keyPress.restart();
					lastPressed = k;
				}			
			}
			if (event.key.code == sf::Keyboard::Return) {
				if (this->player.spells > 0) {
					this->map.explode(this->player);
					this->gui.update("spells", "Spells: " + std::to_string(this->player.spells));
					this->game->audmgr.addBufferToQueue("explode");
				}
			}
		}								
		default:
			break;
		}

	}

}

void GameLevel::playerMove(sf::Keyboard::Key& dirKey) {	
		int mapGems = map.gems;
		sf::Vector2f newPos = player.movePosition(dirKey);
		this->game->animgr.changeDirection(dirKey, player.sprite);
		if (!map.checkCollision(newPos, player)) {
			player.updatePos(newPos);
		}
		if (map.gems < mapGems) {
			player.gems++;
			this->game->audmgr.addBufferToQueue("pickup");
			this->gui.update("gem", "Gems Gathered: " + this->player.getGems());
		}

		if (this->player.beenHit) {
			this->gui.update("player", "Health: " + this->player.getHealth());

		}

		gameOver = player.isDead();
		this->player.beenHit = false;
		if (this->map.unlocked && !this->map.doorOpened) {
			this->game->audmgr.addBufferToQueue("unlockdoor");
			this->map.doorOpened = true;
		}
	
}

GameLevel::GameLevel(Game* game) {
	player = Player(sf::Vector2f(0, 0), game->texmgr.getRef("spritesheet"), game->animgr.firstFrame("player"));
	this->game = game;
	this->start = false;
	this->player = player;
	this->game->bgMusic.openFromFile("assets/sounds/level.wav");
	this->game->bgMusic.setVolume(25);
	this->game->bgMusic.setLoop(true);
	this->game->bgMusic.play();

	for (int map = 1; map <= maxLevel; map++) {
		std::string filename;
		std::string mapName = "map" + std::to_string(map);
		filename = "assets/maps/" + mapName + ".dat";
		mapFiles[mapName] = filename;
	}
	currentLevel = 1;
	map = Map(mapFiles["map1"], currentLevel, 15,15,32, game->tileAtlas, game, player, true);
	mapList["map1"] = map;
	map.distanceCalculation(player);
	this->gui.update("level", "Level: " + std::to_string(currentLevel));
	this->gui.update("spells", "Spells: " + std::to_string(this->player.spells));

}