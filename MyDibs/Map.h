#pragma once

#include <vector>

#include <MyMath.h>


#include "Values.h"

class Actor;
class Tile;
class Room;
class Monster;

// 모든 타일을 저장
// 모든 방을 저장
// 모든 벽을 저장
// (타일 = 벽)이기 때문에 타일만 지운다.
// 벽을 지운다.
class Map
{
public:
	Map(MyVector2 MyVector2, int level);
	~Map();

	Tile& GetTile(int _X, int _Y);
	Tile& GetTile(MyVector2 pos);
	void AddRoom(Room* room) { mAllRoom.push_back(room); }
	bool CheckIsRoom(Room* room);
	bool CheckIsRoom(MyVector2 roomPos, MyVector2 roomMyVector2);
	MyVector2 CheckNorthWall(MyVector2 roomPos, MyVector2 roomMyVector2);
	void SetWallAt(int x, int y);
	void SetWallAt(MyVector2 pos);
	void SetWallAt(MyVector2 pos, wchar_t pixel);
	void SetWallAroundAt(int x, int y);
	void SetWallAroundAt(MyVector2 pos);
	void SetWallAroundAt(MyVector2 pos, wchar_t specialFloor, wchar_t specialWall);
	void SetFloorAt(int x, int y);
	void SetFloorAt(MyVector2 pos);
	void SetFloorAt(MyVector2 pos, wchar_t special);
	void SetDoorAt(MyVector2 pos, TILE_TYPE doorType);
	void SetCornerAt(MyVector2 pos, TILE_TYPE cornerType);

	void SetUpstairAt(MyVector2 pos);
	void SetDownstairAt(MyVector2 pos);

	bool IsFloor(MyVector2 pos);
	bool IsUpstair(MyVector2 pos);
	bool IsDownstair(MyVector2 pos);

	// tile
	// void AddWall(Tile* wall);
	bool IsWall(MyVector2 _Pos);

	const std::vector<Room*> GetAllRoom() { return mAllRoom; }

	void SetHasUpstair() { mHasUpstair = true; }
	void SetHasDownstair() { mHasDownstair = true; }

	bool GetUpstair() const { return mHasUpstair; }
	bool GetDownstair() const { return mHasDownstair; }
	
	void UpgradeWall();
	void UpgradeCorner();

	TILE_TYPE GetTileType(MyVector2 pos);

	int GetMapLevel() const { return mLevel; }
	void SetMapLevel(int level) { mLevel = level; }

	void AddMap(Map* map);

	// 유닛 관련
	MyVector2 GetStartPos() const { return mStartPos; }
	MyVector2 GetDownstairPos() const { return mDownstairPos; }

	// 스태틱 시리즈
	// static void SetUnitAt(MyVector2 pos, MovableObject* obj);
	// static void UnsetUnitAt(MyVector2 pos);
	// static void SetItemAt(MyVector2 pos, StaticObject* obj);

	// static void ClearMaps();

	void ClearTiles();

	void AddWall(Tile* wall);
	void AddMonster(Monster* monster);
private:
	friend class MapGenerator;

	Tile** mAllTile;

	std::vector<Room*> mAllRoom;
	std::vector<Tile*> mAllWall;
	std::vector<Tile*> mCorners;
	std::vector<Monster*> mMonsters;
	

	bool mHasUpstair = false;
	bool mHasDownstair = false;

	MyVector2 mStartPos = { 0,0 };
	MyVector2 mDownstairPos = { 0,0 };

	int mLevel = 1;
};

