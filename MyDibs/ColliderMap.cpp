#include "ColliderMap.h"

#include <MyMacro.h>
#include <MyTextureManager.h>
#include <MyWindowClass.h>
#include <MyInput.h>

#include "RandomMap.h"
#include "Values.h"
#include "Monster.h"
#include "Player.h"

//TileIndex ColliderMap::mDirArr[8]
//{
//	{1, 0},   // R
//	{1, -1},  // RT
//	{0, -1},  // T
//	{-1, -1}, // LT
//	{-1, 0},  // L
//	{-1, 1},  // LB
//	{0, 1},   // B
//	{1, 1},   // RB
//};

TileIndex ColliderMap::mDirArr[8]
{
	{1, 1},   // RB
	{0, 1},   // B
	{-1, 1},  // LB
	{-1, 0},  // L
	{-1, -1}, // LT
	{0, -1},  // T
	{1, -1},  // RT
	{1, 0},   // R
};


//RB,
//B,
//LB,
//L,
//LT,
//T,
//RT,
//R,


ColliderMap::ColliderMap(RandomMap* map)
	: mRandomMap(map)
	, mCount(0)
{
	for (size_t i = 0; i < 300; i++)
	{
		// POP FRONT
		PathNodePool.push_back(new PathNode());
	}

	// PathNodePool.pop_front();
	mCurNodeIter = PathNodePool.begin();
}

ColliderMap::~ColliderMap()
{
	{
		std::list<PathNode*>::iterator Start = PathNodePool.begin();
		std::list<PathNode*>::iterator End = PathNodePool.end();

		for (; Start != End; ++Start)
		{
			delete (*Start);
		}
	}

	//std::map<__int64, TileColRenderer*>::iterator Start = mAllTile.begin();
	//std::map<__int64, TileColRenderer*>::iterator End = mAllTile.end();

	//for (; Start != End; ++Start)
	//{
	//	if (nullptr != Start->second)
	//	{
	//		delete Start->second;
	//		Start->second = nullptr;
	//	}
	//}
}

void ColliderMap::Update()
{
	TileColRenderer::TileDebugSwitch();
}

void ColliderMap::SetPlayer(Player * player)
{
	std::map<__int64, TileColRenderer*>::iterator iter = mAllTile.begin();
	std::map<__int64, TileColRenderer*>::iterator iterEnd = mAllTile.end();

	mPlayer = player;

	for (; iter != iterEnd; ++iter)
	{
		if (nullptr != iter->second)
		{
			iter->second->SetPlayer(player);
		}
	}

	int a = 0;
}

TileColRenderer* ColliderMap::FindTile(int x, int y)
{
	TileIndex index = { x, y };

	std::map<__int64, TileColRenderer*>::iterator iter = mAllTile.find(index.Index);

	if (iter == mAllTile.end())
	{
		return nullptr;
	}

	return iter->second;
}

TileColRenderer* ColliderMap::FindTile(MyVector2 pos)
{
	return FindTile(pos.IntX(), pos.IntY());
}

TileColRenderer * ColliderMap::FindTile(TileIndex index)
{
	return FindTile(index.X, index.Y);
}

TileColRenderer * ColliderMap::CheckTile(int indexX, int indexY)
{
	TileColRenderer* temp = FindTile(indexX, indexY);

	// 타일에 누군가 있으면 역시 nullptr;
	if (nullptr == temp || nullptr != temp->GetActor())
	{
		return nullptr;
	}

	if (false == temp->IsMovable())
	{
		return nullptr;
	}

	return temp;
}

TileColRenderer * ColliderMap::CheckTile(float winX, float winY)
{
	TileColRenderer* temp = FindTile((int)winX, (int)winY);

	// 타일에 누군가 있으면 역시 nullptr;
	if (nullptr != temp->GetActor())
	{
		return nullptr;
	}

	return temp;
}

void ColliderMap::AddTile(int x, int y)
{
	TileColRenderer* findTile = FindTile(x, y);

	if (nullptr == findTile)
	{
		TileIndex indexKey = { x, y };
		findTile = CreateRenderer<TileColRenderer>(LO_DEBUG);
		
		findTile->Image(L"TileDebug.bmp");
		findTile->SetTileIndex(indexKey);
		// findTile->SetPlayer(mRandomMap->GetPlayer());

		mRandomMap->AddSubTile(findTile);

		mAllTile.insert(std::map<__int64, TileColRenderer*>::value_type(indexKey.Index, findTile));
	}
}

void ColliderMap::SetWalkable(int x, int y)
{
	TileColRenderer* findTile = FindTile(x, y);

	if (nullptr != findTile)
	{
		findTile->IsMovable(true);
		findTile->mIsFog = true;
	}
}

// 윈도우 좌표 -> 타일 좌표
TileColRenderer* ColliderMap::GetTile(float winX, float winY)
{
	int X = 0;
	int Y = 0;

	MyVector2 windowPos = { winX - CalPos().X/* - 2*/, winY - CalPos().Y + 32};

	X = (int)std::roundf((windowPos.X  / QUARTER_TILE_WIDTH + windowPos.Y / QUARTER_TILE_HEIGHT) / 2);
	Y = (int)std::roundf((windowPos.Y / QUARTER_TILE_HEIGHT + -(windowPos.X / QUARTER_TILE_WIDTH)) / 2);

	return FindTile(X, Y);
}

TileColRenderer* ColliderMap::GetTile(MyVector2 pos)
{
	int X = 0;
	int Y = 0;

	MyVector2 windowPos = { pos.X - CalPos().X/* - 2*/, pos.Y - CalPos().Y + 32 };

	X = (int)std::roundf((windowPos.X / QUARTER_TILE_WIDTH + windowPos.Y / QUARTER_TILE_HEIGHT) / 2);
	Y = (int)std::roundf((windowPos.Y / QUARTER_TILE_HEIGHT + -(windowPos.X / QUARTER_TILE_WIDTH)) / 2);

	return FindTile(X, Y);
}

TileColRenderer* ColliderMap::GetTile(int indexX, int indexY)
{
	return FindTile(indexX, indexY);
}

TileIndex ColliderMap::PosToIndex(MyVector2 pos)
{
	MyVector2 windowPos = { pos.X - CalPos().X, pos.Y - CalPos().Y + 32 };

	int x = (int)std::roundf((windowPos.X / QUARTER_TILE_WIDTH + windowPos.Y / QUARTER_TILE_HEIGHT) / 2);
	int y = (int)std::roundf((windowPos.Y / QUARTER_TILE_HEIGHT + -(windowPos.X / QUARTER_TILE_WIDTH)) / 2);

	TileIndex returnIndex;
	returnIndex.X = x;
	returnIndex.Y = y;

	return returnIndex;
}

TileIndex ColliderMap::PosToIndexM(MyVector2 pos)
{
	MyVector2 windowPos = { pos.X - CalPos().X/* - 2*/, pos.Y - CalPos().Y + 32 };

	int x = (int)std::roundf((windowPos.X / QUARTER_TILE_WIDTH + windowPos.Y / QUARTER_TILE_HEIGHT) / 2);
	int y = (int)std::roundf((windowPos.Y / QUARTER_TILE_HEIGHT + -(windowPos.X / QUARTER_TILE_WIDTH)) / 2);

	TileIndex returnIndex;
	returnIndex.X = x;
	returnIndex.Y = y;

	return returnIndex;
}

MyVector2 ColliderMap::IndexToPos(TileIndex index)
{
	MyVector2 returnVec;

	returnVec.X = (index.X * QUARTER_TILE_WIDTH) + (index.Y * -QUARTER_TILE_WIDTH) + TILE_SUBOFFSETX - 32;
	returnVec.Y = (index.X * QUARTER_TILE_HEIGHT) + (index.Y * QUARTER_TILE_HEIGHT) + TILE_SUBOFFSETY - 10 - 64 - 32;

	return returnVec;
}

MyVector2 ColliderMap::IndexToPosM(TileIndex index)
{
	MyVector2 returnVec;

	returnVec.X = (index.X * QUARTER_TILE_WIDTH) + (index.Y * -QUARTER_TILE_WIDTH) + TILE_SUBOFFSETX;
	returnVec.Y = (index.X * QUARTER_TILE_HEIGHT) + (index.Y * QUARTER_TILE_HEIGHT) + TILE_SUBOFFSETY - 64 - 32;

	//returnVec.X = (index.X * QUARTER_TILE_WIDTH) + (index.Y * -QUARTER_TILE_WIDTH);
	//returnVec.Y = (index.X * QUARTER_TILE_HEIGHT) + (index.Y * QUARTER_TILE_HEIGHT);

	return returnVec;
}

void ColliderMap::DebugRender()
{
	MyVector2 mouse = MyWindowClass::MainWindow()->MousePos();

	TileColRenderer* tempIndex = GetTile(mouse.X, mouse.Y);

	if (nullptr == tempIndex)
	{
		return;
	}

	wchar_t Arr[256] = { 0, };
	

	if (true == tempIndex->IsMovable())
	{
		swprintf_s(Arr, L"IsWalkable : true");
		TextOutW(MyTextureManager::BackDC(), 0, 75, Arr, lstrlen(Arr));
	}
	else
	{
		swprintf_s(Arr, L"IsWalkable : false");
		TextOutW(MyTextureManager::BackDC(), 0, 90, Arr, lstrlen(Arr));
	}

	if (true == MyInput::Press(L"RMOUSEBTN"))
	{
		swprintf_s(Arr, L"%d, %d", tempIndex->GetTileIndex().X, tempIndex->GetTileIndex().Y);
		TextOutW(MyTextureManager::BackDC(), 0, 105, Arr, lstrlen(Arr));

		swprintf_s(Arr, L"tile pos : %f, %f", tempIndex->CalPos().X, tempIndex->CalPos().Y);
		TextOutW(MyTextureManager::BackDC(), 0, 120, Arr, lstrlen(Arr));

		swprintf_s(Arr, L"mPos : %f, %f", tempIndex->mPos.X, tempIndex->mPos.Y);
		TextOutW(MyTextureManager::BackDC(), 0, 135, Arr, lstrlen(Arr));
	}
}


void ColliderMap::UpdateActorIndex(int x, int y, int prevX, int prevY, Monster* actor)
{
	TileColRenderer* newTile = FindTile(x, y);
	TileColRenderer* prevTile = FindTile(prevX, prevY);

	if (nullptr == newTile || nullptr == prevTile)
	{
		actor->Scene()->MakeMap();

		if (nullptr == newTile || nullptr == prevTile)
		{
			actor->Scene()->MakeMap();
		}

		mPlayer->SetCurPos(mRandomMap->mStartPos1.X, mRandomMap->mStartPos1.Y + 1);

		return;
	}

	newTile->SetActor(actor);
	prevTile->UnsetActor();
}

TileIndex ColliderMap::SetActorIndex(int x, int y, Monster* actor)
{
	if (y >= 80 || x >= 80)
	{
		return { -1, -1 };
	}

	TileColRenderer* newTile = FindTile(x, y);

	if (nullptr == newTile || false == newTile->IsMovable() || nullptr != newTile->GetActor())
	{
		SetActorIndex(x, y + 1, actor);
		mLoopCount++;

		if (mLoopCount >= 80)
		{
			return { -1, -1 };
		}	
	}



	newTile->SetActor(actor);

	return { x, y };
}

void ColliderMap::UnsetUnit(int x, int y)
{
	TileColRenderer* newTile = FindTile(x, y);

	if (nullptr == newTile)
	{
		BOOM;
	}

	newTile->UnsetActor();
}


///////////////////////////////////////////////////////////////////////////////// PathFinding //////////////////////////////////////////////////////////



bool ColliderMap::IsOpenList(TileIndex index)
{
	return mOpenIndexSet.find(index.Index) != mOpenIndexSet.end();
}



ColliderMap::PathNode * ColliderMap::CreatePathNode(MyVector2 pos)
{
	return CreatePathNode(PosToIndex(pos));
}

ColliderMap::PathNode * ColliderMap::CreatePathNode(TileIndex index)
{
	ColliderMap::PathNode* newNode = *mCurNodeIter;
	newNode->Index = index;

	++mCurNodeIter;

	if (mCurNodeIter == PathNodePool.end())
	{
		mCurNodeIter = PathNodePool.begin();
	}

	return newNode;
}





ColliderMap::PathNode * ColliderMap::CreateOpenPathNode(MyVector2 pos, PathNode * parent)
{
	return CreateOpenPathNode(PosToIndex(pos), parent);
}

ColliderMap::PathNode * ColliderMap::CreateOpenPathNode(TileIndex index, PathNode * parent)
{
	PathNode* startNode = CreatePathNode(index);
	startNode->CalData(mEndIndex, parent);

	mOpenList.insert(std::multimap<float, PathNode*>::value_type(startNode->AllDis, startNode));
	mOpenIndexSet.insert(startNode->Index.Index);

	return startNode;
}





bool ColliderMap::FindAdj(PathNode * node)
{
	// 4방향인가 8방향인가를 결정할 수 있다.
	TileIndex curIndex = node->Index;
	TileIndex checkIndex;
	std::map<__int64, TileColRenderer*>::iterator TileDataIter;

	for (size_t i = 0; i < TILEDIR_MAX; ++i)
	{
		checkIndex = curIndex + mDirArr[i];

		if (mOpenIndexSet.end() != mOpenIndexSet.find(checkIndex.Index))
		{
			continue;
		}

		if (mClosedIndexSet.end() != mClosedIndexSet.find(checkIndex.Index))
		{
			continue;
		}

		mTileIter = mAllTile.find(checkIndex.Index);

		if (mAllTile.end() == mTileIter)
		{
			continue;
		}

		// 갈 수 없는 타일
		if (false == mTileIter->second->IsMovable())
		{
			continue;
		}

		// 이미 다른 캐릭터가 있다.
		if (nullptr != mTileIter->second->GetActor())
		{
			// 목적지에 사람이 있다.
			if (checkIndex == mEndIndex)
			{
				mEndNode = CreateOpenPathNode(checkIndex, node);

				return true;

			}
			continue;
		}

		mEndNode = CreateOpenPathNode(checkIndex, node);

		if (checkIndex == mEndIndex)
		{
			return true;
		}
	}

	mEndNode = nullptr;

	return false;
}

bool ColliderMap::FindAdjAttack(PathNode * node)
{
	// 4방향인가 8방향인가를 결정할 수 있다.
	TileIndex curIndex = node->Index;
	TileIndex checkIndex;
	std::map<__int64, TileColRenderer*>::iterator TileDataIter;

	for (size_t i = 0; i < TILEDIR_MAX; ++i)
	{
		checkIndex = curIndex + mDirArr[i];

		if (mOpenIndexSet.end() != mOpenIndexSet.find(checkIndex.Index))
		{
			continue;
		}

		if (mClosedIndexSet.end() != mClosedIndexSet.find(checkIndex.Index))
		{
			continue;
		}

		mTileIter = mAllTile.find(checkIndex.Index);

		if (mAllTile.end() == mTileIter)
		{
			continue;
		}

		// 갈 수 없는 타일
		if (false == mTileIter->second->IsMovable())
		{
			continue;
		}

		// 이미 다른 캐릭터가 있다.
		if (nullptr != mTileIter->second->GetActor())
		{
			if (checkIndex == mEndIndex)
			{
				return true;
			}

			continue;
		}

		mEndNode = CreateOpenPathNode(checkIndex, node);

		if (checkIndex == mEndIndex)
		{
			return true;
		}
	}

	mEndNode = nullptr;

	return false;
}





ColliderMap::PathNode * ColliderMap::OpenListPop()
{
	if (0 >= mOpenList.size())
	{
		return nullptr;
	}

	std::multimap<float, PathNode*>::iterator popNode = mOpenList.begin();
	PathNode* returnNode = popNode->second;
	mOpenList.erase(popNode);
	mOpenIndexSet.erase(returnNode->Index.Index);

	mClosedList.insert(std::multimap<__int64, PathNode*>::value_type(returnNode->Index.Index, returnNode));

	FindAdj(returnNode);

	return returnNode;
}

ColliderMap::PathNode * ColliderMap::OpenListPopAttack()
{
	if (0 >= mOpenList.size())
	{
		return nullptr;
	}

	std::multimap<float, PathNode*>::iterator popNode = mOpenList.begin();
	PathNode* returnNode = popNode->second;
	mOpenList.erase(popNode);
	mOpenIndexSet.erase(returnNode->Index.Index);

	mClosedList.insert(std::multimap<__int64, PathNode*>::value_type(returnNode->Index.Index, returnNode));

	FindAdjAttack(returnNode);

	return returnNode;
}





void ColliderMap::ResetPool()
{
	mCount = 0;

	mCurNodeIter = PathNodePool.begin();

	mOpenList.clear();
	mOpenIndexSet.clear();
	mClosedList.clear();
}

// 둘다 월드 포지션
std::list<MyVector2> ColliderMap::PathFind(MyVector2 startPos, MyVector2 endPos)
{
	mOpenList;
	mClosedList;

	std::list<MyVector2> tempPath;

	if (PosToIndex(startPos) == PosToIndex(endPos))
	{
		return tempPath;
	}

	TileIndex startIndex = PosToIndex(startPos);
	mEndIndex = PosToIndex(endPos);

	// 찍은 곳이 갈수 없는 곳이라면 아웃
	if (nullptr == GetTile(mEndIndex.X, mEndIndex.Y) ||
		false == GetTile(mEndIndex.X, mEndIndex.Y)->IsMovable())
	{
		return tempPath;
	}

	CreateOpenPathNode(startPos, nullptr);

	while (nullptr == mEndNode && 0 < mOpenList.size())
	{
		OpenListPop();

		++mCount;

		if (600 < mCount)
		{
			mEndNode = nullptr;
			ResetPool();

			tempPath.clear();

			return tempPath;
		}
	}

	while (nullptr != mEndNode)
	{
		TileIndex endIndex = mEndNode->Index;
		
		// 타일의 월드 포지션
		MyVector2 pos;

		pos.X = (endIndex.X * QUARTER_TILE_WIDTH) + (endIndex.Y * -QUARTER_TILE_WIDTH);
		pos.Y = (endIndex.X * QUARTER_TILE_HEIGHT) + (endIndex.Y * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;

		/*TileColRenderer* tempTile = FindTile(endIndex.X, endIndex.Y);
		pos = tempTile->mPos;*/

		tempPath.push_front(pos);

		mEndNode = mEndNode->Parent;

		if (600 < tempPath.size())
		{
			mEndNode = nullptr;
			ResetPool();

			tempPath.clear();

			return tempPath;
		}
	}

	mEndNode = nullptr;

	ResetPool();

	return tempPath;
}

std::list<MyVector2> ColliderMap::PathFindM(MyVector2 startPos, MyVector2 endPos)
{
	mOpenList;
	mClosedList;

	std::list<MyVector2> tempPath;

	// PathFindM의 이유
	if (PosToIndexM(startPos) == PosToIndex(endPos))
	{
		return tempPath;
	}

	// PathFindM의 이유
	TileIndex startIndex = PosToIndexM(startPos);
	mEndIndex = PosToIndex(endPos);

	// 찍은 곳이 갈수 없는 곳이라면 아웃
	if (nullptr == GetTile(mEndIndex.X, mEndIndex.Y) || false == GetTile(mEndIndex.X, mEndIndex.Y)->IsMovable())
	{
		return tempPath;
	}

	CreateOpenPathNode(startPos, nullptr);

	while (nullptr == mEndNode && 0 < mOpenList.size())
	{
		OpenListPop();

		++mCount;

		if (600 < mCount)
		{
			mEndNode = nullptr;
			ResetPool();

			tempPath.clear();

			return tempPath;
		}
	}

	while (nullptr != mEndNode)
	{
		TileIndex endIndex = mEndNode->Index;

		// 타일의 월드 포지션
		MyVector2 pos;

		pos.X = (endIndex.X * QUARTER_TILE_WIDTH) + (endIndex.Y * -QUARTER_TILE_WIDTH);
		pos.Y = (endIndex.X * QUARTER_TILE_HEIGHT) + (endIndex.Y * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;

		tempPath.push_front(pos);

		mEndNode = mEndNode->Parent;

		if (600 < tempPath.size())
		{
			mEndNode = nullptr;
			ResetPool();

			tempPath.clear();

			return tempPath;
		}
	}

	mEndNode = nullptr;

	ResetPool();

	return tempPath;
}

std::list<MyVector2> ColliderMap::PathFindAttackM(MyVector2 startPos, MyVector2 endPos)
{
	mOpenList;
	mClosedList;

	std::list<MyVector2> tempPath;

	if (PosToIndexM(startPos) == PosToIndex(endPos))
	{
		return tempPath;
	}

	TileIndex startIndex = PosToIndexM(startPos);
	mEndIndex = PosToIndex(endPos);



	if (nullptr == GetTile(mEndIndex.X, mEndIndex.Y) || false == GetTile(mEndIndex.X, mEndIndex.Y)->IsMovable())
	{
		return tempPath;
	}


	CreateOpenPathNode(startPos, nullptr);

	while (nullptr == mEndNode && 0 < mOpenList.size())
	{
		OpenListPopAttack();
	}

	while (nullptr != mEndNode)
	{
		TileIndex endIndex = mEndNode->Index;

		// 타일의 월드 포지션
		MyVector2 pos;

		pos.X = (endIndex.X * QUARTER_TILE_WIDTH) + (endIndex.Y * -QUARTER_TILE_WIDTH);
		pos.Y = (endIndex.X * QUARTER_TILE_HEIGHT) + (endIndex.Y * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;

		/*TileColRenderer* tempTile = FindTile(endIndex.X, endIndex.Y);
		pos = tempTile->mPos;*/

		tempPath.push_front(pos);

		mEndNode = mEndNode->Parent;
	}

	mEndNode = nullptr;

	ResetPool();

	return tempPath;
}
