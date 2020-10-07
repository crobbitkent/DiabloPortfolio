#pragma once

#include "Values.h"


class Player;
class Map;

class MapGenerator
{
public:
	static Map* GenerateMap(int x, int y, Player* player, int level);
	static Map* GetMap() { return mMap; }

	static void SetM1(Map* map);

	static void MakeCathedralLevel();
	static void MakeButcherLevel();


	static void Make3RoomsY(int level);
	static void Make3RoomsX(int level);

	
	static bool MakeARoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);
	static bool MakeButchersPreRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);
	static bool MakeButchersRealRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);

	static bool MakeADoor(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);
	//static void MakeADoor(MyVector2 basePos, MyVector2 size);
	
	static void MakeAConnection(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);

	static void MakeUpstair(MyVector2 basePos);
	static void MakeDownstair(MyVector2 basePos);

	static void MakeRooms(MyVector2 basePos, MyVector2 size, ROOM_TYPE roomType, int left, int bottom, int right, int top);
	static void MakeButchersRooms(MyVector2 basePos, MyVector2 size, ROOM_TYPE roomType, int left, int bottom, int right, int top);

	static void MakeFloor(MyVector2 basePos, MyVector2 size);
	static void MakeFloor(MyVector2 basePos, MyVector2 size, wchar_t special);
	static void MakeEleven(MyVector2 basePos, MyVector2 size);
	static void MakeEqual(MyVector2 basePos, MyVector2 size);
		
	static void NewUpgradeCorner(MyVector2 basePos, MyVector2 size);

	static void MakeRandomMap(int x, int y, Player* player);

private:
	MapGenerator();
	~MapGenerator();
	static void ResetCount() 
	{
		mLeftCount = 0;
		mBottomCount = 0;
		mRightCount = 0;
		mTopCount = 0;
	}

	static void ResetStairs()
	{ 
		mIsDownstair = false;
		mIsUpstair = false;
	}
public:
protected:
private:
	static Map* mMap;
	static int mSeed;

	static int mLeftCount;
	static int mBottomCount;
	static int mRightCount;
	static int mTopCount;

	static int mLevelCount;

	static int mLeftLimit;
	static int mBottomLimit;
	static int mRightLimit;
	static int mTopLimit;

	static bool mIsDownstair;
	static bool mIsUpstair;

	static bool IsDebugging;

	
};

