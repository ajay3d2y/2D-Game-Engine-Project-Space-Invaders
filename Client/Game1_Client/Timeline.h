#pragma once
#include<chrono>
#include<math.h>

using namespace std::chrono;

class Timeline {
public:
	Timeline() {
		start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		startTime = duration_cast<std::chrono::milliseconds>(start.time_since_epoch());
		tick = 1;
		iniTick = 1;
		pause = 0;
		anchor = nullptr;
	}

	Timeline(float rep) {
		start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		startTime = duration_cast<std::chrono::milliseconds>(start.time_since_epoch());
		tick = rep;
		iniTick = rep;
		pause = 0;
		anchor = nullptr;
	}

	Timeline(float rep, Timeline base) {
		sTime = base.getTime();
		tick = rep;
		iniTick = tick;
		pause = 0;
		*anchor = base;
	}

	float getTick() {
		return iniTick;
	}

	long getTime() {
		if (anchor == nullptr) {
			time_point<std::chrono::system_clock> current = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			milliseconds currentTime = duration_cast<std::chrono::milliseconds>(current.time_since_epoch());
			if (pause == 0) {
				return (currentTime.count() - startTime.count()) / tick;
			}
			else {
				return (pauseTime.count() - startTime.count()) / tick;
			}
		}
		else {
			long currentTime = anchor->getTime();
			if (pause == 0) {
				return ((currentTime - sTime) / tick);
			}
			else {
				return ((pTime - sTime) / tick);
			}
		}

	}

	void setTick(float rep) {
		time_point<std::chrono::system_clock> current = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		milliseconds currentTime = duration_cast<std::chrono::milliseconds>(current.time_since_epoch());
		milliseconds delta = (currentTime - startTime) / (int)std::round(tick);
		milliseconds newdelta = delta * (int)std::round(rep);
		startTime = currentTime - newdelta;
		tick = rep;
	}

	void scaleTick(float scale) {
		tick = iniTick * scale;
	}

	void restart() {
		tick = iniTick;
		if (anchor == nullptr) {
			start = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			startTime = duration_cast<std::chrono::milliseconds>(start.time_since_epoch());
		}
		else {
			sTime = anchor->getTime();
		}
	}

	void switchPause() {
		if (anchor == nullptr) {
			if (pause == 0) {
				pause = 1;
				pauseTimePoint = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				pauseTime = duration_cast<std::chrono::milliseconds>(pauseTimePoint.time_since_epoch());

			}
			else {
				pause = 0;
				time_point<std::chrono::system_clock> current = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
				pauseDuration = duration_cast<std::chrono::milliseconds>(current.time_since_epoch() - pauseTimePoint.time_since_epoch());
				startTime = startTime + pauseDuration;

			}

		}
		else {
			if (pause == 0) {
				pause = 1;
				pTime = anchor->getTime();

			}
			else {
				pause = 0;
				long currentTime = anchor->getTime();
				pDur = currentTime - pTime;
				sTime = sTime + pTime;

			}
		}

	}

	long getLastPause() {
		return (pauseDuration.count() / tick);
	}

private:
	time_point<std::chrono::system_clock> start;
	time_point<std::chrono::system_clock> pauseTimePoint;
	Timeline* anchor;
	long sTime;
	long pTime;
	long pDur;
	milliseconds startTime;
	milliseconds pauseTime;
	milliseconds pauseDuration;
	float tick;
	float iniTick;
	int pause;
};