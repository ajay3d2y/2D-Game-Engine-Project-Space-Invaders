#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>

class RigidBody : public sf::Shape {
public:
	RigidBody(char a, float b) {
		if (a == 'S') {
			drawPoint = sf::Vector2f(0.f, 0.f);
			type = a;
			side_length = b;
			update();
		}
	}

	RigidBody(char a, float b, float c) {
		if (a == 'R') {
			drawPoint = sf::Vector2f(0.f, 0.f);
			type = a;
			width = b;
			height = c;
			update();
		}
	}
	virtual std::size_t getPointCount() const {
		return 4; // fixed, but could be an attribute of the class if needed
	}

	virtual sf::Vector2f getPoint(unsigned int index) const
	{
		if (type == 'S') {
			if (index == 0) {
				return sf::Vector2f(0.f, 0.f);
			}
			else if (index == 1) {
				return sf::Vector2f(0.f, side_length);
			}
			else if (index == 2) {
				return sf::Vector2f(side_length, side_length);
			}
			else {
				return sf::Vector2f(side_length, 0.f);
			}
		}
		if (type == 'R') {
			if (index == 0) {
				return sf::Vector2f(0.f, 0.f);
			}
			else if (index == 1) {
				return sf::Vector2f(width, 0.0f);
			}
			else if (index == 2) {
				return sf::Vector2f(width, height);
			}
			else {
				return sf::Vector2f(0.f, height);
			}
		}
		
	}

private:
	char type;
	float width, height, side_length, radius;
	sf::Vector2f drawPoint;
};