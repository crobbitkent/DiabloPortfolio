#pragma once

#include <map>
#include <list>
#include <set>

#include <MyActor.h>

#include "TileColRenderer.h"

class Monster;
class RandomMap;
class TileColRenderer;
class Player;
class ItemManager;

// DebugRender를 위해서 MyActor 상속
// 나중에 상속 풀어도 괜찮음
class ColliderMap : public MyActor
{
	friend ItemManager;

public:
	ColliderMap(RandomMap* map);
	~ColliderMap();

	void Update() override;

	void SetPlayer(Player* player);

	TileColRenderer* FindTile(int x, int y);
	TileColRenderer* FindTile(MyVector2 pos);
	TileColRenderer* FindTile(TileIndex index);

	TileColRenderer* CheckTile(int indexX, int indexY);
	TileColRenderer* CheckTile(float winX, float winY);

	TileColRenderer* GetTile(float winX, float winY);
	TileColRenderer* GetTile(int indexX, int indexY);
	TileColRenderer* GetTile(MyVector2 pos);

	TileIndex PosToIndex(MyVector2 pos);
	TileIndex PosToIndexM(MyVector2 pos);

	MyVector2 IndexToPos(TileIndex index);
	MyVector2 IndexToPosM(TileIndex index);

	void AddTile(int x, int y);
	void SetWalkable(int x, int y);


	void DebugRender() override;

	// 인덱스 x, y 위치에 Actor를 저장.
	void UpdateActorIndex(int x, int y, int prevX, int prevY, Monster* actor);
	TileIndex SetActorIndex(int x, int y, Monster* actor);

	void UnsetUnit(int x, int y);
public:


	///////////////////////////////////////////////////////////////////////////////// PathFinding //////////////////////////////////////////////////////////
	class PathNode
	{
	public:
		// 대각선 이동이 되냐 안되냐?
		float StartDis; // 시작점까지 오기위해서 걸린 거리
		float EndDis; // 최단거리
		float AllDis; // StartDis + EndDis 둘을 합친 거리

		PathNode* Parent;
		TileIndex Index;

		void CalData(TileIndex endIndex, PathNode* parent)
		{
			// 내가 첫 노드인 경우 => startDis를 계산하지 않음
			if (nullptr == parent)
			{
				Parent = nullptr;
				StartDis = 0.0f;
			}
			else
			{
				StartDis = parent->StartDis;
				Parent = parent;
				MyVector2 End = { Parent->Index.X, Parent->Index.Y };
				MyVector2 Start = { Index.X, Index.Y };
				MyVector2 Dis = Start - End;
				StartDis += Dis.Length();
			}

			{
				MyVector2 End = { endIndex.X, endIndex.Y };
				MyVector2 Start = { Index.X, Index.Y };
				MyVector2 Dis = Start - End;
				EndDis = Dis.Length();
				AllDis = StartDis + EndDis;
			}
		}
	};

	std::list<MyVector2> PathFind(MyVector2 startPos, MyVector2 endPos);
	// std::list<MyVector2> PathFindAttack(MyVector2 startPos, MyVector2 endPos);

	std::list<MyVector2> PathFindM(MyVector2 startPos, MyVector2 endPos);
	std::list<MyVector2> PathFindAttackM(MyVector2 startPos, MyVector2 endPos);

private:
	bool IsOpenList(TileIndex index);

	void ResetPool();

	PathNode* OpenListPop(); // 탐색을 위해서 제거한다.
	PathNode* OpenListPopAttack();

	PathNode* CreateOpenPathNode(MyVector2 pos, PathNode* parent);
	PathNode* CreateOpenPathNode(TileIndex index, PathNode* parent);
	PathNode* CreatePathNode(MyVector2 pos);
	PathNode* CreatePathNode(TileIndex index);

	bool FindAdj(PathNode* node);
	bool FindAdjAttack(PathNode* node);

public:
	static TileIndex mDirArr[8];

private:
	enum TILEDIR
	{
		TILEDIR_MAX = 8
	};



	std::list<PathNode*>::iterator mCurNodeIter;
	std::list<PathNode*> PathNodePool;

	std::multimap<float, PathNode*> mOpenList; // 아직 검사하지 않은 노드
	std::set<__int64> mOpenIndexSet; // 아직 검사하지 않은 노드

	std::multimap<__int64, PathNode*> mClosedList; // 검사 끝낸 노드
	std::set<__int64> mClosedIndexSet; // 검사 끝낸 노드-

	std::map<__int64, TileColRenderer*>::iterator mTileIter;
	std::map<__int64, TileColRenderer*>::iterator mTileEndIter;
	std::map<__int64, TileColRenderer*> mAllTile;


	PathNode* mEndNode;
	TileIndex mEndIndex;
	RandomMap* mRandomMap;

	int mCount;
	Player* mPlayer;
	//std::vector<TileColRenderer*> mNe

	int mLoopCount = 0;
};

