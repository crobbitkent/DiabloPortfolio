
#include "Map.h"
#include "Tile.h"
#include "Room.h"
#include "Monster.h"

#include "GameManager.h"

#include <Windows.h>

Map::Map(MyVector2 size, int level)
{
	mLevel = level;

	mCorners = std::vector<Tile*>();
	mAllWall = std::vector<Tile*>();
	mAllRoom = std::vector<Room*>();
	mMonsters = std::vector<Monster*>();

	mAllTile = new Tile*[MAPSIZE_Y];

	for (int i = 0; i < size.Y; i++)
	{
		// mAllTile[i].Resize(size.X);
		mAllTile[i] = new Tile[MAPSIZE_X]();

		for (int j = 0; j < size.X; j++)
		{
			//mAllTile[i][j] = new Tile({ j, i }, PIXEL_INWALL, this);
		
			mAllTile[i][j].Init({ j, i }, PIXEL_INWALL, this);
		}
	}


}


Map::~Map()
{
	// int test1 = MAPSIZE_Y;
	for (int y = 0; y < MAPSIZE_Y; y++)
	{
		if (nullptr != mAllTile[y])
		{
			delete[] mAllTile[y];
			mAllTile[y] = nullptr;
		}		
	}

	if (nullptr != mAllTile)
	{
		delete[] mAllTile;
		mAllTile = nullptr;
	}

	int test = mAllRoom.capacity();

	for (int y = 0; y < mAllRoom.size(); y++)
	{
		if (nullptr != mAllRoom[y])
		{
			delete mAllRoom[y];
			mAllRoom[y] = nullptr;
		}
	}

	for (int y = 0; y < mMonsters.size(); y++)
	{
		if (nullptr != mMonsters[y])
		{
			delete mMonsters[y];
			mMonsters[y] = nullptr;
		}
	}
}
Tile& Map::GetTile(int _X, int _Y)
{
	if (true == OverCheck({ _X, _Y }))
	{

	}	
	return mAllTile[_Y][_X];
}

Tile& Map::GetTile(MyVector2 pos)
{
	if (true == OverCheck(pos))
	{
		Tile temp;
		return temp;
	}


	return mAllTile[pos.IntY()][pos.IntX()];
}

// true면 거기에 방이 있다.
bool Map::CheckIsRoom(Room* room)
{
	//MyVector2 size = room->GetSize();

	//MyVector2 basePos = room->GetPos();

	//for (int i = 0; i < mAllRoom.size(); ++i)
	//{	
	//	for (int y = 0; y < size.Y; y++)
	//	{
	//		MyVector2 travellerPos = basePos;
	//		for (int x = 0; x < size.X; x++)
	//		{
	//			int tempSizeY = mAllRoom[i]->GetSize().Y;
	//			int tempSizeX = mAllRoom[i]->GetSize().X;
	//			int tempPosX = mAllRoom[i]->GetPos().X;
	//			int tempPosY = mAllRoom[i]->GetPos().Y;

	//			// 범위 안에 들어오면
	//			if (tempPosX <= travellerPos.X &&
	//				tempPosY <= travellerPos.Y &&
	//				tempPosX + tempSizeX + 1 >= travellerPos.X &&
	//				tempPosY + tempSizeY + 1 >= travellerPos.Y)
	//			{
	//				// 겹쳐있다는 뜻
	//				return true;
	//			}
	//			travellerPos += MyVector2::RIGHT;
	//		}
	//		basePos = basePos + MyVector2::DOWN;
	//	}
	//}

	//// 안겹쳤다
	//return false;
}

bool Map::CheckIsRoom(MyVector2 roomPos, MyVector2 roomSize)
{
	MyVector2 size = roomSize;

	MyVector2 basePos = roomPos;

	for (int i = 0; i < mAllRoom.size(); ++i)
	{
		size = roomSize;
		basePos = roomPos;
		for (int y = 0; y < size.Y; y++)
		{
			MyVector2 travellerPos = basePos;
			for (int x = 0; x < size.X; x++)
			{
				int tempSizeY = mAllRoom[i]->GetSize().Y;
				int tempSizeX = mAllRoom[i]->GetSize().X;
				int tempPosX = mAllRoom[i]->GetPos().X;
				int tempPosY = mAllRoom[i]->GetPos().Y;

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

// true면 북벽에 문이 없다. 
MyVector2 Map::CheckNorthWall(MyVector2 roomPos, MyVector2 roomSize)
{
	int count = 0;
	int limit = 3;

	int x = 0;

	for (; x < roomSize.X; x++)
	{
		if (roomPos.X + x + 1 >= GameManager::ScreenSize().X || 
			roomPos.X + x + 1< 0)
		{
			return { -1, -1 };
		}

		TILE_TYPE a = mAllTile[roomPos.IntY()][roomPos.IntX() + x].GetTileType();
		TILE_TYPE b = mAllTile[roomPos.IntY()][roomPos.IntX() + x].GetTileType();

		if (TT_BOTTOM_DOOR == mAllTile[roomPos.IntY()][roomPos.IntX() + x + 1].GetTileType() ||
			TT_TOP_DOOR ==    mAllTile[roomPos.IntY()][roomPos.IntX() + x + 1].GetTileType() ||
			TT_FLOOR ==		  mAllTile[roomPos.IntY()][roomPos.IntX() + x + 1].GetTileType()
			)
		{			
			break;
		}
		++count;


	}

	int random = 0;

	if (count < limit)
	{
		return { -1, -1 };
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

	MyVector2 stairPosition = { roomPos.X + x - 2 - random, roomPos.Y };

	return stairPosition;
}

void Map::SetWallAt(int x, int y)
{
	if (x < 0 || x >= MAPSIZE_X ||
		y < 0 || y >= MAPSIZE_Y)
	{
		return;
	}

	mAllTile[y][x].SetWall();

	mAllWall.push_back(&mAllTile[y][x]);
}

void Map::SetWallAt(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return;
	}

	mAllTile[pos.IntY()][pos.IntX()].SetWall();

	mAllWall.push_back(&mAllTile[pos.IntY()][pos.IntX()]);
}


void Map::SetWallAt(MyVector2 pos, wchar_t pixel)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return;
	}

	mAllTile[pos.IntY()][pos.IntX()].SetWall(pixel);

	mAllWall.push_back(&mAllTile[pos.IntY()][pos.IntX()]);
}

void Map::SetWallAroundAt(int x, int y)
{
}

void Map::SetWallAroundAt(MyVector2 pos)
{
	if ((6 == pos.IntX()) && (43 == pos.IntY()))
	{
		int a = 0;
	}

	// mAllTile[pos.IntY()][pos.IntX()].SetWall();
	for (int i = 0; i < DT_MAX; i++)
	{
		MyVector2 newPos = pos + MyVector2::ARRDIR[i];

		if (newPos.Y >= GameManager::ScreenSize().Y ||
			newPos.X >= GameManager::ScreenSize().X || 
			newPos.Y < 0 ||
			newPos.X < 0 )
		{
			continue;
		}

		Tile temp = mAllTile[newPos.IntY()][newPos.IntX()];

		if (true == IsFloor(newPos) || 
			true == IsUpstair(newPos) || 
			true == IsDownstair(newPos) ||
			//TT_RIGHTTOPWALL ==  mAllTile[newPos.IntY()][newPos.IntX()].GetTileType() ||
			//TT_RIGHTDOWNWALL == mAllTile[newPos.IntY()][newPos.IntX()].GetTileType() ||
			//TT_LEFTTOPWALL ==   mAllTile[newPos.IntY()][newPos.IntX()].GetTileType() ||
			//TT_LEFTDOWNWALL ==  mAllTile[newPos.IntY()][newPos.IntX()].GetTileType() ||

			TT_INWALL != mAllTile[newPos.IntY()][newPos.IntX()].GetTileType()
			)
		{
			if (true == IsFloor(newPos))
			{
				// floor만 넣는다.
				// mAllTile[pos.IntY()][pos.IntX()].AddNeighbor(mAllTile[newPos.Y][newPos.X]);
			}
			continue;
		}

		if (newPos.X < 0 || newPos.X >= MAPSIZE_X ||
			newPos.Y < 0 || newPos.Y >= MAPSIZE_Y)
		{
			continue;
		}

		SetFloorAt(pos);
		SetWallAt(newPos);

		//system("cls");
		//GameManager::Print(0);
		//int a = 0;
	}
}

void Map::SetWallAroundAt(MyVector2 pos, wchar_t specialFloor, wchar_t specialWall)
{
	if ((6 == pos.IntX()) && (43 == pos.IntY()))
	{
		int a = 0;
	}

	// mAllTile[pos.IntY()][pos.IntX()].SetWall();
	for (int i = 0; i < DT_MAX; i++)
	{
		MyVector2 newPos = pos + MyVector2::ARRDIR[i];

		if (newPos.Y >= GameManager::ScreenSize().Y ||
			newPos.X >= GameManager::ScreenSize().X ||
			newPos.Y < 0 ||
			newPos.X < 0)
		{
			continue;
		}

		if (newPos.X < 0 || newPos.X >= MAPSIZE_X ||
			newPos.Y < 0 || newPos.Y >= MAPSIZE_Y)
		{
			continue;
		}

		SetFloorAt(pos, specialFloor);
		SetWallAt(newPos, specialWall);

		// system("cls");
		// GameManager::Print(0);

		mAllWall.push_back(&mAllTile[newPos.IntY()][newPos.IntX()]);
	}
}

void Map::SetFloorAt(int x, int y)
{
}

void Map::SetFloorAt(MyVector2 pos)
{
	if (pos.IntX() <= 0 || pos.IntX() >= MAPSIZE_X - 1 ||
		pos.IntY() <= 0 || pos.IntY() >= MAPSIZE_Y - 1 ||
		TT_DOWNSTAIR == mAllTile[pos.IntY()][pos.IntX()].GetTileType() ||
		TT_UPSTAIR == mAllTile[pos.IntY()][pos.IntX()].GetTileType() )
	{
		return;
	}
	mAllTile[pos.IntY()][pos.IntX()].SetFloor();

	std::vector<Tile*>::iterator iter = mAllWall.begin();
	std::vector<Tile*>::iterator iterEnd = mAllWall.end();

	std::vector<Tile*>::iterator tempiter;

	for (; iter != iterEnd; iter++)
	{
		if (true == (*iter)->IsWalkable())
		{
			tempiter = iter;
			mAllWall.erase(tempiter);
			break;
		}
	}
}

void Map::SetFloorAt(MyVector2 pos, wchar_t special)
{
	if (pos.IntX() <= 0 || pos.IntX() >= MAPSIZE_X - 1 ||
		pos.IntY() <= 0 || pos.IntY() >= MAPSIZE_Y - 1 ||
		TT_DOWNSTAIR == mAllTile[pos.IntY()][pos.IntX()].GetTileType() ||
		TT_UPSTAIR == mAllTile[pos.IntY()][pos.IntX()].GetTileType())
	{
		return;
	}
	mAllTile[pos.IntY()][pos.IntX()].SetFloor(special);

	std::vector<Tile*>::iterator iter = mAllWall.begin();
	std::vector<Tile*>::iterator iterEnd = mAllWall.end();

	for(; iter != iterEnd; iter++)
	{
		if (true == (*iter)->IsWalkable())
		{
			mAllWall.erase(iter);
			break;
		}

	}
}

void Map::SetDoorAt(MyVector2 pos, TILE_TYPE doorType)
{
	if (pos.IntX() <= 0 || pos.IntX() >= MAPSIZE_X - 1 ||
		pos.IntY() <= 0 || pos.IntY() >= MAPSIZE_Y - 1)
	{
		return;
	}

	switch (doorType)
	{
	case TT_LEFT_DOOR:
		mAllTile[pos.IntY()][pos.IntX()].SetLeftDoor();
		break;
	case TT_BOTTOM_DOOR:
		mAllTile[pos.IntY()][pos.IntX()].SetBottomDoor();
		break;
	case TT_RIGHT_DOOR:
		mAllTile[pos.IntY()][pos.IntX()].SetRightDoor();
		break;
	case TT_TOP_DOOR:
		mAllTile[pos.IntY()][pos.IntX()].SetTopDoor();
		break;
	default:
		break;
	}
}

void Map::SetCornerAt(MyVector2 pos, TILE_TYPE cornerType)
{
	if (GameManager::ScreenSize().Y <= pos.IntY() ||
		GameManager::ScreenSize().X <= pos.IntX() || 
		0 > pos.IntY() || 
		0 > pos.IntY() )
	{
		return;
	}

	mAllTile[pos.IntY()][pos.IntX()].SetCorner(cornerType);
}

void Map::SetUpstairAt(MyVector2 pos)
{
	mAllTile[pos.IntY()][pos.IntX()].SetUpstair();

	mStartPos = { pos.IntX() - 1, pos.IntY() + 1 };

	mHasUpstair = true;
}

void Map::SetDownstairAt(MyVector2 pos)
{

	mAllTile[pos.IntY()][pos.IntX()].SetDownstair();

	mHasDownstair = true;
}



bool Map::IsFloor(MyVector2 pos)
{
	// 걷기 가능하면 Floor

	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	if (TT_FLOOR == mAllTile[pos.IntY()][pos.IntX()].GetTileType())
	{
		return true;
	}

	return false;
}

bool Map::IsUpstair(MyVector2 pos)
{
	// 걷기 가능하면 Floor

	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	if (true == mAllTile[pos.IntY()][pos.IntX()].IsUpstair())
	{
		return true;
	}

	return false;
}

bool Map::IsDownstair(MyVector2 pos)
{
	// 걷기 가능하면 Floor

	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return false;
	}

	if (true == mAllTile[pos.IntY()][pos.IntX()].IsDownstair())
	{
		return true;
	}

	return false;
}

bool Map::IsWall(MyVector2 _Pos)
{
	if (TT_WALL == GetTileType(_Pos)
		//TT_BOTTOMWALL == GetTileType(_Pos) ||
		//TT_LEFTDOWNWALL == GetTileType(_Pos) ||
		//TT_LEFTTOPWALL == GetTileType(_Pos) ||
		//TT_LEFTWALL == GetTileType(_Pos) ||
		//TT_RIGHTDOWNWALL == GetTileType(_Pos) ||
		//TT_RIGHTTOPWALL == GetTileType(_Pos) ||
		//TT_RIGHTWALL == GetTileType(_Pos) ||
		//TT_TOPWALL == GetTileType(_Pos))
		)
	{
		return true;
	}

	return false;
}


void Map::UpgradeCorner()
{
	for (size_t i = 0; i < mAllWall.size(); i++)
	{
		/*if (PIXEL_WALL == mAllWall[i]->Pixel())
		{
			mCorners.push_back(mAllWall[i]);
		}*/
	}


	for (int i = 0; i < mCorners.size(); i++)
	{


		MyVector2 pos = mCorners[i]->Pos();

		if (pos == MyVector2{15, 33})
		{
			int a = 0;
		}

		//if (nullptr != mCorners[i])
		//{
		
			/* 코너 확인*/

			// North West ↖
			if ((true == IsWall(pos + MyVector2::ARRDIR[2])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[4]))  )
			{
				mCorners[i]->SetWall(PIXEL_NW);
				//system("cls");
				//GameManager::Print(0);
			}

			// West South ↙
			if ((true == IsWall(pos + MyVector2::ARRDIR[4])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[6])))
			{
				mCorners[i]->SetWall(PIXEL_WS);
			}

			// South East ↘ 
			if ((true == IsWall(pos + MyVector2::ARRDIR[0])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[6])))
			{
				mCorners[i]->SetWall(PIXEL_SE);
			}

			//Tile temp1 = GetTile(pos + MyVector2::ARRDIR[0]);
			//Tile temp2 = GetTile(pos + MyVector2::ARRDIR[2]);

			// East North ↗
			if ((true == IsWall(pos + MyVector2::ARRDIR[0])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[2])))
			{				
				mCorners[i]->SetWall(PIXEL_EN);
			}


			// North West ↖
			if ((true == IsWall(pos + MyVector2::ARRDIR[2])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[4])))
			{
				if (TT_FLOOR == GetTileType(pos + MyVector2::ARRDIR[3]))
				{
					mCorners[i]->SetWall(PIXEL_NW);
				}
		
			}

			// West South ↙
			if ((true == IsWall(pos + MyVector2::ARRDIR[4])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[6])))
			{
				if (TT_FLOOR == GetTileType(pos + MyVector2::ARRDIR[5]))
				{
					mCorners[i]->SetWall(PIXEL_WS);
				}
				
			}

			// South East ↘ 
			if ((true == IsWall(pos + MyVector2::ARRDIR[0])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[6])))
			{
				if ((true == IsWall(pos + MyVector2::ARRDIR[7])))
				{
					mCorners[i]->SetWall(PIXEL_SE);
				}
				
			}

			// East North ↗
			if ((true == IsWall(pos + MyVector2::ARRDIR[0])) &&
				(true == IsWall(pos + MyVector2::ARRDIR[2])))
			{
				if (true == IsWall(pos + MyVector2::ARRDIR[1]))
				{
					mCorners[i]->SetWall(PIXEL_EN);
				}
				
			}

			


				// 혼자
				if ((true == IsFloor(pos + MyVector2::ARRDIR[0])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[1])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[2])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[3])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[4])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[5])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[6])) &&
					(true == IsFloor(pos + MyVector2::ARRDIR[7])))
				{
					mCorners[i]->SetWall(PIXEL_ONE);
				}


			

			// 둘러싸임
			if ((false == IsFloor(pos + MyVector2::ARRDIR[0])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[1])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[2])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[3])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[4])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[5])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[6])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[7])))
			{
				mCorners[i]->SetWall(PIXEL_INWALL);
			}

		//}
	}
}

void Map::UpgradeWall()
{
	for (int i = 0; i < mAllWall.size(); i++)
	{

		if (TT_BOTTOM_DOOR == mAllWall[i]->GetTileType() ||
			TT_TOP_DOOR == mAllWall[i]->GetTileType() ||
			TT_LEFT_DOOR == mAllWall[i]->GetTileType() ||
			TT_RIGHT_DOOR == mAllWall[i]->GetTileType() ||
			TT_UPSTAIR == mAllWall[i]->GetTileType() ||
			TT_DOWNSTAIR ==    mAllWall[i]->GetTileType() 
/*			TT_LEFTDOWNWALL == mAllWall[i]->GetTileType() || 
			TT_LEFTTOPWALL == mAllWall[i]->GetTileType() ||
			TT_RIGHTTOPWALL == mAllWall[i]->GetTileType() ||
			TT_RIGHTDOWNWALL == mAllWall[i]->GetTileType() */)
		{
			continue;
		}

		MyVector2 pos = mAllWall[i]->Pos();

		//if (nullptr != mAllWall[i])
		//{

			/* 직선 확인*/
					// North
			if ((false == IsFloor(pos + MyVector2::ARRDIR[0])) && // 왼쪽
				(false == IsFloor(pos + MyVector2::ARRDIR[4])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[5])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[6])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[7])))
			{
				if (true == IsFloor(pos + MyVector2::ARRDIR[2])) // 아래
				{
					mAllWall[i]->SetWall(PIXEL_N);
				}
			}

			// West
			else if ((false == IsFloor(pos + MyVector2::ARRDIR[0])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[1])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[2])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[6])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[7])))
			{
				if (true == IsFloor(pos + MyVector2::ARRDIR[4])) // 오른쪽
				{
					mAllWall[i]->SetWall(PIXEL_W);
				}
			}

			// South
			else if ((false == IsFloor(pos + MyVector2::ARRDIR[0])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[1])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[2])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[3])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[4])))
			{
				if (true == IsFloor(pos + MyVector2::ARRDIR[6])) // 위
				{
					mAllWall[i]->SetWall(PIXEL_S);
				}
			}

			// East
			else if ((false == IsFloor(pos + MyVector2::ARRDIR[2])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[3])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[4])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[5])) &&
				(false == IsFloor(pos + MyVector2::ARRDIR[6])))
			{
				if (true == IsFloor(pos + MyVector2::ARRDIR[0])) // 왼쪽
				{
					mAllWall[i]->SetWall(PIXEL_E);
				}
			}

			// 겹치는 벽 가로
			if ((true == IsFloor(pos + MyVector2::ARRDIR[2])) &&
				(true == IsFloor(pos + MyVector2::ARRDIR[6])))
			{
				mAllWall[i]->SetWall(PIXEL_N);
			}
			// 겹치는 벽 세로
			if ((true == IsFloor(pos + MyVector2::ARRDIR[0])) &&
				(true == IsFloor(pos + MyVector2::ARRDIR[4])))
			{
				mAllWall[i]->SetWall(PIXEL_W);
			}

			// 디버깅 출력
			if (i == 600)
			{
				/*system("cls");
				GameManager::Print(0);*/
			}
	
		//}
	}


}

TILE_TYPE Map::GetTileType(MyVector2 pos)
{
	if (pos.IntX() < 0 || pos.IntX() >= MAPSIZE_X ||
		pos.IntY() < 0 || pos.IntY() >= MAPSIZE_Y)
	{
		return TT_MAX;
	}

	return mAllTile[pos.IntY()][pos.IntX()].GetTileType();
}




//void Map::SetUnitAt(MyVector2 pos, MyActor* obj)
//{
//	GameManager::GetMap()->mAllTile[pos.IntY()][pos.IntX()].SetUnit(obj);
//}
//
//void Map::UnsetUnitAt(MyVector2 pos)
//{
//	GameManager::GetMap()->mAllTile[pos.IntY()][pos.IntX()].UnsetUnit();
//}
//
//void Map::SetItemAt(MyVector2 pos, MyActor* obj)
//{
//	GameManager::GetMap()->mAllTile[pos.IntY()][pos.IntX()].SetItem(obj);
//}

void Map::ClearTiles()
{
	for (int y = 0; y < GameManager::ScreenSize().Y; y++)
	{
		for (int x = 0; x < GameManager::ScreenSize().X;  x++)
		{
			mAllTile[y][x].ClearTile();
		}
	}
}

void Map::AddWall(Tile* wall)
{
	mAllWall.push_back(wall);

}

void Map::AddMonster(Monster * monster)
{
	mMonsters.push_back(monster);
}

