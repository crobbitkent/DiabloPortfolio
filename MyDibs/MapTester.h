#pragma once

#include <vector>

#include <MyActor.h>

class RandomMap;

class MapTester : public MyActor
{
	friend RandomMap;

	
public:
	MapTester();
	~MapTester();

	void Update() override;
	void DebugRender() override;


	void SetRandomMap(RandomMap* map);
	void MakeAMap();

private:
	RandomMap* mRandomMap;
	int mMapIndex;
	std::vector<RandomMap> mAllMap;
};

