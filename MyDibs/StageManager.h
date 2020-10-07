#pragma once

#include "Values.h"

class Map;
class Monster;

class StageManager
{
public:
	// static Map* GetCurrentMap() { return mAllMap[currentIndex]; }
	static void ClearMaps();
	static void AddMap(Map * map);
	static Map* GoToNextLevel();
	static Map* GoToPrevLevel();

private:
	StageManager();
	~StageManager();

private:
	// static std::vector<Map*> mAllMap;
	// static int currentIndex;
};

