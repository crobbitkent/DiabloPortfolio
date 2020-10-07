#include "MainTitle.h"

#include <MyRenderer.h>
#include <MyAnimationRenderer.h>
#include <MyWindowClass.h>
#include <MyInput.h>

#include "OpeningDiablo.h"

MainTitle::MainTitle()
{
}


MainTitle::~MainTitle()
{
}

void MainTitle::Loading()
{
	//mAnimationRenderer = CreateAniRenderer(LO_BG);
	//mAnimationRenderer->SubSize({ 640.f * 1.3f , 640 / 2.f * 1.3f });

	//mAnimationRenderer->CreateAnimation(L"Opening", L"BlizzardOpening.bmp", 0, 239, true, 0.07f);

	//mAnimationRenderer->SetCurrentAnimation(L"Opening");
	mStartX = 140;
	mStartY = 190;

	mMain = new MyActor();
	mMain->Pos({ 640 / 2.f, 480 / 2.f });
	mMainRenderer = mMain->CreateRenderer<MyRenderer>(LO_BBG);
	mMainRenderer->Image(L"Select.bmp");
	mMainRenderer->SubPos({ 0 , 0 });
	mMainRenderer->SubSize({ 640 , 480 });
	mMainRenderer->SetRenderStyle(RS_BIT);

	mFire = mMain->CreateAniRenderer(LO_BG);
	mFire->SubPos({ 0 , -160 });
	mFire->SubSize({ 390 , 154 });
	mFire->CreateAnimation(L"Fire", L"SmallFire.bmp", 0, 14, true);
	mFire->TransColor(255, 255, 255);

	mStar1 = mMain->CreateAniRenderer(LO_BG);
	mStar1->SubPos({ -200 , -25 });
	mStar1->SubSize({ 49 , 48 });
	mStar1->CreateAnimation(L"Star1", L"Pentagram.bmp", 0, 8, true);
	mStar1->TransColor(255, 255, 255);

	mStar2 = mMain->CreateAniRenderer(LO_BG);
	mStar2->SubPos({ 200 , -25 });
	mStar2->SubSize({ 49 , 48 });
	mStar2->CreateAnimation(L"Star2", L"Pentagram.bmp", 0, 8, true);
	mStar2->TransColor(255, 255, 255);



}

void MainTitle::SceneUpdate()
{
	mMousePos = MyWindowClass::MainWindow()->MousePos();

	// SINGLE PLAYER
	if (mMousePos.IntX() >= mStartX && mMousePos.IntX() < mStartX + mGapX && mMousePos.IntY() >= mStartY && mMousePos.IntY() < mStartY + mGapY)
	{
		if (false == mMoveOn1)
		{
			SSmove = MyGameSound::SoundPlay(L"titlemov.wav");
			mStar1->SubPos({ -200 , -25 });
			mStar2->SubPos({ 200 , -25 });
			mStar1->On();
			mStar2->On();
			mMoveOn1 = true;
			mMoveOn2 = false;
			mMoveOn3 = false;
			mMoveOn4 = false;
			mMoveOn5 = false;
			return;
		}
		else
		{
			if (true == MyInput::Down(L"LMOUSEBTN"))
			{
				if (true == mSelectOn)
				{
					SSselect->Stop();
					mSelectOn = false;
				}

				SSselect = MyGameSound::SoundPlay(L"titlslct.wav");
				mSelectOn = true;
			
				mOpening->GetSound()->Stop();

				SetCurScene(L"TestScene");
			}
		}
	}
	// MULTI PLAYER
	else if (mMousePos.IntX() >= mStartX && mMousePos.IntX() < mStartX + mGapX && mMousePos.IntY() >= mStartY + mGapY && mMousePos.IntY() < mStartY + mGapY * 2)
	{
		if (false == mMoveOn2)
		{
			SSmove = MyGameSound::SoundPlay(L"titlemov.wav");
			mStar1->SubPos({ -200, 24 });
			mStar2->SubPos({ 200, 24 });
			mStar1->On();
			mStar2->On();
			mMoveOn1 = false;
			mMoveOn2 = true;
			mMoveOn3 = false;
			mMoveOn4 = false;
			mMoveOn5 = false;
			return;
		}
	}
	// REPLAY
	else if (mMousePos.IntX() >= mStartX && mMousePos.IntX() < mStartX + mGapX && mMousePos.IntY() >= mStartY + mGapY*2 && mMousePos.IntY() < mStartY + mGapY * 3)
	{
		if (false == mMoveOn3)
		{
			SSmove = MyGameSound::SoundPlay(L"titlemov.wav");
			mStar1->SubPos({ -200, 24 + mStartGap });
			mStar2->SubPos({ 200, 24 + mStartGap });
			mStar1->On();
			mStar2->On();
			mMoveOn1 = false;
			mMoveOn2 = false;
			mMoveOn3 = true;
			mMoveOn4 = false;
			mMoveOn5 = false;

			return;
		}
	}
	// CREDIT
	else if (mMousePos.IntX() >= mStartX && mMousePos.IntX() < mStartX + mGapX && mMousePos.IntY() >= mStartY + mGapY*3 && mMousePos.IntY() < mStartY + mGapY * 4)
	{
		if (false == mMoveOn4)
		{
			SSmove = MyGameSound::SoundPlay(L"titlemov.wav");
			mStar1->SubPos({ -200, 24 + mStartGap *2 });
			mStar2->SubPos({ 200, 24 + mStartGap *2 });
			mStar1->On();
			mStar2->On();
			mMoveOn1 = false;
			mMoveOn2 = false;
			mMoveOn3 = false;
			mMoveOn4 = true;
			mMoveOn5 = false;

			return;
		}
	}
	// EXIT
	else if (mMousePos.IntX() >= mStartX && mMousePos.IntX() < mStartX + mGapX && mMousePos.IntY() >= mStartY + mGapY*4 && mMousePos.IntY() < mStartY + mGapY * 5)
	{
		if (false == mMoveOn5)
		{
			SSmove = MyGameSound::SoundPlay(L"titlemov.wav");
			mStar1->SubPos({ -200, 24 + mStartGap * 3 });
			mStar2->SubPos({ 200, 24 + mStartGap * 3 });
			mStar1->On();
			mStar2->On();
			mMoveOn1 = false;
			mMoveOn2 = false;
			mMoveOn3 = false;
			mMoveOn4 = false;
			mMoveOn5 = true;

			return;
		}
	}
	
	//// EXIT
	//if (mMousePos.IntX() >= mStartX && mMousePos.IntX() <= mStartX + mGapX)
	//{
	//	if (mMousePos.IntY() >= mStartY && mMousePos.IntY() <= mStartY + mGapY)
	//	{
	//		if (false == mMoveOn5)
	//		{
	//			SSmove = MyGameSound::SoundPlay(L"titlemov.wav");
	//			mStar1->On();
	//			mStar2->On();
	//			mMoveOn5 = true;
	//		}
	//	}
	//	else
	//	{
	//		if (true == mMoveOn1)
	//		{
	//			SSmove->Stop();
	//			mStar1->Off();
	//			mStar2->Off();
	//		}

	//		mMoveOn5 = false;
	//	}
	//}








	if (true == MyInput::Down(L"LMOUSEBTN"))
	{
		if (true == mSelectOn)
		{
			SSselect->Stop();
			mSelectOn = false;
		}

		SSselect = MyGameSound::SoundPlay(L"titlslct.wav");
		mSelectOn = true;
	}

}

void MainTitle::Init()
{
	mFire->SetCurrentAnimation(L"Fire");
	mStar1->SetCurrentAnimation(L"Star1");
	mStar2->SetCurrentAnimation(L"Star2");

	mStar1->Off();
	mStar2->Off();


}
