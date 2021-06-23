#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<RigidBody.h>
class TextureObject {
public:
	TextureObject(RigidBody* b, std::string path) {
		if (!texture.loadFromFile(path)) {
			if (path.compare("red")==0) {
				col = sf::Color(253, 49, 49);
				b->setFillColor(col);
			}
			else if (path.compare("blue")==0) {
				col = sf::Color(44, 161, 229);
				b->setFillColor(col);
			}
		}
		else {
			b->setTexture(&texture);
		}
	}
private:
	sf::Texture texture;
	sf::Color col;
};