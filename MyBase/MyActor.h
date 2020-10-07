#pragma once

#include "MyBaseUpdater.h"

#include <list>

#include "MyMacro.h"
#include "MyMath.h"
#include "MyRenderer.h"

class MyScene;
class MyAnimationRenderer;
class MyCollider;

// Actor를 상속받는 모든 클래스는
// 반드시 생성자에 MyRenderer를 만들어 주어아 한다. => CreateRenderer 함수 사용하여 생성.
// 화면에 띄우지 않을 것이라면 추가하지 않아도 괜찮다.
class MyActor : public MyBaseUpdater
{
	friend MyScene;

public:
	MyActor();
	virtual ~MyActor();


	// 움직이게 한다.
	virtual void Update();
	virtual void DebugRender();

	// Actor를 생성하면 반드시 생성자에 MyRenderer를 할당해주어야 한다.
	// Actor가 속한 Scene의 RendererLeague 에 넣어준다.
	// Scene은 각각의 League를 가지고 있다.
	template<typename T>
	T* CreateRenderer(LEAGUE_ORDER order)
	{
		T* newRender = new T();

		newRender->mActor = this;
		newRender->mOrder = order;
		mScene->AddRenderer(newRender);

		mAllRenderList.push_back(newRender);

		return newRender;
	}

	MyAnimationRenderer* CreateAniRenderer(LEAGUE_ORDER order = LO_BG);

	MyScene* Scene() { return mScene; }
	void Scene(MyScene* scene) { mScene = scene; }

	void Pos(const MyVector2& pos) { mRect.Pos = pos; }
	MyVector2 Pos() const { return mRect.Pos; }
	// void Size(const MyVector2& size) { mRect.Size = size; }
	// MyVector2 Size() const { return mRect.Size; }

	MyVector2 CalPos();
	MyVector2 CamPos();

	void Move(const MyVector2& pos) { mRect.Pos += pos; }

	void KillThis() { mIsDead = true; }
	bool IsDead() { return mIsDead; }

	////////////////////////////////////////// Collision 관련 ////////////////////////////////////////////////
	virtual void ColStay(MyCollider* _ThisCol, MyCollider* _OtherCol) {}
	virtual void ColEnter(MyCollider* _ThisCol, MyCollider* _OtherCol) {}
	virtual void ColExit(MyCollider* _ThisCol, MyCollider* _OtherCol) {}

	MyCollider* CreateCol(COL_TYPE _Type, COL_LEAGUE _League);



protected:

private:
	void Release();

public:
protected:
	MyRect mRect;
	int mLevel;

private:
	MyScene* mScene;

	// bool mIsUpdate;
	bool mIsDead;

	///////////// Iter
	std::list<MyRenderer*> mAllRenderList;
	std::list<MyRenderer*>::iterator mRenderIter;
	std::list<MyRenderer*>::iterator mRenderEndIter;

	std::list<MyCollider*> mAllColList;
	std::list<MyCollider*>::iterator mColStartIter;
	std::list<MyCollider*>::iterator mColEndIter;


};

