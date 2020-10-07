#pragma once

#include <vector>

#include <MyMath.h>
#include <MyActor.h>

#include "Values.h"

class Map;
class RandomMap;
class PathFinding;

class Tile : public MyActor
{
	friend PathFinding;


public:
	Tile();
	Tile(RandomMap* map, MyVector2 pos);
	Tile(MyVector2 pos, wchar_t basePixel, Map* map);
	~Tile();

	void Init(MyVector2 pos, wchar_t basePixel, Map* map);

	void SetWall();
	void SetWall(wchar_t pixel);
	void SetFloor();
	void SetFloor(wchar_t special);
	void SetLeftDoor();
	void SetTopDoor();
	void SetRightDoor();
	void SetBottomDoor();
	void SetUpstair();
	void SetDownstair();

	bool GetOnUpstair() const { return mIsUpstair; }
	bool GetOnDownstair() const { return mIsDownstair; }

	void SetCorner(TILE_TYPE cornerType);

	void SetUnit(MyActor* obj);
	void SetItem(MyActor* obj);
	void UnsetItem(); 
	void UnsetUnit();

	bool IsWalkable() const { return mIsWalkable; }
	bool IsUpstair() const { return mIsUpstair; }
	bool IsDownstair() const { return mIsDownstair; }
	
	TILE_TYPE GetTileType() const { return mTileType; }

	static bool IsWall(MyVector2 pos);
	static bool IsUnitOn(MyVector2 pos);

	bool IsFloor() const { return mIsWalkable; }
	bool IsWall() const { return !mIsWalkable; }
	bool IsUnitOn();

	float GetF() const { return mF; }
	void FindNeighbor();
	void AddNeighbor(Tile* tile) { mNeighbors.push_back(tile); }

	void ClearTile();

	float GetTileCost() const;

	MyActor* GetUnit() const { return mUnit; }
protected:
private:

public:
	static int WallCount;

protected:
private:

	std::vector<Tile*> mNeighbors;

	bool mIsWalkable = false;
	Map* mMap = nullptr;
	
	TILE_TYPE mTileType = TT_INWALL;
	
	MyActor* mUnit = nullptr;
	MyActor* mItem = nullptr;
	
	wchar_t mPrevPixel;
	bool mIsUpstair = false;
	bool mIsDownstair = false;
	bool mIsTarget = false;

	float mF = 0;
	float mG = 0;
	float mH = 0;

	Tile* mParentTile = nullptr;
};

