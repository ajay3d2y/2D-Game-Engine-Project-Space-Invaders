#pragma once
#include<GameObject.h>
#include<SFML/Graphics.hpp>
#include<SpawnPoint.h>
class DeathZone:public GameObject {
public:
	DeathZone() {

	}
	DeathZone(sf::Vector2f dime, sf::Vector2f pos,SpawnPoint sp) {
		rb = new RigidBody('R', dime.x, dime.y);
		rb->setPosition(pos);
		x = pos.x;
		y = pos.y;
		target = sp.getPos();
	}

	sf::Vector2f getRespawnLocation() {
		return target;
	}

private:
	sf::Vector2f target;

};