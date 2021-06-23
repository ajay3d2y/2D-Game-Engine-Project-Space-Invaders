#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<GameObject.h>

class Platform : public GameObject {
public:
	Platform() {
	}

	Platform(sf::Vector2f dime, std::string path) {
		rb = new RigidBody('R', dime.x, dime.y);
		tb = new TextureObject(rb, path);
	}

	Platform(sf::Vector2f dime, sf::Vector2f spos, std::string path) {
		rb = new RigidBody('R', dime.x, dime.y);
		tb = new TextureObject(rb, path);
		x = spos.x;
		y = spos.y;
		rb->setPosition(spos);
	}

private:

};