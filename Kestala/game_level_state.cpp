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
	if (start) {
		map.draw(this->game->window, this->player, this->game, dt);
		player.draw(this->game->window);
	}
	return;
}

void GameLevel::update(sf::Clock& clock) {
	if (!start) {
		if (startClock.getElapsedTime().asSeconds() >= 10) {
			this->start = true;
			this->gui.intro.setString("");
		}
		else {
			int time = 11 - this->startClock.getElapsedTime().asSeconds();
			this->gui.update("intro", std::to_string(time));
		}
		
	}
	if (start) {
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
			map.returnMap(player);

		}

		/*Update enemies*/
		float dt = clock.getElapsedTime().asSeconds();	
		if (dt > this->game->gameSpeed) {	
			this->map.enemyMove(player, this->game);			
			if (this->player.beenHit) {
				this->game->audmgr.addBufferToQueue("playerhit");
			}
			gameOver = player.isDead();
			this->player.beenHit = false;
		}
			
				

		this->gui.update("health", "Health: " + this->player.getHealth());
		this->gui.update("spells", "Spells: " + this->player.getSpells());
		this->gui.update("gem", "Gems Gathered: " + this->player.getGems());
		this->gui.update("gold", "Gold Found: " + this->player.getGold());
		this->gui.update("keys", "Keys Collected: " + this->player.getKeys());
		if (this->map.clueFound) {
			this->gui.update("t_clue", this->map.clueText());
		}
		this->game->audmgr.playSound(this->game->SFX, this->game->SFXbackup);
		return;
	}
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
				if (keyPress.getElapsedTime().asSeconds() > player.moveSpeed || k != lastPressed) {					
					this->playerMove(k);
					this->game->animgr.update(player.sprite, player.spriteOrigin);
					keyPress.restart();
					lastPressed = k;
				}			
			}
			if (event.key.code == sf::Keyboard::Return) {
				if (this->player.spells > 0) {
					this->map.spell(this->player, this->game);
					this->game->audmgr.addBufferToQueue("spell");
				}
			}
 			if (event.key.code == sf::Keyboard::Space) {
				std::string soundEffect = "";
				if (this->map.interact(this->player, soundEffect)) {
					this->altarsUnlocked++;
					if (altarsUnlocked == 4) {
						this->map.unlockTreasure();
						soundEffect = "unlock";
					}
				}
				if (soundEffect.length() > 0) {
					this->game->audmgr.addBufferToQueue(soundEffect);
				}
			}
		}								
		default:
			break;
		}

	}

}

void GameLevel::playerMove(sf::Keyboard::Key& dirKey) {	
	sf::Vector2f newPos = player.movePosition(dirKey);
	if (dirKey != lastPressed) {
		this->game->animgr.changeDirection(dirKey, player.sprite, player.spriteOrigin);
	}
	std::string soundEffect = "";
	if (!map.playerCollision(newPos, player, soundEffect)) {
		player.updatePos(newPos);
	}	
	gameWon = player.grimoire;
	gameOver = player.isDead();
	this->player.beenHit = false;
	if (soundEffect.length() > 0) {
		this->game->audmgr.addBufferToQueue(soundEffect);
	}	
}

GameLevel::GameLevel(Game* game) {
	player = Player(sf::Vector2f(0, 0), game->texmgr.getRef("spritesheet"), game->animgr.firstFrame("player"));
	this->game = game;
	this->start = false;
	this->altarsUnlocked = 0;
	this->player = player;
	this->itemCollected = {false, false, false, false};
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
	this->startClock.restart();
	this->gui.update("intro", std::to_string(this->startClock.getElapsedTime().asSeconds()));
	this->gui.update("level", "Level: " + std::to_string(currentLevel));
	this->gui.update("spells", "Spells: " + std::to_string(this->player.spells));
	

}