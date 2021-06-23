#include <Game.h>
#include <Player.h>
#include <Timeline.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <thread>
#include <Event.h>
#include <EventManager.h>
#include <dukglue/dukglue.h>
#include<ScriptManager.h>
bool gravity;

EventManager em;

Game::Game() {
	window.create(sf::VideoMode(WIDTH, HEIGHT), "Assignment 2 - Server", sf::Style::Close | sf::Style::Resize | sf::Style::Titlebar);
	gameTime = *new Timeline(10);
	realTime = *new Timeline(1);
	player = new Player(0, 80.f, "res/player.png");
	player->rb->setPosition(512.f, 600.f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			enemy.push_back(new Enemy((7 * i + j), 50.f, "res/enemy.png", sf::Vector2f(125.f * (j + 1), 50.f * (i + 1))));
		}
	}
	for (int i = 0; i < enemy.size(); i++) {
		enemy[i]->setPointer(&plShots, player);
	}
	player->setPointer(&enShots);
	press = 0;
	lastTime = 0;
	em = *new EventManager(&sc, this, player, &enemy, &plShots,&enShots, &gameTime);
	sc = *new ScriptManager();
	score = 0;
	killCount = 0;
	plAmmo = 0;
	enAmmo = 0;
	lastShot = 400;
	lastShotID = -1;
	std::this_thread::sleep_for(std::chrono::seconds(5));
}

void Game::start() {
	gameTime.switchPause();
}

void Game::run() {
	long currentTime = 0;
	long rcurrentTime = 0;
	long lastTime = 0;
	while (window.isOpen()) {
		sf::Event event;
		window.pollEvent(event);
		if (event.type == sf::Event::Closed)
			window.close();
		currentTime = gameTime.getTime();
		if ((currentTime - lastTime) > 1) {
			//std::cout << currentTime << "\n";
			lastTime = currentTime;
			update();
			em.eventDispatch();
			render();
		}
	}
}

void Game::update() {
	if (player->getPlayerStatus() == 0) {
		sc.setupScript("Move.js");
		for (int i = 0; i < enemy.size(); i++) {
			sc.moveEnemy(enemy[i]);
		}
		for (int i = 0; i < plShots.size(); i++) {
			plShots[i]->moveAmmo();
		}
		for (int i = 0; i < enShots.size(); i++) {
			enShots[i]->moveAmmo();
		}
		for (int i = 0; i < enemy.size(); i++) {
			if (enemy[i]->getStatus() == 0) {
				enemy[i]->checkHit();
			}
		}
		player->checkHit();
		if (killCount == 28) {
			float arg[] = { 0 };
			em.enterQueue(Event(PLAYER_WIN, 0, gameTime.getTime(), arg));
		}
		enemyfireCheck();
		overflowCheck();
		ammoCheck();
	}

}

void Game::enemyfireCheck() {
	long currentTime = gameTime.getTime();
	if ((currentTime - lastShot) > 150) {
		double r = ((double)rand() / (RAND_MAX));
		int id = r * 27;
		if (enemy[id]->getStatus() == 0) {
			lastShotID = id;
			lastShot = currentTime;
			enemy[id]->Shoot();
		}
	}
}

void Game::overflowCheck() {
	for (int i = 0; i < enemy.size(); i++) {
		if (enemy[i]->rb->getPosition().y > 680.f) {
			float arg[] = { 0 };
			em.enterQueue(Event(PLAYER_DEAD, 0, gameTime.getTime(), arg));
		}
	}
}

void Game::ammoCheck() {
	for (int i = 0; i < plShots.size();i++) {
		if (plShots[i]->rb->getPosition().y < 0.f || plShots[i]->getStatus()) {
			Ammo *tmp = plShots[i];
			plShots.erase(plShots.begin() + i);
			delete tmp;
		}
	}
	for (int i = 0; i < enShots.size(); i++) {
		if (enShots[i]->rb->getPosition().y > 700.f || enShots[i]->getStatus()) {
			Ammo* tmp = enShots[i];
			enShots.erase(enShots.begin() + i);
		}
	}
}

void Game::render() {
		window.clear();
		if (player->getPlayerStatus() == 0) {
			for (int i = 0; i < plShots.size(); i++) {
				if (plShots[i]->getStatus() == 0) {
					window.draw(*(plShots[i]->rb));
				}
			}
			for (int i = 0; i < enShots.size(); i++) {
				if (enShots[i]->getStatus() == 0) {
					window.draw(*(enShots[i]->rb));
				}
			}
			for (int i = 0; i < enemy.size(); i++) {
				if (enemy[i]->getStatus() == 0) {
					window.draw(*(enemy[i]->rb));
				}
			}
			window.draw(*(player->rb));
		}
		else {

		}
		
		window.display();
}

void Game::recvEvent(std::string eventStr) {
	long time = gameTime.getTime();
	em.enterQueue(Event(eventStr, time));
}

void Game::onEvent(Event event) {
	if (event.eType == PLAYER_SHOOT) {
		sf::Vector2f pos(event.argList[0], event.argList[1]);
		plShots.push_back(new Ammo(plAmmo, sf::Vector2f(10.f,20.f),pos, "red", 1));
		plAmmo++;
	}
	else if (event.eType == ENEMY_DEAD) {
		score = score + 50;
		killCount++;
	}
	else if (event.eType == ENEMY_SHOOT) {
		sf::Vector2f pos(event.argList[0], event.argList[1]);
		enShots.push_back(new Ammo(enAmmo, sf::Vector2f(10.f, 20.f), pos, "blue", 0));
	}
	else if (event.eType == GAME_START) {
		player->setPlayerStatus(0);
	}
}

std::vector<std::string> Game::getData() {
	std::vector<std::string> vec;
	sf::Vector2f data = player->rb->getPosition();
	std::string datastr = std::to_string(score)+" "+std::to_string(player->getPlayerStatus()) + " " + std::to_string(data.x) + " " + std::to_string(data.y);
	vec.push_back(datastr);
	for (int i = 0; i < enemy.size(); i++) {
		data = enemy[i]->rb->getPosition();
		std::string datastr = std::to_string(enemy[i]->getStatus()) + " " + std::to_string(data.x) + " " + std::to_string(data.y);
		vec.push_back(datastr);
	}
	datastr = std::to_string(plShots.size());
	vec.push_back(datastr);
	for (int i = 0; i < plShots.size(); i++) {
		try {
			Ammo* tmp = plShots[i];
			data = tmp->rb->getPosition();
			std::string datastr = std::to_string(tmp->getStatus()) + " " + std::to_string(data.x) + " " + std::to_string(data.y);
			vec.push_back(datastr);
		}
		catch (const std::exception& e) {
			std::cout << "Error";
		}
	}
	datastr = std::to_string(enShots.size());
	vec.push_back(datastr);
	for (int i = 0; i < enShots.size(); i++) {
		try {
			Ammo* tmp = enShots[i];
			data = tmp->rb->getPosition();
			std::string datastr = std::to_string(tmp->getStatus()) + " " + std::to_string(data.x) + " " + std::to_string(data.y);
			vec.push_back(datastr);
		}
		catch (const std::exception& e) {
			std::cout << "Error";
		}
	}
	return vec;
}