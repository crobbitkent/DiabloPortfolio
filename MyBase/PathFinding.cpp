#include "PathFinding.h"
#include "Map.h"
#include "Tile.h"

PathFinding::PathFinding()
{
}


PathFinding::~PathFinding()
{
}

void PathFinding::SetTargetTile(Position pos)
{
	// mTargetTile = mMap->GetTile(pos.X, pos.Y);
}

// 가장 작은 F
Tile* PathFinding::FindLowestF(std::vector<Tile*>& list)
{
	Tile* lowest = list[0];

	
	std::vector<Tile*>::iterator lowestIter = list.begin();
	std::vector<Tile*>::iterator iter = list.begin();
	std::vector<Tile*>::iterator iterEnd = list.end();

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

Tile* PathFinding::FindEndHex(Tile* tile)
{
	std::stack<Tile*> tempPath;

	Tile* next = tile->mParentTile;

	while (nullptr != next)
	{
		tempPath.push(next);
		next = next->mParentTile;
	}

	Tile* endTile = nullptr;

	for (int i = 0; i < tempPath.size(); ++i)
	{
		endTile = tempPath.top();
		tempPath.pop();
	}

	return endTile;
}

float PathFinding::GetDistance(Tile* a, Tile* b)
{
	float distance = (float)(a->Pos().X - b->Pos().X) * float(a->Pos().X - b->Pos().X)
		+ (float)(a->Pos().Y - b->Pos().Y) * float(a->Pos().Y - b->Pos().Y);

	return (float)sqrtf(distance);
}

void PathFinding::GoToNextTile(Tile* tile)
{
	int Size = (int)mPath.size();
	for (int i = 0; i < Size; ++i)
	{
		mPath.pop();
	}
	
	if (true == mPath.empty())
	{
		tile->mIsTarget = true;

		Tile* next = tile;

		while (nullptr != next)
		{
			mPath.push(next);
			next = next->mParentTile;
		}
	}

	// 맨 위 = 제자리 => 없앤다.
	mPath.pop();
}

void PathFinding::FindPath(Tile* start, Tile* target)
{
	mStartTile = start;
	GameManager::GetMap()->ClearTiles();

	std::vector<Tile*> openList;
	std::vector<Tile*> closedList;

	openList.push_back(mStartTile);

	mStartTile->mH = GetDistance(target, mStartTile);
	mStartTile->mF = mStartTile->mH;

	while (openList.size() > 0)
	{
		Tile* currentTile = FindLowestF(openList);

		closedList.push_back(currentTile);

		if (currentTile == target)
		{
			GoToNextTile(currentTile);
			mStartTile = currentTile;
			return;
		}

		// 이웃
		std::vector<Tile*>::iterator iter = currentTile->mNeighbors.begin();
		std::vector<Tile*>::iterator iterEnd = currentTile->mNeighbors.end();

		bool CheckClosedList = false;
		bool CheckOpenList = false;

		for (; iter != iterEnd; ++iter)
		{
			CheckOpenList = false;
			CheckClosedList = false;

			std::vector<Tile*>::iterator iterC = closedList.begin();
			std::vector<Tile*>::iterator iterEndC = closedList.end();
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
				std::vector<Tile*>::iterator iterO = openList.begin();
				std::vector<Tile*>::iterator iterEndO = openList.end();
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