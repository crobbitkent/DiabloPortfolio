#pragma once

#include "MyBaseUpdater.h"

#include <list>

#include "MyMacro.h"
#include "MyMath.h"
#include "MyRenderer.h"

class MyScene;
class MyAnimationRenderer;
class MyCollider;

// Actor�� ��ӹ޴� ��� Ŭ������
// �ݵ�� �����ڿ� MyRenderer�� ����� �־�� �Ѵ�. => CreateRenderer �Լ� ����Ͽ� ����.
// ȭ�鿡 ����� ���� ���̶�� �߰����� �ʾƵ� ������.
class MyActor : public MyBaseUpdater
{
	friend MyScene;

public:
	MyActor();
	virtual ~MyActor();


	// �����̰� �Ѵ�.
	virtual void Update();
	virtual void DebugRender();

	// Actor�� �����ϸ� �ݵ�� �����ڿ� MyRenderer�� �Ҵ����־�� �Ѵ�.
	// Actor�� ���� Scene�� RendererLeague �� �־��ش�.
	// Scene�� ������ League�� ������ �ִ�.
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

	////////////////////////////////////////// Collision ���� ////////////////////////////////////////////////
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

