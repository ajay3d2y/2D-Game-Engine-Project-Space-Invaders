#pragma once
#include<SFML/Graphics.hpp>
#include<Player.h>
#include<Timeline.h>
#include<Enemy.h>
#include<Event.h>
#include<Ammo.h>
#include<list>
#define WIDTH 1024
#define HEIGHT 720

class Game {
public:	
		Game();
		void run();
		void serverUpdate(std::vector<std::string> data);
		std::string getEvent();
		int peekEventQueue();


		float started;
private:
	
	void render();
	void update();
	void updateScore(std::string scr);
	void maintainplShots(std::string cnt);
	void maintainenShots(std::string cnt);

	sf::RenderWindow window;
	sf::Font font;

	sf::Text scoreDisplay;
	sf::Text winDisplay;
	sf::Text loseDisplay;
	std::list<Event> eventQueue;

	Player* player;
	vector<Enemy*> enemy;
	vector<Ammo*> plShots;
	vector<Ammo*> enShots;

	int plAmmo;
	int enAmmo;

	Timeline gameTime;
	Timeline realTime;
	sf::Clock clock;

	char windowFocus;
	float press;
	long score;
	
};