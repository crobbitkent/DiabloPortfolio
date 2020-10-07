#include "MyActor.h"

#include "MyScene.h"
#include "MyWindowClass.h"
#include "MyRenderer.h"
#include "MyAnimationRenderer.h"

#include "Mytexture.h"
#include "MyTextureManager.h"
#include "MyCollider.h"

// 외부 라이브러리 추가하는 방법
#pragma comment(lib, "msimg32.lib")




MyCollider* MyActor::CreateCol(COL_TYPE _Type, COL_LEAGUE _League)
{
	MyCollider* NewCol = new MyCollider();
	NewCol->mActor = this;
	NewCol->mColType = _Type;
	NewCol->mLeague = _League;

	NewCol->Init();

	mScene->AddCollider(NewCol);

	mAllColList.push_back(NewCol);

	return NewCol;
}

MyActor::MyActor()
	: mIsDead(false)
{
	// 알아서 자신의 Scene에 들어간다.
	MyScene::AddActor(this);
}

// Renderer 삭제 권한은 Actor
MyActor::~MyActor()
{
	mRenderIter = mAllRenderList.begin();
	mRenderEndIter = mAllRenderList.end();

	for (; mRenderIter != mRenderEndIter; ++mRenderIter)
	{
		if (nullptr != (*mRenderIter))
		{
			delete (*mRenderIter);
			(*mRenderIter) = nullptr;
		}
	}

	mAllRenderList.clear();

	// 콜라이더 지우기
	mColStartIter = mAllColList.begin();
	mColEndIter = mAllColList.end();

	for (; mColStartIter != mColEndIter; ++mColStartIter)
	{
		if (nullptr != (*mColStartIter))
		{
			delete (*mColStartIter);
			(*mColStartIter) = nullptr;
		}
	}

	mAllColList.clear();
}

void MyActor::Release()
{
	mRenderIter = mAllRenderList.begin();
	mRenderEndIter = mAllRenderList.end();

	for (; mRenderIter != mRenderEndIter; )
	{
		if (true == (*mRenderIter)->IsDead())
		{
			delete (*mRenderIter);
			mRenderIter = mAllRenderList.erase(mRenderIter);
			continue;
		}

		++mRenderIter;
	}
}


void MyActor::Update()
{

}

void MyActor::DebugRender()
{
}




MyAnimationRenderer * MyActor::CreateAniRenderer(LEAGUE_ORDER order)
{
	MyAnimationRenderer* newRender = new MyAnimationRenderer();

	newRender->mActor = this;
	newRender->mOrder = order;
	mScene->AddRenderer(newRender);

	mAllRenderList.push_back(newRender);

	return newRender;
}

MyVector2 MyActor::CalPos()
{
	return Pos() - mScene->CamPos();
}

MyVector2 MyActor::CamPos()
{
	return mScene->CamPos();
}

//void MyActor::Render()
//{
//	// Rectangle(MyWindowClass::MainDC(), mRect.IntLeft(), mRect.IntTop(), mRect.IntRight(), mRect.IntBottom());
//
//	MyTexture* tex = MyTextureManager::Inst().FindTexture(L"0Diablo.bmp");
//
//	// 이미지를 찾지 못하면 터뜨린다.
//	// 이미지가 있어도 MyTextureManager에 등록이 되어 있지 않으면 터진다.
//	if (nullptr == tex)
//	{
//		BOOM;
//	}
//
//	TransparentBlt(MyWindowClass::MainDC()
//		, mRect.IntLeft(), mRect.IntTop(), 800, 600
//		, tex->TextureDC()
//		, 0, 0, 800, 600
//		, 1);
//
//	// 아래 코드는 RGBA를 뽑아내는 코드
//
//*int getColor = tex->GetTexturePixel(0, 0);
//char arrColor[4];
//
//memcpy_s(arrColor, sizeof(int), &getColor, sizeof(int));
//
//int color = 0;
//char* textureColor = (char*)&color;*/
//}
