#include "RandomMap.h"

#include "Values.h"

#include <RandomNumber.h>

#include "DibTile.h"
#include "DibRoom.h"
#include "MapTester.h"
#include "ColliderMap.h"
#include "TileColRenderer.h"
#include "Player.h"

#include <MyTexture.h>
#include <MyActor.h>
#include <MyScene.h>
#include <MyImage.h>

#include <MyTextureManager.h>

// std::vector<RandomMap*> RandomMap::mAllMap;

int RandomMap::mLevel = 0;

// 지도를 만들때 반드시 층을 적어주어야 한다.
RandomMap::RandomMap()
{
	mLevel = -1;


	// mAllMap.push_back(this);
}

// 지도를 만들때 반드시 층을 적어주어야 한다.
RandomMap::RandomMap(MyScene* scene)
{
	mHasUpstair = false;
	mHasDownstair = false;

	mLastRoom = new DibRoom();

	mAllTile = new DibTile**[MAPCOUNT_Y];
	mAllCopyTile = new DibTile**[MAPCOUNT_Y];


	mColliderMap = new ColliderMap(this);

	//// 타일 생성
	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		mAllTile[i] = new DibTile*[MAPCOUNT_X]();

		for (int j = 0; j < MAPCOUNT_X; j++)
		{ 
			mAllTile[i][j] = new DibTile(this, j, i, LO_UPTILE);
		}
	}

	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		mAllCopyTile[i] = new DibTile*[MAPCOUNT_X]();

		for (int j = 0; j < MAPCOUNT_X; j++)
		{
			mAllCopyTile[i][j] = new DibTile(this, j, i, LO_DOWNTILE);
		}
	}

	mBackTile = new MyActor();

	// 랜덤맵 시드 설정
	RandomNumber::SetSeed(RandomNumber::TimeRandomUInt());



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// mBackTile->CreateRenderer


	if (nullptr == mBackTexture)
	{
		MyRenderer* temp = mBackTile->CreateRenderer<MyRenderer>(LO_TILE);
		temp->SetRenderStyle(RS_BIT);
		temp->SubSize(MyVector2(BACK_TILE_X, BACK_TILE_Y));
		temp->SubPos(MyVector2(0.f - 18.f , BACK_TILE_Y / 2.f - 166));

		wchar_t atmax[4];

		++mLevel;
		_itow_s(mLevel, atmax, 4);

		std::wstring tName = atmax;
		tName.append(L"Tile");

		mBackTexture = MyTextureManager::Inst().CreateTexture(tName.c_str(), BACK_TILE_X, BACK_TILE_Y);

		MyTextureManager::Inst().CreateImage(tName.c_str(), 1, 1);

		temp->Image(tName.c_str());
	}
}

RandomMap::RandomMap(const RandomMap & other)
{
	mAllTile = new DibTile**[MAPCOUNT_Y];

	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		mAllTile[i] = new DibTile*[MAPCOUNT_X]();

		for (int j = 0; j < MAPCOUNT_X; j++)
		{
			mAllTile[i][j] = other.mAllTile[i][j];
		}
	}
}

RandomMap::~RandomMap()
{
	//// 모든 타일 지우기
	for (int y = 0; y < MAPCOUNT_Y; y++)
	{
		delete[] mAllTile[y];
		mAllTile[y] = nullptr;
	}

 	 delete[] mAllTile;
	 mAllTile = nullptr;

	 for (int y = 0; y < MAPCOUNT_Y; y++)
	 {
		 delete[] mAllCopyTile[y];
		 mAllCopyTile[y] = nullptr;
	 }

	 delete[] mAllCopyTile;
	 mAllCopyTile = nullptr;

	 delete mLastRoom;
	 mLastRoom = nullptr;

	 for (size_t i = 0; i < mAllRoom.size(); ++i)
	 {
		 delete mAllRoom[i];
	 }

	 mAllRoom.clear();
}

void RandomMap::Update()
{
	
}

void RandomMap::SetPlayer(Player * player)
{
	mPlayer = player;

	for (size_t y = 0; y < MAPCOUNT_Y; y++)
	{
		for (size_t x = 0; x < MAPCOUNT_X; x++)
		{
			mAllTile[y][x]->mPlayer = player;
			mAllCopyTile[y][x]->mPlayer = player;
		}
	}

}

Player * RandomMap::GetPlayer() const
{
	return mPlayer;
}

DibTile* RandomMap::GetTile(int _X, int _Y)
{
	if (true == OverCheck({ _X, _Y }))
	{
		return nullptr;
	}

	return mAllTile[_Y][_X];
}

DibTile* RandomMap::GetTile(MyVector2 pos)
{
	if (true == OverCheck(pos))
	{
		return nullptr;
	}

	return mAllTile[pos.IntY()][pos.IntX()];
}

void RandomMap::Make3RoomsY(int level)
{

}

bool RandomMap::MakeARoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	int X = basePos.IntX();
	int Y = basePos.IntY();

	MyVector2 TravellerBase = basePos + MyVector2::DOWNRIGHT;

	int y = 0;
	int x = 0;

	int tempY = 0;
	int tempX = 0;

	for (; y < size.IntY(); y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (MAPCOUNT_Y <= TravellerPos.IntY() + 1 ||
			0 > TravellerPos.IntY())
		{
			tempY = y - 1;
			break;
		}

		x = 0;

		for (; x < size.X; x++)
		{
			if (MAPCOUNT_X <= TravellerPos.IntX() + 1 ||
				0 > TravellerPos.IntX())
			{
				tempX = x - 1;
				break;
			}

			SetWallAroundAt(TravellerPos.IntX(), TravellerPos.IntY());

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MyVector2 newSize;

	if (0 != tempY && 0 != tempX)
	{
		newSize = { (float)tempX, (float)tempY };
	}
	else if (0 != tempY)
	{
		newSize = { (float)x, (float)tempY };
	}
	else if (0 != tempX)
	{
		newSize = { (float)tempX, (float)y };
	}
	else
	{
		newSize = { (float)x, (float)y };
	}

	// 새로운 사이즈가 0보다 작으면... 방이 생성되지 않음
	if (0 >= y || 0 >= x)
	{
		return false;
	}

	mLastRoom->Pos = basePos;
	mLastRoom->Size = newSize;

	// 방 등록
	DibRoom* newRoom = new DibRoom();

	newRoom->Pos = basePos;
	newRoom->Size = newSize;

	mAllRoom.push_back(newRoom);

	MyVector2 tempSize = { (float)x, (float)y };

	// 여기서 문제가 발생
	UpgradeRoomCorner(basePos, tempSize);

	for (int y = newRoom->Pos.IntY(); y < newRoom->Pos.IntY() + newRoom->Size.IntY() + 1; y++)
	{
		for (int x = newRoom->Pos.IntX(); x < newRoom->Pos.IntX() + newRoom->Size.IntX() + 2; x++)
		{
			mAllTile[y][x]->mRoom = newRoom;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////// 문
	// 문 생성하기 (뚫림 1:2 문)
	int doorRandom = RandomNumber::GetRandomNumber(1, 10);

	// 	if (6 >= doorRandom)
	if (6 >= doorRandom)
	{
		// 문이 생성되지 않으면 아웃
		if (false == MakeADoor(basePos, tempSize, prevPos, prevSize, roomType))
		{
			// BOOM;
			return false;
		}
	}
	else
	{
		if (false == MakeAConnection(basePos, tempSize, prevPos, prevSize, roomType))
		{
			// BOOM;
			return false;
		}
	
	}

	return true;
}

void RandomMap::MakeTopEleven(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize)
{
	MakeARoom(basePos, size, { 0, 0 }, { 0, 0 }, RT_MAX);
	//MakeAConnection(basePos, size, prevPos, prevSize, RT_TOP);
	MakeAConnection(basePos, size, prevPos, prevSize, RT_BOTTOM);

	


	//MyVector2 TravellerBase = basePos;

	//for (int y = 0; y < size.Y; y++)
	//{
	//	MyVector2 TravellerPos = TravellerBase;
	//	for (int x = 0; x < size.X; x++)
	//	{
	//		if (0 == x || size.X - 1 == x)
	//		{
	//			SetWallAt(TravellerPos);
	//			TravellerPos += MyVector2::RIGHT;
	//			continue;
	//		}

	//		mMap->SetFloorAt(TravellerPos);
	//		// mMap->SetWallAroundAt(TravellerPos);

	//		TravellerPos += MyVector2::RIGHT;
	//	}

	//	TravellerBase = TravellerBase + MyVector2::DOWN;
	//}



	//Room* tempRoom = new Room(basePos, size - MyVector2{ 2, 2 });
	//mMap->AddRoom(tempRoom);
}

void RandomMap::MakeLeftEleven(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize)
{
}

void RandomMap::MakeArchY(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize)
{
	MyVector2 leftTop = basePos;
	MyVector2 leftDown = basePos + MyVector2(0.f, size.Y + 1);
	MyVector2 rightTop = basePos + MyVector2(size.X + 1, 0.f);
	MyVector2 rightDown = basePos + size + 1;
	
	// RIGHT DOWN
	mAllTile[rightDown.IntY()][rightDown.IntX()]->WallType(WT_ARCH_TOPWALL);
	mAllTile[rightDown.IntY()][rightDown.IntX() - 1]->WallType(WT_ARCH_TOP);
	mAllTile[rightDown.IntY()][rightDown.IntX() - 2]->WallType(WT_ARCH_LASTTOP);
	mAllTile[rightDown.IntY()][rightDown.IntX() - 3]->WallType(WT_LDSHADOW2);
	mAllTile[rightDown.IntY() - 1][rightDown.IntX() - 3]->WallType(WT_ARCH_LDSHADOW3);
	mAllTile[rightDown.IntY() - 2][rightDown.IntX() - 3]->WallType(WT_ARCH_LASTTOP_SHADOW);
	mAllTile[rightDown.IntY() - 3][rightDown.IntX() - 3]->WallType(WT_ARCH_LASTTOP_SHADOW);
	mAllTile[rightDown.IntY() - 4][rightDown.IntX() - 3]->WallType(WT_ARCH_LASTTOP_SHADOW2);
	mAllTile[rightDown.IntY() - 4][rightDown.IntX() - 3]->WallType(WT_ARCH_TOPSHADOW);


	mAllTile[rightDown.IntY() - 1][rightDown.IntX() - 1]->WallType(WT_ARCH_TOPSHADOW);
	mAllTile[rightDown.IntY() - 1][rightDown.IntX() - 2]->WallType(WT_ARCH_TOPSHADOWWALL);

	mAllTile[rightDown.IntY() - 2][rightDown.IntX() - 2]->WallType(WT_ARCH_LEFT);
	mAllTile[rightDown.IntY() - 3][rightDown.IntX() - 2]->WallType(WT_ARCH_LEFTTOP);
	mAllTile[rightDown.IntY() - 4][rightDown.IntX() - 2]->WallType(WT_ARCH_TOPSHADOW);

	mAllTile[rightDown.IntY() - 3][rightDown.IntX() - 1]->WallType(WT_ARCH_TOP);
	mAllTile[rightDown.IntY() - 4][rightDown.IntX() - 1]->WallType(WT_ARCH_TOPSHADOW);

	// LEFT
	mAllTile[leftDown.IntY() - 1][leftDown.IntX()]->WallType(WT_ARCH_LEFTWALL_SHADOW);
	mAllTile[leftDown.IntY()][leftDown.IntX()]->WallType(WT_ARCH_TOP);
	mAllTile[leftDown.IntY()][leftDown.IntX() + 1]->WallType(WT_ARCH_TOP);
	mAllTile[leftDown.IntY()][leftDown.IntX() + 2]->WallType(WT_RIGHTDOWNWALL);


	// 위로 올라가기 시작
	mAllTile[leftDown.IntY() - 1][leftDown.IntX() + 2]->WallType(WT_ARCH_LEFT);
	mAllTile[leftDown.IntY() - 2][leftDown.IntX() + 2]->WallType(WT_ARCH_LEFT);
	mAllTile[leftDown.IntY() - 3][leftDown.IntX() + 2]->WallType(WT_ARCH_LASTLEFT);
	// 위로 올라가기 시작의 그림자
	mAllTile[leftDown.IntY() - 1][leftDown.IntX() + 1]->WallType(WT_ARCH_RIGHTDOWNWALL_SHADOW);
	mAllTile[leftDown.IntY() - 2][leftDown.IntX() + 1]->WallType(WT_ARCH_LASTTOP_SHADOW);

	
	
	// 왼쪽으로 다시 꺾임
	mAllTile[leftDown.IntY() - 3][leftDown.IntX() + 1]->WallType(WT_ARCH_TOPWALL_SHADOW);
	mAllTile[leftDown.IntY() - 3][leftDown.IntX()]->WallType(WT_ARCH_RIGHTTOPWALL);

	// 왼다꺾 그림자
	mAllTile[leftDown.IntY() - 4][leftDown.IntX()]->WallType(WT_ARCH_TOPSHADOW);
	mAllTile[leftDown.IntY() - 4][leftDown.IntX() + 1]->WallType(WT_ARCH_TOPSHADOW);
}

void RandomMap::MakeArchX(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize)
{
}

void RandomMap::MakeRandomRooms(MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType, int left, int bottom, int right, int top)
{
	mLeftLimit = left;
	mBottomLimit = bottom;
	mRightLimit = right;
	mTopLimit = top;

	// 방크기 랜덤 생성
	int sizeX = RandomNumber::GetRandomNumber(2, 8);
	int sizeY = RandomNumber::GetRandomNumber(2, 8);

	int posX = -1;
	int posY = -1;

	switch (roomType)
	{
	case RT_LEFT:
		if (0 == prevPos.X || prevSize.X == 0 ||
			mLeftCount >= mLeftLimit)
		{
			return;
		}

		posY = (int)(prevPos.Y + (int)(prevSize.Y / 2) - (int)(sizeY / 2));
		posX = (int)(prevPos.X - sizeX - 1);
		break;
	case RT_BOTTOM:
		if (MAPCOUNT_Y <= prevPos.Y + prevSize.Y ||
			mBottomCount > mBottomLimit)
		{
			return;
		}

		posX = (int)(prevPos.X + (int)(prevSize.X / 2) - (int)(sizeX / 2));
		posY = (int)(prevPos.Y + prevSize.Y + 1);
		break;
	case RT_RIGHT:
		if (MAPCOUNT_X <= prevPos.X + prevSize.X ||
			mRightCount > mRightLimit)
		{
			return;
		}

		posY = (int)(prevPos.Y + (int)(prevSize.Y / 2) - (int)(sizeY / 2));
		posX = (int)(prevPos.X + prevSize.X + 1);

		break;
	case RT_TOP:
		if (0 == prevPos.Y || prevSize.Y == 0 ||
			mTopCount > mTopLimit)
		{
			return;
		}

		posX = (int)(prevPos.X + (int)(prevSize.X / 2) - (int)(sizeX / 2));
		posY = (int)(prevPos.Y - sizeY - 1);
		break;
	default:
		break;
	}

	// OverCheck
	if (0 > posY)
	{
		posY = 0;
	}
	if (0 > posX)
	{
		posX = 0;
	}
	if (MAPCOUNT_X <= posX)
	{
		posX = MAPCOUNT_X - 1;
	}
	if (MAPCOUNT_Y <= posY)
	{
		posY = MAPCOUNT_Y - 1;
	}


	MyVector2 pos = { posX, posY };
	MyVector2 size = { sizeX, sizeY };

	// true면 거기에 방이 있다.
	// 이미 방이 있으면 반환
	if (true == CheckIsRoom(pos , size) )
	{
		return;
	}

	// 현재 자리에 방 만들기
	// 실패하면 방만들기 끝
	if (false == MakeARoom(pos, size, prevPos, prevSize, roomType))
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////   계단 만들기

	/*if (false == mHasUpstair)
	{
		if (sizeX >= 4 && sizeY >= 4)
		{
			MyVector2 stairPos = CheckNorthWall(pos, size);

			if (MyVector2{ -1, -1 } != stairPos)
			{
				stairPos += { 0, -1 };

				MakeUpstair(stairPos);
			}
		}
	}
	else if (false == mHasDownstair)
	{
		if (sizeX >= 4 && sizeY >= 5)
		{
			MyVector2 stairPos = CheckNorthWall(pos, size);

			if (MyVector2{ -1, -1 } != MyVector2{ posX, posY })
			{
				MakeDownstair(pos);
			}
		}
	}*/

	

	// NewUpgradeCorner(basePos, newSize);

	//////////////////////////////////////////////////////////////////////////////////////////////////////   다음 방들 만들기
	switch (roomType)
	{
		// 왼쪽으로 만들으라는 뜻
		// 그렇다면 오른쪽에 방이 있다는 뜻
		// 그렇다면 오른쪽에 방 만들지 말라는 뜻
		mLeftLimit = left;
		mBottomLimit = bottom;
		mRightLimit = right;
		mTopLimit = top;
	case RT_LEFT:
		++mLeftCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_BOTTOM:
		++mBottomCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_RIGHT:
		++mRightCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_TOP:
		++mTopCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	default:
		break;
	}
}

void RandomMap::MakeRandomBRooms(MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType, int left, int bottom, int right, int top)
{
	mLeftLimit = left;
	mBottomLimit = bottom;
	mRightLimit = right;
	mTopLimit = top;

	// 방크기 랜덤 생성
	int sizeX = RandomNumber::GetRandomNumber(3, 7);
	int sizeY = RandomNumber::GetRandomNumber(3, 7);

	int posX = -1;
	int posY = -1;

	switch (roomType)
	{
	case RT_LEFT:
		if (0 == prevPos.X || prevSize.X == 0 ||
			mLeftCount >= mLeftLimit)
		{
			return;
		}

		posY = (int)(prevPos.Y + (int)(prevSize.Y / 2) - (int)(sizeY / 2));
		posX = (int)(prevPos.X - sizeX - 1);
		break;
	case RT_BOTTOM:
		if (MAPCOUNT_Y <= prevPos.Y + prevSize.Y ||
			mBottomCount > mBottomLimit)
		{
			return;
		}

		posX = (int)(prevPos.X + (int)(prevSize.X / 2) - (int)(sizeX / 2));
		posY = (int)(prevPos.Y + prevSize.Y + 1);
		break;
	case RT_RIGHT:
		if (MAPCOUNT_X <= prevPos.X + prevSize.X ||
			mRightCount > mRightLimit)
		{
			return;
		}

		posY = (int)(prevPos.Y + (int)(prevSize.Y / 2) - (int)(sizeY / 2));
		posX = (int)(prevPos.X + prevSize.X + 1);

		break;
	case RT_TOP:
		if (0 == prevPos.Y || prevSize.Y == 0 ||
			mTopCount > mTopLimit)
		{
			return;
		}

		posX = (int)(prevPos.X + (int)(prevSize.X / 2) - (int)(sizeX / 2));
		posY = (int)(prevPos.Y - sizeY - 1);
		break;
	default:
		break;
	}

	// OverCheck
	if (0 > posY)
	{
		posY = 0;
	}
	if (0 > posX)
	{
		posX = 0;
	}
	if (MAPCOUNT_X <= posX)
	{
		posX = MAPCOUNT_X - 1;
	}
	if (MAPCOUNT_Y <= posY)
	{
		posY = MAPCOUNT_Y - 1;
	}


	MyVector2 pos = { posX, posY };
	MyVector2 size = { sizeX, sizeY };

	// true면 거기에 방이 있다.
	// 이미 방이 있으면 반환
	if (true == CheckIsRoom(pos, size))
	{
		return;
	}

	// 현재 자리에 방 만들기
	// 실패하면 방만들기 끝
	if (false == MakeARoom(pos, size, prevPos, prevSize, roomType))
	{
		return;
	}

	//////////////////////////////////////////////////// 현재 위치 설정 2

	MyVector2 BSize = { 6, 6 };
	MyVector2 BPos = { 0, 0 };
	switch (roomType)
	{
	case RT_LEFT:
		if (0 == posX || posY == 0 ||
			mLeftCount >= mLeftLimit)
		{
			return;
		}

		BPos.Y = posY + (int)(sizeY / 2) - (int)(BSize.Y / 2);
		BPos.X = posX - BSize.X - 1;
		break;
	case RT_BOTTOM:
		if (MAPCOUNT_Y <= posY + sizeY ||
			mBottomCount > mBottomLimit)
		{
			return;
		}

		BPos.X = posX + (int)(sizeX / 2) - (int)(BSize.X / 2);
		BPos.Y = posY + sizeY + 1;
		break;
	case RT_RIGHT:
		if (MAPCOUNT_X <= posX + sizeX ||
			mRightCount > mRightLimit)
		{
			return;
		}

		BPos.Y = posY + (int)(sizeY / 2) - (int)(BSize.Y / 2);
		BPos.X = posX + sizeX + 1;
		break;
	case RT_TOP:
		if (0 == posY || sizeY == 0 ||
			mTopCount > mTopLimit)
		{
			return;
		}

		BPos.X = posX + (int)(sizeX / 2) - (int)(BSize.X / 2);
		BPos.Y = posY - BSize.Y - 1;
		break;
	default:
		break;
	}

	if (0 > BPos.Y || 0 > BPos.X ||
		mTopCount > mTopLimit)
	{
		return;
	}

	MakeButchersPreRoom(BPos, BSize, { posX, posY }, { sizeX, sizeY }, roomType);
	//////////////////////////////////////////////////////////////////////////////////////////////////////   다음 방들 만들기
	switch (roomType)
	{
		// 왼쪽으로 만들으라는 뜻
		// 그렇다면 오른쪽에 방이 있다는 뜻
		// 그렇다면 오른쪽에 방 만들지 말라는 뜻
		mLeftLimit = left;
		mBottomLimit = bottom;
		mRightLimit = right;
		mTopLimit = top;
	case RT_LEFT:
		++mLeftCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_BOTTOM:
		++mBottomCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_RIGHT:
		++mRightCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_TOP:
		++mTopCount;
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRandomRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	default:
		break;
	}
}

bool RandomMap::MakeButchersPreRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	if (1 > basePos.IntX())
	{
		int X = 1;
		int Y = basePos.IntY();
	}
	else
	{
		int X = basePos.IntX();
		int Y = basePos.IntY();
	}

	MyVector2 TravellerBase = basePos + MyVector2::DOWNRIGHT;

	int y = 0;
	int x = 0;

	int tempY = 0;
	int tempX = 0;

	for (; y < size.IntY(); y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (MAPCOUNT_Y <= TravellerPos.IntY() + 1 ||
			0 > TravellerPos.IntY())
		{
			tempY = y - 1;
			break;
		}

		x = 0;

		for (; x < size.X; x++)
		{
			if (MAPCOUNT_X <= TravellerPos.IntX() + 1 ||
				0 > TravellerPos.IntX())
			{
				tempX = x - 1;
				break;
			}

			SetWallAroundAt(TravellerPos.IntX(), TravellerPos.IntY());

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MyVector2 newSize;

	if (0 != tempY && 0 != tempX)
	{
		newSize = { (float)tempX, (float)tempY };
	}
	else if (0 != tempY)
	{
		newSize = { (float)x, (float)tempY };
	}
	else if (0 != tempX)
	{
		newSize = { (float)tempX, (float)y };
	}
	else
	{
		newSize = { (float)x, (float)y };
	}

	// 새로운 사이즈가 0보다 작으면... 방이 생성되지 않음
	if (0 >= y || 0 >= x)
	{
		return false;
	}



	mLastRoom->Pos = basePos;
	mLastRoom->Size = newSize;

	// 방 등록
	DibRoom* newRoom = new DibRoom();

	newRoom->Pos = basePos;
	newRoom->Size = newSize;

	mAllRoom.push_back(newRoom);

	MyVector2 tempSize = { (float)x, (float)y };

	// 여기서 문제가 발생
	UpgradeRoomCorner(basePos, tempSize);

	for (int y = newRoom->Pos.IntY(); y < newRoom->Pos.IntY() + newRoom->Size.IntY() + 1; y++)
	{
		for (int x = newRoom->Pos.IntX(); x < newRoom->Pos.IntX() + newRoom->Size.IntX() + 2; x++)
		{
			mAllTile[y][x]->mRoom = newRoom;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////// 문
	// 문 생성하기 (뚫림 1:2 문)
	int doorRandom = RandomNumber::GetRandomNumber(1, 10);

	// 	if (6 >= doorRandom)
	if (6 >= doorRandom)
	{
		// 문이 생성되지 않으면 아웃
		if (false == MakeADoor(basePos, tempSize, prevPos, prevSize, roomType))
		{
			// BOOM;
			return false;
		}
	}
	else
	{
		if (false == MakeAConnection(basePos, tempSize, prevPos, prevSize, roomType))
		{
			// BOOM;
			return false;
		}

	}

	MakeButchersRealRoom({ basePos.X + 2, basePos.Y + 2 }, newSize - MyVector2({ 4, 4 }), prevPos, prevSize, RT_MAX);

	return true;
}

bool RandomMap::MakeButchersRealRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	int X = basePos.IntX();
	int Y = basePos.IntY();

	MyVector2 TravellerBase = basePos + MyVector2::DOWNRIGHT;

	int y = 0;
	int x = 0;

	int tempY = 0;
	int tempX = 0;

	for (; y < size.IntY(); y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (MAPCOUNT_Y <= TravellerPos.IntY() + 1 ||
			0 > TravellerPos.IntY())
		{
			tempY = y - 1;
			break;
		}

		x = 0;

		for (; x < size.X; x++)
		{
			if (MAPCOUNT_X <= TravellerPos.IntX() + 1 ||
				0 > TravellerPos.IntX())
			{
				tempX = x - 1;
				break;
			}

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MyVector2 newSize;

	if (0 != tempY && 0 != tempX)
	{
		newSize = { (float)tempX, (float)tempY };
	}
	else if (0 != tempY)
	{
		newSize = { (float)x, (float)tempY };
	}
	else if (0 != tempX)
	{
		newSize = { (float)tempX, (float)y };
	}
	else
	{
		newSize = { (float)x, (float)y };
	}

	// 새로운 사이즈가 0보다 작으면... 방이 생성되지 않음
	if (0 >= y || 0 >= x)
	{
		return false;
	}

	
	y = 0;
	x = 0;

	////////////////////// 이제 실제 방을 만들자
	TravellerBase = basePos + MyVector2::DOWNRIGHT;
	for (; y < size.IntY(); y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (MAPCOUNT_Y <= TravellerPos.IntY() + 1 ||
			0 > TravellerPos.IntY())
		{
			tempY = y - 1;
			break;
		}

		x = 0;

		for (; x < size.X; x++)
		{
			if (MAPCOUNT_X <= TravellerPos.IntX() + 1 ||
				0 > TravellerPos.IntX())
			{
				tempX = x - 1;
				break;
			}

			SetBWallAroundAt(TravellerPos.IntX(), TravellerPos.IntY());

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MakeBFloor(basePos + MyVector2::DOWNRIGHT, { 2, 2 });


	mLastRoom->Pos = basePos;
	mLastRoom->Size = newSize;

	// 방 등록
	/*DibRoom* newRoom = new DibRoom();

	newRoom->Pos = basePos;
	newRoom->Size = newSize;

	mAllRoom.push_back(newRoom);*/

	MyVector2 tempSize = { (float)x, (float)y };

	// 여기서 문제가 발생
	// UpgradeRoomCorner(basePos, tempSize);

	/*for (int y = newRoom->Pos.IntY(); y < newRoom->Pos.IntY() + newRoom->Size.IntY() + 1; y++)
	{
		for (int x = newRoom->Pos.IntX(); x < newRoom->Pos.IntX() + newRoom->Size.IntX() + 2; x++)
		{
			mAllTile[y][x]->mRoom = newRoom;
		}
	}*/

	///////////////////////////////////////////////////////////////////////////////////////////////// 문
	// 문 생성하기 (뚫림 1:2 문)
	int doorRandom = RandomNumber::GetRandomNumber(1, 10);

	// 	if (6 >= doorRandom)
	//if (6 >= doorRandom)
	//{
	//	// 문이 생성되지 않으면 아웃
	//	if (false == MakeADoor(basePos, tempSize, prevPos, prevSize, roomType))
	//	{
	//		// BOOM;
	//		return false;
	//	}
	//}
	//else
	//{
	//	if (false == MakeAConnection(basePos, tempSize, prevPos, prevSize, roomType))
	//	{
	//		// BOOM;
	//		return false;
	//	}

	//}

	SetDoorAt(MyVector2 { basePos.X + 3, basePos.Y + 2 }, WT_BDOOR);

	mBPos = { basePos.IntX() * 2 + 5, basePos.IntY()  * 2 + 5 };

	mHasBRoom = true;

	return true;
}

bool RandomMap::SetUpstair()
{
	for (size_t i = 7; i < mAllRoom.size(); ++i)
	{
		int sizeX = mAllRoom[i]->Size.IntX();
		int sizeY = mAllRoom[i]->Size.IntY();
		MyVector2 pos = mAllRoom[i]->Pos;
		MyVector2 size = mAllRoom[i]->Size;

		if (false == mHasUpstair)
		{
			if (sizeX >= 4 && sizeY >= 4)
			{
				MyVector2 stairPos = CheckNorthWall(pos, size);

				if (MyVector2{ -1, -1 } != stairPos)
				{
					stairPos += { 0, -1 };

					MakeUpstair(stairPos);
				}
			}
		}
		else if (false == mHasDownstair)
		{
			if (sizeX >= 4 && sizeY >= 5)
			{
				MyVector2 stairPos = CheckNorthWall(pos, size);

				if (MyVector2{ -1, -1 } != MyVector2{ pos.X, pos.Y })
				{
					MakeDownstair(pos);
				}
			}
		}
	}

	return mHasUpstair;
}

void RandomMap::SetDownstair()
{
}

void RandomMap::MakeUpstair(MyVector2& basePos)
{
	if (basePos.Y <= 0)
	{
		return;
	}

	// 계단 타일의 크기는 3, 2( y에 벽 포함, 따라서 1)
	// 따라서 최고 크기 4, 3
	MyVector2 startPos = basePos;
	

	// 예외처리 (벽 바깥)
	if (startPos.IntY() + 3 + 1 >= MAPCOUNT_X)
	{
		return;
	}

	TILE_TYPE temp;

	// 예외처리 (바닥)
	for (size_t i = 0; i < 3; i++)
	{
		temp = mAllTile[startPos.IntY() - 1][startPos.IntX() + i]->TileType();

		if (TT_FLOOR == temp)
		{
			return;
		}
	}

	for (size_t i = 0; i < 3; i++)
	{
		temp = mAllTile[startPos.IntY()][startPos.IntX() + i]->TileType();

		if (TT_FLOOR == temp)
		{
			return;
		}
	}

	MyVector2 pos = basePos;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			mAllTile[pos.IntY()][pos.IntX()]->SetUpstair();
			// mStartPos = { pos.X - 1, pos.Y + 1 };

			pos += MyVector2::RIGHT;
		}
		startPos += MyVector2::DOWN;
		pos = startPos;
	}

	// 계단도 방이다
	/*DibRoom* newRoom;
	newRoom.Pos = basePos;
	newRoom.Size = MyVector2(3, 4);
	mAllRoom.push_back(newRoom);*/

	mHasUpstair = true;

	mStartPos1 = { (basePos.IntX() * 2 + 2) , basePos.IntY() * 2 +1 };
	mStartPos2 = { (basePos.IntX() * 2 + 2) + 1 , basePos.IntY() * 2 + 1 };
}

void RandomMap::MakeDownstair(MyVector2& basePos)
{
	MyVector2 pos = basePos + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::RIGHT + MyVector2::DOWN;

	// 예외처리
	if (MAPCOUNT_X - 3 <= pos.X ||
		MAPCOUNT_Y - 3 <= pos.Y)
	{
		return;
	}

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (true == CheckIsTile(pos.IntX(), pos.IntY(), WT_LASTRIGHT1) ||
				true == CheckIsTile(pos.IntX(), pos.IntY(), WT_LEFTDOWNWALL) ||
				true == CheckIsTile(pos.IntX(), pos.IntY(), WT_FLOOR1_B))
			{
				return;
			}

			pos += MyVector2::RIGHT;
		}
		pos = basePos + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::DOWN;
	}

	pos = basePos + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::RIGHT + MyVector2::DOWN;

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			mAllTile[pos.IntY()][pos.IntX()]->SetDownstair();

			pos += MyVector2::RIGHT;
		}
		pos = basePos + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::DOWN;
	}
			

	// 계단도 방이다
	/*DibRoom newRoom;
	newRoom.Pos = basePos;
	newRoom.Size = MyVector2(3, 2);
	mAllRoom.push_back(newRoom);*/

	mHasDownstair = true;

	mDownPos = { (basePos.IntX()*2) + 7 , basePos.IntY()*2 +6 };

}

bool RandomMap::MakeADoor(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	// 큰 순으로 정렬
	float arrX[4] = { basePos.X, basePos.X + size.X, prevPos.X, prevPos.X + prevSize.X };
	float arrY[4] = { basePos.Y, basePos.Y + size.Y, prevPos.Y, prevPos.Y + prevSize.Y };

	for (int i = 3; i >= 0; --i)
	{
		for (int j = 0; j < i; j++)
		{
			if (arrX[j] > arrX[j + 1])
			{
				float temp = arrX[j + 1];
				arrX[j + 1] = arrX[j];
				arrX[j] = temp;
			}
			if (arrY[j] > arrY[j + 1])
			{
				float temp = arrY[j + 1];
				arrY[j + 1] = arrY[j];
				arrY[j] = temp;
			}
		}
	}

	// 비교
	MyVector2 bigPos = prevPos;

	if (basePos.Y > prevPos.Y)
	{
		bigPos.Y = basePos.Y;
	}

	if (basePos.X > prevPos.X)
	{
		bigPos.X = basePos.X;
	}

	float randomSizeX = arrX[2] - arrX[1];
	float randomSizeY = arrY[2] - arrY[1];

	int ranY = RandomNumber::GetRandomNumber(1, (int)randomSizeY);
	int ranX = RandomNumber::GetRandomNumber(1, (int)randomSizeX);

	switch (roomType)
	{
	case RT_LEFT:
		if (TT_STAIR == mAllTile[bigPos.IntY() + ranY][bigPos.IntX()]->TileType())
		{
			return false;
		}

		SetDoorAt(MyVector2{ bigPos.X, bigPos.Y + ranY }, WT_LEFTDOOR);
		break;
	case RT_BOTTOM:
		if (TT_STAIR == mAllTile[bigPos.IntY()][bigPos.IntX() + ranX]->TileType())
		{
			return false;
		}

		SetDoorAt(MyVector2{ bigPos.X + ranX, bigPos.Y }, WT_BOTTOMDOOR);
		break;
	case RT_RIGHT:
		if (TT_STAIR == mAllTile[bigPos.IntY() + ranY][bigPos.IntX()]->TileType())
		{
			return false;
		}

		SetDoorAt(MyVector2{ bigPos.X, bigPos.Y + ranY }, WT_RIGHTDOOR);
		break;
	case RT_TOP:
		if (TT_STAIR == mAllTile[bigPos.IntY()][bigPos.IntX() + ranX]->TileType())
		{
			return false;
		}

		SetDoorAt(MyVector2{ bigPos.X + ranX, bigPos.Y }, WT_TOPDOOR);
		break;
	case RT_MAX:
		break;
	default:
		break;
	}

	return true;
}

bool RandomMap::MakeAConnection(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	// 큰 순으로 정렬
	float arrX[4] = { basePos.X, basePos.X + size.X, prevPos.X, prevPos.X + prevSize.X };
	float arrY[4] = { basePos.Y, basePos.Y + size.Y, prevPos.Y, prevPos.Y + prevSize.Y };

	for (int i = 3; i >= 0; --i)
	{
		for (int j = 0; j < i; j++)
		{
			if (arrX[j] > arrX[j + 1])
			{
				float temp = arrX[j + 1];
				arrX[j + 1] = arrX[j];
				arrX[j] = temp;
			}
			if (arrY[j] > arrY[j + 1])
			{
				float temp = arrY[j + 1];
				arrY[j + 1] = arrY[j];
				arrY[j] = temp;
			}
		}
	}

	// 비교
	MyVector2 bigPos = prevPos;

	if (basePos.Y > prevPos.Y)
	{
		bigPos.Y = basePos.Y;
	}

	if (basePos.X > prevPos.X)
	{
		bigPos.X = basePos.X;
	}

	float randomSizeX = arrX[2] - arrX[1];
	float randomSizeY = arrY[2] - arrY[1];

	int ranY = RandomNumber::GetRandomNumber(1, (int)randomSizeY);
	int ranX = RandomNumber::GetRandomNumber(1, (int)randomSizeX);

	switch (roomType)
	{
	case RT_LEFT:
		if (TT_STAIR == mAllTile[bigPos.IntY() + ranY][bigPos.IntX()]->TileType())
		{
			return false;
		}
		MakeWideFloor(bigPos + MyVector2::DOWN, { 1.f, arrY[2] - arrY[1] - 1 });

		break;
	case RT_BOTTOM:
		if (TT_STAIR == mAllTile[bigPos.IntY()][bigPos.IntX() + ranX]->TileType())
		{
			return false;
		}
		MakeWideFloor(bigPos + MyVector2::RIGHT, { arrX[2] - arrX[1] - 1, 1.f });

		break;
	case RT_RIGHT:
		if (TT_STAIR == mAllTile[bigPos.IntY() + ranY][bigPos.IntX()]->TileType())
		{
			return false;
		}
		MakeWideFloor(bigPos + MyVector2::DOWN, { 1.f, arrY[2] - arrY[1] - 1 });

		break;
	case RT_TOP:
		if (TT_STAIR == mAllTile[bigPos.IntY()][bigPos.IntX() + ranX]->TileType())
		{
			return false;
		}
		MakeWideFloor(bigPos + MyVector2::RIGHT, { arrX[2] - arrX[1] - 1, 1.f });

		break;
	case RT_MAX:
		break;
	default:
		break;
	}

	return true;
}



void RandomMap::MakeFloor(MyVector2 basePos, MyVector2 size)
{
	MyVector2 TravellerBase = basePos;
	MyVector2 TravellerPos;
	for (int y = 0; y < size.Y; y++)
	{
		TravellerPos = TravellerBase;
		for (int x = 0; x < size.X; x++)
		{

			mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->FloorType(WT_FLOOR);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	
	int a = 0;

	// 가로일 경우
	// 가로의 오른쪽(위쪽)
	switch (mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType())
	{
	case WT_LEFTWALL:
	case WT_DOWNWALL:
	case WT_TOPWALL:
	case WT_LEFTTOP:
		mAllTile[basePos.IntY()][basePos.IntX()]->WallType(WT_LASTRIGHT2);
		break;
	case WT_RIGHTTOP:
		
		break;
	default:
		break;
	}

	switch (mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType())
	{
	case WT_LEFTWALL:
	case WT_LEFTTOP:
		// mAllTile[basePos.IntY()][basePos.IntX()]->WallType(WT_LASTRIGHT2);
		break;
	case WT_TOPWALL:
		mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_LASTRIGHT1);
	case WT_RIGHTTOP:

		break;
	default:
		break;
	}

	// 세로일 경우
	switch (mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType())
	{
	case WT_LEFTWALL:
	case WT_LEFTTOP:
		mAllTile[basePos.IntY()][basePos.IntX()]->WallType(WT_LASTLEFT2);
		break;
	case WT_TOPWALL:
		// mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTRIGHT1);
	case WT_RIGHTTOP:

		break;
	default:
		break;
	}

	TravellerPos = TravellerPos + MyVector2::DOWN + MyVector2::LEFT;

	if (0 > TravellerPos.IntY() - 1 ||
		0 > TravellerPos.IntX() - 1||
		MAPCOUNT_X <= TravellerPos.IntX() ||
		MAPCOUNT_Y <= TravellerPos.IntY())
	{
		return;
	}


	switch (mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType())
	{
	case WT_LEFTWALL:
		mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
	case WT_LEFTTOP:
		// mAllTile[basePos.IntY()][basePos.IntX()]->WallType(WT_LASTRIGHT2);
		break;
	case WT_TOPWALL:
		// mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
	case WT_RIGHTTOP:

		break;
	default:
		break;
	}
}

void RandomMap::MakeBFloor(MyVector2 basePos, MyVector2 size)
{
	MyVector2 TravellerBase = basePos;
	MyVector2 TravellerPos;
	for (int y = 0; y < size.Y; y++)
	{
		TravellerPos = TravellerBase;
		for (int x = 0; x < size.X; x++)
		{

			mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->FloorType(WT_FLOOR1_B);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}
}

void RandomMap::MakeWideFloor(MyVector2 basePos, MyVector2 size)
{
	MyVector2 newSize = size;

	if (0 == size.IntX())
	{
		newSize.X = 1;
	}
	if (0 == size.IntY())
	{
		newSize.Y = 1;
	}

	//if (0 >= basePos.IntY() - 1 ||
	//	0 >= basePos.IntX() - 1 ||
	//	MAPCOUNT_X <= basePos.IntX() + 1 ||
	//	MAPCOUNT_Y <= basePos.IntY() + 1)
	//{
	//	return;
	//}

	if (basePos.IntY() == 18 && basePos.IntX() == 38)
	{
		int a = 0;
	}

	MyVector2 TravellerBase = basePos;
	MyVector2 TravellerPos;

	for (int y = 0; y < newSize.IntY(); y++)
	{
		TravellerPos = TravellerBase;
		for (int x = 0; x < newSize.IntX(); x++)
		{

			mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->FloorType(WT_FLOOR);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}



	

	// 여기가 오른쪽에 벽이 있던 부분
	// temp = mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 1]->WallType();

	if (TravellerPos.IntY() == 36 && TravellerPos.IntX() + 1 == 33)
	{
		int a = 0;
	}

	

	if (0 > TravellerPos.IntY() ||
		0 > TravellerPos.IntX() ||
		MAPCOUNT_X <= TravellerPos.IntX() + 1 ||
		MAPCOUNT_Y <= TravellerPos.IntY() + 1)
	{

		if (WT_LEFTTOP == mAllTile[basePos.IntY()][basePos.IntX() - 2]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_RIGHTDOWNWALL);
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() - 1]->WallType() &&
			WT_LEFTWALL == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LEFTWALL);
		}
		else if (WT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType() ||
			TT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTRIGHT2);
		}
		else
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_RIGHTTOPWALL);
		}

		return;
	}


	// 가로의 오른쪽...
	mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->FloorType(WT_FLOOR);

	switch (mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 1]->WallType())
	{
	case WT_RIGHTWALL:
	case WT_RIGHTTOP:
		/*if (WT_FLOOR == mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX() + 1]->WallType() ||
			TT_FLOOR == mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX() + 1]->TileType())
		{
			mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 1]->WallType(WT_LASTLEFT1);
		}
	*/
		break;

	case WT_RIGHTDOWN:
	case WT_LEFTTOP:
		if (WT_TOPWALL == mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 2]->WallType())
		{

		}
		else
		{
			mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 1]->WallType(WT_LASTLEFT1);
		}
		break;
	case WT_TOPWALL:
		mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 1]->WallType(WT_LASTRIGHT1);
		break;
	case WT_LEFTWALL:
		if (WT_FLOOR == mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX() + 1]->WallType() ||
			TT_FLOOR == mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX() + 1]->TileType())
		{
			mAllTile[TravellerPos.IntY()][TravellerPos.IntX() + 1]->WallType(WT_LASTLEFT1);
		}

	default:
		break;
	}

/////////////////////////////////////////////////////////////////

	int a = 0;

	//switch (mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType())
	//{
	//case WT_LEFTWALL:
	//case WT_DOWNWALL:
	//case WT_TOPWALL:
	//case WT_LEFTTOP:
	//	if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() - 1]->WallType() &&
	//		WT_LEFTWALL == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType())
	//	{
	//		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LEFTWALL);
	//	}
	//	else if (WT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType() ||
	//		TT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->TileType())
	//	{
	//		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTRIGHT2);
	//	}
	//	else
	//	{
	//		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_RIGHTTOPWALL);
	//	}

	//	break;
	//}

	if (0 >= basePos.IntY() - 1||
		0 >= basePos.IntX() - 1||
		MAPCOUNT_X <= basePos.IntX() + 1 ||
		MAPCOUNT_Y <= basePos.IntY() + 1)
	{
	if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() - 1]->WallType() &&
		WT_LEFTWALL == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType())
	{
		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LEFTWALL);
	}
	else if (WT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType() ||
		TT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->TileType())
	{
		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTRIGHT2);
	}
	else
	{
		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_RIGHTTOPWALL);
	}

		return;
	}

	WALL_TYPE temp = mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType();

	// 가로일 경우
	// 가로의 왼쪽
	switch (mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType())
	{
	case WT_LEFTWALL:
	case WT_DOWNWALL:
	case WT_TOPWALL:
	case WT_LEFTTOP:
		if (WT_LEFTTOP == mAllTile[basePos.IntY()][basePos.IntX() - 2]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_RIGHTDOWNWALL);
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() - 1]->WallType() &&
			WT_LEFTWALL == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType())
		{
 			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LEFTWALL);
		}
		else if (WT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->WallType() ||
			TT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTRIGHT2);
		}
		else
		{
			mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_RIGHTTOPWALL);
		}
		
		break;
	case WT_RIGHTTOP:

		break;
	case WT_LASTRIGHT1:
		mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTLEFT2);
		break;
	default:
		break;
	}


	if (basePos.IntY()== 31 && basePos.IntX() == 32)
	{
		int a = 0;
	}




	

	// 세로일 경우
	// 세로 중 위

	if (basePos.IntY() - 1 == 31 && basePos.IntX() == 32)
	{
		int a = 0;
	}

	temp = mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType();
	WALL_TYPE temp2 = mAllTile[basePos.IntY() - 2][basePos.IntX()]->WallType();

	switch (mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType())
	{
	case WT_LEFTWALL:
	case WT_LEFTTOP:

		if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() - 1][basePos.IntX() + 1]->TileType())
		{
			mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType(WT_LASTLEFT2); // 2
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() - 1][basePos.IntX() + 1]->WallType())
		{
			mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType(WT_LASTLEFT2); // 2
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() - 2][basePos.IntX()]->WallType() &&
			WT_TOPWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() + 1]->WallType())
		{
			mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType(WT_LASTRIGHT1);
		}
		else if (WT_RIGHTTOP == mAllTile[basePos.IntY() - 2][basePos.IntX()]->WallType() &&
			WT_TOPWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() + 1]->WallType())
		{
			mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType(WT_LASTRIGHT1);
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType())
		{
			mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType(WT_RIGHTDOWNWALL); // 2
		}
		else
		{
			mAllTile[basePos.IntY() - 1][basePos.IntX()]->WallType(WT_LASTRIGHT1);
		}
		
		break;
	case WT_TOPWALL:
		// mAllTile[basePos.IntY()][basePos.IntX() - 1]->WallType(WT_LASTRIGHT1);
	
	default:
		break;
	}

	// 세로일 경우
	// 세로 중 아래


	TravellerPos = TravellerPos + MyVector2::DOWN + MyVector2::LEFT;

	if (0 > TravellerPos.IntY() ||
		0 > TravellerPos.IntX() ||
		MAPCOUNT_X <= TravellerPos.IntX() + 1 ||
		MAPCOUNT_Y <= TravellerPos.IntY() + 1)
	{
		return;
	}

	


	/*switch (mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType())
	{
	case WT_LEFTWALL:
		mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_FLOOR);

		if (WT_LEFTWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType() &&
			WT_DOWNWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX() - 1]->WallType())
		{
			mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		}
		else if (WT_LEFTWALL == mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX()]->WallType())
		{
			mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		}
		break;
	}
*/

	if (TravellerPos.IntY() == 13 && TravellerPos.IntX() == 30)
	{
		int a = 0;
	}


	 temp = mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType();
	 temp2 = mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX() - 1]->WallType();

	switch (mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType())
	{
	case WT_LEFTWALL:
		mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->FloorType(WT_FLOOR);

		if(WT_LEFTWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType() &&
			WT_DOWNWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX() - 1]->WallType())
		{
			mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		}
		else if (WT_LEFTWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType() &&
			WT_LEFTTOPWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX() - 1]->WallType())
		{
			mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		}		
		else if (WT_TOPWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX() - 1]->WallType() &&
			WT_LEFTWALL == mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType())
		{
			mAllTile[TravellerPos.IntY() + 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		}
		else if (WT_LEFTWALL == mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX()]->WallType())
		{
			mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		}
		
	case WT_LEFTTOP:
		//mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_LASTRIGHT1);
		
		// mAllTile[basePos.IntY()][basePos.IntX()]->WallType(WT_LASTRIGHT2);
		break;
	case WT_TOPWALL:
		// mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
		break;
	case WT_LEFTDOWN:

	case WT_RIGHTDOWN:
		mAllTile[TravellerPos.IntY() - 1][TravellerPos.IntX()]->WallType(WT_LASTLEFT1);
	case WT_RIGHTTOP:
		mAllTile[TravellerPos.IntY()][TravellerPos.IntX()]->WallType(WT_LASTRIGHT1);
		break;
	default:
		break;
	}
}

void RandomMap::ResetCount()
{
	mLeftCount = 0;
	mBottomCount = 0;
	mRightCount = 0;
	mTopCount = 0;
}



void RandomMap::UpgradeRoomCorner(MyVector2 basePos, MyVector2 size)
{
	int x = basePos.IntX();
	int y = basePos.IntY();

	if (x < 0)
	{
		return;
	}

	SetWallAt(x, y, WT_LEFTTOP);
	UpgradeAdjCorner(x, y, mAllTile[y][x]->WallType());
	x += size.IntX() + 1;

	SetWallAt(x, y, WT_RIGHTTOP);
	UpgradeAdjCorner(x, y, mAllTile[y][x]->WallType());
	x = basePos.IntX();
	y += size.IntY() + 1;

	SetWallAt(x, y, WT_LEFTDOWN);
	UpgradeAdjCorner(x, y, mAllTile[y][x]->WallType());
	x += size.IntX() + 1;

	SetWallAt(x, y, WT_RIGHTDOWN);
	UpgradeAdjCorner(x, y, mAllTile[y][x]->WallType());
}

void RandomMap::UpgradeAdjCorner(int x, int y, WALL_TYPE type)
{
	if (x == 39 && y == 1)
	{
		int a = 0;
	}

	if (WT_NONE == type)		
	{
		return;
	}

	// 나의 타입
	switch (type)
	{
	case WT_NONE:
		break;
	case WT_LEFTTOP:

		/*if (false == (y - 1 < 0))
		{
			if (WT_LEFTTOPWALL == mAllTile[y - 1][x]->WallType())
			{
				mAllTile[y - 1][x]->mWallType = WT_LEFTWALL;
			}
		}*/
		
		if (false == (y + 1 > MAPSIZE_Y - 1))
		{
			if (WT_LEFTDOWNWALL == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y + 1][x]->mWallType = WT_LEFTWALL;
			}
		}

		/*if (false == (y - 1 < 0))
		{
			if (WT_DOWNWALL == mAllTile[y - 1][x]->WallType())
			{
				 mAllTile[y - 1][x]->mWallType = WT_LEFTTOPWALL;
			}
		}*/

			break;
	case WT_RIGHTTOP:
		if (false == (y - 1 < 0))
		{
			if (WT_RIGHTTOPWALL == mAllTile[y - 1][x]->WallType())
			{
				mAllTile[y - 1][x]->mWallType = WT_RIGHTWALL;
			}


		}

		// 이전 방 = 오른쪽
		// 생성되는 방 = 왼쪽
		// 문제 = 곂치는 벽
		if (false == (y + 1 >= MAPSIZE_Y - 1))
		{
			if (WT_LEFTWALL == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTWALL;
			}

			if(WT_LEFTDOWN == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTWALL;
			}
		}


		// 확정
		// 오른쪽이 down이라면
		if (false == (x + 1 >= MAPSIZE_X - 1))
		{
			if (WT_DOWNWALL == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}

			if (WT_TOPWALL == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTTOP;
			}

			if (WT_RIGHTDOWN == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}

			// 36, 31의 문제
			if (WT_RIGHTTOP == mAllTile[y][x + 1]->WallType())
			{
				// mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}
		}

		// 아래쪽이 Right라면
		/*if (false == (y + 1 >= MAPSIZE_X - 1))
		{
			if (WT_RIGHTWALL == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}
		}*/

		break;
	case WT_TOPWALL:
		if (false == (x - 1 < 0))
		{
			if (WT_LEFTTOPWALL == mAllTile[y][x - 1]->WallType())
			{
				mAllTile[y][x - 1]->mWallType = WT_TOPWALL;
				mAllTile[y][x - 1]->mIsT = true;
			}
		}

		if (false == (x + 1 > MAPSIZE_X - 1))
		{
			if (WT_RIGHTTOPWALL == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x + 1]->mWallType = WT_TOPWALL;
			}
		}
		break;
	case WT_DOWNWALL:
		if (false == (x - 1 < 0))
		{
			if (WT_LEFTDOWNWALL == mAllTile[y][x - 1]->WallType())
			{
				mAllTile[y][x - 1]->mWallType = WT_DOWNWALL;
			}

			if (WT_RIGHTTOPWALL == mAllTile[y][x - 1]->WallType())
			{
				mAllTile[y][x - 1]->mWallType = WT_LEFTTOPWALL;
				mAllTile[y][x - 1]->mIsT = true;
			}
		}

		/*if (false == (x + 1 >= MAPSIZE_X - 1))
		{
			if (WT_RIGHTDOWNWALL == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x + 1]->mWallType = WT_DOWNWALL;
			}
		}*/
		break;

	case WT_RIGHTDOWN:

		// 확정
		// 오른쪽이 down이라면
		if (false == (x + 1 >= MAPSIZE_X - 1))
		{
			if (WT_DOWNWALL == mAllTile[y][x + 1]->WallType())
			{
				// mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}

			if (WT_TOPWALL == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTDOWNWALL;
			}
		}

		// 아래쪽이 Right라면
		if (false == (y + 1 >= MAPSIZE_X - 1))
		{
			if (WT_LEFTWALL == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTWALL;
			}

			if (WT_RIGHTWALL == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_RIGHTWALL;
			}
		}

		if (false == (y - 1 < 0))
		{
			if (WT_LEFTDOWN == mAllTile[y - 1][x]->WallType())
			{
				mAllTile[y - 1][x]->mWallType = WT_LEFTTOPWALL;
			}
		}

		break;


	case WT_LEFTDOWN:
		// 확정
		if (false == (y + 1 >= MAPSIZE_Y - 1))
		{
			if (WT_RIGHTWALL == mAllTile[y + 1][x]->WallType()
				|| WT_RIGHTDOWN == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}

			if (WT_RIGHTTOP == mAllTile[y + 1][x]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_LEFTTOPWALL;
			}


		}

		if (false == (x + 1 >= MAPSIZE_X - 1))
		{
			if (WT_TOPWALL == mAllTile[y][x + 1]->WallType())
			{
				mAllTile[y][x]->mWallType = WT_TOPWALL;
			}
		}
		break;
	case WT_MAX:
		break;
	default:
		break;
	}
}

void RandomMap::UpgradeAllAdjCorner()
{
	for (int i = 0; i < mAllRoom.size(); ++i)
	{
		UpgradeRoomCorner(mAllRoom[i]->Pos, mAllRoom[i]->Size);

		/*UpgradeAdjCorner(mAllRoom[i].Pos.IntX(), mAllRoom[i].Pos.IntY(), WT_LEFTTOP);
		UpgradeAdjCorner(mAllRoom[i].Pos.IntX() + mAllRoom[i].Size.IntX() + 1, mAllRoom[i].Pos.IntY(), WT_RIGHTTOP);
		UpgradeAdjCorner(mAllRoom[i].Pos.IntX(), mAllRoom[i].Pos.IntY() + mAllRoom[i].Size.IntY() + 1, WT_LEFTDOWN);
		UpgradeAdjCorner(mAllRoom[i].Pos.IntX() + mAllRoom[i].Size.IntX() + 1, mAllRoom[i].Pos.IntY() + mAllRoom[i].Size.IntY() + 1, WT_RIGHTDOWN);*/
	}
}

void RandomMap::SetFloorAt(int x, int y)
{
	if (x <= 0 || x >= MAPSIZE_X - 1 ||
		y <= 0 || y >= MAPSIZE_Y - 1 ||
		TT_STAIR == mAllTile[y][x]->TileType() )
	{
		return;
	}

	mAllTile[y][x]->SetFloor();
}

void RandomMap::SetFloorAt(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() > MAPSIZE_X - 1 ||
		pos.IntY() < 0 || pos.IntY() > MAPSIZE_Y - 1 ||
		TT_STAIR == mAllTile[pos.IntY()][pos.IntX()]->TileType() )
	{
		return;
	}

	mAllTile[pos.IntY()][pos.IntX()]->SetFloor();
}


void RandomMap::SetWallAt(int x, int y, WALL_TYPE type)
{
	if (x < 0 || x > MAPSIZE_X - 1 ||
		y < 0 || y > MAPSIZE_Y - 1 )
	{
		return;
	}



	// 기존 타일 타입
	WALL_TYPE prevWT = mAllTile[y][x]->WallType();

	if (39 == x && 1 == y)
	{
		int a = 0;
	}

	// 기본은 덮어쓰기
	// 이미 있던 타일이 벽이라면... 덮어쓰지 마라
	switch (prevWT)
	{
	case WT_NONE:
		break;
		// 왼쪽 벽
	case WT_LEFTWALL:
		// 덮어쓰려고 하는 것은 오른쪽 벽(INWALL)
		if (WT_RIGHTWALL == type)
		{
			return;
		}

		if (WT_RIGHTDOWN == type)
		{
			return;
		}

		// 8, 37 문제
		if (WT_RIGHTTOP == type)
		{
			return;
		}
		break;
	case WT_RIGHTWALL:
		if (WT_LEFTDOWN == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL);
			return;
		}
		break;
	case WT_TOPWALL:
		// 덮어쓰려고 하는 것은 아래쪽 벽(INWALL)
		if (WT_DOWNWALL == type)
		{
			return;
		}
		if (WT_RIGHTDOWN == type ||
			WT_LEFTDOWN == type) // seed 1000 (34, 11)
		{
			return;
		}
		break;
	case WT_DOWNWALL:
		// 35, 27
		if (WT_RIGHTTOP == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL);
			return;
		}

		if (WT_UP1 == type ||
			WT_UP2 == type ||
			WT_UP3 == type)
		{
			return;
		}
		break;
	case WT_LEFTDOWN:
		if (WT_RIGHTWALL == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL);
			return;
		}
		
		if (WT_TOPWALL == type)
		{
			// return; // seed 1 (2, 15)
		}
		// seed 1 (6, 10)
		if (WT_RIGHTDOWN == type)
		{
			return;
		}

		if (WT_RIGHTTOP == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL); // seed 1 (15, 25)
		}
		break;
	case WT_RIGHTDOWN:
		break;
	case WT_RIGHTTOP:
		if (WT_RIGHTDOWN == type)
		{
			return;
		}
		if (WT_DOWNWALL == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL); // seed 1 (32, 13)
			return; 
		}
		if (WT_LEFTDOWN == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL); // (2, 7) seed 1 (31, 11)
			return; // seed 100 (37, 29)
		}
		break;
	case WT_LEFTTOP:
		if (WT_DOWNWALL == type ||
			WT_RIGHTWALL == type ||
			WT_RIGHTTOP == type ||
			WT_RIGHTDOWN == type) // seed 2 (4, 30)
		{
			return;
		}

		if (WT_LEFTDOWN == type)
		{
			return;
		}
		break;
	case WT_LEFTDOWNWALL:

		break;
	case WT_RIGHTDOWNWALL:
		break;
	case WT_RIGHTTOPWALL:

		break;
	case WT_LEFTTOPWALL:
		if (WT_DOWNWALL == type ||
			WT_RIGHTWALL == type ||
			// test 35, 27
			WT_RIGHTTOP == type ||
			// test 4, 9
			WT_LEFTDOWN == type ||
			// seed 1 (15, 25)
			WT_RIGHTDOWN == type)
		{
			return;
		}
		break;
	case WT_COLUMN:
		break;
	case WT_COLUMNSHADOW1:
		break;
	case WT_COLUMNSHADOW2:
		break;
	case WT_MAX:
		break;
	case WT_UP1:
		if (WT_DOWNWALL == type)
		{

		}
	case WT_UP2:
	case WT_UP3:
		return;
	case WT_LASTRIGHT1:
	case WT_LASTRIGHT2:
	case WT_LASTLEFT1:
	case WT_LASTLEFT2:
		return;
	default:
		break;
	}

	mAllTile[y][x]->WallType(type);
}

void RandomMap::SetWallAt(MyVector2 pos, WALL_TYPE type)
{
	SetWallAt(pos.IntX(), pos.IntY(), type);
}

void RandomMap::SetDoorAt(MyVector2 pos, WALL_TYPE doorType)
{
	if (pos.X <= 0 || pos.X >= MAPCOUNT_X - 1 ||
		pos.Y <= 0 || pos.Y >= MAPCOUNT_Y - 1)
	{
		return;
	}

	mAllTile[pos.IntY()][pos.IntX()]->WallType(doorType);
}




void RandomMap::SetCornerAt(int x, int y, WALL_TYPE type)
{
	if (x < 0 || x > MAPSIZE_X - 1 ||
		y < 0 || y > MAPSIZE_Y - 1)
	{
		return;
	}

	switch (mAllTile[y][x]->WallType())
	{
	case WT_NONE:
		break;
		// 왼쪽 벽
	case WT_LEFTWALL:
		// 덮어쓰려고 하는 것은 오른쪽 벽(INWALL)
		if (WT_RIGHTWALL == type)
		{
			return;
		}
		break;
	case WT_RIGHTWALL:
		break;
	case WT_TOPWALL:
		// 덮어쓰려고 하는 것은 아래쪽 벽(INWALL)
		if (WT_DOWNWALL == type)
		{
			return;
		}
		break;
	case WT_DOWNWALL:

		break;
	case WT_LEFTDOWN:
		if (WT_RIGHTWALL == type)
		{
			mAllTile[y][x]->WallType(WT_LEFTTOPWALL);
			return;
		}
		
		break;
	case WT_RIGHTDOWN:
		if (WT_LEFTWALL == type)
		{
			return;
		}
		break;
	case WT_RIGHTTOP:
		break;
	case WT_LEFTTOP:
		if (WT_DOWNWALL == type ||
			WT_RIGHTWALL == type)
		{
			return;
		}
		break;
	case WT_LEFTDOWNWALL:

		break;
	case WT_RIGHTDOWNWALL:
		break;
	case WT_RIGHTTOPWALL:
		break;
	case WT_LEFTTOPWALL:
		if (WT_DOWNWALL == type ||
			WT_RIGHTWALL == type)
		{
			return;
		}
		break;
	case WT_COLUMN:
		break;
	case WT_COLUMNSHADOW1:
		break;
	case WT_COLUMNSHADOW2:
		break;
	case WT_MAX:
		break;
	default:
		break;
	}

	mAllTile[y][x]->WallType(type);
}

void RandomMap::SetCornerAt(MyVector2 pos, WALL_TYPE type)
{
	SetCornerAt(pos.IntX(), pos.IntY(), type);
}

// 덮어쓰기 하지 않음
void RandomMap::SetNormalCornerAt(MyVector2 pos, WALL_TYPE type)
{
	if (pos.IntX() < 0 || pos.IntX() > MAPSIZE_X - 1 ||
		pos.IntY() < 0 || pos.IntY() > MAPSIZE_Y - 1)
	{
		return;
	}

	mAllTile[pos.IntY()][pos.IntX()]->WallType(type);
}

void RandomMap::SetColumnAt(int x, int y)
{
	if (x < 0 || x > MAPSIZE_X - 1 ||
		y < 0 || y > MAPSIZE_Y - 1)
	{
		return;
	}

	mAllTile[y][x]->WallType(WT_COLUMN);
	mAllTile[y][x-1]->WallType(WT_COLUMNSHADOW1);
	mAllTile[y-1][x-1]->WallType(WT_COLUMNSHADOW2);
}


void RandomMap::SetWallAroundAt(int x, int y)
{
	// 1. x, y 타일 주변 8방향을 모두 벽으로 만든다.
	for (int i = 0; i < DT_MAX; i++)
	{
		MyVector2 newPos = MyVector2(x, y) + MyVector2::ARRDIR[i];

		// 지도 밖이라면 skip
		if (y >= MAPCOUNT_Y || x >= MAPCOUNT_X ||
			y < 0 || x < 0)
		{
			continue;
		}

		// 그 타일에다가
		// DibTile temp = mAllTile[newPos.IntY()][newPos.IntX()];

		// 바꾸려는 타일이 이미 수정되었다면
		// 바꾸려는 타일이 바닥이라면...
		// 바꾸려는 타일이 지도 밖에 있다면
		// SKIP
		if (true == IsFloor(newPos) ||
			true == IsUpstair(newPos) ||
			true == IsDownstair(newPos) ||			
			/*TT_INWALL != mAllTile[newPos.IntY()][newPos.IntX()]->TileType() ||*/
			x < 0 || x >= MAPSIZE_X ||
			y < 0 || y >= MAPSIZE_Y)
		{
			continue;
		}

		//// 만들려는 곳이 코너라면 겹쳐져야한다.
		//if (TT_RIGHTTOPWALL == mAllTile[newPos.IntY()][newPos.IntX()].TileType() ||
		//	TT_RIGHTDOWNWALL == mAllTile[newPos.IntY()][newPos.IntX()].TileType() ||
		//	TT_LEFTTOPWALL == mAllTile[newPos.IntY()][newPos.IntX()].TileType() ||
		//	TT_LEFTDOWNWALL == mAllTile[newPos.IntY()][newPos.IntX()].TileType())
		//{

		//}
	
		switch (i)
		{
		case 0:
 			SetWallAt(newPos, WT_LEFTWALL);
			break;
		case 2:
			SetWallAt(newPos, WT_DOWNWALL);
			break;
		case 4:
			SetWallAt(newPos, WT_RIGHTWALL);
			break;
		case 6:
			SetWallAt(newPos, WT_TOPWALL);
			break;
		case 1:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_LEFTDOWNWALL);
			}
			break;
		case 3:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_RIGHTDOWNWALL);
			}
			break;
		case 5:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_RIGHTTOPWALL);
			}
			break;
		case 7:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_LEFTTOPWALL);
			}		
			break;
		}
		
		// 만들려는 곳이 일반 벽
		if (0 == i || 2 == i || 4 == i || 6 == i)
		{
			
		}
		else
		{

		}		
	}

	SetFloorAt(x, y);
}

void RandomMap::SetBWallAroundAt(int x, int y)
{
	// 1. x, y 타일 주변 8방향을 모두 벽으로 만든다.
	for (int i = 0; i < DT_MAX; i++)
	{
		MyVector2 newPos = MyVector2(x, y) + MyVector2::ARRDIR[i];

		// 지도 밖이라면 skip
		if (y >= MAPCOUNT_Y || x >= MAPCOUNT_X ||
			y < 0 || x < 0)
		{
			continue;
		}

		// 그 타일에다가
		// DibTile temp = mAllTile[newPos.IntY()][newPos.IntX()];

		// 바꾸려는 타일이 이미 수정되었다면
		// 바꾸려는 타일이 바닥이라면...
		// 바꾸려는 타일이 지도 밖에 있다면
		// SKIP




		switch (i)
		{
		case 0:
			SetWallAt(newPos, WT_LEFTWALL1_B);
			break;
		case 2:
			SetWallAt(newPos, WT_TOPWALL);
			break;
		case 4:
			SetWallAt(newPos, WT_LEFTWALL);
			break;
		case 6:
			SetWallAt(newPos, WT_TOPWALL1_B);
			break;
		case 1:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_LEFTDOWNWALL);
			}
			break;
		case 3:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_RIGHTDOWNWALL);
			}
			break;
		case 5:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_RIGHTTOPWALL);
			}
			break;
		case 7:
			if (false == IsWall(newPos))
			{
				SetCornerAt(newPos, WT_LEFTTOPWALL_B);
			}
			break;
		}


	}
}

void RandomMap::SetWallAroundAt(MyVector2 pos)
{
}



bool RandomMap::CheckIsRoom(MyVector2& roomPos, MyVector2& roomSize)
{
	MyVector2 size = roomSize;

	MyVector2 basePos = roomPos;

	for (int i = 0; i < mAllRoom.size(); ++i)
	{
		size = roomSize;
		basePos = roomPos;
		for (int y = 0; y < size.Y + 1; y++)
		{
			MyVector2 travellerPos = basePos;
			for (int x = 0; x < size.X + 1; x++)
			{
				int tempSizeY = mAllRoom[i]->Size.IntY();
				int tempSizeX = mAllRoom[i]->Size.IntX();
				int tempPosX = mAllRoom[i]->Pos.IntX();
				int tempPosY = mAllRoom[i]->Pos.IntY();

				// 범위 안에 들어오면
				if (tempPosX <= travellerPos.X &&
					tempPosY <= travellerPos.Y &&
					tempPosX + tempSizeX >= travellerPos.X &&
					tempPosY + tempSizeY >= travellerPos.Y)
				{
					// 겹쳐있다는 뜻
					return true;
				}
				travellerPos += MyVector2::RIGHT;
			}
			basePos = basePos + MyVector2::DOWN;
		}
	}

	// 안겹쳤다
	return false;
}

MyVector2 RandomMap::CheckNorthWall(MyVector2 & roomPos, MyVector2 & roomSize)
{
	int count = 0;
	int limit = 3;

	int x = 0;

	for (; x < roomSize.IntX(); x++)
	{
		if (roomPos.IntX() + x + 1 >= MAPCOUNT_X ||
			roomPos.IntX() + x + 1 < 0 ||
			roomPos.IntX() + x + 2 >= MAPCOUNT_X ||
			roomPos.IntX() + x + 2 < 0 ||
			roomPos.IntY() - 1 < 0)
		{
			MyVector2 returnValue = { -1.f, -1.f };
			return returnValue;
		}

		WALL_TYPE a = mAllTile[roomPos.IntY()][roomPos.IntX() + x]->WallType();
		WALL_TYPE b = mAllTile[roomPos.IntY()][roomPos.IntX() + x]->WallType();

		if (WT_BOTTOMDOOR == mAllTile[roomPos.IntY()][roomPos.IntX() + x + 1]->WallType() ||
			WT_TOPDOOR == mAllTile[roomPos.IntY()][roomPos.IntX() + x + 1]->WallType() ||
			TT_FLOOR == mAllTile[roomPos.IntY()][roomPos.IntX() + x + 1]->TileType()
			)
		{
			break;
		}

		if (WT_BOTTOMDOOR == mAllTile[roomPos.IntY() - 1][roomPos.IntX() + x + 1]->WallType() ||
			WT_TOPDOOR == mAllTile[roomPos.IntY() - 1][roomPos.IntX() + x + 1]->WallType() ||
			TT_FLOOR == mAllTile[roomPos.IntY() - 1][roomPos.IntX() + x + 1]->TileType() ||
			WT_FLOOR == mAllTile[roomPos.IntY() - 1][roomPos.IntX() + x + 1]->WallType()
			)
		{
			count = 0;
			break;
		}

		++count;
	}

	int random = 0;

	if (count < limit)
	{
		MyVector2 returnValue = { -1.f, -1.f };
		return returnValue;
	}
	else
	{
		if (0 != (limit - count))
		{
			int index = limit - count;

			if (index < 0)
			{
				index = index * -1;
			}

			random = rand() % (index);

		}
	}

	MyVector2 stairPosition = { roomPos.IntX() + x - 3 - random, roomPos.IntY() };

	return stairPosition;
}

// 인자로 넣은 타일이 주변에 있는지 확인
bool RandomMap::CheckIsTile(int x, int y, WALL_TYPE type)
{
	if (type == mAllTile[y][x - 1]->WallType() ||		   // l
		type == mAllTile[y + 1][x - 1]->WallType() ||	   // lb
		type == mAllTile[y - 1][x - 1]->WallType() ||	   // lt
		type == mAllTile[y][x + 1]->WallType() ||		   // r	   //
		type == mAllTile[y + 1][x + 1]->WallType() ||	   // rb
		type == mAllTile[y + 1][x]->WallType() ||		   // b
		type == mAllTile[y - 1][x]->WallType() ||          // t
		type == mAllTile[y - 1][x + 1]->WallType())
	{
		 return true;
	}


	return false;
}

// 인자로 넣은 타일이 주변에 있는지 확인
bool RandomMap::CheckIsTile(int x, int y, TILE_TYPE type)
{
	if (type == mAllTile[y][x - 1]->TileType() ||		   // l
		type == mAllTile[y + 1][x - 1]->TileType() ||	   // lb
		type == mAllTile[y - 1][x - 1]->TileType() ||	   // lt
		type == mAllTile[y][x + 1]->TileType() ||		   // r	   //
		type == mAllTile[y + 1][x + 1]->TileType() ||	   // rb
		type == mAllTile[y + 1][x]->TileType() ||		   // t
		type == mAllTile[y + 1][x]->TileType() ||           // b
		type == mAllTile[y - 1][x]->TileType() ||
		type == mAllTile[y - 1][x + 1]->WallType())
	{
		return true;
	}

	return false;
}



bool RandomMap::IsFloor(int x, int y)
{
	if (x < 0 || x >= MAPSIZE_X ||
		y < 0 || y >= MAPSIZE_Y)
	{
		return false;
	}

	if (TT_FLOOR == mAllTile[y][x]->TileType())
	{
		return true;
	}

	return false;
}

bool RandomMap::IsFloor(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntX() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	if (TT_FLOOR == mAllTile[pos.IntY()][pos.IntX()]->TileType())
	{
		return true;
	}

	return false;
}

bool RandomMap::IsWall(int x, int y)
{
	if (x < 0 || x >= MAPSIZE_X ||
		y < 0 || y >= MAPSIZE_Y)
	{
		return false;
	}

	if (TT_WALL == mAllTile[y][x]->TileType())
	{
		return true;
	}

	return false;
}

bool RandomMap::IsWall(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntX() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	if (TT_WALL == mAllTile[pos.IntY()][pos.IntX()]->TileType() ||
		TT_CORNER == mAllTile[pos.IntY()][pos.IntX()]->TileType() )
	{
		return true;
	}

	return false;
}

bool RandomMap::IsRoom(int x, int y)
{
	if (x < 0 || x >= MAPSIZE_X ||
		y < 0 || y >= MAPSIZE_Y)
	{
		return false;
	}

	if (nullptr == mAllTile[y][x]->GetRoom())
	{
		return false;
	}

	return true;
}

bool RandomMap::IsRoom(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	if (nullptr == mAllTile[pos.IntY()][pos.IntX()]->GetRoom())
	{
		return false;
	}

	return true;
}

bool RandomMap::IsUpstair(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntX() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	return mAllTile[pos.IntY()][pos.IntX()]->IsUpstair();
}

bool RandomMap::IsDownstair(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntX() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	return mAllTile[pos.IntY()][pos.IntX()]->IsDownstair();
}


















RandomMap& RandomMap::CopyMap(RandomMap * map)
{
	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		for (int j = 0; j < MAPCOUNT_X; j++)
		{
			mAllTile[i][j] = map->mAllTile[i][j];
		}
	}

	return *this;
}



WALL_TYPE RandomMap::StyleFloor()
{
	int random = RandomNumber::GetRandomNumber(1, 9);

	switch (random)
	{
	case 1:
		return WT_STONE1;
		break;
	case 2:
		return WT_STONE2;
		break;
	case 3:
		return WT_STONE3;
		break;

	case 4:	
		return WT_STONE4;
		break;
	case 5:
		return WT_STONE5;
		break;
	case 6:
		return WT_STONE6;
		break;
	case 7:
		return WT_BLOOD1;
		break;
	case 8:
		return WT_BLOOD2;
		break;
	case 9:
		return WT_BLOOD3;
		break;
	default:
		break;
	}
	BOOM;
	return WALL_TYPE();
}


WALL_TYPE RandomMap::StyleLeftWall()
{
	int random = RandomNumber::GetRandomNumber(1, 8);

	switch (random)
	{
	case 1:
		return WT_BLOOD_LEFTWALL1;
			break; 	
	case 2:
		return WT_BLOOD_LEFTWALL2;
			break;		
	case 3:	   		
		return WT_STATUE_LEFTWALL1;
		break;			
	case 4:
		return WT_STATUE_LEFTWALL2;
		break;
	case 5:
		return WT_STATUE_LEFTWALL3;
		break;
	case 6:
		return WT_STATUE_LEFTWALL4;
		break;
	case 7:
		return WT_BROKEN_LEFTWALL1;
		break;
	case 8:
		return WT_BROKEN_LEFTWALL2;
		break;

	default:
		break;
	}
	BOOM;
	return WALL_TYPE();
}

WALL_TYPE RandomMap::StyleTopWall()
{
	int random = RandomNumber::GetRandomNumber(1, 10);

	switch (random)
	{
	case 1:
		return WT_BLOOD_TOPWALL1;
		break;
			
	case 2:
		return WT_BLOOD_TOPWALL2;
		break;

	case 3:
		return WT_BLOOD_TOPWALL3;			
		break;
	case 4:
		return WT_STATUE_TOPWALL1;
		break; 
	case 5:			
		return WT_STATUE_TOPWALL2;
		break;	
	case 6:			
		return WT_STATUE_TOPWALL3;
		break;
	case 7:
		return WT_STATUE_TOPWALL4;
		break;
	case 8:
		return WT_BROKEN_TOPWALL1;
		break;
	case 9:
		return WT_BROKEN_TOPWALL2;
		break;
	case 10:
		return WT_BROKEN_TOPWALL3;
		break;

	default:
		break;
	}
	BOOM;
	return WALL_TYPE();
}


void RandomMap::SetAllShadow()
{
	for (size_t y = 0; y < MAPCOUNT_Y; y++)
	{
		for (size_t x = 0; x < MAPCOUNT_X; x++)
		{
			
			TILE_TYPE temp1;
			TILE_TYPE temp2;
			if (x == 14 && y == 17)
			{
				int a = 0;
				temp1 = mAllTile[y - 1][x - 1]->TileType();
				temp2 = mAllTile[y][x - 1]->TileType();
			}

			if (WT_LEFTDOWNWALL == mAllTile[y][x]->WallType() ||
				WT_LASTRIGHT1 == mAllTile[y][x]->WallType() ||
				WT_LASTLEFT2 == mAllTile[y][x]->WallType() )
			{
				if (//WT_FLOOR != mAllTile[y][x - 1]->WallType() ||
					TT_WALL != mAllTile[y][x - 1]->TileType())
				{
					if (WT_TOPWALL == mAllTile[y - 1][x - 1]->WallType())
					{
						mAllTile[y][x - 1]->WallType(WT_LDSHADOW2);
						mAllTile[y - 1][x - 1]->WallType(WT_TOPSHADOW);
					}
					else if (WT_LASTRIGHT1 == mAllTile[y - 1][x - 1]->WallType() ||
						WT_LEFTDOWNWALL == mAllTile[y - 1][x - 1]->WallType() )
					{
						mAllTile[y][x - 1]->WallType(WT_LDSHADOW2);
						mAllTile[y - 1][x - 1]->WallType(WT_RIGHTTOPSHADOW);
					}					
					else if (//WT_FLOOR == mAllTile[y - 1][x - 1]->WallType() ||
						TT_WALL != mAllTile[y - 1][x - 1]->TileType())
					{
						mAllTile[y][x - 1]->WallType(WT_LDSHADOW2);
						mAllTile[y - 1][x - 1]->WallType(WT_LDSHADOW1);
					}
				}

			}
			else if (WT_CANDLE == mAllTile[y][x]->WallType())
			{
				mAllTile[y][x - 1]->WallType(WT_CANDLESHADOW2);
				mAllTile[y - 1][x - 1]->WallType(WT_CANDLESHADOW1);
			}
			else if (WT_BIGBOX == mAllTile[y][x]->WallType())
			{
				mAllTile[y][x - 1]->WallType(WT_BOXSHADOW);
			}
		}
	}
}

void RandomMap::PlaceBox(int x, int y, WALL_TYPE tile)
{
	// 주변에 벽이 없고 박스가 없으면
	if (false == CheckIsTile(x, y, TT_WALL) &&
		false == CheckIsTile(x, y, tile) &&
		false == CheckIsTile(x, y, TT_STAIR))
	{
		mAllTile[y][x]->WallType(tile);
	}
}

void RandomMap::SetIronWallX(MyVector2 basePos, int size)
{
	for (size_t i = 0; i < size; ++i)
	{
		if (basePos.IntY() - 1 < 0 || basePos.IntX() + i > 40)
		{
			return;
		}

		if (basePos.IntY() + i >= 40 || basePos.IntX() + i >= 40)
		{
			return;
		}

		WALL_TYPE temp1 = mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType();
		WALL_TYPE temp2 = mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->WallType();

		if (WT_TOPWALL == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_TOPWALL);
		}
		else if (WT_TOPWALL == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->TileType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_TOPWALL);
		}
		else if (WT_TOPWALL == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_TOPWALL);
		}

		else if (WT_DOWNWALL == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_DOWNWALL);
		}
		else if (WT_DOWNWALL == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() + i]->TileType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_DOWNWALL);
		}
		// 위
		else if (WT_LEFTTOP == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->TileType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_LEFTTOP_RIGHT);
		}
		else if (WT_LEFTTOP == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() -  1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_LEFTTOP_RIGHT);
		}

		else if (WT_LEFTTOP == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_LEFTTOP_RIGHT);
		}
		else if (WT_LEFTTOP == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_IRON_LEFTWALL == mAllTile[basePos.IntY() - 1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_LEFTTOP_RIGHT);
		}

		// 아래
		else if (WT_LEFTDOWN == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() + i]->WallType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_LEFTDOWN);
		}
		else if (WT_LEFTDOWN == mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType() &&
			 TT_FLOOR == mAllTile[basePos.IntY() + 1][basePos.IntX() + i]->TileType())
		{
			mAllTile[basePos.IntY()][basePos.IntX() + i]->WallType(WT_IRON_LEFTDOWN);
		}
	}
}

void RandomMap::SetIronWallY(MyVector2 basePos, int size)
{
	for (size_t i = 0; i < size; ++i)
	{
		if (basePos.IntY() + i >= 40 || basePos.IntX() + i >= 40)
		{
			return;
		}

		if (basePos.IntY() - 1 < 0 || basePos.IntX() + 1 >= 40)
		{
			return;
		}

		WALL_TYPE temp1 = mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType();
		WALL_TYPE temp2 = mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType();

		if (WT_RIGHTWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() + 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTWALL);
		}
		else if (WT_RIGHTWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() + 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTWALL);
		}		

		else if (WT_LEFTWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTWALL);
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTWALL);
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_LASTRIGHT1 == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTWALL);
		}
		else if (WT_LEFTWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_TOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTWALL);
		}
		

		else if (WT_RIGHTDOWN == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() + 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}
		else if (WT_RIGHTDOWN == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() + 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}

		else if (WT_LASTLEFT1 == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}
		else if (WT_LASTLEFT1 == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}

		// SPECIAL
		else if (WT_LEFTTOP == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_IRON_LEFTWALL == mAllTile[basePos.IntY() + i - 1][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			//mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTTOP_RIGHT);
		}
		else if (WT_LEFTTOP == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_IRON_LEFTWALL == mAllTile[basePos.IntY() + i - 1][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			//mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTTOP_RIGHT);
		}

		else if (WT_LEFTTOP == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTTOP_LEFT);
		}
		else if (WT_LEFTTOP == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTTOP_LEFT);
		}
		else if (WT_LEFTTOP == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_TOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			if (mAllTile[basePos.IntY() + i][basePos.IntX()]->mRoom == mAllTile[basePos.IntY() + i][basePos.IntX() - 2]->mRoom)
			{
				mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTTOP_LEFT);
			}		
		}

		// 하나짜리
		else if (WT_RIGHTTOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i - 1][basePos.IntX() - 1]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}
		else if (WT_RIGHTTOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i - 1][basePos.IntX() - 1]->TileType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}
		else if (WT_RIGHTTOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i - 1][basePos.IntX() - 1]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}
		else if (WT_RIGHTTOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i - 1][basePos.IntX() - 1]->TileType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_RIGHTDOWN);
		}


		else if (WT_RIGHTTOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			WT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->WallType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTWALL);
		}
		else if (WT_RIGHTTOPWALL == mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType() &&
			TT_FLOOR == mAllTile[basePos.IntY() + i][basePos.IntX() - 1]->TileType())
		{
			mAllTile[basePos.IntY() + i][basePos.IntX()]->WallType(WT_IRON_LEFTWALL);
		}
	}
}


bool RandomMap::UpgradeIronWall()
{
	for (size_t i = 0; i < mAllRoom.size(); ++i)
	{
		int random = RandomNumber::GetRandomNumber(1, 4);
	
		switch (random)
		{
		case 1:
		// 위
				MakeIronWall(  mAllRoom[i]->LeftTop(), mAllRoom[i]->RightTop(),  RT_TOP);

			break;
		case 2:
			// 아래
				MakeIronWall(mAllRoom[i]->LeftDown(), mAllRoom[i]->RightDown(), RT_BOTTOM);

			break;
		case 3:
			// 왼쪽s
				MakeIronWall(mAllRoom[i]->LeftTop(), mAllRoom[i]->LeftDown(), RT_LEFT);

			break;
		case 4:
			// 오른
				MakeIronWall(mAllRoom[i]->RightTop(), mAllRoom[i]->RightDown(), RT_RIGHT);

			break;
		default:
			BOOM;
		}
	}

	return false;
}


bool RandomMap::MakeIronWall(MyVector2 basePos, MyVector2 size, ROOM_TYPE type)
{
	int x = size.IntX() - basePos.IntX();
	int y = size.IntY() - basePos.IntY();
	int z = 0;

	if (0 >= size.IntX() ||
		0 >= size.IntY() ||
		MAPCOUNT_Y <= size.IntY() + 1 ||
		MAPCOUNT_X < size.IntX() + 1)
	{
		return false;
	}

	if (0 >= basePos.IntX() ||
		0 >= basePos.IntY() ||
		MAPCOUNT_Y <= basePos.IntY() + 1 ||
		MAPCOUNT_X < basePos.IntX() + 1)
	{
		return false;
	}
	
	switch (type)
	{
	case RT_LEFT:
		SetIronWallY(basePos, y);
		break;
	case RT_BOTTOM:
		SetIronWallX(basePos, x);

		break;
	case RT_RIGHT:
		SetIronWallY(basePos, y);

		break;
	case RT_TOP:
		SetIronWallX(basePos, x);

		break;
	case RT_MAX:
		break;
	default:
		break;
	}

	return true;
}


void RandomMap::UpgradeWallStyle()
{
	WALL_TYPE temp;

	for (size_t y = 0; y < MAPCOUNT_Y; y++)
	{
		for (size_t x = 0; x < MAPCOUNT_X; x++)
		{
			if (WT_FLOOR1_B == mAllTile[y][x]->WallType() ||
				WT_FLOOR2_B == mAllTile[y][x]->WallType() ||
				WT_FLOOR3_B == mAllTile[y][x]->WallType() ||
				WT_FLOOR4_B == mAllTile[y][x]->WallType())
			{
				continue;
			}

			if (TT_FLOOR == mAllTile[y][x]->TileType() ||
				WT_FLOOR == mAllTile[y][x]->WallType())
			{
				int random = RandomNumber::GetRandomNumber(1, 20);

				if (15 <= random)
				{
					temp = StyleFloor();

					mAllTile[y][x]->FloorType(temp);
				}
				else if (13 <= random)
				{
					// PlaceBox((int)x, (int)y, WT_CANDLE);
				}
				else if (11 <= random)
				{
					// PlaceBox((int)x, (int)y, WT_BIGBOX);
				}
			}
			else if (WT_LEFTWALL1_B == mAllTile[y][x]->WallType() ||
				WT_LEFTWALL2_B == mAllTile[y][x]->WallType() ||
				WT_TOPWALL1_B == mAllTile[y][x]->WallType() ||
				WT_TOPWALL2_B == mAllTile[y][x]->WallType() ||
				WT_LEFTTOPWALL_B == mAllTile[y][x]->WallType())
			{
				continue;
			}
			else if (WT_LEFTWALL == mAllTile[y][x]->WallType())
			{
				int random = RandomNumber::GetRandomNumber(1, 20);
				temp = StyleLeftWall();
				if (12 <= random)
				{
					mAllTile[y][x]->WallType(temp);
				}
			}
			else if (WT_TOPWALL == mAllTile[y][x]->WallType())
			{
				int random = RandomNumber::GetRandomNumber(1, 20);
				temp = StyleTopWall();
				if (12 <= random)
				{
					mAllTile[y][x]->WallType(temp);
				}
			}
		}
	}
}


bool RandomMap::FinishMap()
{
	bool check = SetUpstair();

	if (false == check)
	{
		return check;
	}

	UpgradeIronWall();
	UpgradeWallStyle();
	SetAllShadow();

	SetImageIndex();

	SetColliderMap();

	CopyAll();
	SetCopyImageIndex();

	/////////////////////////////////// SetStartRoom
	SetStartRoom();
	
	return true;
}



void RandomMap::SetColliderMap()
{
	for (size_t y = 0; y < MAPCOUNT_Y; ++y)
	{
		for (size_t x = 0; x < MAPCOUNT_X; ++x)
		{
			int newX = (int)x * 2;
			int newY = (int)y * 2;



			if (TT_FLOOR == mAllTile[y][x]->TileType())
			{
				AddAll(newX, newY);
			}


			WALL_TYPE temp = mAllTile[y][x]->WallType();

			switch (temp)
			{

			case WT_LEFTDOWN:
			case WT_RIGHTDOWN:
			case WT_RIGHTTOP:
			case WT_LEFTTOPWALL:

			case WT_UP1:
			case WT_UP3:
			case WT_DOWN2:

			case WT_RIGHTWALL:
			case WT_DOWNWALL:

			case WT_NONE:
				AddNone(newX, newY);
				break;

			case WT_FLOOR:		
			case WT_UP5:
			case WT_UP7:
			case WT_UP8:
			case WT_UP9:
			case WT_DOWN1:
			case WT_DOWN4:
			case WT_LDSHADOW1:
			case WT_LDSHADOW2:
			case WT_TOPSHADOW:
			case WT_RIGHTTOPSHADOW:
			case WT_CANDLESHADOW1:
			case WT_CANDLESHADOW2:
			case WT_BOXSHADOW:
			case WT_STONE1:
			case WT_STONE2:
			case WT_STONE3:
			case WT_STONE4:
			case WT_STONE5:
			case WT_STONE6:
			case WT_BLOOD1:
			case WT_BLOOD2:
			case WT_BLOOD3:
			case WT_ARCH_TOPSHADOW:
			case WT_ARCH_LASTTOP_SHADOW:
			case WT_ARCH_LDSHADOW3:
			case WT_ARCH_RIGHTDOWNWALL_SHADOW:
			case WT_COLUMNSHADOW1:
			case WT_COLUMNSHADOW2:
				AddAll(newX, newY);
				break;


			case WT_LEFTWALL:
			case WT_LASTLEFT1:
			case WT_RIGHTTOPWALL:
			case WT_DOWN3:
			case WT_BIGBOX:
			case WT_BLOOD_LEFTWALL1:
			case WT_BLOOD_LEFTWALL2:
			case WT_STATUE_LEFTWALL1:
			case WT_STATUE_LEFTWALL2:
			case WT_STATUE_LEFTWALL3:
			case WT_STATUE_LEFTWALL4:
			case WT_BROKEN_LEFTWALL1:
			case WT_BROKEN_LEFTWALL2:
			case WT_IRON_LEFTWALL:
			case WT_IRON_RIGHTWALL:
			case WT_IRON_RIGHTDOWN:
			case WT_ARCH_LEFTWALL:
			case WT_ARCH_LEFTWALL_SHADOW:
			case WT_ARCH_RIGHTTOPWALL:
				AddLeft(newX, newY);
				break;

			case WT_TOPWALL:

			case WT_LASTRIGHT1:
			case WT_LEFTDOWNWALL:
			case WT_UP2:
			case WT_UP4:
			case WT_UP6:
			case WT_BLOOD_TOPWALL1:
			case WT_BLOOD_TOPWALL2:
			case WT_BLOOD_TOPWALL3:
			case WT_STATUE_TOPWALL1:
			case WT_STATUE_TOPWALL2:
			case WT_STATUE_TOPWALL3:
			case WT_STATUE_TOPWALL4:
			case WT_BROKEN_TOPWALL1:
			case WT_BROKEN_TOPWALL2:
			case WT_BROKEN_TOPWALL3:
			case WT_IRON_TOPWALL:
			case WT_IRON_DOWNWALL:
			case WT_IRON_LEFTDOWN:
			case WT_ARCH_TOPWALL:
				AddRight(newX, newY);
				break;

			case WT_LASTRIGHT2:
			case WT_LASTLEFT2:

			case WT_DOWN5: // 계단 내려가기 이벤트 발동
			case WT_DOWN6:
			case WT_LEFTDOOR:
			case WT_TOPDOOR:
			case WT_RIGHTDOOR:
			case WT_BOTTOMDOOR:
			case WT_CANDLE:
			case WT_ARCH_LEFT:
			case WT_ARCH_LASTLEFT:
			case WT_ARCH_TOP:
			case WT_ARCH_LASTTOP:
			case WT_ARCH_LEFTTOP:
			case WT_ARCH_TOPSHADOWWALL:
			case WT_ARCH_LASTTOP_SHADOW2:
			case WT_ARCH_TOPWALL_SHADOW:
			case WT_COLUMN:
			case WT_RIGHTDOWNWALL:
			case WT_BDOOR:
				AddLT(newX, newY);
				break;



		
			case WT_IRON_LEFTTOP_LEFT:
			case WT_IRON_LEFTTOP_RIGHT:
			case WT_LEFTTOP:
				AddT(newX, newY);
				break;

			case WT_MAX:
				BOOM;
				break;
			default:
				break;
			}
		}
	}

	int a = 0;


}

// 벽이 왼쪽이라는 뜻
void RandomMap::AddLeft(int x, int y)
{
	mColliderMap->AddTile(x, y);
	mColliderMap->AddTile(x + 1, y);
	mColliderMap->AddTile(x, y + 1);
	mColliderMap->AddTile(x + 1, y + 1);

	mColliderMap->SetWalkable(x + 1, y);
	mColliderMap->SetWalkable(x + 1, y + 1);
}

void RandomMap::AddRight(int x, int y)
{
	mColliderMap->AddTile(x, y);
	mColliderMap->AddTile(x + 1, y);
	mColliderMap->AddTile(x, y + 1);
	mColliderMap->AddTile(x + 1, y + 1);


	mColliderMap->SetWalkable(x, y + 1);
	mColliderMap->SetWalkable(x + 1, y + 1);
}

void RandomMap::AddAll(int x, int y)
{
	mColliderMap->AddTile(x, y);
	mColliderMap->AddTile(x + 1, y);
	mColliderMap->AddTile(x, y + 1);
	mColliderMap->AddTile(x + 1, y + 1);

	mColliderMap->SetWalkable(x, y);
	mColliderMap->SetWalkable(x + 1, y);
	mColliderMap->SetWalkable(x, y + 1);
	mColliderMap->SetWalkable(x + 1, y + 1);
}

void RandomMap::AddLT(int x, int y)
{
	mColliderMap->AddTile(x, y);
	mColliderMap->AddTile(x + 1, y);
	mColliderMap->AddTile(x, y + 1);
	mColliderMap->AddTile(x + 1, y + 1);

	mColliderMap->SetWalkable(x + 1, y);
	mColliderMap->SetWalkable(x, y + 1);
	mColliderMap->SetWalkable(x + 1, y + 1);
}

void RandomMap::AddT(int x, int y)
{
	mColliderMap->AddTile(x, y);
	mColliderMap->AddTile(x + 1, y);
	mColliderMap->AddTile(x, y + 1);
	mColliderMap->AddTile(x + 1, y + 1);

	mColliderMap->SetWalkable(x + 1, y + 1);
}

void RandomMap::AddNone(int x, int y)
{
	mColliderMap->AddTile(x, y);
	mColliderMap->AddTile(x + 1, y);
	mColliderMap->AddTile(x, y + 1);
	mColliderMap->AddTile(x + 1, y + 1);
}

MyVector2 RandomMap::SubToDib(int x, int y)
{
	int tempX = (int)(x / 2);
	int tempY = (int)(y / 2);

	return MyVector2(tempX, tempY);
}

void RandomMap::AddSubTile(TileColRenderer* subTile)
{
	TileIndex tempIndex = subTile->GetTileIndex();

	MyVector2 tempDib = SubToDib(tempIndex.X, tempIndex.Y);

	mAllTile[tempDib.IntX()][tempDib.IntY()]->mSubTiles.push_back(subTile);
}

void RandomMap::RenderTile()
{

}

void RandomMap::SetBRoom()
{
	for (size_t i = 7; i < mAllRoom.size(); ++i)
	{
		int sizeX = mAllRoom[i]->Size.IntX();
		int sizeY = mAllRoom[i]->Size.IntY();
		MyVector2 pos = mAllRoom[i]->Pos;
		MyVector2 size = mAllRoom[i]->Size;

		if (sizeX >= 6 && sizeY >= 6)
		{
			MakeButchersRealRoom(pos + 2, { 3, 3 }, pos, size, RT_MAX);

			return;
		}



	}
}

void RandomMap::SetStartRoom()
{
	for (size_t i = 0; i < mAllRoom.size(); i++)
	{
		if (mAllRoom[i]->LeftTop().IntX() * 2 <= mStartPos1.X && mAllRoom[i]->RightTop().IntX() * 2 >= mStartPos1.X)
		{
			if (mAllRoom[i]->LeftTop().IntY() * 2 <= mStartPos1.Y + 8 && mAllRoom[i]->RightDown().IntY() * 2 >= mStartPos1.Y)
			{
				mAllRoom[i]->mIsStartRoom = true;
				mStartRoom = mAllRoom[i];
				break;
			}
		}
	}
}

void RandomMap::SetImageIndex()
{
	int index = 0;

	for (size_t y = 0; y < MAPCOUNT_Y; ++y)
	{
		for (size_t x = 0; x < MAPCOUNT_X; ++x)
		{
			WALL_TYPE tempWall = mAllTile[y][x]->WallType();
			TILE_TYPE tempTile = mAllTile[y][x]->TileType();

			int style = 0;

			switch (tempTile)
			{
			case TT_INWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_INWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_INWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_INWALL);
				index = (int)II_INWALL;
				break;
			case TT_WALL:
				break;
			case TT_CORNER:
				break;
			case TT_FLOOR:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_FLOOR);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR);
				index = (int)II_FLOOR;
				break;
			case TT_LEFT_DOOR:
				break;
			case TT_BOTTOM_DOOR:
				break;
			case TT_RIGHT_DOOR:
				break;
			case TT_TOP_DOOR:
				break;
			case TT_STAIR:
				break;
			case TT_PILLAR:
				break;
			case TT_MAX:
				// // mAllTile[y][x]->mTileRenderer->Off();
				BOOM;
				break;
			default:
				break;
			}

			switch (tempWall)
			{
			case WT_NONE:
				break;
			case WT_FLOOR:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_FLOOR);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR);
				index = (int)II_FLOOR;
				break;
			case WT_LEFTWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTWALL);
				index = (int)II_LEFTWALL;
				break;
			case WT_RIGHTWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTWALL);
				index = (int)II_RIGHTWALL;

				break;
			case WT_TOPWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_TOPWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPWALL);
				index = (int)II_TOPWALL;
				break;
			case WT_DOWNWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWNWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWNWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWNWALL);
				index = (int)II_DOWNWALL;
				break;
			case WT_COLUMN:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_COLUMN);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_COLUMN);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_COLUMN);
				index = (int)II_COLUMN;
				break;
			case WT_COLUMNSHADOW1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_COLUMNSHADOW1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_COLUMNSHADOW1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_COLUMNSHADOW1);
				index = (int)II_COLUMNSHADOW1;
				break;
			case WT_COLUMNSHADOW2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_COLUMNSHADOW2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_COLUMNSHADOW2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_COLUMNSHADOW2);
				index = (int)II_COLUMNSHADOW2;
				break;
			case WT_LEFTDOWN:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTDOWN);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTDOWN);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTDOWN);
				index = (int)II_LEFTDOWN;
				break;
			case WT_RIGHTDOWN:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTDOWN);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOWN);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOWN);
				index = (int)II_RIGHTDOWN;
				break;
			case WT_RIGHTTOP:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTTOP);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTTOP);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTTOP);
				index = (int)II_RIGHTTOP;
				break;
			case WT_LEFTTOP:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTTOP);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTTOP);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTTOP);
				index = (int)II_LEFTTOP;
				break;
			case WT_LEFTDOWNWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTDOWNWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTDOWNWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTDOWNWALL);
				index = (int)II_LEFTDOWNWALL;
				break;
			case WT_RIGHTDOWNWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTDOWNWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOWNWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOWNWALL);
				index = (int)II_RIGHTDOWNWALL;
				break;
			case WT_RIGHTTOPWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTTOPWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTTOPWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTTOPWALL);
				index = (int)II_RIGHTTOPWALL;
				break;
			case WT_LEFTTOPWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTTOPWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTTOPWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTTOPWALL);
				index = (int)II_LEFTTOPWALL;
				break;

				////////////////////////// 문
			case WT_LEFTDOOR:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTDOOR);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTDOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTDOOR);
				index = (int)II_LEFTDOOR;
				break;
			case 	WT_BOTTOMDOOR:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BOTTOMDOOR);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BOTTOMDOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BOTTOMDOOR);
				index = (int)II_BOTTOMDOOR;
				break;
			case 	WT_RIGHTDOOR:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTDOOR);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOOR);
				index = (int)II_RIGHTDOOR;
				break;
			case 	WT_TOPDOOR:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_TOPDOOR);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPDOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPDOOR);
				index = (int)II_TOPDOOR;
				break;

				///////////////////////////// 아치
			case WT_ARCH_LEFT:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFT);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFT);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFT);
				index = (int)II_ARCH_LEFT;
				break;
			case WT_ARCH_TOP:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOP);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOP);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOP);
				index = (int)II_ARCH_TOP;
				break;
			case WT_ARCH_LEFTWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFTWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFTWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFTWALL);
				index = (int)II_ARCH_LEFTWALL;
				break;
			case WT_ARCH_LASTLEFT:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTLEFT);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTLEFT);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTLEFT);
				index = (int)II_ARCH_LASTLEFT;
				break;
			case WT_ARCH_TOPWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPWALL);
				index = (int)II_ARCH_TOPWALL;
				break;
			case WT_ARCH_LASTTOP:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTTOP);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTTOP);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTTOP);
				index = (int)II_ARCH_LASTTOP;
				break;
			case WT_ARCH_LEFTTOP:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFTTOP);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFTTOP);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFTTOP);
				index = (int)II_ARCH_LEFTTOP;
				break;
			case WT_ARCH_TOPSHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPSHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPSHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPSHADOW);
				index = (int)II_ARCH_TOPSHADOW;
				break;
			case WT_ARCH_TOPSHADOWWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPSHADOWWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPSHADOWWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPSHADOWWALL);
				index = (int)II_ARCH_TOPSHADOWWALL;
				break;
			case WT_ARCH_RIGHTTOPWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_RIGHTTOPWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_RIGHTTOPWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_RIGHTTOPWALL);
				index = (int)II_ARCH_RIGHTTOPWALL;
				break;




				////////////////////////// 계단
			case WT_UP1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP1);
				index = (int)II_UP1;
				break;
			case 	WT_UP2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP2);
				index = (int)II_UP2;
				break;
			case 	WT_UP3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP3);
				index = (int)II_UP3;
				break;
			case 	WT_UP4:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP4);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP4);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP4);
				index = (int)II_UP4;
				break;
			case 	WT_UP5:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP5);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP5);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP5);
				index = (int)II_UP5;
				break;
			case 	WT_UP6:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP6);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP6);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP6);
				index = (int)II_UP6;
				break;
			case 	WT_UP7:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP7);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP7);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP7);
				index = (int)II_UP7;
				break;
			case 	WT_UP8:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP8);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP8);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP8);
				index = (int)II_UP8;
				break;
			case 	WT_UP9:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_UP9);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_UP9);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_UP9);
				index = (int)II_UP9;
				break;

			case 	WT_DOWN1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN1);
				index = (int)II_DOWN1;
				break;
			case 	WT_DOWN2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN2);
				index = (int)II_DOWN2;
				break;
			case 	WT_DOWN3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN3);
				index = (int)II_DOWN3;
				break;
			case 	WT_DOWN4:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN4);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN4);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN4);
				index = (int)II_DOWN4;
				break;
			case 	WT_DOWN5:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN5);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN5);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN5);
				index = (int)II_DOWN5;
				break;
			case 	WT_DOWN6:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN6);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN6);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN6);
				index = (int)II_DOWN6;
				break;


			case WT_LASTLEFT1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTLEFT1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTLEFT1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTLEFT1);
				index = (int)II_LASTLEFT1;
				break;
			case WT_LASTLEFT2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTLEFT2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTLEFT2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTLEFT2);
				index = (int)II_LASTLEFT2;
				break;
			case WT_LASTRIGHT1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTRIGHT1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTRIGHT1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTRIGHT1);
				index = (int)II_LASTRIGHT1;
				break;
			case WT_LASTRIGHT2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTRIGHT2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTRIGHT2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTRIGHT2);
				index = (int)II_LASTRIGHT2;
				break;


				// 그림자

			case WT_LDSHADOW1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LDSHADOW1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LDSHADOW1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LDSHADOW1);
				index = (int)II_LDSHADOW1;
				break;
			case WT_LDSHADOW2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_LDSHADOW2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LDSHADOW2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LDSHADOW2);
				index = (int)II_LDSHADOW2;
				break;
			case WT_TOPSHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_TOPSHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPSHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPSHADOW);
				index = (int)II_TOPSHADOW;
				break;
			case WT_RIGHTTOPSHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTTOPSHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTTOPSHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTTOPSHADOW);
				index = (int)II_RIGHTTOPSHADOW;
				break;
			case WT_CANDLESHADOW1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_CANDLESHADOW1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_CANDLESHADOW1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_CANDLESHADOW1);
				index = (int)II_CANDLESHADOW1;
				break;
			case WT_CANDLESHADOW2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_CANDLESHADOW2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_CANDLESHADOW2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_CANDLESHADOW2);
				index = (int)II_CANDLESHADOW2;
				break;
			case WT_BOXSHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BOXSHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BOXSHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BOXSHADOW);
				index = (int)II_BOXSHADOW;
				break;

				// 바닥 스타일

			case	WT_CANDLE:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_CANDLE);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_CANDLE);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_CANDLE);
				index = (int)II_CANDLE;
				break;
			case	WT_BIGBOX:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BIGBOX);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BIGBOX);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BIGBOX);
				index = (int)II_BIGBOX;
				break;
			case	WT_STONE1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE1);
				index = (int)II_STONE1;
				break;
			case	WT_STONE2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE2);
				index = (int)II_STONE2;
				break;
			case	WT_STONE3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE3);
				index = (int)II_STONE3;
				break;
			case	WT_STONE4:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE4);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE4);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE4);
				index = (int)II_STONE4;
				break;
			case	WT_STONE5:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE5);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE5);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE5);
				index = (int)II_STONE5;
				break;
			case	WT_STONE6:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE6);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE6);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE6);
				index = (int)II_STONE6;
				break;
			case	WT_BLOOD1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD1);
				index = (int)II_BLOOD1;
				break;
			case	WT_BLOOD2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD2);
				index = (int)II_BLOOD2;
				break;

			case	WT_BLOOD3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD3);
				index = (int)II_BLOOD3;
				break;

				// 벽 스타일



			case WT_BLOOD_TOPWALL1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_TOPWALL1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_TOPWALL1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_TOPWALL1);
				index = (int)II_BLOOD_TOPWALL1;
				break;
			case WT_BLOOD_TOPWALL2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_TOPWALL2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_TOPWALL2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_TOPWALL2);
				index = (int)II_BLOOD_TOPWALL2;
				break;
			case WT_BLOOD_TOPWALL3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_TOPWALL3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_TOPWALL3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_TOPWALL3);
				
				index = (int)II_BLOOD_TOPWALL3;
				break;
			case WT_BLOOD_LEFTWALL1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_LEFTWALL1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_LEFTWALL1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_LEFTWALL1);
				index = (int)II_BLOOD_LEFTWALL1;
				break;
			case WT_BLOOD_LEFTWALL2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_LEFTWALL2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_LEFTWALL2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_LEFTWALL2);
				index = (int)II_BLOOD_LEFTWALL2;
				break;

			case WT_STATUE_LEFTWALL1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL1);
				index = (int)II_STATUE_LEFTWALL1;
				break;
			case WT_STATUE_LEFTWALL2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL2);
				index = (int)II_STATUE_LEFTWALL2;
				break;
			case WT_STATUE_LEFTWALL3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL3);
				index = (int)II_STATUE_LEFTWALL3;
				break;
			case WT_STATUE_LEFTWALL4:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL4);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL4);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL4);
				index = (int)II_STATUE_LEFTWALL4;
				break;
			case WT_STATUE_TOPWALL1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL1);
				index = (int)II_STATUE_TOPWALL1;
				break;
			case WT_STATUE_TOPWALL2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL2);
				index = (int)II_STATUE_TOPWALL2;
				break;
			case WT_STATUE_TOPWALL3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL3);
				index = (int)II_STATUE_TOPWALL3;
				break;
			case WT_STATUE_TOPWALL4:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL4);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL4);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL4);
				index = (int)II_STATUE_TOPWALL4;
				break;
			case WT_BROKEN_TOPWALL1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_TOPWALL1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_TOPWALL1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_TOPWALL1);
				index = (int)II_BROKEN_TOPWALL1;
				break;
			case WT_BROKEN_TOPWALL2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_TOPWALL2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_TOPWALL2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_TOPWALL2);
				index = (int)II_BROKEN_TOPWALL2;
				break;
			case WT_BROKEN_TOPWALL3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_TOPWALL3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_TOPWALL3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_TOPWALL3);
				index = (int)II_BROKEN_TOPWALL3;
				break;
			case WT_BROKEN_LEFTWALL1:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_LEFTWALL1);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_LEFTWALL1);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_LEFTWALL1);
				index = (int)II_BROKEN_LEFTWALL1;
				break;
			case WT_BROKEN_LEFTWALL2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_LEFTWALL2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_LEFTWALL2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_LEFTWALL2);
				 index = (int)II_BROKEN_LEFTWALL2;
				 break;
				// 쇠창살
			case WT_IRON_TOPWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_TOPWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_TOPWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_TOPWALL);
				 index = (int)II_IRON_TOPWALL;
				 break;
			case WT_IRON_LEFTWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTWALL);
				 index = (int)II_IRON_LEFTWALL;
				 break;
			case WT_IRON_DOWNWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_DOWNWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_DOWNWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_DOWNWALL);
				 index = (int)II_IRON_DOWNWALL;
				 break;
			case WT_IRON_RIGHTWALL:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_RIGHTWALL);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_RIGHTWALL);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_RIGHTWALL);
				 index = (int)II_IRON_RIGHTWALL;
				 break;
			case WT_IRON_LEFTDOWN:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTDOWN);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTDOWN);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTDOWN);
				 index = (int)II_IRON_LEFTDOWN;
				 break;
			case WT_IRON_RIGHTDOWN:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_RIGHTDOWN);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_RIGHTDOWN);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_RIGHTDOWN);
				 index = (int)II_IRON_RIGHTDOWN;
				 break;
			case WT_IRON_LEFTTOP_LEFT:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTTOP_LEFT);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTTOP_LEFT);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTTOP_LEFT);
				 index = (int)II_IRON_LEFTTOP_LEFT;
				 break;
			case WT_IRON_LEFTTOP_RIGHT:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTTOP_RIGHT);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTTOP_RIGHT);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTTOP_RIGHT);
				 index = (int)II_IRON_LEFTTOP_RIGHT;
				 break;
			case WT_ARCH_LASTTOP_SHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW);
				 index = (int)II_ARCH_LASTTOP_SHADOW;
				 break;
			case WT_ARCH_LASTTOP_SHADOW2:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW2);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW2);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW2);
				 index = (int)II_ARCH_LASTTOP_SHADOW2;
				 break;
			case WT_ARCH_LDSHADOW3:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LDSHADOW3);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LDSHADOW3);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LDSHADOW3);
				 index = (int)II_ARCH_LDSHADOW3;
				 break;
			case WT_ARCH_LEFTWALL_SHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFTWALL_SHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFTWALL_SHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFTWALL_SHADOW);
				 index = (int)II_ARCH_LEFTWALL_SHADOW;
				 break;
			case WT_ARCH_RIGHTDOWNWALL_SHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_RIGHTDOWNWALL_SHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_RIGHTDOWNWALL_SHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_RIGHTDOWNWALL_SHADOW);
				 index = (int)II_ARCH_RIGHTDOWNWALL_SHADOW;
				 break;
			case WT_ARCH_TOPWALL_SHADOW:
				// mAllTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPWALL_SHADOW);
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPWALL_SHADOW);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPWALL_SHADOW);
				 index = (int)II_ARCH_TOPWALL_SHADOW;
				 break;
			case WT_LEFTWALL1_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTWALL1_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTWALL1_B);
				index = (int)II_LEFTWALL1_B;
				break;
			case WT_LEFTWALL2_B :
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTWALL2_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTWALL2_B);
				index = (int)II_LEFTWALL2_B;
				break;
			case WT_TOPWALL1_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPWALL1_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPWALL1_B);
				index = (int)II_TOPWALL1_B;
				break;
			case  WT_TOPWALL2_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPWALL2_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPWALL2_B);
				index = (int)II_TOPWALL2_B;
				break;
			case WT_LEFTTOPWALL_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTTOPWALL_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTTOPWALL_B);
				index = (int)II_LEFTTOPWALL_B;
				break;
			case WT_FLOOR1_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR1_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR1_B);
				index = (int)II_FLOOR1_B;
				break;
			case WT_FLOOR2_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR2_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR2_B);
				index = (int)II_FLOOR2_B;
				break;
			case WT_FLOOR3_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR3_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR3_B);
				index = (int)II_FLOOR3_B;
				break;
			case WT_FLOOR4_B:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR4_B);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR4_B);
				index = (int)II_FLOOR4_B;
				break;

			case WT_BDOOR:
				mAllTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOOR);
				mAllTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOOR);
				index = (int)II_BDOOR;
				break;

			case WT_MAX:
				BOOM;
				break;
			default:
				break;
			}

			

			MyImage* image = MyTextureManager::Inst().FindImage(L"ChurchDungeonTile.bmp");

			MyRect rc = image->GetRect(index);

			// 타일 위치 설정
			float offsetX = (TILE_OFFSETX) / 2.f;
			float offsetY = offsetX / 2.f;

			float startX = MAPSIZE_X / 2.f - offsetX - (y * offsetX);
			float startY = y * offsetY;

			float newX = (startX + x * offsetX);
			float newY = startY + (x * offsetY);

			MyVector2 pos = { newX + BACK_TILE_X / 2, newY };
			MyVector2 size = { TILE_WIDTH, TILE_HEIGHT };

			int transColor = MyGraphic::ColorToInt(255, 255, 255, 0);

			TransparentBlt(mBackTexture->TextureDC()
				, pos.IntX()
				, pos.IntY()
				, size.IntX()
				, size.IntY()
				, image->ImageHDC()
				, rc.Pos.IntX()
				, rc.Pos.IntY()
				, rc.Size.IntX()
				, rc.Size.IntY()
				, transColor
			);
		}
	}
}

void RandomMap::SetCopyImageIndex()
{
	for (size_t y = 0; y < MAPCOUNT_Y; ++y)
	{
		for (size_t x = 0; x < MAPCOUNT_X; ++x)
		{
			WALL_TYPE tempWall = mAllCopyTile[y][x]->WallType();
			TILE_TYPE tempTile = mAllCopyTile[y][x]->TileType();


			switch (tempTile)
			{
			case TT_INWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_INWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_INWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_INWALL);
				break;
			case TT_WALL:
				break;
			case TT_CORNER:
				break;
			case TT_FLOOR:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_FLOOR);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR);
				break;
			case TT_LEFT_DOOR:
				break;
			case TT_BOTTOM_DOOR:
				break;
			case TT_RIGHT_DOOR:
				break;
			case TT_TOP_DOOR:
				break;
			case TT_STAIR:
				break;
			case TT_PILLAR:
				break;
			case TT_MAX:
				// // mAllCopyTile[y][x]->mTileRenderer->Off();
				BOOM;
				break;
			default:
				break;
			}

			switch (tempWall)
			{
			case WT_NONE:
				break;
			case WT_FLOOR:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_FLOOR);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR);
				break;
			case WT_LEFTWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTWALL);

				break;
			case WT_RIGHTWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTWALL);


				break;
			case WT_TOPWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_TOPWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPWALL);
				break;
			case WT_DOWNWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWNWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWNWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWNWALL);

				break;
			case WT_COLUMN:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_COLUMN);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_COLUMN);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_COLUMN);

				break;
			case WT_COLUMNSHADOW1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_COLUMNSHADOW1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_COLUMNSHADOW1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_COLUMNSHADOW1);

				break;
			case WT_COLUMNSHADOW2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_COLUMNSHADOW2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_COLUMNSHADOW2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_COLUMNSHADOW2);

				break;
			case WT_LEFTDOWN:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTDOWN);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTDOWN);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTDOWN);
				break;
			case WT_RIGHTDOWN:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTDOWN);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOWN);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOWN);
				break;
			case WT_RIGHTTOP:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTTOP);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTTOP);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTTOP);
				break;
			case WT_LEFTTOP:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTTOP);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTTOP);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTTOP);
				break;
			case WT_LEFTDOWNWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTDOWNWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTDOWNWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTDOWNWALL);
				break;
			case WT_RIGHTDOWNWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTDOWNWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOWNWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOWNWALL);
				break;
			case WT_RIGHTTOPWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTTOPWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTTOPWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTTOPWALL);
				break;
			case WT_LEFTTOPWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTTOPWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTTOPWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTTOPWALL);
				break;

				////////////////////////// 문
			case WT_LEFTDOOR:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LEFTDOOR);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTDOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTDOOR);
				break;
			case 	WT_BOTTOMDOOR:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BOTTOMDOOR);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BOTTOMDOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BOTTOMDOOR);
				break;
			case 	WT_RIGHTDOOR:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTDOOR);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOOR);
				break;
			case 	WT_TOPDOOR:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_TOPDOOR);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPDOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPDOOR);
				break;

				///////////////////////////// 아치
			case WT_ARCH_LEFT:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFT);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFT);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFT);
				break;
			case WT_ARCH_TOP:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOP);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOP);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOP);
				break;
			case WT_ARCH_LEFTWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFTWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFTWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFTWALL);
				break;
			case WT_ARCH_LASTLEFT:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTLEFT);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTLEFT);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTLEFT);
				break;
			case WT_ARCH_TOPWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPWALL);
				break;
			case WT_ARCH_LASTTOP:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTTOP);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTTOP);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTTOP);
				break;
			case WT_ARCH_LEFTTOP:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFTTOP);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFTTOP);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFTTOP);
				break;
			case WT_ARCH_TOPSHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPSHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPSHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPSHADOW);
				break;
			case WT_ARCH_TOPSHADOWWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPSHADOWWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPSHADOWWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPSHADOWWALL);
				break;
			case WT_ARCH_RIGHTTOPWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_RIGHTTOPWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_RIGHTTOPWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_RIGHTTOPWALL);
				break;




				////////////////////////// 계단
			case WT_UP1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP1);
				break;
			case 	WT_UP2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP2);
				break;
			case 	WT_UP3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP3);
				break;
			case 	WT_UP4:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP4);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP4);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP4);
				break;
			case 	WT_UP5:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP5);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP5);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP5);
				break;
			case 	WT_UP6:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP6);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP6);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP6);
				break;
			case 	WT_UP7:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP7);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP7);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP7);
				break;
			case 	WT_UP8:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP8);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP8);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP8);
				break;
			case 	WT_UP9:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_UP9);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_UP9);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_UP9);
				break;

			case 	WT_DOWN1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN1);
				break;
			case 	WT_DOWN2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN2);
				break;
			case 	WT_DOWN3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN3);
				break;
			case 	WT_DOWN4:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN4);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN4);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN4);
				break;
			case 	WT_DOWN5:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN5);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN5);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN5);
				break;
			case 	WT_DOWN6:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_DOWN6);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_DOWN6);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_DOWN6);
				break;


			case WT_LASTLEFT1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTLEFT1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTLEFT1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTLEFT1);
				break;
			case WT_LASTLEFT2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTLEFT2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTLEFT2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTLEFT2);
				break;
			case WT_LASTRIGHT1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTRIGHT1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTRIGHT1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTRIGHT1);
				break;
			case WT_LASTRIGHT2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LASTRIGHT2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LASTRIGHT2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LASTRIGHT2);
				break;


				// 그림자

			case WT_LDSHADOW1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LDSHADOW1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LDSHADOW1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LDSHADOW1);
				break;
			case WT_LDSHADOW2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_LDSHADOW2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LDSHADOW2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LDSHADOW2);
				break;
			case WT_TOPSHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_TOPSHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPSHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPSHADOW);
				break;
			case WT_RIGHTTOPSHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_RIGHTTOPSHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTTOPSHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTTOPSHADOW);
				break;
			case WT_CANDLESHADOW1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_CANDLESHADOW1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_CANDLESHADOW1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_CANDLESHADOW1);
				break;
			case WT_CANDLESHADOW2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_CANDLESHADOW2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_CANDLESHADOW2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_CANDLESHADOW2);
				break;
			case WT_BOXSHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BOXSHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BOXSHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BOXSHADOW);
				break;

				// 바닥 스타일

			case	WT_CANDLE:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_CANDLE);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_CANDLE);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_CANDLE);
				break;
			case	WT_BIGBOX:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BIGBOX);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BIGBOX);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BIGBOX);
				break;
			case	WT_STONE1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE1);
				break;
			case	WT_STONE2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE2);
				break;
			case	WT_STONE3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE3);
				break;
			case	WT_STONE4:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE4);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE4);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE4);
				break;
			case	WT_STONE5:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE5);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE5);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE5);
				break;
			case	WT_STONE6:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STONE6);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STONE6);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STONE6);
				break;
			case	WT_BLOOD1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD1);
				break;
			case	WT_BLOOD2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD2);
				break;

			case	WT_BLOOD3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD3);
				break;

				// 벽 스타일



			case WT_BLOOD_TOPWALL1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_TOPWALL1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_TOPWALL1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_TOPWALL1);
				break;
			case WT_BLOOD_TOPWALL2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_TOPWALL2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_TOPWALL2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_TOPWALL2);
				break;
			case WT_BLOOD_TOPWALL3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_TOPWALL3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_TOPWALL3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_TOPWALL3);
				break;
			case WT_BLOOD_LEFTWALL1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_LEFTWALL1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_LEFTWALL1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_LEFTWALL1);
				break;
			case WT_BLOOD_LEFTWALL2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BLOOD_LEFTWALL2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BLOOD_LEFTWALL2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BLOOD_LEFTWALL2);
				break;

			case WT_STATUE_LEFTWALL1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL1);
				break;
			case WT_STATUE_LEFTWALL2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL2);
				break;
			case WT_STATUE_LEFTWALL3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL3);
				break;
			case WT_STATUE_LEFTWALL4:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_LEFTWALL4);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_LEFTWALL4);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_LEFTWALL4);
				break;
			case WT_STATUE_TOPWALL1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL1);
				break;
			case WT_STATUE_TOPWALL2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL2);
				break;
			case WT_STATUE_TOPWALL3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL3);
				break;
			case WT_STATUE_TOPWALL4:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_STATUE_TOPWALL4);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_STATUE_TOPWALL4);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_STATUE_TOPWALL4);
				break;
			case WT_BROKEN_TOPWALL1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_TOPWALL1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_TOPWALL1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_TOPWALL1);
				break;
			case WT_BROKEN_TOPWALL2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_TOPWALL2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_TOPWALL2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_TOPWALL2);
				break;
			case WT_BROKEN_TOPWALL3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_TOPWALL3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_TOPWALL3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_TOPWALL3);
				break;
			case WT_BROKEN_LEFTWALL1:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_LEFTWALL1);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_LEFTWALL1);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_LEFTWALL1);
				break;
			case WT_BROKEN_LEFTWALL2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_BROKEN_LEFTWALL2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_BROKEN_LEFTWALL2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_BROKEN_LEFTWALL2);
				break;
				// 쇠창살
			case WT_IRON_TOPWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_TOPWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_TOPWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_TOPWALL);
				break;
			case WT_IRON_LEFTWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTWALL);
				break;
			case WT_IRON_DOWNWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_DOWNWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_DOWNWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_DOWNWALL);
				break;
			case WT_IRON_RIGHTWALL:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_RIGHTWALL);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_RIGHTWALL);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_RIGHTWALL);
				break;
			case WT_IRON_LEFTDOWN:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTDOWN);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTDOWN);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTDOWN);
				break;
			case WT_IRON_RIGHTDOWN:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_RIGHTDOWN);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_RIGHTDOWN);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_RIGHTDOWN);
				break;
			case WT_IRON_LEFTTOP_LEFT:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTTOP_LEFT);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTTOP_LEFT);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTTOP_LEFT);
				break;
			case WT_IRON_LEFTTOP_RIGHT:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_IRON_LEFTTOP_RIGHT);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_IRON_LEFTTOP_RIGHT);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_IRON_LEFTTOP_RIGHT);
				break;
			case WT_ARCH_LASTTOP_SHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW);
				break;
			case WT_ARCH_LASTTOP_SHADOW2:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW2);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW2);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LASTTOP_SHADOW2);
				break;
			case WT_ARCH_LDSHADOW3:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LDSHADOW3);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LDSHADOW3);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LDSHADOW3);
				break;
			case WT_ARCH_LEFTWALL_SHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_LEFTWALL_SHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_LEFTWALL_SHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_LEFTWALL_SHADOW);
				break;
			case WT_ARCH_RIGHTDOWNWALL_SHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_RIGHTDOWNWALL_SHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_RIGHTDOWNWALL_SHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_RIGHTDOWNWALL_SHADOW);
				break;
			case WT_ARCH_TOPWALL_SHADOW:
				// mAllCopyTile[y][x]->mTileRenderer->ImageIndex((int)II_ARCH_TOPWALL_SHADOW);
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_ARCH_TOPWALL_SHADOW);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_ARCH_TOPWALL_SHADOW);
				break;
			case WT_LEFTWALL1_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTWALL1_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTWALL1_B);
				break;
			case WT_LEFTWALL2_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTWALL2_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTWALL2_B);
				break;
			case WT_TOPWALL1_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPWALL1_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPWALL1_B);
				break;
			case  WT_TOPWALL2_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_TOPWALL2_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_TOPWALL2_B);
				break;
			case WT_LEFTTOPWALL_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_LEFTTOPWALL_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_LEFTTOPWALL_B);
				break;
			case WT_FLOOR1_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR1_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR1_B);
				break;
			case WT_FLOOR2_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR2_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR2_B);
				break;
			case WT_FLOOR3_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR3_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR3_B);
				break;
			case WT_FLOOR4_B:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_FLOOR4_B);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_FLOOR4_B);
				break;

			case WT_BDOOR:
				mAllCopyTile[y][x]->mWallRenderer->ImageIndex((int)II_RIGHTDOOR);
				mAllCopyTile[y][x]->mFogRenderer->ImageIndex((int)II_RIGHTDOOR);

				break;
			case WT_MAX:
				BOOM;
				break;
			default:
				break;
			}
		}
	}
}

void RandomMap::CopyAll()
{
	for (size_t y = 0; y < MAPCOUNT_Y; y++)
	{
		for (size_t x = 0; x < MAPCOUNT_X; x++)
		{
			mAllCopyTile[y][x]->WallType(mAllTile[y][x]->WallType());
			mAllCopyTile[y][x]->TileType(mAllTile[y][x]->TileType());
		}
	}
}

void RandomMap::On()
{
	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		for (int j = 0; j < MAPCOUNT_X; j++)
		{

			mAllTile[i][j]->On();	
		}
	}

	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		for (int j = 0; j < MAPCOUNT_X; j++)
		{

			mAllCopyTile[i][j]->On();
		}
	}
}

void RandomMap::Off()
{
	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		for (int j = 0; j < MAPCOUNT_X; j++)
		{

			mAllTile[i][j]->Off();
		}
	}

	for (int i = 0; i < MAPCOUNT_Y; i++)
	{
		for (int j = 0; j < MAPCOUNT_X; j++)
		{

			mAllCopyTile[i][j]->Off();
		}
	}
}
