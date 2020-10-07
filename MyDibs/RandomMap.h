#pragma once

#include <vector>

#include <MyMath.h>

#include "Values.h"

class DibTile;
class DibRoom;

class MapTester;
class TileColRenderer;
class ColliderMap;
class Monster;
class Player;

class MyActor;
class MyTexture;
class MyScene;
// MapGenerator로 생성된 Map이다.


class RandomMap
{
	friend ColliderMap;
	friend DibTile;
	friend Monster;
	friend Player;
public:
	RandomMap();
	RandomMap(MyScene* scene);
	RandomMap(const RandomMap& other);
	~RandomMap();

	void On();
	void Off();

	void SetPlayer(Player* player);
	Player* GetPlayer() const;

	DibTile* GetTile(int _X, int _Y);
	DibTile* GetTile(MyVector2 pos);

	////////////////////////////////////////////// 맵 생성 관련 /////////////////////////////////////////////////////
	void Make3RoomsY(int level);

	bool MakeARoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);

	void MakeTopEleven(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize);
	void MakeLeftEleven(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize);

	void MakeArchY(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize);
	void MakeArchX(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize);

	void MakeRandomRooms(MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType, int left, int bottom, int right, int top);
	void MakeRandomBRooms(MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType, int left, int bottom, int right, int top);
	bool MakeButchersPreRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);
	bool MakeButchersRealRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);
	
	bool SetUpstair();
	void SetDownstair();

	void MakeUpstair(MyVector2& basePos);
	void MakeDownstair(MyVector2& basePos);

	bool MakeADoor(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);
	bool MakeAConnection(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType);

	

	void MakeFloor(MyVector2 basePos, MyVector2 size);
	void MakeBFloor(MyVector2 basePos, MyVector2 size);

	void MakeWideFloor(MyVector2 basePos, MyVector2 size);

	void ResetCount();




	///////////////////////////////////////////// 타일 변환 관련 /////////////////////////////////////////////
	bool MakeIronWall(MyVector2 basePos, MyVector2 size, ROOM_TYPE type);
	bool UpgradeIronWall();

	void UpgradeWallStyle();

	void UpgradeRoomCorner(MyVector2 basePos, MyVector2 size);

	void UpgradeAdjCorner(int x, int y, WALL_TYPE type);
	void UpgradeAllAdjCorner();

	void SetFloorAt(int x, int y);
	void SetFloorAt(MyVector2 pos);
	// void SetFloorAt(MyVector2 pos, wchar_t special);

	void SetWallAt(int x, int y, WALL_TYPE type);
	void SetWallAt(MyVector2 pos, WALL_TYPE type);

	void SetDoorAt(MyVector2 pos, WALL_TYPE doorType);

	void SetCornerAt(int x, int y, WALL_TYPE type);
	void SetCornerAt(MyVector2 pos, WALL_TYPE type);
	void SetNormalCornerAt(MyVector2 pos, WALL_TYPE type);

	void SetColumnAt(int x, int y);

	void SetWallAroundAt(int x, int y);
	void SetBWallAroundAt(int x, int y);
	void SetWallAroundAt(MyVector2 pos);
	// void SetWallAroundAt(MyVector2 pos, wchar_t specialFloor, wchar_t specialWall);

	void SetAllShadow();

	bool CheckIsRoom(MyVector2& roomPos, MyVector2& roomSize);
	MyVector2 CheckNorthWall(MyVector2& roomPos, MyVector2& roomSize);
	bool CheckIsTile(int x, int y, WALL_TYPE type);
	bool CheckIsTile(int x, int y, TILE_TYPE type);

	bool IsFloor(int x, int y);
	bool IsFloor(MyVector2 pos);
	bool IsWall(int x, int y);
	bool IsWall(MyVector2 pos);
	bool IsRoom(int x, int y);
	bool IsRoom(MyVector2 pos);
	bool IsUpstair(MyVector2 pos);
	bool IsDownstair(MyVector2 pos);

	bool HasDownstair() { return mHasDownstair; }

	RandomMap& CopyMap(RandomMap* map);


	void Update();

	///////////////////////////////////////////////////////////////////////////// 타일 스타일 세팅
	WALL_TYPE StyleFloor();
	WALL_TYPE StyleLeftWall();
	WALL_TYPE StyleTopWall();

	void PlaceBox(int x, int y, WALL_TYPE tile);
	// void PlaceCandle(int x, int y, WALL_TYPE candle);

	void SetIronWallX(MyVector2 basePos, int size);
	void SetIronWallY(MyVector2 basePos, int size);

	void SetColliderMap();

	bool FinishMap();

	ColliderMap* ColMap() { return mColliderMap; }

	void AddLeft(int x, int y);
	void AddRight(int x, int y);
	void AddAll(int x, int y);
	void AddLT(int x, int y);
	void AddT(int x, int y);
	void AddNone(int x, int y);
	MyVector2 SubToDib(int x, int y);
	void AddSubTile(TileColRenderer* subTile);

	std::vector<DibRoom*> GetAllRoom() { return mAllRoom; }

	TileIndex GetStartPos1() const { return mStartPos1; }
	TileIndex GetStartPos2() const { return mStartPos2; }
	TileIndex GetDownPos() const { return mDownPos; }
	TileIndex GetBPos() const { return mBPos; }
	bool GetIsChanging() const {	return mIsChanging;	}

	void RenderTile();

	////////////////////////////// FINISHING
	void SetBRoom();
	bool HasBRoom() const { return mHasBRoom; }
	void SetStartRoom();
private:
	void SetImageIndex();
	void SetCopyImageIndex();
	void CopyAll();



	// static void MakeBack();

private:
	// static std::vector<RandomMap*> mAllMap;

	DibTile*** mAllTile;
	DibTile*** mAllCopyTile;

	DibRoom* mLastRoom;
	DibRoom* mStartRoom = nullptr;
	std::vector<DibRoom*> mAllRoom;
	std::vector<DibTile*> mAllWall;
	std::vector<DibTile*> mCorners;

	static int mLevel;

	int mLeftLimit = 3;
	int mBottomLimit = 2;
	int mRightLimit = 0;
	int mTopLimit = 0;

	int mLeftCount = 0;
	int mBottomCount = 0;
	int mRightCount = 0;
	int mTopCount = 0;

	bool mHasUpstair;
	bool mHasDownstair;

	ColliderMap* mColliderMap;

	Player* mPlayer;

	MyActor* mBackTile;
	MyTexture* mBackTexture;

	TileIndex mStartPos1;
	TileIndex mStartPos2;

	TileIndex mDownPos;
	TileIndex mBPos;


	MyScene* mCurScene;

	bool mIsChanging = false;
	bool mHasBRoom = false;
};

