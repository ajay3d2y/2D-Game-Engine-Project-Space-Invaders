#include<Player.h>


Player::Player() {

}

Player::Player(int id,float side, std::string path) {
	rb = new RigidBody('S', side);
	tb = new TextureObject(rb, path);
	playerId = id;
	playerStatus = 0;
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

void Player::onEvent(Event event) {
	
}

//void Player::setPointers(std::vector<Platform*>* tpf, std::vector<MovPlatform*>* tmpf, std::vector<DeathZone*>* tdz,Timeline* gT) {
//	pf = tpf;
//	mpf = tmpf;
//	dz = tdz;
//	gameTime = gT;
//}