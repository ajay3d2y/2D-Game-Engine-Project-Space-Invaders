#pragma once
#include<SFML/Graphics.hpp>
#include<Player.h>
#include<Enemy.h>
#include<Ammo.h>
#include<Timeline.h>
#include<Event.h>
#include<list>
#include<string>
#include<queue>
#include<ScriptManager.h>
#include<time.h>
#include <dukglue/dukglue.h>

#define WIDTH 1024
#define HEIGHT 720
#define sleep(n)    Sleep(n)
class Game {
public:	
		Game();
		void run();
		std::vector<std::string> getData();
		void onEvent(Event event);
		void start();
		void recvEvent(std::string eventStr);
		
private:
	void render();
	void update();
	void ammoCheck();
	void overflowCheck();
	void enemyfireCheck();
	sf::RenderWindow window;

	Player* player;
	vector<Enemy*> enemy;
	vector<Ammo*> plShots;
	vector<Ammo*> enShots;
	int plAmmo;
	int enAmmo;

	//std::list<Event> eventQueue;
	std::priority_queue<Event, vector<Event>, less<vector<Event>::value_type>> eventQueue;
	Timeline gameTime;
	Timeline realTime;
	ScriptManager sc;


	long lastTime;
	long lastShot;
	long lastShotID;
	long score;
	long killCount;
	int press = 0;
};