#pragma once
#include <map>
#include <list>
#include <set>

#include "MyMacro.h"

#include "MyMath.h"


class MyActor;
class MyRenderer;
class MyCollider;

class MyScene
{
	friend MyActor;

	//////////////////////// inner class //////////////////////////////////////////////////
	class StaticConDes
	{
	public:
		~StaticConDes();
	};

	friend StaticConDes;
	static StaticConDes mConDes;

public:
	//////////////////////// virtual ////////////////////////////////////////
	virtual void Loading();
	virtual void SceneUpdate();
	virtual void Init();
	static void DebugUpdate();

	// 액터의 업데이트는 자식들이 구현할 필요가 없음
	void ActorUpdate();
	void ActorRender();
	void ActorCollision();

	void Link(int src, int dest);

	void DebugRender();
	void ActorDebugRender();
	void ColDebugRender();

	void Release();

	virtual void MakeMap() {}

	/////////////////////// public static /////////////////////////////////////
	template<typename T>
	static MyScene* CreateScene(const wchar_t* sceneName)
	{
		MyScene* newScene = new T();

		// 장면 로딩!
		mLoadingScene = newScene;
		// Loading은 장면의 Init 같은 것이다.
		newScene->Loading();
		mLoadingScene = nullptr;

		mAllScene.insert(std::map<std::wstring, MyScene*>::value_type(sceneName, newScene));

		return newScene;
	}

	// 이 씬을 현재 씬으로 설정
	static void SetCurScene(const wchar_t* sceneName);

	// 씬 실행
	static void Progress();

	void ColDebugSwitch()
	{
		mIsColDebug = !mIsColDebug;
	}

	//////////////////////////////////////////////////////////// Camera 관련 ///////////////////////////////////////////
	void CamPos(MyVector2 pos) { 
		mCamera = pos; 
		mCamera.X = std::floorf(mCamera.X);
		mCamera.Y = std::floorf(mCamera.Y);
	}
	MyVector2 CamPos() 
	{
		mCamera.X = std::floorf(mCamera.X);
		mCamera.Y = std::floorf(mCamera.Y);

		return mCamera;
	}
	void CamMove(MyVector2 pos) {
		mCamera += pos; 
		mCamera.X = std::floorf(mCamera.X);
		mCamera.Y = std::floorf(mCamera.Y);
	}

	void SortOnOff(LEAGUE_ORDER index)
	{
		if (mSortOption.end() == mSortOption.find(index))
		{
			mSortOption.insert(index);
			return;
		}
		else
		{
			mSortOption.erase(index);
			return;
		}
	}
protected:
	MyScene();
	virtual ~MyScene();

private:
	static MyScene* FindScene(const wchar_t* sceneName);
	static void AddActor(MyActor* actor);

	void AddRenderer(MyRenderer* renderer);

	////////////////////////////////////////////////////////////// Collider 관련 ///////////////////////////////////////////////
	void AddCollider(MyCollider* col);

	
	void CheckLeagueCol(
		std::map<int, std::list<MyCollider*>>::iterator& _Src) {};

	void CheckLeagueCol(
		std::map<int, std::list<MyCollider*>>::iterator& _Src
		, std::map<int, std::list<MyCollider*>>::iterator& _Other);

	
	
public:
	int mLevel = 1;
	int mChanging = false;
protected:
private:
	static std::map<std::wstring, MyScene*> mAllScene;
	static MyScene* mCurScene;
	static MyScene* mLoadingScene;

	MyVector2 mCamera;
	// std::set<int> mSortOption;

	// Actor
	std::list<MyActor*> mAllActor;

	// Renderer
	std::map<int, std::list<MyRenderer*>> mAllRendererLeague;

	// Iterators
	std::list<MyActor*>::iterator mActorIter;
	std::list<MyActor*>::iterator mActorEndIter;
	std::list<MyRenderer*>::iterator mRenderIter;
	std::list<MyRenderer*>::iterator mRenderEndIter;

	std::set<int> mSortOption;

	////////////////////////////////////////////////////////////// Collider 관련 ///////////////////////////////////////////////
	static bool mIsDebug;
	static bool mIsColDebug;

	std::map<int, std::set<int>> mLinkData;
	std::map<int, std::set<int>>::iterator mLinkStart;
	std::map<int, std::set<int>>::iterator mLinkEnd;
	std::set<int>::iterator mOtherLinkStart;
	std::set<int>::iterator mOtherLinkEnd;

	std::map<int, std::list<MyCollider*>> mAllColLeague;

	std::map<int, std::list<MyCollider*>>::iterator mDebugStart;
	std::map<int, std::list<MyCollider*>>::iterator mDebugEnd;

	std::map<int, std::list<MyCollider*>>::iterator mSrc;
	std::map<int, std::list<MyCollider*>>::iterator mOther;

	std::list<MyCollider*>::iterator mDebugListStart;
	std::list<MyCollider*>::iterator mDebugListEnd;

	std::list<MyCollider*>::iterator mSrcStart;
	std::list<MyCollider*>::iterator mSrcEnd;
	std::list<MyCollider*>::iterator mOtherStart;
	std::list<MyCollider*>::iterator mOtherEnd;

};

