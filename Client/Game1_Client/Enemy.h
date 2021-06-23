#pragma once
#include<GameObject.h>
class Enemy : public GameObject {
public:
	Enemy() {
	}
	Enemy(int id, float side, std::string path) {
		rb = new RigidBody('S', side);
		tb = new TextureObject(rb, path);
		enemyID = id;
	}
	Enemy(int id, float side, std::string path,sf::Vector2f spos) {
		rb = new RigidBody('S', side);
		tb = new TextureObject(rb, path);
		enemyID = id;
		rb->setPosition(spos);
		enemyStatus = 0;
	}

	int getStatus() {
		return enemyStatus;
	}

	void setStatus(int st) {
		enemyStatus = st;
	}

private:
	int enemyID;
	int enemyStatus;
};
