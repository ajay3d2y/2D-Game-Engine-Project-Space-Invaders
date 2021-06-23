#pragma once
#include<RigidBody.h>
#include<TextureObject.h>
#include<SFML/Graphics.hpp>
class GameObject {
public:
	RigidBody* rb;
	TextureObject* tb;

	GameObject() {
		rb = nullptr;
		tb = nullptr;
	}

	~GameObject() {
		printf("Test");
		delete rb;
		delete tb;
	}

	void worldPos(sf::Vector2f val) {
		tX = val.x;
		tY = val.y;
	}
	sf::Vector2f getPos() {
		return sf::Vector2f(x, y);
	}

	void Translate(sf::Vector2f val) {
		tX = tX + val.x;
		tY = tY + val.y;
	}

	void locPos(sf::Vector2f val) {
		x = val.x;
		y = val.y;
	}

	void actPos() {
		rb->setPosition(x + tX, y + tY);
	}
protected:
	float x;
	float y;
	float tX;
	float tY;
};