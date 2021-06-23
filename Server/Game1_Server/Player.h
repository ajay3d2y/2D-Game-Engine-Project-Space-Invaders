#pragma once
#include <SFML/Graphics.hpp>
#include <GameObject.h>
#include <iostream>
#include <DeathZone.h>
#include <list>
#include <queue>
#include<Event.h>
#include<Timeline.h>
#include<Ammo.h>

class Player : public GameObject
{
public:
	Player();
	Player(int id,float side, std::string path);
	void onEvent(Event event);
	Event getEvent();
	int getQueueSize();
	int getPlayerStatus();
	void setPlayerStatus(int st);
	void setPointer(std::vector<Ammo*>* a);
	void checkHit();
private:
	sf::Vector2f drawPoint;
	sf::Texture playerTexture;
	int playerId;
	int playerStatus;
	std::priority_queue<Event, vector<Event>, greater<vector<Event>::value_type>> pQueue;
	Timeline* gameTime;
	std::vector<Ammo*>* am;
};