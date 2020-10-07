#include "MyActor.h"

#include "MyScene.h"
#include "MyWindowClass.h"
#include "MyRenderer.h"
#include "MyAnimationRenderer.h"

#include "Mytexture.h"
#include "MyTextureManager.h"
#include "MyCollider.h"

// �ܺ� ���̺귯�� �߰��ϴ� ���
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
	// �˾Ƽ� �ڽ��� Scene�� ����.
	MyScene::AddActor(this);
}

// Renderer ���� ������ Actor
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

	// �ݶ��̴� �����
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
//	// �̹����� ã�� ���ϸ� �Ͷ߸���.
//	// �̹����� �־ MyTextureManager�� ����� �Ǿ� ���� ������ ������.
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
//	// �Ʒ� �ڵ�� RGBA�� �̾Ƴ��� �ڵ�
//
//*int getColor = tex->GetTexturePixel(0, 0);
//char arrColor[4];
//
//memcpy_s(arrColor, sizeof(int), &getColor, sizeof(int));
//
//int color = 0;
//char* textureColor = (char*)&color;*/
//}
