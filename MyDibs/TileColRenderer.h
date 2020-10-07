#pragma once

#include <map>

#include <MyMath.h>
#include <MyRenderer.h>

#include "Values.h"
#include "DibTile.h"

#include "Monster.h"
#include "ColliderMap.h"

class Monster;
class Player;
class Item;

class TileColRenderer : public MyRenderer
{
	friend ColliderMap;
	friend DibTile;

public:
	TileColRenderer();
	~TileColRenderer();

	void SetPlayer(Player* player);
	void SetActor(Monster* actor);
	void SetItem(Item* item) { mItem = item; }
	void UnsetItem() { mItem = nullptr; }
	void UnsetActor() { mMyActor = nullptr;}

	Item* GetItem() { return mItem; }

	Monster* GetActor() { return mMyActor; }

	Monster* MoveOutActor() { Monster* temp = mMyActor; mMyActor = nullptr; return temp; }


	void Render() override;
	void DebugRender() override;
	void Image(const wchar_t* name, int index = 0) override;

	void IsMovable(bool movable) { mIsMovable = movable; }
	bool IsMovable() { return mIsMovable; }

	static void TileDebugSwitch();


	MyVector2 Pos() const { return MySubActor::GetPos(); }
public:


private:
	static bool mIsDebugging;

	bool mIsFog;
	bool mIsMovable = false;
	Monster* mMyActor = nullptr;
	MyVector2 mPos;

	Player* mPlayer;
	Item* mItem = nullptr;
};

