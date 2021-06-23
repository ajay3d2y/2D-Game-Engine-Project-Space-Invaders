#pragma once
#include <Platform.h>
#include <SFML/Graphics.hpp>
#include <list>

class MovPlatform : public Platform {
public:
	MovPlatform() {}
	MovPlatform(sf::Vector2f dim, std::string path) : Platform(dim,path) {

	}

	MovPlatform(sf::Vector2f dim, std::string path, sf::Vector2f destPoints[], int points, int rate) : Platform(dim, destPoints[0], path) {
		spos = destPoints[0];
		dpos = destPoints[1];
		currentPos = destPoints[0];
		delta = dpos - spos;
		arsize = points;
		for (int i = 0; i < points; i++) {
			pointSet.push_back(destPoints[i]);
		}
		loop = 0;
		speed = rate;
	}
	sf::Vector2f moving() {
		if (currentPos == dpos) {
			pointSet.push_back(dpos);
			spos = dpos;
			dpos = pointSet.front();
			pointSet.pop_front();
			//std::cout <<"SPOS "<< spos.x << " " << spos.y << "\n";
			//std::cout <<"DPOS "<< dpos.x << " " << dpos.y << "\n";
			delta = dpos - spos;
		}
		else {
			//std::cout << delta.x << " " << delta.y << "\n";
			currentPos.x = currentPos.x + delta.x / speed;
			currentPos.y = currentPos.y + delta.y / speed;
		}
		rb->setPosition(currentPos);
		return sf::Vector2f(delta.x / speed, delta.y / speed);
	}
	sf::Vector2f getChange() {
		return sf::Vector2f(delta.x / speed, delta.y / speed);
	}

private:
	sf::Vector2f spos;
	sf::Vector2f dpos;
	sf::Vector2f delta;
	sf::Vector2f currentPos;
	std::list <sf::Vector2f> pointSet;
	int loop;
	int arsize;
	int speed;

};