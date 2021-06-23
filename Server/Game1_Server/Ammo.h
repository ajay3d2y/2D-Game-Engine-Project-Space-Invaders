#pragma once
#include<GameObject.h>

class Ammo : public GameObject {
public:
	Ammo(){
	}
	Ammo(int id, sf::Vector2f dime, std::string path, int src) {
		rb = new RigidBody('R', dime.x, dime.y);
		tb = new TextureObject(rb, path);
		ammoID = id;
		ammoStatus = 0;
		source = src;
	}
	Ammo(int id, sf::Vector2f dime,sf::Vector2f pos, std::string path,int src) {
		rb = new RigidBody('R', dime.x,dime.y);
		tb = new TextureObject(rb, path);
		rb->setPosition(pos);
		ammoID = id;
		ammoStatus = 0;
		source = src;
	}

	int getID() {
		return ammoID;
	}

	void moveAmmo() {
		if (source == 0) {
			rb->move(0.f, 4.f);
		}
		else {
			rb->move(0.f, -4.f);
		}
	}

	int getStatus() {
		return ammoStatus;
	}

	void onEvent(Event e) {
		if (e.eType == AMMO_HIT_ENEMY) {
			ammoStatus = 1;
		}
	}
private:
	int ammoID;
	int source;
	int ammoStatus;
};