#include "MapTester.h"

#include <MyInput.h>

#include "RandomMap.h"


MapTester::MapTester()
	: mMapIndex(0)
{
}


MapTester::~MapTester()
{
	// mAllMap.clear(); 
}

void MapTester::Update()
{
	if (MyInput::Down(L"MakeARoom"))
	{
		++mMapIndex;
	}

	if (true == mAllMap.empty())
	{
		return;
	}

	mAllMap[mMapIndex].Update();
}

void MapTester::DebugRender()
{
}

void MapTester::SetRandomMap(RandomMap * map)
{
	mRandomMap = map;
}

void MapTester::MakeAMap()
{
	
}
