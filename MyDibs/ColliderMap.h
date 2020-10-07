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

// DebugRender�� ���ؼ� MyActor ���
// ���߿� ��� Ǯ� ������
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

	// �ε��� x, y ��ġ�� Actor�� ����.
	void UpdateActorIndex(int x, int y, int prevX, int prevY, Monster* actor);
	TileIndex SetActorIndex(int x, int y, Monster* actor);

	void UnsetUnit(int x, int y);
public:


	///////////////////////////////////////////////////////////////////////////////// PathFinding //////////////////////////////////////////////////////////
	class PathNode
	{
	public:
		// �밢�� �̵��� �ǳ� �ȵǳ�?
		float StartDis; // ���������� �������ؼ� �ɸ� �Ÿ�
		float EndDis; // �ִܰŸ�
		float AllDis; // StartDis + EndDis ���� ��ģ �Ÿ�

		PathNode* Parent;
		TileIndex Index;

		void CalData(TileIndex endIndex, PathNode* parent)
		{
			// ���� ù ����� ��� => startDis�� ������� ����
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

	PathNode* OpenListPop(); // Ž���� ���ؼ� �����Ѵ�.
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

	std::multimap<float, PathNode*> mOpenList; // ���� �˻����� ���� ���
	std::set<__int64> mOpenIndexSet; // ���� �˻����� ���� ���

	std::multimap<__int64, PathNode*> mClosedList; // �˻� ���� ���
	std::set<__int64> mClosedIndexSet; // �˻� ���� ���-

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

