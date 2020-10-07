#pragma once

#include <vector>
#include <stack>

#include "MyMath.h"

template <typename TILE>
class PathFinding
{
public:
	PathFinding() {}
	~PathFinding() {}

	// void SetTargetTile(MyVector2 pos);

	TILE* FindLowestF(std::vector<TILE*>& list)
	{
		TILE* lowest = list[0];


		std::vector<TILE*>::iterator lowestIter = list.begin();
		std::vector<TILE*>::iterator iter = list.begin();
		std::vector<TILE*>::iterator iterEnd = list.end();

		for (; iter != iterEnd; ++iter)
		{
			float a = (*iter)->GetF();
			float b = lowest->GetF();

			if ((*iter)->GetF() < lowest->GetF())
			{
				lowest = (*iter);
				lowestIter = iter;
			}
		}

		list.erase(lowestIter);

		return lowest;
	}

	TILE* FindEndHex(TILE* tile)
	{
		std::stack<TILE*> tempPath;

		TILE* next = tile->mParentTile;

		while (nullptr != next)
		{
			tempPath.push(next);
			next = next->mParentTile;
		}

		TILE* endTile = nullptr;

		for (int i = 0; i < tempPath.size(); ++i)
		{
			endTile = tempPath.top();
			tempPath.pop();
		}

		return endTile;
	}

	float GetDistance(TILE* a, TILE* b)
	{
		float distance = (float)(a->Pos().X - b->Pos().X) * float(a->Pos().X - b->Pos().X)
			+ (float)(a->Pos().Y - b->Pos().Y) * float(a->Pos().Y - b->Pos().Y);

		return (float)sqrtf(distance);
	}

	void GoToNextTile(TILE* tile)
	{
		int Size = (int)mPath.size();
		for (int i = 0; i < Size; ++i)
		{
			mPath.pop();
		}

		if (true == mPath.empty())
		{
			tile->mIsTarget = true;

			TILE* next = tile;

			while (nullptr != next)
			{
				mPath.push(next);
				next = next->mParentTile;
			}
		}

		// 맨 위 = 제자리 => 없앤다.
		mPath.pop();
	}

	void FindPath(TILE* start, TILE* target)
	{
		mStartTile = start;
		// GameManager::GetMap()->ClearTiles();

		std::vector<TILE*> openList;
		std::vector<TILE*> closedList;

		openList.push_back(mStartTile);

		mStartTile->mH = GetDistance(target, mStartTile);
		mStartTile->mF = mStartTile->mH;

		while (openList.size() > 0)
		{
			TILE* currentTile = FindLowestF(openList);

			closedList.push_back(currentTile);

			if (currentTile == target)
			{
				GoToNextTile(currentTile);
				mStartTile = currentTile;
				return;
			}

			// 이웃
			std::vector<TILE*>::iterator iter = currentTile->mNeighbors.begin();
			std::vector<TILE*>::iterator iterEnd = currentTile->mNeighbors.end();

			bool CheckClosedList = false;
			bool CheckOpenList = false;

			for (; iter != iterEnd; ++iter)
			{
				CheckOpenList = false;
				CheckClosedList = false;

				std::vector<TILE*>::iterator iterC = closedList.begin();
				std::vector<TILE*>::iterator iterEndC = closedList.end();
				for (; iterC != iterEndC; ++iterC)
				{
					CheckOpenList = false;
					CheckClosedList = false;

					if ((*iter) == (*iterC))
					{
						CheckClosedList = true;
						break;
					}
				}

				// closedList에 없으면
				if (false == CheckClosedList)
				{
					std::vector<TILE*>::iterator iterO = openList.begin();
					std::vector<TILE*>::iterator iterEndO = openList.end();
					for (; iterO != iterEndO; ++iterO)
					{

						if ((*iter) == (*iterO))
						{
							CheckOpenList = true;

							float tempG = (float)currentTile->mG + (*iter)->GetTileCost();

							// 현재 헥스까지의 비용과 이웃 헥스까지의 비용을 비교
							// 현재 헥스까지의 비용이 낮으면
							if (tempG < (*iter)->mG)
							{
								(*iter)->mParentTile = currentTile;

								(*iter)->mG = tempG;
								(*iter)->mF = (*iter)->mG + (*iter)->mH;
							}
						}
					}
				}

				// 둘다 없으면
				if (false == CheckClosedList && false == CheckOpenList)
				{
					(*iter)->mParentTile = currentTile;

					(*iter)->mG = currentTile->mG + (*iter)->GetTileCost();
					(*iter)->mH = GetDistance((*iter), target);
					(*iter)->mF = (*iter)->mG + (*iter)->mH;

					openList.push_back((*iter));
				}
			}
		}
	}

protected:
	std::stack<TILE*> mPath;

private:
	TILE* mStartTile;
	// Tile* mTargetTile;
};




/*
----- TILE 클래스에 있어야할 변수
friend PathFinding;


std::vector<Tile*> mNeighbors
int mG
int mF
int mH
Tile* mParentTile
MyVector2 mPositon;

----- TILE 클래스에 있어야할 변수
float GetF()
float GetH()
float GetG()
float GetTileCost()
MyVector2 Pos();
*/