#pragma once
#include<GameObject.h>
#include<Ammo.h>
#include<Event.h>

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
		offset = 0.f;
		direction = 1.f;
		enemyStatus = 0;
	}

	int getStatus() {
		return enemyStatus;
	}

	void setStatus(int st) {
		enemyStatus = st;
	}
	void changeDirection() {
		if (direction == 1.0)
			direction = -1.0f;
		else
			direction = 1.0f;
	}

	float getX() {
		return rb->getPosition().x;
	}

	float getY() {
		return rb->getPosition().y;
	}

	float getOffset() {
		return offset;
	}

	float getDirection() {
		return direction;
	}

	Event getEvent() {
		Event tst = pQueue.top();
		pQueue.pop();
		return tst;
	}

	int getQueueSize() {
		return pQueue.size();
	}


	void setPosition(float x, float y,float offSet) {
		offset = offSet;
		rb->setPosition(sf::Vector2f(x, y));
	}

	void setPointer(std::vector<Ammo*>* a,Player *tp) {
		am = a;
		p = tp;
	}

	int getID() {
		return enemyID;
	}

	void Shoot() {
		float arg[] = { rb->getPosition().x + 40.f,rb->getPosition().y };
		pQueue.push(Event(ENEMY_SHOOT, 2, 0, arg));
	}
	void checkHit() {
		for (Ammo* a : *am) {
			if (rb->getGlobalBounds().intersects(a->rb->getGlobalBounds())) {
				float arg[] = { a->getID() };
				pQueue.push(Event(AMMO_HIT_ENEMY, 1, 0, arg));
				float tst[] = { enemyID };
				pQueue.push(Event(ENEMY_DEAD, 1, 2, tst));
			}
		}
		if (rb->getGlobalBounds().intersects(p->rb->getGlobalBounds())) {
			float arg[] = { 0 };
			pQueue.push(Event(PLAYER_DEAD, 0, 0, arg));
		}
	}

	void onEvent(Event e) {
		if (e.eType == ENEMY_DEAD) {
			std::cout << "ENEMY KILLED";
			enemyStatus = 1;
		}
	}
private:
	int enemyID;
	int enemyStatus;
	float offset;
	float direction;

	std::vector<Ammo*>* am;
	Player* p;
	std::priority_queue<Event, vector<Event>, greater<vector<Event>::value_type>> pQueue;

};
