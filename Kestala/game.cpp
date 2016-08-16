#include <stack>
#include <string>

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

#include "game.hpp"
#include "game_state.hpp"



void Game::loadTextures() {
	texmgr.loadTexture("spritesheet", "assets/art/spritesheet.png");
	texmgr.loadTexture("background", "assets/art/background.png");
	
}

void Game::loadTiles() {
	sf::Texture& spriteSheet(texmgr.getRef("spritesheet"));
	this->tileAtlas["floor"] = Tile(this->tileHeight, spriteSheet, animgr.firstFrame("floor"), false, Entity::entityType::FLOOR);
	this->tileAtlas["wall"] = Tile(this->tileHeight, spriteSheet, animgr.firstFrame("wall"), true, Entity::entityType::WALL);
	this->tileAtlas["exit"] = Tile(this->tileHeight, spriteSheet, animgr.firstFrame("opendoor"), false, Entity::entityType::EXIT);
	this->tileAtlas["blockedexit"] = Tile(this->tileHeight, spriteSheet, animgr.firstFrame("blockedExit"), false, Entity::entityType::EXIT);
	this->tileAtlas["start"] = Tile(this->tileHeight, spriteSheet, animgr.firstFrame("opendoor"), false, Entity::entityType::START);
	this->tileAtlas["gem"] = Tile(this->tileHeight, spriteSheet, animgr.firstFrame("gem"), false, Entity::entityType::PICKUP);
}

void Game::loadAudio() {
	audmgr.loadSoundBuffer("pickup", "assets/sounds/pickup.wav");
	audmgr.loadSoundBuffer("playerhit", "assets/sounds/playerhit.wav");
	audmgr.loadSoundBuffer("unlockdoor", "assets/sounds/unlockdoor.wav");
	audmgr.loadSoundBuffer("explode", "assets/sounds/explode.wav");
}


void Game::pushBackState(GameState* state) {
	states.push_back(state);
	states.back()->init();
}

void Game::popBackState(){
	if (!states.empty()) {
		states.pop_back();
	}
}

void Game::changeState(GameState* state){
	states.push_back(state);
	states.back()->init();
}

void Game::goBackState() {
	int penultState = states.size() - 2;
	changeState(states[penultState]);
}

GameState* Game::checkState(){
	if (this->states.empty())
		return nullptr;
	return this->states.back();
}

void Game::draw(const float dt) {
	states.back()->draw(dt);
}

void Game::update(sf::Clock& clock) {
	states.back()->update(clock);
}

void Game::eventHandler() {
	states.back()->eventHandler();
}

void Game::cleanUp() {
	//clean all states
	while (!states.empty()) {
		states.pop_back();
	}
}

void Game::gameLoop(){
	sf::Clock clock;

	while (this->window.isOpen()) {		
		float dt = clock.getElapsedTime().asSeconds();
		if (checkState() == nullptr)
			continue;
		this->eventHandler();
		this->update(clock);
		this->window.clear(sf::Color::Black);
		this->draw(dt);
		this->window.display();
		if (dt > this->gameSpeed) clock.restart();
	
	}
}

void Game::newGame(GameState* state) {
	cleanUp();
	changeState(state);
}

Game::Game(){
	this->loadTextures();
	this->loadTiles();
	this->loadAudio();
	this->window.create(sf::VideoMode(704, 480), "Kestala", sf::Style::Titlebar | sf::Style::Close);
	this->window.setFramerateLimit(60);
	this->background.setTexture(this->texmgr.getRef("background"));
	this->SFX.setVolume(50);
}

Game::~Game(){
	while (!this->states.empty())
		popBackState();
}
