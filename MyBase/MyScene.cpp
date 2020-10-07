#include "MyScene.h"
#include "MyActor.h"

#include "MyRenderer.h"
#include "MyTextureManager.h"
#include "MyWindowClass.h"
#include "MyCollider.h"
#include "MyInput.h"
#include "MyBaseUpdater.h"

/////////////////// static initialising //////////////////////
std::map<std::wstring, MyScene*> MyScene::mAllScene;
MyScene* MyScene::mCurScene = nullptr;
MyScene* MyScene::mLoadingScene = nullptr;

bool MyScene::mIsDebug = false;
bool MyScene::mIsColDebug = false;

/////////////////// Auto Construct Destruct //////////////////////
MyScene::StaticConDes MyScene::mConDes;

MyScene::StaticConDes::~StaticConDes()
{
	std::map<std::wstring, MyScene*>::iterator StartIter = MyScene::mAllScene.begin();
	std::map<std::wstring, MyScene*>::iterator EndIter = MyScene::mAllScene.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	MyScene::mAllScene.clear();
}
/////////////////// Auto Construct Destruct Over ///////////////////

//////////////////// virtual ///////////////////////////////////

void MyScene::Loading()
{
	CamPos(MyVector2(-320.f, -240.f + HALF_TILE_HEIGHT * 2));
}

void MyScene::SceneUpdate()
{
	
}

void MyScene::Init()
{
}

void MyScene::DebugUpdate()
{
	if (true == MyInput::Down(L"DEBUG"))
	{
		if (true == mIsDebug)
		{
			mIsDebug = false;
		}
		else
		{
			mIsDebug = true;
		}
	}

	if (true == MyInput::Down(L"COLDEBUG"))
	{
		if (true == mIsColDebug)
		{
			mIsColDebug = false;
		}
		else
		{
			mIsColDebug = true;
		}
	}
}

/////////////////////// static public ///////////////////////

// ���� ���� Scene�� Render�Ѵ�.
void MyScene::Progress()
{
	//if (nullptr != m_NextState)
	//{
	//	m_CurState = m_NextState;
	//	m_NextState = nullptr;
	//	HGAMETIME::Reset();
	//	HGAMETIME::Update();
	//	HVEC2::DIRUPDATE();
	//}

	DebugUpdate();

	mCurScene->SceneUpdate();
	mCurScene->ActorUpdate();

	// Render ���� ����� �ʱ�ȭ
	Rectangle(MyTextureManager::BackDC(), 0, 0, MyTextureManager::BackDCSize().IntX(), MyTextureManager::BackDCSize().IntY());

	// ��� Actor ����
	mCurScene->ActorRender();

	// Collider ����� ����
	if (true == mIsColDebug)
	{
		mCurScene->ColDebugRender();
		mCurScene->DebugRender();
	}

	// ����� ����
	if (true == mIsDebug)
	{
		mCurScene->ActorDebugRender();
	}

	// Render �� �����쿡 �ѹ��� ��´�.
	BitBlt(MyWindowClass::MainDC(), 0, 0, MyTextureManager::BackDCSize().IntX(), MyTextureManager::BackDCSize().IntY()
		, MyTextureManager::BackDC(), 0, 0, SRCCOPY);

	mCurScene->ActorCollision();

	mCurScene->Release();
}



MyScene* MyScene::FindScene(const wchar_t * sceneName)
{
	std::map<std::wstring, MyScene*>::iterator findIter = mAllScene.find(sceneName);

	if (mAllScene.end() == findIter)
	{
		return nullptr;
	}

	return findIter->second;
}

// ��� Actor�� �ڽ��� Scene�� ����ȴ�.
void MyScene::AddActor(MyActor * actor)
{
	if (nullptr != mLoadingScene)
	{
		actor->mScene = mLoadingScene;
		mLoadingScene->mAllActor.push_back(actor);
	}
	else if (nullptr != mCurScene)
	{
		actor->mScene = mCurScene;
		mCurScene->mAllActor.push_back(actor);
	}
}

// Actor�� ���� Renderer�� List�� 
// Order�� ���� Map�� ����ȴ�. 
void MyScene::AddRenderer(MyRenderer* renderer)
{
	// ���� �������� ����
	auto iter = mAllRendererLeague.find(renderer->Order());

	if (iter == mAllRendererLeague.end())
	{
		mAllRendererLeague.insert(
			std::map<LEAGUE_ORDER, std::list<MyRenderer*>>::value_type(
				renderer->Order(),
				std::list<MyRenderer*>()
				));	

		iter = mAllRendererLeague.find(renderer->Order());
	}

	iter->second.push_back(renderer);
}

void MyScene::AddCollider(MyCollider * col)
{
	std::map<int, std::list<MyCollider*>>::iterator FindIter = mAllColLeague.find(col->mLeague);

	if (FindIter == mAllColLeague.end())
	{
		mAllColLeague.insert(
			std::map<int, std::list<MyCollider*>>::value_type(
				col->mLeague,
				std::list<MyCollider*>()
			));

		FindIter = mAllColLeague.find(col->mLeague);
	}

	FindIter->second.push_back(col);
}

// � �浹ü�� �浹�� �� �ִ��� ����
void MyScene::Link(int src, int dest)
{
	std::map<int, std::set<int>>::iterator Find = mLinkData.find(src);

	if (mLinkData.end() == Find)
	{
		mLinkData.insert(std::map<int, std::set<int>>::value_type(src, std::set<int>()));
		Find = mLinkData.find(src);
	}

	std::set<int>::iterator FindDest = Find->second.find(dest);

	if (FindDest != Find->second.end())
	{
		BOOM;
	}

	Find->second.insert(dest);
}

// �浹ü ���׿� �ٸ� �浹ü ���׿� �浹�ߴ��� Ȯ��
void MyScene::CheckLeagueCol(std::map<int, std::list<MyCollider*>>::iterator & _Src, std::map<int, std::list<MyCollider*>>::iterator & _Other)
{
	mSrcStart = _Src->second.begin();
	mSrcEnd = _Src->second.end();

	for (; mSrcStart != mSrcEnd; ++mSrcStart)
	{
		mOtherStart = _Other->second.begin();
		mOtherEnd = _Other->second.end();

		// ����ȭ�� => ȭ�� �ٱ��� �ִ� �ֵ��� �浹 �Ƚ�Ű�� ����� �ִ�.
		for (; mOtherStart != mOtherEnd; ++mOtherStart)
		{
			if (false == (*mSrcStart)->IsUpdated())
			{
				continue;
			}
			
			(*mSrcStart)->CollisionCheck(*mOtherStart);
		}
	}
}


/////////////////// normal public ///////////////////////////////

void MyScene::ActorUpdate()
{
	mActorIter = mAllActor.begin();
	mActorEndIter = mAllActor.end();

	for (; mActorIter != mActorEndIter; ++mActorIter)
	{
		if (nullptr != *mActorIter)
		{
			if (false == (*mActorIter)->IsUpdated())
			{
				continue;
			}

			(*mActorIter)->Update();

		}
	}
}



bool YSort(MyRenderer * left, MyRenderer * right)
{
	 return left->CalPos().Y < right->CalPos().Y;
}

	//bool XSort(MyRenderer * left, MyRenderer * right)
	//{
	//	return left->GetTileIndex().X < right->GetTileIndex().X;
	//}

// Actor�� ���� RendererList
void MyScene::ActorRender()
{
	std::map<int, std::list<MyRenderer*>>::iterator leagueIter = mAllRendererLeague.begin();
	std::map<int, std::list<MyRenderer*>>::iterator leagueEndIter = mAllRendererLeague.end();

	for (; leagueIter != leagueEndIter; ++leagueIter)
	{
		// YYYYYYYYYYYYYYYYYY SSSSSSSSSSSSSS OOOOOOOOOOOOOOOOOOOOO RRRRRRRRRRRRRR TTTTTTTTTTTTTTTTT
		if (mSortOption.end() != mSortOption.find(leagueIter->first))
		{
			leagueIter->second.sort(YSort);
		}
		// YYYYYYYYYYYYYYYYYY SSSSSSSSSSSSSS OOOOOOOOOOOOOOOOOOOOO RRRRRRRRRRRRRR TTTTTTTTTTTTTTTTT

		mRenderIter = leagueIter->second.begin();
		mRenderEndIter = leagueIter->second.end();

		for (; mRenderIter != mRenderEndIter; ++mRenderIter)
		{
			if (false == (*mRenderIter)->IsUpdated())
			{
				continue;
			}

			(*mRenderIter)->Render();
			
		}
	}

	//mActorStartIter = mAllActor.begin();
	//mActorEndIter = mAllActor.end();

	//for (; mActorStartIter != mActorEndIter; ++mActorStartIter)
	//{
	//	if (nullptr != *mActorStartIter)
	//	{
	//		(*mActorStartIter)->Render();
	//	}
	//}
}

void MyScene::DebugRender()
{
	std::map<int, std::list<MyRenderer*>>::iterator leagueIter = mAllRendererLeague.begin();
	std::map<int, std::list<MyRenderer*>>::iterator leagueEndIter = mAllRendererLeague.end();

	for (; leagueIter != leagueEndIter; ++leagueIter)
	{
		// YYYYYYYYYYYYYYYYYY SSSSSSSSSSSSSS OOOOOOOOOOOOOOOOOOOOO RRRRRRRRRRRRRR TTTTTTTTTTTTTTTTT
		if (mSortOption.end() != mSortOption.find(leagueIter->first))
		{
			leagueIter->second.sort(YSort);
		}
		// YYYYYYYYYYYYYYYYYY SSSSSSSSSSSSSS OOOOOOOOOOOOOOOOOOOOO RRRRRRRRRRRRRR TTTTTTTTTTTTTTTTT

		mRenderIter = leagueIter->second.begin();
		mRenderEndIter = leagueIter->second.end();

		for (; mRenderIter != mRenderEndIter; ++mRenderIter)
		{

			if (false == (*mRenderIter)->IsUpdated())
			{
				continue;
			}
			(*mRenderIter)->DebugRender();
		}
	}

	//mActorStartIter = mAllActor.begin();
	//mActorEndIter = mAllActor.end();

	//for (; mActorStartIter != mActorEndIter; ++mActorStartIter)
	//{
	//	if (nullptr != *mActorStartIter)
	//	{
	//		(*mActorStartIter)->Render();
	//	}
	//}
}




// ���� �浹 ó��
void MyScene::ActorCollision()
{
	mLinkStart = mLinkData.begin();
	mLinkEnd = mLinkData.end();

	for (; mLinkStart != mLinkEnd; ++mLinkStart)
	{
		mOtherLinkStart = mLinkStart->second.begin();
		mOtherLinkEnd = mLinkStart->second.end();

		// 1�� �浹ü ���׸� ã�´�.
		mSrc = mAllColLeague.find(mLinkStart->first);

		// (MONSTER, MONSTERATT)
		for (; mOtherLinkStart != mOtherLinkEnd; ++mOtherLinkStart)
		{
			// 2�� �浹ü ����
			mOther = mAllColLeague.find((*mOtherLinkStart));

			// ã�� 2�� �浹ü ���װ� ������ ����...
			if (mOther == mAllColLeague.end())
			{
				continue;
			}

			if (mLinkStart->first != (*mOtherLinkStart))
			{
				// 1���� 2�� �浹ü ���� Ȯ��
				CheckLeagueCol(mSrc, mOther);
			}
			else
			{
				// 1���� 1�� �浹ü ���� Ȯ��
				CheckLeagueCol(mSrc);				
			}
		}
	}
}

void MyScene::ActorDebugRender()
{
	if (true == mIsDebug)
	{
		mActorIter = mAllActor.begin();
		mActorEndIter = mAllActor.end();

		for (; mActorIter != mActorEndIter; ++mActorIter)
		{
			if (nullptr != *mActorIter)
			{
				if (false == (*mActorIter)->IsUpdated())
				{
					continue;
				}
				(*mActorIter)->DebugRender();
			}
		}
	}
}



void MyScene::ColDebugRender()
{

	if (true == mIsDebug)
	{
		mDebugStart = mAllColLeague.begin();
		mDebugEnd = mAllColLeague.end();

		for (; mDebugStart != mDebugEnd; ++mDebugStart)
		{
			mDebugListStart = mDebugStart->second.begin();
			mDebugListEnd = mDebugStart->second.end();

			for (; mDebugListStart != mDebugListEnd; ++mDebugListStart)
			{
				(*mDebugListStart)->DebugRender();
			}
		}
	}
}

void MyScene::Release()
{
	std::map<int, std::list<MyRenderer*>>::iterator StartLeague = mAllRendererLeague.begin();
	std::map<int, std::list<MyRenderer*>>::iterator EndLeague = mAllRendererLeague.end();

	// ��������??? �Ⱥ�����.
	for (; StartLeague != EndLeague; ++StartLeague)
	{
		mRenderIter = StartLeague->second.begin();
		mRenderEndIter = StartLeague->second.end();

		for (; mRenderIter != mRenderEndIter; )
		{
			// �θ� �׾��ų�
			// �θ����(����)�� ������ �ڻ��ϰ� ��ɳ�������.
			if (true == (*mRenderIter)->IsDead())
			{
				// �������� ���ߴ�. 
				mRenderIter = StartLeague->second.erase(mRenderIter);
				continue;
			}

			++mRenderIter;
		}
	}

	mDebugStart = mAllColLeague.begin();
	mDebugEnd = mAllColLeague.end();

	for (; mDebugStart != mDebugEnd; ++mDebugStart)
	{
		mDebugListStart = mDebugStart->second.begin();
		mDebugListEnd = mDebugStart->second.end();

		for (; mDebugListStart != mDebugListEnd; )
		{
			(*mDebugListStart)->DebugRender();

			if (true == (*mDebugListStart)->IsDead())
			{
				// �������� ���ߴ�. 
				mDebugListStart = mDebugStart->second.erase(mDebugListStart);
				continue;
			}

			++mDebugListStart;
		}
	}




	mActorIter = mAllActor.begin();
	mActorEndIter = mAllActor.end();

	for (; mActorIter != mActorEndIter; )
	{
		if (nullptr != *mActorIter &&
			true == (*mActorIter)->IsDead())
		{
			delete *mActorIter;
			mActorIter = mAllActor.erase(mActorIter);
			continue;
		}

		++mActorIter;
	}


}


void MyScene::SetCurScene(const wchar_t * sceneName)
{
	mCurScene = FindScene(sceneName);

	if (nullptr == mCurScene)
	{
		BOOM;
	}

	mCurScene->Init();
}



MyScene::MyScene()
{
}


MyScene::~MyScene()
{
	mActorIter = mAllActor.begin();
	mActorEndIter = mAllActor.end();

	for (; mActorIter != mActorEndIter; ++mActorIter)
	{
		if (nullptr != *mActorIter)
		{
			delete *mActorIter;
			*mActorIter = nullptr;
		}
	}

	mAllActor.clear();
}

