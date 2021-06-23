#pragma once
#include<queue>
#include<Event.h>
#include<Player.h>
#include<Timeline.h>
#include<fstream>
#include<streambuf>
#include<Game.h>
#include <dukglue/dukglue.h>
#include <ScriptManager.h>
class EventManager {
public:
	EventManager() {

	}
	EventManager(ScriptManager* sc1,Game *gc1, Player *p,std::vector<Enemy*> *e,std::vector<Ammo*> *plam,std::vector<Ammo*> *enam,Timeline *tgameTime ) {
		player = p;
		en = e;
		eventTime = tgameTime;
		gc = gc1;
		sc = sc1;
		plShots = plam;
		enShots = enam;
	}

	void enterQueue(Event e) {
		eventQueue.push(e);
	}

	void eventDispatch() {
		retrieveEvents();
		if (eventQueue.size() != 0) {
			Event event = eventQueue.top();
			while (eventQueue.size() != 0 && event.timeStamp < eventTime->getTime()) {
				eventQueue.pop();
				if (event.eType == USER_INPUT||event.eType == PLAYER_DEAD || event.eType == PLAYER_WIN) {
					player->onEvent(event);
				}
				else if (event.eType == PLAYER_SHOOT||event.eType == ENEMY_SHOOT||event.eType == GAME_START) {
					gc->onEvent(event);
				}
				else if (event.eType == ENEMY_DEAD) {
					Enemy* e = en->at(event.argList[0]);
					e->onEvent(event);
					gc->onEvent(event);
				}
				else if (event.eType == AMMO_HIT_ENEMY) {
					for (Ammo* a : *plShots) {
						if (a->getID() == event.argList[0]) {
							a->onEvent(event);
							break;
						}
					}
				}
				else if (event.eType == AMMO_HIT_PLAYER) {
					for (Ammo* a : *enShots) {
						if (a->getID() == event.argList[0]) {
							a->onEvent(event);
							break;
						}
					}
				}
				else {
					std::cout << "STRANGE " << event.argList[0];
				}
				retrieveEvents();
				if (eventQueue.size() != 0) {
					event = eventQueue.top();
				}
				
			}
			
		}
	}

	void retrieveEvents() {
		while (player->getQueueSize() != 0) {
			Event tmp = player->getEvent();
			this->enterQueue(tmp);
		}
		for (Enemy* e : *en) {
			if (e->getStatus() == 0) {
				while (e->getQueueSize() != 0) {
					Event tmp = e->getEvent();
					this->enterQueue(tmp);
				}
			}
		}
		//for (Player* p1 : *players) {
		//	if (p1 != 0) {
		//		while (p1->getQueueSize() != 0) {
		//			Event tmp = p1->getEvent();
		//			
		//			//tmp.timeStamp = tmp.timeStamp + eventTime->getTime();
		//			this->enterQueue(tmp);
		//		}
		//	}
		//}
		/*for (MovPlatform* mp1 : *mpf) {
			while (mp1->getQueueSize() != 0) {
				Event tmp = mp1->getEvent();
				tmp.timeStamp = tmp.timeStamp + eventTime->getTime();
				this->enterQueue(tmp);
			}
		}*/
	}

	

private:
	std::priority_queue<Event, vector<Event>, greater<vector<Event>::value_type>> eventQueue;
	Player* player;
	std::vector<Enemy*>* en;
	std::vector<Ammo*>* plShots;
	std::vector<Ammo*>* enShots;
	Timeline* eventTime;
	Game* gc;
	ScriptManager* sc;
};