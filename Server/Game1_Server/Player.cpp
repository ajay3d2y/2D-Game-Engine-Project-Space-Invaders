#include<Player.h>
#include<EventManager.h>


Player::Player() {

}

Player::Player(int id,float side, std::string path) {
	rb = new RigidBody('S', side);
	tb = new TextureObject(rb, path);
	playerId = id;
	playerStatus = -1;
}

Event Player::getEvent() {
	Event tst = pQueue.top();
	pQueue.pop();
	return tst;
}

int Player::getQueueSize() {
	return pQueue.size();
}

int Player::getPlayerStatus() {
	return playerStatus;
}

void Player::setPlayerStatus(int st) {
	playerStatus = st;
}

void Player::setPointer(std::vector<Ammo*>* a) {
	am = a;
}

void Player::checkHit() {
	for (Ammo* a : *am) {
		if (rb->getGlobalBounds().intersects(a->rb->getGlobalBounds())) {
			float arg[] = { a->getID() };
			pQueue.push(Event(AMMO_HIT_PLAYER, 1, 0, arg));
			float arg1[] = { 0 };
			pQueue.push(Event(PLAYER_DEAD, 0, 0, arg1));
		}
	}
}

void Player::onEvent(Event event) {
	if (event.eType == USER_INPUT) {
		if (event.argList[0] == 0) {
			rb->move(-4.f, 0.f);
			if (rb->getPosition().x <= 0.f) {
				rb->move(4.f, 0.f);
			}
		}
		else if (event.argList[0] == 1) {
			rb->move(4.f, 0.f);
			if (rb->getPosition().x >= 934.f) {
				rb->move(-4.f, 0.f);
			}
		}
		else if(event.argList[0]==2){
			float arg[] = { rb->getPosition().x + 40.f,rb->getPosition().y };
			pQueue.push(Event(PLAYER_SHOOT, 2, 0, arg));
		}
	}
	else if (event.eType == PLAYER_DEAD) {
		playerStatus = 1;
	}
	else if (event.eType == PLAYER_WIN) {
		playerStatus = 2;
	}
}

//void Player::setPointers(std::vector<Platform*>* tpf, std::vector<MovPlatform*>* tmpf, std::vector<DeathZone*>* tdz,Timeline* gT) {
//	pf = tpf;
//	mpf = tmpf;
//	dz = tdz;
//	gameTime = gT;
//}