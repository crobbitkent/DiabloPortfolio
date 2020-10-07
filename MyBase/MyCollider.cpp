#include "MyCollider.h"

#include <Windows.h>
#include "MyTextureManager.h"
#include "MyActor.h"

bool(__stdcall *MyCollider::CollisionCheckFunc[COL_TYPE::CT_MAX][COL_TYPE::CT_MAX])(const MyRect& _Left, const MyRect& _Right)
{
	{ &MyMath::RectToRect, &MyMath::RectToCircle, &MyMath::RectToPoint },
	{ &MyMath::CircleToRect, &MyMath::CircleToCircle, &MyMath::CircleToPoint },
	{ &MyMath::PointToRect, &MyMath::PointToCircle, &MyMath::PointToPoint }
};

MyCollider::MyCollider()
{
}


MyCollider::~MyCollider()
{
	EndColCheck();
}

void MyCollider::Init()
{
	switch (mColType)
	{
	case CT_RECT2D:
		DebugRenderFunc = &MyCollider::RectRender;
		break;
	case CT_CIRCLE2D:
		DebugRenderFunc = &MyCollider::CircleRender;
		break;
	case CT_POINT2D:
		DebugRenderFunc = &MyCollider::PointRender;
		break;
	case CT_MAX:
		BOOM;
		break;
	default:
		break;
	}
}

void MyCollider::DebugRender()
{
	(this->*DebugRenderFunc)();
}

void MyCollider::RectRender()
{
	MyRect rc = CalRect();
	Rectangle(MyTextureManager::BackDC(), rc.IntLeft(), rc.IntTop(), rc.IntRight(), rc.IntBottom());
}

void MyCollider::CircleRender()
{
}

void MyCollider::PointRender()
{
}

void MyCollider::CollisionCheck(MyCollider * other)
{
	// 충돌 했다면...
	if (true == CollisionCheckFunc[mColType][other->mColType](CalRect(), other->CalRect()))
	{
		// 상대편의 충돌체이다.

		if (mColSet.end() == mColSet.find(other))
		{
			mColSet.insert(other);
			other->mColSet.insert(this);
			Actor()->ColEnter(this, other);
			other->Actor()->ColEnter(other, this);
		}
		else {
			Actor()->ColStay(this, other);
			other->Actor()->ColStay(other, this);
		}
		// 객체와 객체가 소통하는 일반적인 방식.
	}
	else
	{
		if (mColSet.end() != mColSet.find(other))
		{
			mColSet.erase(other);
			other->mColSet.erase(this);
			Actor()->ColExit(this, other);
			other->Actor()->ColExit(other, this);
		}
	}
}

void MyCollider::EndColCheck()
{
	std::set<MyCollider*>::iterator start = mColSet.begin();
	std::set<MyCollider*>::iterator end = mColSet.end();

	for (; start != end; ++start)
	{
		(*start)->mColSet.erase(this);
		(*start)->Actor()->ColExit((*start), this);
		Actor()->ColExit(this, (*start));
	}

	mColSet.clear();
}
