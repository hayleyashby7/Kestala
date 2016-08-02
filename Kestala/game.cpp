#include <stack>
#include <string>

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

#include "game.hpp"
#include "game_state.hpp"



void Game::loadTextures() {
	texmgr.loadTexture("background", "assets/art/background.png");
	texmgr.loadTexture("floor", "assets/art/floor.png");
	texmgr.loadTexture("wall", "assets/art/wall.png");
	texmgr.loadTexture("player", "assets/art/player.png");
	texmgr.loadTexture("enemy", "assets/art/enemy.png");
	texmgr.loadTexture("exit", "assets/art/exit.png");
	texmgr.loadTexture("blockedexit", "assets/art/blockedExit.png");
	texmgr.loadTexture("start", "assets/art/start.png");
	texmgr.loadTexture("gem", "assets/art/gem.png");
}

void Game::loadTiles() {
	this->tileAtlas["floor"] = Tile(this->tileHeight, texmgr.getRef("floor"), false, Entity::entityType::FLOOR);
	this->tileAtlas["wall"] = Tile(this->tileHeight, texmgr.getRef("wall"), true, Entity::entityType::WALL);
	this->tileAtlas["exit"] = Tile(this->tileHeight, texmgr.getRef("exit"), false, Entity::entityType::EXIT);
	this->tileAtlas["blockedexit"] = Tile(this->tileHeight, texmgr.getRef("blockedexit"), false, Entity::entityType::EXIT);
	this->tileAtlas["start"] = Tile(this->tileHeight, texmgr.getRef("start"), false, Entity::entityType::START);
	this->tileAtlas["gem"] = Tile(this->tileHeight, texmgr.getRef("gem"), false, Entity::entityType::PICKUP);
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
		states.back()->cleanUp();
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
		states.back()->cleanUp();
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
		if (dt > 0.25f) clock.restart();
	
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
	this->window.create(sf::VideoMode(750, 550), "Kestala", sf::Style::Titlebar | sf::Style::Close);
	this->window.setFramerateLimit(60);
	this->background.setTexture(this->texmgr.getRef("background"));
	this->bgMusic.openFromFile("assets/sounds/mainmenu.wav");
	this->bgMusic.setVolume(50);
	this->SFX.setVolume(50);
}

Game::~Game(){
	while (!this->states.empty())
		popBackState();
}
