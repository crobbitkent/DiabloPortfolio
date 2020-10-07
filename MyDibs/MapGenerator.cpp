#include "MapGenerator.h"

#include <time.h>
#include <Windows.h>
#include <iostream>

#include <RandomNumber.h>

#include "GameManager.h"
#include "MonsterManager.h"
#include "Map.h"
#include "Tile.h"
#include "Player.h"
#include "Room.h"

Map* MapGenerator::mMap = nullptr;
int MapGenerator::mSeed = 1200;

int MapGenerator::mLeftCount = 0;
int MapGenerator::mBottomCount = 0;
int MapGenerator::mRightCount = 0;
int MapGenerator::mTopCount = 0;

int MapGenerator::mLevelCount = 1;

int MapGenerator::mLeftLimit = 3;
int MapGenerator::mBottomLimit = 2;
int MapGenerator::mRightLimit = 0;
int MapGenerator::mTopLimit = 0;

bool MapGenerator::mIsDownstair = false;
bool MapGenerator::mIsUpstair = false;

bool MapGenerator::IsDebugging = true;

Map* MapGenerator::GenerateMap(int x, int y, Player* player, int level)
{
	// true면 방 만들때마다 인쇄
	MapGenerator::IsDebugging = true;

	mMap = new Map({ x, y }, mLevelCount++);

	// MakeRandomMap(x, y, player);

	// 디버깅용
	// GameManager::SetMap(mMap);

	switch (level)
	{
	case 1:
		MakeCathedralLevel();
		SetM1(mMap);
		break;
	case 2:
		MakeButcherLevel();
		SetM1(mMap);
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}

	for (int y = 0; y < GameManager::ScreenSize().IntY(); y++)
	{
		for (int x = 0; x < GameManager::ScreenSize().IntX(); x++)
		{
			mMap->mAllTile[y][x].FindNeighbor();
		}
	}

	return mMap;
}

void MapGenerator::SetM1(Map* map)
{
	//system("cls");
	//GameManager::Print(0);

	std::vector<Room*> rooms = mMap->GetAllRoom();

	// 일단 방마다 하나씩
	for (size_t i = 0; i < rooms.size(); i++)
	{
		MyVector2 roomSize = rooms[i]->GetSize();

		int count = RandomNumber::GetRandomNumber(6, int(roomSize.X * roomSize.Y));
		count = (int)(count / 3.f);

		int maxCount = 5;
		int rcount = 0;

		for (int j = 0;  j < count; j++)
		{
			int newX = RandomNumber::GetRandomNumber(1, roomSize.IntX());
			int newY = RandomNumber::GetRandomNumber(1, roomSize.IntY());

			

			MyVector2 newPos = MyVector2(newX, newY);
			// 몬스터의 위치

			// 바닥이고 유닛이 없으면 
			if (true == mMap->IsFloor(rooms[i]->GetPos() + newPos) &&
				false == mMap->GetTile(rooms[i]->GetPos() + newPos).IsUnitOn())
			{
				MyVector2 room = rooms[i]->GetPos();
				MyVector2 test = rooms[i]->GetPos() + newPos;
				// mMap->AddMonster(MonsterManager::CloneM1(0, rooms[i]->GetPos() + newPos));
			}
			else
			{
				--j;
				++rcount;

				if (rcount == maxCount)
				{
					rcount = 0;
					++j;
				}
				continue;
			}

			if (true == IsDebugging)
			{
			/*	system("cls");
				GameManager::Clear();
				GameManager::Print(0);*/
			}
		}
				

		
	}

	int a = 0;
}

void MapGenerator::MakeCathedralLevel()
{
	int ran = RandomNumber::GetRandomNumber(1, 2);

	if (1 == ran)
	{
		Make3RoomsX(1);
	}
	else
	{
		Make3RoomsY(1);
	}
}

void MapGenerator::MakeButcherLevel()
{
	int ran = RandomNumber::GetRandomNumber(1, 2);

	if (1 == ran)
	{
		Make3RoomsX(2);
	}
	else
	{
		Make3RoomsY(2);
	}
}

void MapGenerator::Make3RoomsY(int level)
{
	MakeARoom({ 15, 0 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	MakeARoom({ 15, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	MakeARoom({ 15, 28 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	MakeEleven({ 19, 11 }, { 4, 4 });
	MakeEleven({ 19, 25 }, { 4, 4 });

	// 왼쪽으로 방 만들어라
	//                                          좌하우상
	if (1 == level)
	{
		
		MakeRooms({ 15, 0 }, { 10, 10 }, RT_LEFT, 3, 2, 2, 1); 	
	}

	else
	{
		MakeButchersRooms({ 15, 0 }, { 10, 10 }, RT_LEFT, 3, 2, 2, 1);		
	}
	ResetCount();
	MakeRooms({ 15, 0 }, { 10, 10 }, RT_RIGHT, 2, 2, 3, 1); 	ResetCount();
	MakeRooms({ 15, 14 }, { 10, 10 }, RT_LEFT, 3, 2, 2, 2); 	ResetCount();
	MakeRooms({ 15, 14 }, { 10, 10 }, RT_RIGHT, 2, 2, 3, 2); 	ResetCount();
	MakeRooms({ 15, 28 }, { 10, 10 }, RT_LEFT, 4, 1, 2, 4); 	ResetCount();
	MakeRooms({ 15, 28 }, { 10, 10 }, RT_RIGHT, 2, 1, 4, 4);	ResetCount();

	// 아직 내려가는 걸 못만들었다면
	if (false == mMap->GetDownstair())
	{
		MakeDownstair({ 15, 14 });
	}

	mMap->UpgradeWall();
}

void MapGenerator::Make3RoomsX(int level)
{
	MakeARoom({ 0, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	MakeARoom({ 14, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	MakeARoom({ 28, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	MakeEqual({ 11, 18 }, { 4, 4 });
	MakeEqual({ 25, 18 }, { 4, 4 });

	if (1 == level)
	{

		MakeRooms({ 0, 14 }, { 10, 10 }, RT_BOTTOM, 3, 3, 3, 3);
	}

	else
	{
		MakeButchersRooms({ 0, 14 }, { 10, 10 }, RT_BOTTOM, 3, 2, 2, 1);	
	}

	ResetCount();
	MakeRooms({ 0, 14 }, { 10, 10 }, RT_TOP, 3, 3, 3, 3);
	ResetCount();
	MakeRooms({ 14, 14 }, { 10, 10 }, RT_BOTTOM, 3, 3, 3, 3);
	ResetCount();
	MakeRooms({ 14, 14 }, { 10, 10 }, RT_TOP, 3, 3, 3, 3);
	ResetCount();
	MakeRooms({ 28, 14 }, { 10, 10 }, RT_BOTTOM, 3, 3, 3, 3);
	ResetCount();
	MakeRooms({ 28, 14 }, { 10, 10 }, RT_TOP, 3, 3, 3, 3);
	ResetCount();

	if (false == mMap->GetDownstair())
	{
		MakeDownstair({ 14, 14 });
	}

	mMap->UpgradeWall();
	mMap->UpgradeCorner();
}

// 실제로 방이 만들어진다.
bool MapGenerator::MakeARoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	// 코너를 제외한 벽으로 업그레이드
	// mMap->UpgradeWall();

	int X = basePos.X;
	int Y = basePos.Y;

	MyVector2 TravellerBase = basePos + MyVector2::DOWNRIGHT;


	int y = 0;
	int x = 0;

	for (; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (GameManager::ScreenSize().IntY() <= TravellerPos.Y + 1 ||
			0 > TravellerPos.Y)
		{
			break;
		}

		x = 0;
		for (; x < size.X; x++)
		{
			if (GameManager::ScreenSize().IntX() <= TravellerPos.X + 1 ||
				0 > TravellerPos.X)
			{
				break;
			}

			mMap->SetFloorAt(TravellerPos);

			mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;

			// 디버깅 출력
			// system("cls");
			// GameManager::Print(0);
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MyVector2 newSize = { x, y };

	// 새로운 사이즈가 0보다 작으면... 방이 생성되지 않음
	if (0 >= y || 0 >= x)
	{
		return false;
	}

	NewUpgradeCorner(basePos, newSize);



	// 문 생성하기 (뚫림 1:2 문)
	int doorRandom = RandomNumber::GetRandomNumber(1, 3);

	if (2 >= doorRandom)
	{
		// 문이 생성되지 않으면 아웃
		if (false == MakeADoor(basePos, newSize, prevPos, prevSize, roomType))
		{
			return false;
		}
	}
	else
	{
		MakeAConnection(basePos, newSize, prevPos, prevSize, roomType);
	}

	NewUpgradeCorner(basePos, newSize);

	// 디버깅 출력
	if (true == IsDebugging)
	{
		 //system("cls");
		 //GameManager::Clear();
		 //GameManager::Print(0);		
	}

	// 방으로 저장!
	Room *tempRoom = new Room(basePos, newSize);
	mMap->AddRoom(tempRoom);


	return true;
	// mMap->UpgradeCorner();
}

bool MapGenerator::MakeButchersPreRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	int X = basePos.X;
	int Y = basePos.Y;

	MyVector2 TravellerBase = basePos + MyVector2::DOWNRIGHT;

	int y = 0;
	int x = 0;

	for (; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (GameManager::ScreenSize().IntY() <= TravellerPos.Y + 1 ||
			0 > TravellerPos.Y)
		{
			break;
		}

		x = 0;
		for (; x < size.X; x++)
		{
			if (GameManager::ScreenSize().IntX() <= TravellerPos.X + 1 ||
				0 > TravellerPos.X)
			{
				break;
			}

			mMap->SetFloorAt(TravellerPos);
			mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;

		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MyVector2 newSize = { x, y };

	// 새로운 사이즈가 0보다 작으면... 방이 생성되지 않음
	if (0 >= y || 0 >= x)
	{
		return false;
	}

	// 방으로 저장!
	Room* tempRoom = new Room(basePos, newSize);
	mMap->AddRoom(tempRoom);

	// 문 생성하기 (뚫림 1:2 문)
	int doorRandom = RandomNumber::GetRandomNumber(1, 3);

	if (2 >= doorRandom)
	{
		if (false == MakeADoor(basePos, newSize, prevPos, prevSize, roomType))
		{
			return false;
		}
	}
	else
	{
		MakeAConnection(basePos, newSize, prevPos, prevSize, roomType);
	}

	NewUpgradeCorner(basePos, newSize);

	// 찐방
	MakeButchersRealRoom({ basePos.X + 2, basePos.Y + 2 }, newSize - MyVector2{ 4, 4 }, prevPos, prevSize, RT_MAX);

	// 디버깅 출력
	if (true == IsDebugging)
	{
		//system("cls");
		//GameManager::Clear();
		//GameManager::Print(0);
	}

	return true;


}

bool MapGenerator::MakeButchersRealRoom(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	int X = basePos.X;
	int Y = basePos.Y;

	MyVector2 TravellerBase = basePos + MyVector2::DOWNRIGHT;

	int y = 0;
	int x = 0;

	for (; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;

		if (GameManager::ScreenSize().IntY() <= TravellerPos.Y + 1 ||
			0 > TravellerPos.Y)
		{
			break;
		}

		x = 0;
		for (; x < size.X; x++)
		{
			if (GameManager::ScreenSize().IntX() <= TravellerPos.X + 1 ||
				0 > TravellerPos.X)
			{
				break;
			}

			mMap->SetWallAroundAt(TravellerPos, PIXEL_BTILE, PIXEL_BWALL);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	MakeFloor(basePos + MyVector2::DOWNRIGHT, { 2, 2 }, PIXEL_BTILE);

	MyVector2 newSize = { x, y };

	// 새로운 사이즈가 0보다 작으면... 방이 생성되지 않음
	if (0 >= y || 0 >= x)
	{
		return false;
	}

	// 방으로 저장!
	Room* tempRoom = new Room(basePos, newSize);
	mMap->AddRoom(tempRoom);

	NewUpgradeCorner(basePos, newSize);

	mMap->SetDoorAt(MyVector2{ basePos.X + 3, basePos.Y + 2 }, TT_RIGHT_DOOR);

	return true;
}



bool MapGenerator::MakeADoor(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	// 큰 순으로 정렬
	int arrX[4] = { basePos.X, basePos.X + size.X, prevPos.IntX(), prevPos.IntX() + prevSize.IntX() };
	int arrY[4] = { basePos.Y, basePos.Y + size.Y, prevPos.IntY(), prevPos.IntY() + prevSize.IntY() };

	for (int i = 3; i >= 0; --i)
	{
		for (int j = 0; j < i; j++)
		{
			if (arrX[j] > arrX[j + 1])
			{
				int temp = arrX[j + 1];
				arrX[j + 1] = arrX[j];
				arrX[j] = temp;
			}
			if (arrY[j] > arrY[j + 1])
			{
				int temp = arrY[j + 1];
				arrY[j + 1] = arrY[j];
				arrY[j] = temp;
			}
		}
	}

	// 비교
	MyVector2 bigPos = prevPos;

	if (basePos.Y > prevPos.IntY())
	{
		bigPos.Y = basePos.Y;
	}

	if (basePos.X > prevPos.IntX())
	{
		bigPos.X = basePos.X;
	}

	int randomSizeX = arrX[2] - arrX[1];
	int randomSizeY = arrY[2] - arrY[1];

	int ranY = RandomNumber::GetRandomNumber(1, randomSizeY);
	int ranX = RandomNumber::GetRandomNumber(1, randomSizeX);

	switch (roomType)
	{
	case RT_LEFT:
		if (TT_UPSTAIR == mMap->GetTileType({ bigPos.X, bigPos.Y + ranY }))
		{
			return false;
		}

		mMap->SetDoorAt(MyVector2{ bigPos.X, bigPos.Y + ranY }, TT_LEFT_DOOR);
		break;
	case RT_BOTTOM:
		if (TT_UPSTAIR == mMap->GetTileType({ bigPos.X + ranX, bigPos.Y }))
		{
			return false;
		}

		mMap->SetDoorAt(MyVector2{ bigPos.X + ranX, bigPos.Y }, TT_BOTTOM_DOOR);
		break;
	case RT_RIGHT:
		if (TT_UPSTAIR == mMap->GetTileType({ bigPos.X, bigPos.Y + ranY }))
		{
			return false;
		}

		mMap->SetDoorAt(MyVector2{ bigPos.X, bigPos.Y + ranY }, TT_RIGHT_DOOR);
		break;
	case RT_TOP:
		if (TT_UPSTAIR == mMap->GetTileType({ bigPos.X + ranX, bigPos.Y }))
		{
			return false;
		}

		mMap->SetDoorAt(MyVector2{ bigPos.X + ranX, bigPos.Y }, TT_TOP_DOOR);
		break;
	case RT_MAX:
		break;
	default:
		break;
	}

	return true;
}

void MapGenerator::MakeAConnection(MyVector2 basePos, MyVector2 size, MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType)
{
	// 큰 순으로 정렬
	int arrX[4] = { basePos.X, basePos.X + size.X, prevPos.IntX(), prevPos.IntX() + prevSize.IntX() };
	int arrY[4] = { basePos.Y, basePos.Y + size.Y, prevPos.IntY(), prevPos.IntY() + prevSize.IntY() };

	for (int i = 3; i >= 0; --i)
	{
		for (int j = 0; j < i; j++)
		{
			if (arrX[j] > arrX[j + 1])
			{
				int temp = arrX[j + 1];
				arrX[j + 1] = arrX[j];
				arrX[j] = temp;
			}
			if (arrY[j] > arrY[j + 1])
			{
				int temp = arrY[j + 1];
				arrY[j + 1] = arrY[j];
				arrY[j] = temp;
			}
		}
	}

	// 비교
	MyVector2 bigPos = prevPos;

	if (basePos.Y > prevPos.IntY())
	{
		bigPos.Y = basePos.Y;
	}

	if (basePos.X > prevPos.IntX())
	{
		bigPos.X = basePos.X;
	}

	switch (roomType)
	{
	case RT_LEFT:
		MakeFloor(bigPos + MyVector2::DOWN, { 1, arrY[2] - arrY[1] });
		//mMap->SetDoorAt(Position{ bigPos.X, bigPos.Y + ranY }, TT_LEFT_DOOR);

		break;
	case RT_BOTTOM:
		MakeFloor(bigPos + MyVector2::RIGHT, { arrX[2] - arrX[1], 1 });
		//mMap->SetDoorAt(Position{ bigPos.X + ranX, bigPos.Y }, TT_BOTTOM_DOOR);
		break;
	case RT_RIGHT:
		MakeFloor(bigPos + MyVector2::DOWN, { 1, arrY[2] - arrY[1] });
		//mMap->SetDoorAt(Position{ bigPos.X, bigPos.Y + ranY }, TT_RIGHT_DOOR);
		break;
	case RT_TOP:
		MakeFloor(bigPos + MyVector2::RIGHT, { arrX[2] - arrX[1], 1 });
		//mMap->SetDoorAt(Position{ bigPos.X + ranX, bigPos.Y }, TT_TOP_DOOR);
		break;
	case RT_MAX:
		break;
	default:
		break;
	}
}

void MapGenerator::MakeUpstair(MyVector2 basePos)
{
	// 계단 타일의 크기는 3, 2( y에 벽 포함, 따라서 1)
	// 따라서 최고 크기 4, 3
	MyVector2 pos = basePos;

	// 예외처리
	if (pos.Y + 3 + 1 >= GameManager::ScreenSize().IntX())
	{
		return;
	}

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			mMap->SetUpstairAt(pos);

			pos = pos + MyVector2::RIGHT;
		}
		pos = basePos + MyVector2::DOWN;

	}

	mMap->SetHasUpstair();
}

void MapGenerator::MakeDownstair(MyVector2 basePos)
{
	// // 계단 타일의 크기는 2, 2( y에 벽 포함, 따라서 1)
	// 따라서 최고 크기 4, 4
	MyVector2 pos = basePos + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::RIGHT + MyVector2::DOWN;

	// 예외처리
	if (GameManager::ScreenSize().IntX() - 3 <= pos.X ||
		GameManager::ScreenSize().IntY() - 3 <= pos.Y)
	{
		return;
	}

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			mMap->SetDownstairAt(pos);
			pos = pos + MyVector2::RIGHT;
		}
		pos = basePos + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::RIGHT + MyVector2::DOWN + MyVector2::DOWN;
	}

	mMap->SetHasDownstair();
}

// roomType은 왼쪽으로 만들으라는 뜻
void MapGenerator::MakeRooms(MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType, int left, int bottom, int right, int top)
{
	mLeftLimit = left;
	mBottomLimit = bottom;
	mRightLimit = right;
	mTopLimit = top;

	// 방 크기 설정
	// 1 ~ 6
	srand(++mSeed);
	int sizeX = (rand() % 6) + 1;
	srand(++mSeed);
	int sizeY = (rand() % 6) + 1;

	int posY = -1;
	int posX = -1;

	// 현재 위치 설정
	switch (roomType)
	{
	case RT_LEFT:
		if (0 == prevPos.IntX() || prevSize.IntX() == 0 ||
			mLeftCount >= mLeftLimit)
		{
			return;
		}

		posY = prevPos.IntY() + (int)(prevSize.IntY() / 2) - (int)(sizeY / 2);
		posX = prevPos.IntX() - sizeX - 1;
		break;
	case RT_BOTTOM:
		if (GameManager::ScreenSize().IntY() <= prevPos.IntY() + prevSize.IntY() ||
			mBottomCount > mBottomLimit)
		{
			return;
		}

		posX = prevPos.IntX() + (int)(prevSize.IntX() / 2) - (int)(sizeX / 2);
		posY = prevPos.IntY() + prevSize.IntY() + 1;
		break;
	case RT_RIGHT:
		if (GameManager::ScreenSize().IntX() <= prevPos.IntX() + prevSize.IntX() ||
			mRightCount > mRightLimit)
		{
			return;
		}

		posY = prevPos.IntY() + (int)(prevSize.IntY() / 2) - (int)(sizeY / 2);
		posX = prevPos.IntX() + prevSize.IntX() + 1;

		break;
	case RT_TOP:
		if (0 == prevPos.IntY() || prevSize.IntY() == 0 ||
			mTopCount > mTopLimit)
		{
			return;
		}

		posX = prevPos.IntX() + (int)(prevSize.IntX() / 2) - (int)(sizeX / 2);
		posY = prevPos.IntY() - sizeY - 1;
		break;
	default:
		break;
	}

	// overcheck
	if (0 > posY)
	{
		posY = 0;
	}
	if (0 > posX)
	{
		posX = 0;
	}
	if (GameManager::ScreenSize().IntX() <= posX)
	{
		posX = GameManager::ScreenSize().IntX() - 1;
	}
	if (GameManager::ScreenSize().IntY() <= posY)
	{
		posY = GameManager::ScreenSize().IntY() - 1;
	}

	// true면 거기에 방이 있다.
	if (true == mMap->CheckIsRoom({ posX, posY }, { sizeX + 1, sizeY + 1 }))
	{
		return;
	}

	// 현재 자리에 방 만들기
	// 실패하면 방만들기 끝
	if (false == MakeARoom({ posX, posY }, { sizeX, sizeY }, prevPos, prevSize, roomType))
	{
		return;
	}

	// 계단 만들기

	if (false == mMap->GetUpstair())
	{
		if (sizeX >= 4 && sizeY >= 3)
		{
			MyVector2 stairPos = mMap->CheckNorthWall({ posX, posY }, { sizeX, sizeY });

			if (MyVector2{ -1, -1 } != stairPos)
			{
				MakeUpstair(stairPos);
			}
		}
	}
	else if (false == mMap->GetDownstair())
	{
		if (sizeX >= 4 && sizeY >= 4)
		{
			MyVector2 stairPos = mMap->CheckNorthWall({ posX, posY }, { sizeX, sizeY });

			if (MyVector2{ -1, -1 } != MyVector2{ posX, posY })
			{
				MakeDownstair({ posX, posY });
			}
		}
	}

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
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_BOTTOM:
		++mBottomCount;
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_RIGHT:
		++mRightCount;
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_TOP:
		++mTopCount;
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms({ posX, posY }, { sizeX, sizeY }, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	default:
		break;
	}
}

void MapGenerator::MakeButchersRooms(MyVector2 prevPos, MyVector2 prevSize, ROOM_TYPE roomType, int left, int bottom, int right, int top)
{
	mLeftLimit = left;
	mBottomLimit = bottom;
	mRightLimit = right;
	mTopLimit = top;

	// 방 크기 설정
	// 1 ~ 6
	srand(++mSeed);
	int sizeX = (rand() % 6) + 1;
	srand(++mSeed);
	int sizeY = (rand() % 6) + 1;

	int posY = -1;
	int posX = -1;

	// 현재 위치 설정
	switch (roomType)
	{
	case RT_LEFT:
		if (0 == prevPos.IntX() || prevSize.IntX() == 0 ||
			mLeftCount >= mLeftLimit)
		{
			return;
		}

		posY = prevPos.IntY() + (int)(prevSize.IntY() / 2) - (int)(sizeY / 2);
		posX = prevPos.IntX() - sizeX - 1;
		break;
	case RT_BOTTOM:
		if (GameManager::ScreenSize().IntY() <= prevPos.IntY() + prevSize.IntY() ||
			mBottomCount > mBottomLimit)
		{
			return;
		}

		posX = prevPos.IntX() + (int)(prevSize.IntX() / 2) - (int)(sizeX / 2);
		posY = prevPos.IntY() + prevSize.IntY() + 1;
		break;
	case RT_RIGHT:
		if (GameManager::ScreenSize().IntX() <= prevPos.IntX() + prevSize.IntX() ||
			mRightCount > mRightLimit)
		{
			return;
		}

		posY = prevPos.IntY() + (int)(prevSize.IntY() / 2) - (int)(sizeY / 2);
		posX = prevPos.IntX() + prevSize.IntX() + 1;
		break;
	case RT_TOP:
		if (0 == prevPos.IntY() || prevSize.IntY() == 0 ||
			mTopCount > mTopLimit)
		{
			return;
		}

		posX = prevPos.IntX() + (int)(prevSize.IntX() / 2) - (int)(sizeX / 2);
		posY = prevPos.IntY() - sizeY - 1;
		break;
	default:
		break;
	}

	// overcheck
	if (0 > posY)
	{
		posY = 0;
	}
	if (0 > posX)
	{
		posX = 0;
	}
	if (GameManager::ScreenSize().IntX() <= posX)
	{
		posX = GameManager::ScreenSize().IntX() - 1;
	}
	if (GameManager::ScreenSize().IntY() <= posY)
	{
		posY = GameManager::ScreenSize().IntY() - 1;
	}

	// true면 거기에 방이 있다.
	if (true == mMap->CheckIsRoom({ posX, posY }, { sizeX + 1, sizeY + 1 }))
	{
		return;
	}

	// 현재 자리에 방 만들기
	// 실패하면 방만들기 끝
	if (false == MakeARoom({ posX, posY }, { sizeX, sizeY }, prevPos, prevSize, roomType))
	{
		return;
	}

	// 현재 위치 설정 2

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

		BPos.Y = (float)posY + (int)(sizeY / 2) - (int)(BSize.Y / 2);
		BPos.X = (float)posX - BSize.X - 1;
		break;
	case RT_BOTTOM:
		if (GameManager::ScreenSize().IntY() <= posY + sizeY ||
			mBottomCount > mBottomLimit)
		{
			return;
		}

		BPos.X = (float)posX + (int)(sizeX / 2) - (int)(BSize.X / 2);
		BPos.Y = (float)posY + sizeY + 1;
		break;
	case RT_RIGHT:
		if (GameManager::ScreenSize().IntX() <= posX + sizeX ||
			mRightCount > mRightLimit)
		{
			return;
		}

		BPos.Y = (float)posY + (int)(sizeY / 2) - (int)(BSize.Y / 2);
		BPos.X = (float)posX + sizeX + 1;
		break;
	case RT_TOP:
		if (0 == posY || sizeY == 0 ||
			mTopCount > mTopLimit)
		{
			return;
		}

		BPos.X = (float)posX + (int)(sizeX / 2) - (int)(BSize.X / 2);
		BPos.Y = (float)posY - BSize.Y - 1;
		break;
	default:
		break;
	}

	MakeButchersPreRoom(BPos, BSize, { posX, posY }, { sizeX, sizeY }, roomType);

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
		MakeRooms(BPos, BSize, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_BOTTOM:
		++mBottomCount;
		MakeRooms(BPos, BSize, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_RIGHT:
		++mRightCount;
		MakeRooms(BPos, BSize, RT_BOTTOM, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	case RT_TOP:
		++mTopCount;
		MakeRooms(BPos, BSize, RT_LEFT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_TOP, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		MakeRooms(BPos, BSize, RT_RIGHT, mLeftLimit, mBottomLimit, mRightLimit, mTopLimit);
		break;
	default:
		break;
	}
}

void MapGenerator::MakeFloor(MyVector2 basePos, MyVector2 size)
{
	MyVector2 TravellerBase = basePos;

	for (int y = 0; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;
		for (int x = 0; x < size.X; x++)
		{

			mMap->SetFloorAt(TravellerPos);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;

		// 디버깅 출력
		//system("cls");
		//GameManager::Print(100);
	}

	//Room* tempRoom = new Room(basePos, size);
	//mMap->AddRoom(tempRoom);
}

void MapGenerator::MakeFloor(MyVector2 basePos, MyVector2 size, wchar_t special)
{
	MyVector2 TravellerBase = basePos;

	for (int y = 0; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;
		for (int x = 0; x < size.X; x++)
		{

			mMap->SetFloorAt(TravellerPos, special);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;

		// 디버깅 출력
		//system("cls");
		//GameManager::Print(100);
	}

	Room* tempRoom = new Room(basePos, size);
	mMap->AddRoom(tempRoom);
}

void MapGenerator::MakeEleven(MyVector2 basePos, MyVector2 size)
{
	MyVector2 TravellerBase = basePos;

	for (int y = 0; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;
		for (int x = 0; x < size.X; x++)
		{
			if (0 == x || size.X - 1 == x)
			{
				mMap->SetWallAt(TravellerPos);
				TravellerPos += MyVector2::RIGHT;
				continue;
			}

			mMap->SetFloorAt(TravellerPos);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;

		// 디버깅 출력
		//system("cls");
		//GameManager::Print(100);
	}

	NewUpgradeCorner(basePos, size - MyVector2{ 2, 2 });

	Room* tempRoom = new Room(basePos, size - MyVector2{ 2, 2 });
	mMap->AddRoom(tempRoom);
}

void MapGenerator::MakeEqual(MyVector2 basePos, MyVector2 size)
{
	MyVector2 TravellerBase = basePos;

	for (int y = 0; y < size.Y; y++)
	{
		MyVector2 TravellerPos = TravellerBase;
		for (int x = 0; x < size.X; x++)
		{
			if (0 == y || size.Y - 1 == y)
			{
				mMap->SetWallAt(TravellerPos);
				TravellerPos += MyVector2::RIGHT;
				continue;
			}


			mMap->SetFloorAt(TravellerPos);
			// mMap->SetWallAroundAt(TravellerPos);

			TravellerPos += MyVector2::RIGHT;
		}

		TravellerBase = TravellerBase + MyVector2::DOWN;
	}

	Room* tempRoom = new Room(basePos, size);
	mMap->AddRoom(tempRoom);

	NewUpgradeCorner(basePos, size - MyVector2{ 2, 2 });
}

void MapGenerator::NewUpgradeCorner(MyVector2 basePos, MyVector2 size)
{
	//if (TT_UPSTAIR != mMap->GetTileType(basePos))
	//{
	//	mMap->SetCornerAt(basePos, TT_LEFTTOPWALL);
	//	mMap->AddWall(&mMap->GetTile({ basePos.X + size.X + 1, basePos.Y }));
	//}

	//if (TT_UPSTAIR != mMap->GetTileType({ basePos.X + size.X + 1, basePos.Y }))
	//{
	//	mMap->SetCornerAt({ basePos.X + size.X + 1, basePos.Y }, TT_RIGHTTOPWALL);
	//	mMap->AddWall(&mMap->GetTile(basePos));
	//}

	//if (TT_UPSTAIR != mMap->GetTileType({ basePos.X + size.X + 1, basePos.Y + size.Y + 1 }))
	//{
	//	mMap->SetCornerAt({ basePos.X + size.X + 1, basePos.Y + size.Y + 1 }, TT_RIGHTDOWNWALL);
	//	mMap->AddWall(&mMap->GetTile({ basePos.X + size.X + 1, basePos.Y + size.Y + 1 }));
	//}

	//if (TT_UPSTAIR != mMap->GetTileType({ basePos.X, basePos.Y + size.Y + 1 }))
	//{
	//	mMap->SetCornerAt({ basePos.X, basePos.Y + size.Y + 1 }, TT_LEFTDOWNWALL);
	//	mMap->AddWall(&mMap->GetTile({ basePos.X, basePos.Y + size.Y + 1 }));
	//}
}

void MapGenerator::MakeRandomMap(int x, int y, Player* player)
{
	srand(++mSeed);

	// 첫 위치는 중앙
	MyVector2 TravellerPos = { (int)(x / 2.f), (int)(x / 2.f) };

	srand(++mSeed);
	// 방 사이즈 = 1 ~ 7
	int sizeX = (rand() % 7 + 1) * 2;
	srand(++mSeed);
	int sizeY = (rand() % 7 + 1) * 2;

	MyVector2 roomSize = MyVector2({ sizeX, sizeY });

	MyVector2 PlayerPos = TravellerPos;

	MyVector2 prevPos = TravellerPos;



	for (size_t i = 0; i < 5; i++)
	{
		if (i == 4)
		{
			int a = 0;
		}

		srand(++mSeed);
		// 0 ~ 7
		int randomIndex = (rand() % 4) * 2;

		MyVector2 DIR = MyVector2::ARRDIR[randomIndex];

		// 여행자가 한번에 앞으로 가는 길이
		int Count = (rand() % 10) + 1;

		if (Count == 0)
		{
			int a = 0;
		}

		// 왔던 길은 가지 않는다.
		if (prevPos == TravellerPos + DIR) // || OverCheck(TravellerPos + DIR) )
		{
			randomIndex += 1;

			if (8 <= randomIndex)
			{
				randomIndex = 0;
			}
			// 방향 리셋
			DIR = MyVector2::ARRDIR[randomIndex];
		}

		for (int j = 0; j < Count; j++)
		{
			// 여행자 업데이트
			TravellerPos += DIR;

			mMap->SetFloorAt(TravellerPos);
			mMap->SetWallAroundAt(TravellerPos);
		}

		prevPos = TravellerPos;

		//system("cls");
		//GameManager::SetMap(mMap);
		//GameManager::Print(300);
	}

	mMap->UpgradeWall();

	// 플레이어 위치 지정...
	for (int i = 0; i < x * y; i++)
	{
		if (TT_FLOOR == mMap->GetTile(PlayerPos.IntX(), PlayerPos.IntY()).GetTileType())
		{


			player->Pos(PlayerPos);
			mMap->GetTile(PlayerPos.X, PlayerPos.Y).SetUnit(player);

			break;
		}

		PlayerPos += MyVector2::ARRDIR[(rand() % 4) * 2];
		srand(++mSeed);
	}
}

MapGenerator::~MapGenerator()
{
}



