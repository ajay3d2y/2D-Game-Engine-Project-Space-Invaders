#include <Game.h>
#include <Player.h>
#include <Platform.h>
#include <Enemy.h>
#include <Timeline.h>
#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <thread>
#include <Event.h>

bool gravity;

Game::Game() {
	window.create(sf::VideoMode(WIDTH, HEIGHT), "Assignment 2 - Client", sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar);
	gameTime = *new Timeline(10);
	realTime = *new Timeline(1);

	if (!font.loadFromFile("res/ITCKRIST.ttf")) {
		std::cout << "ERROR\n";
	}

	scoreDisplay.setFont(font);
	scoreDisplay.setPosition(sf::Vector2f(10.f, 10.f));
	scoreDisplay.setCharacterSize(20);
	winDisplay.setString("Congratulations");
	winDisplay.setFont(font);
	winDisplay.setCharacterSize(60);
	winDisplay.setPosition(sf::Vector2f(200.f, 400.f));
	loseDisplay.setString("Game Over");
	loseDisplay.setFont(font);
	loseDisplay.setCharacterSize(60);
	loseDisplay.setPosition(sf::Vector2f(300.f, 325.f));
	
	player = new Player(0, 80.f, "res/player.png");
	player->rb->setPosition(512.f, 600.f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			enemy.push_back(new Enemy((7 * i + j), 50.f, "res/enemy.png"));
		}
	}
	windowFocus = 'Y';
	press = 0.f;
	started = 0.f;
	plAmmo = 0;
	enAmmo = 0;
}

void Game::run() {
	long lastTime = 0;
	long currentTime = 0;
	float arg[] = { 1 };
	eventQueue.push_back(Event(GAME_START, 1, gameTime.getTime(), arg));
	while (window.isOpen()) {
		sf::Event event;
		window.pollEvent(event);
		if (event.type == sf::Event::Closed) {
			window.close();/*
			float arg[] = { playerId - 1 };
			eventQueue.push_back(Event(GAME_EXIT, 1, gameTime.getTime(), arg));
			while(eventQueue.size()!=0){}*/
		}
			
		if (event.type == sf::Event::GainedFocus) {
			windowFocus = 'Y';
		}
		if (event.type == sf::Event::LostFocus) {
			windowFocus = 'N';
		}
		
		currentTime = gameTime.getTime();
		if ((currentTime - lastTime) > 1) {
			lastTime = currentTime;
			update();
			render();
		}
		
	}
}

void Game::update() {
	scoreDisplay.setString("Score = " + std::to_string(score));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)&&(windowFocus=='Y')) {
		float arg[] = { 0 };
		eventQueue.push_back(Event(USER_INPUT, 1, gameTime.getTime(), arg));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&&(windowFocus == 'Y')) {
		float arg[] = { 1 };
		eventQueue.push_back(Event(USER_INPUT, 1, gameTime.getTime(), arg));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&(windowFocus == 'Y')) {
		if (press == 0.f) {
			float arg[] = { 2 };
			eventQueue.push_back(Event(USER_INPUT, 1, gameTime.getTime(), arg));
			press = 1.f;
			clock.restart();
		}
	}
	else {
		sf::Time time = clock.getElapsedTime();
		if (time > sf::seconds(.7f))
			press = 0.f;
	}

	if (player->getPlayerStatus()>0) {
		scoreDisplay.setPosition(400.f, 500.f);
	}
}

void Game::updateScore(std::string scr) {
	score = std::stol(scr);
}

void Game::maintainplShots(std::string cnt) {
	int count = std::stoi(cnt);
	if (count > plShots.size()) {
		while (count != plShots.size()) {
			plShots.push_back(new Ammo(plAmmo, sf::Vector2f(10.f, 25.f), "blue", 1));
			plAmmo++;
		}
	}
	else if (count < plShots.size()) {
		while (count != plShots.size()) {
			plShots.pop_back();
		}
	}
}

void Game::maintainenShots(std::string cnt) {
	int count = std::stoi(cnt);
	if (count > enShots.size()) {
		while (count != enShots.size()) {
			enShots.push_back(new Ammo(plAmmo, sf::Vector2f(10.f, 25.f), "red", 1));
			plAmmo++;
		}
	}
	else if (count < enShots.size()) {
		while (count != enShots.size()) {
			enShots.pop_back();
		}
	}
}
std::string Game::getEvent() {
	std::string eventString = "";
	if (eventQueue.size() != 0) {
		Event event = eventQueue.front();
		eventQueue.pop_front();
		eventString = event.eventToString();
	}
	return eventString;
}

void Game::serverUpdate(std::vector <std::string> data) {
	int iter = 1;
	std::string info = data.at(0);
	std::istringstream test(info);
	std::string score;
	std::string status;
	std::string posx;
	std::string posy;
	test >> score;
	test >> status;
	test >> posx;
	test >> posy;
	updateScore(score);
	player->setPlayerStatus(std::stoi(status));
	player->rb->setPosition(sf::Vector2f(std::stof(posx), std::stof(posy)));
	for (int i = 0; i < enemy.size(); i++) {
		info = data.at(iter);
		std::istringstream tmp(info);
		tmp >> status;
		tmp >> posx;
		tmp >> posy;
		enemy[i]->setStatus(std::stoi(status));
		enemy[i]->rb->setPosition(sf::Vector2f(std::stof(posx), std::stof(posy)));
		iter++;
	}
	info = data.at(iter);
	std::istringstream test1(info);
	test1 >> status;
	maintainplShots(status);
	iter++;
	for (int i = 0; i < plShots.size(); i++) {
		if (iter < data.size()) {
			info = data.at(iter);
			std::istringstream tmp(info);
			tmp >> status;
			tmp >> posx;
			tmp >> posy;
			plShots[i]->setStatus(std::stoi(status));
			plShots[i]->rb->setPosition(sf::Vector2f(std::stof(posx), std::stof(posy)));
			iter++;
		}
		
	}
	if (iter < data.size()) {
		info = data.at(iter);
		std::istringstream test2(info);
		test2 >> status;
		maintainenShots(status);
		iter++;
	}
	for (int i = 0; i < enShots.size(); i++) {
		if (iter < data.size()) {
			info = data.at(iter);
			std::istringstream tmp(info);
			tmp >> status;
			tmp >> posx;
			tmp >> posy;
			enShots[i]->setStatus(std::stoi(status));
			enShots[i]->rb->setPosition(sf::Vector2f(std::stof(posx), std::stof(posy)));
			iter++;
		}
	}

}

int Game::peekEventQueue() {
	return eventQueue.size();
}


void Game::render() {
		window.clear();
		if (player->getPlayerStatus() == 0) {
			try {
				for (int i = 0; i < enShots.size(); i++) {
					if (enShots[i]->getStatus() == 0) {
						window.draw(*(enShots[i]->rb));
					}
				}
				for (int i = 0; i < plShots.size(); i++) {
					if (plShots[i]->getStatus() == 0) {
						window.draw(*(plShots[i]->rb));
					}
				}
			}
			catch (const std::exception& e) {

			}
			window.draw(*(player->rb));
			for (int i = 0; i < enemy.size(); i++) {
				if (enemy[i]->getStatus() == 0) {
					window.draw(*(enemy[i]->rb));
				}
			}
			window.draw(scoreDisplay);
		}
		else if (player->getPlayerStatus() == 1) {
			window.draw(scoreDisplay);
			window.draw(loseDisplay);
		}
		else if (player->getPlayerStatus() == 2) {
			window.draw(scoreDisplay);
			window.draw(winDisplay);
		}
		
		window.display();
}
