#pragma once

#include <vector>

#include <MyActor.h>

#include "TileColRenderer.h"

#include "Values.h"
#include "DibRoom.h"

class Monster;
class RandomMap;
class TileColRenderer;
class Player;


class DibTile :	public MyActor
{
	friend RandomMap;
	friend Player;
	friend Monster;

	class TileData
	{
	public:
		TileIndex mIndex;
		MyVector2 mPos;
		MyVector2 mSize;
		int mBF;
	};

	class WallData
	{
	public:
		TileIndex mIndex;
		MyVector2 mPos;
		MyVector2 mSize;
		int mBF;
	};

	class BigFogData
	{
	public:
		TileIndex mIndex;
		MyVector2 mPos;
		MyVector2 mSize;
		int mBF;
	};

	/*class TileRenderer : public MyRenderer
	{
	public:

	private:

	};

	class WallRenderer : public MyRenderer
	{

	};

	class BigFogRenderer : public MyRenderer
	{

	};*/

private:
	DibTile();
	DibTile(const DibTile& other);
	DibTile(RandomMap* map, int x, int y, LEAGUE_ORDER order);
	~DibTile();

	// void Init();

	void Update() override;
	// void FindNeighbors();

	TILE_TYPE TileType() { return mTileType; }
	void TileType(TILE_TYPE type)
	{
		mTileType = type;
	}
	WALL_TYPE WallType()
	{ 
		return mWallType; 
	}

	void WallType(WALL_TYPE type)
	{
		mIsWall = true;
		mTileType = TT_WALL;
		mWallType = type;
	}

	void FloorType(WALL_TYPE type)
	{
		mIsWall = false;
		mTileType = TT_FLOOR;
		mWallType = type;
	}

	/*CORNER_TYPE CornerType() { return mCornerType; }
	void CornerType(CORNER_TYPE type)
	{
		mIsWall = true;
		mTileType = TT_CORNER;
		mCornerType = type;
	}*/
	
	void SetRenderOrder(LEAGUE_ORDER order) { mRenderOrder = order; }

	void SetFloor();
	void SetUpstair();
	void SetDownstair();
	void SetRoom(DibRoom* room) { mRoom = room; }
	DibRoom* GetRoom() { return mRoom; }

	bool IsWall() { return mIsWall; }
	bool IsUpstair() { return mIsUpstair; }
	bool IsDownstair() { return mIsDownstair; }
	bool IsRoom();

	void DebugSwitch();


	void DebugRender();
	void DebugOutputTile();
	void DebugSetPosition();
	void DebugSetTypeName();



	// void Update() override;

	void LastCheck1();
	void LastCheck2();

private:
	// MyRenderer* mTileRenderer;
	MyRenderer* mWallRenderer;
	MyRenderer* mFogRenderer;

	MyRenderer* mDebugRenderer;

	RandomMap* mMap;
	DibRoom* mRoom;

	TILE_TYPE mTileType;
	WALL_TYPE mWallType;
	// CORNER_TYPE mCornerType;

	bool mIsT = false;

	bool mIsWall;
	bool mIsUpstair;
	bool mIsDownstair;

	int mX;
	int mY;

	static int mUpstairCount;
	static int mDownstairCount;

	float mMoveSpeed;

	///////////////////// Debug
	bool mIsDebug = false;
	const wchar_t* mAllTileType[WT_MAX];

	// Sub Tiles
	std::vector<TileColRenderer*> mSubTiles;

	// 모든 타일이 플레이어를 
	Player* mPlayer;

	LEAGUE_ORDER mRenderOrder;

	std::vector<BigFogData*> mAllBigFog;
	std::vector<WallData*> mAllWall;
	std::vector<TileData*> mAllTile;

	bool mIsInRoom;

	int mIncrease;
	int mBase;

	template <typename F>
	F* Init(int x, int y)
	{
		F* temp = new F();

		temp->mIndex.X = x;
		temp->mIndex.Y = y;

		float offsetX = (TILE_OFFSETX) / 2.f;
		float offsetY = offsetX / 2.f;

		float startX = MAPSIZE_X / 2.f - offsetX - (y * offsetX);
		float startY = y * offsetY;

		float newX = (startX + x * offsetX);
		float newY = startY + (x * offsetY);

		temp->mPos = MyVector2(newX, newY);

		temp->mSize = { TILE_WIDTH, TILE_HEIGHT };

		return temp;
	}
};

