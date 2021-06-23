#pragma once
#include<vector>
#include<string>
#include<sstream>

using namespace std;
enum  Type {
	USER_INPUT,
	PLAYER_SHOOT,
	AMMO_HIT_ENEMY,
	AMMO_HIT_PLAYER,
	ENEMY_DEAD,
	ENEMY_SHOOT,
	PLAYER_DEAD,
	PLAYER_WIN,
	GAME_START,
	GAME_EXIT
};

struct Event {

	Event(Type test, int count, long time, float args[]) {
		eType = test;
		argCount = count;
		for (int i = 0; i < argCount; i++) {
			argList.push_back(args[i]);
		}
		timeStamp = time;
	}

	Event(std::string eventString, long time) {
		istringstream test(eventString);
		std::string tmp;
		test >> tmp;
		eType = static_cast<Type>(stoi(tmp));
		test >> tmp;
		argCount = stoi(tmp);
		if (eType == GAME_EXIT) {
			timeStamp = 0;
		}
		else {
			timeStamp = time;
		}

		for (int i = 0; i < argCount; i++) {
			test >> tmp;
			argList.push_back(stof(tmp));
		}
	}
	std::string eventToString() {
		string tmp = to_string((int)eType) + " " + to_string(argCount) + " ";
		for (int i = 0; i < argCount; i++) {
			tmp = tmp + to_string(argList[i]) + " ";
		}
		return tmp;
	}


	friend bool operator < (const Event& lhs, const Event& rhs) {
		return lhs.timeStamp < rhs.timeStamp;
	}
	friend bool operator > (const Event& lhs, const Event& rhs) {
		return lhs.timeStamp > rhs.timeStamp;
	}

	Type eType;
	int argCount;
	long timeStamp;
	std::vector<float> argList;
};