#pragma once
#include "MySubActor.h"
#include <set>

#include "MyMacro.h"
#include "MyMath.h"

class MyScene;

class MyCollider : public MySubActor
{
	friend MyScene;
	friend MyActor;

public:
	static bool(__stdcall *CollisionCheckFunc[COL_TYPE::CT_MAX][COL_TYPE::CT_MAX])(const MyRect& _Left, const MyRect& _Right);

	MyCollider();
	~MyCollider();

protected:
private:
	void Init();

	void DebugRender();
	void(MyCollider::*DebugRenderFunc)();

	void RectRender();
	void CircleRender();
	void PointRender();

	void CollisionCheck(MyCollider* other);

	void Off() override
	{
		MyBaseUpdater::Off();
		EndColCheck();
	}

	void EndColCheck();
public:
protected:
private:
	COL_LEAGUE mLeague;
	COL_TYPE mColType;
	std::set<MyCollider*> mColSet;
};

