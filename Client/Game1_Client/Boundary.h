#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<GameObject.h>
class Boundary : public GameObject {
public:
	Boundary() {

	}

	Boundary(sf::Vector2f dime, sf::Vector2f spos) {
		rb = new RigidBody('R', dime.x, dime.y);
		rb->setPosition(spos);
		x = spos.x;
		y = spos.y;
	}

	sf::Vector2f position() {
		return sf::Vector2f(x, y);
	}
};