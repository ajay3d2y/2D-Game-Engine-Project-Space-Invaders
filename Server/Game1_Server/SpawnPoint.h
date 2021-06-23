#pragma once
#include<GameObject.h>
class SpawnPoint : public GameObject {
public:
	SpawnPoint() {
		
	}

	SpawnPoint(float xPos,float yPos) {
		x = xPos;
		y = yPos;
	}
};