#include "MyAnimationRenderer.h"

#include "MyTextureManager.h"
#include "MyTime.h"
#include "MyImage.h"

MyAnimationRenderer::MyAnimationRenderer()
	: mCurrentAnimation(nullptr)
	, mIsAlpha(false)
{
}


MyAnimationRenderer::~MyAnimationRenderer()
{
	std::map<std::wstring, MyAnimation*>::iterator iter = mAnimationMap.begin();
	std::map<std::wstring, MyAnimation*>::iterator endIter = mAnimationMap.end();

	for (; iter != endIter; ++iter)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
}


MyAnimationRenderer::MyAnimation * MyAnimationRenderer::FindAnimation(const wchar_t * name)
{
	mFindIter = mAnimationMap.find(name);

	if (mAnimationMap.end() == mFindIter)
	{
		return nullptr;
	}

	return mFindIter->second;
}


bool MyAnimationRenderer::CreateAnimation(const wchar_t * name, const wchar_t * imageName, unsigned int start, unsigned int end, bool loop, float frameTimeGap /*= 0.1f*/)
{
	MyAnimation* ani = FindAnimation(name);

	// 이미 존재한다면... 폭발
	if (nullptr != ani)
	{
		BOOM;

		return false;
	}

	MyImage* image = MyTextureManager::Inst().FindImage(imageName);

	if (nullptr == image)
	{
		BOOM;

		return false;
	}

	ani = new MyAnimation();
	ani->mImage = image;
	ani->mIsLoop = loop;

	ani->mStartFrame = start;
	ani->mEndFrame = end;
	ani->mFrameTimeGap = frameTimeGap;

	mAnimationMap.insert(std::map<std::wstring, MyAnimation*>::value_type(name, ani));

	return true;
}

bool MyAnimationRenderer::SetCurrentAnimation(const wchar_t * name)
{
	mCurrentAnimation = FindAnimation(name);

	if (nullptr == mCurrentAnimation)
	{
		BOOM;
		return false;
	}

	mImage = mCurrentAnimation->mImage;
	mCurrentAnimation->mIsEnd = false;
	mCurrentAnimation->mCurFrame = mCurrentAnimation->mStartFrame;
	mCurrentAnimation->mCurFrameTime = mCurrentAnimation->mFrameTimeGap;

	return true;
}



void MyAnimationRenderer::Render()
{
	if (nullptr == mActor)
	{
		BOOM;
	}

	MyRect rc = CalRect();

	mRect = mImage->GetRect(mCurrentAnimation->mCurFrame);

	mCurrentAnimation->mCurFrameTime -= MyTime::DeltaTime();

	if (0.0f >= mCurrentAnimation->mCurFrameTime)
	{
		++mCurrentAnimation->mCurFrame;
		mCurrentAnimation->mCurFrameTime = mCurrentAnimation->mFrameTimeGap;

		if (mCurrentAnimation->mCurFrame > (int)mCurrentAnimation->mEndFrame)
		{
			mCurrentAnimation->mIsEnd = true;

			if (true == mCurrentAnimation->mIsLoop)
			{
				mCurrentAnimation->mCurFrame = mCurrentAnimation->mStartFrame;
			}
			else
			{
				mCurrentAnimation->mCurFrame = mCurrentAnimation->mEndFrame;
			}
		}
	}

	if (true == mIsAlpha)
	{
		AlphaBlend(MyTextureManager::BackDC()
			, rc.IntLeft()
			, rc.IntTop()
			, rc.Size.IntX()
			, rc.Size.IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX() // 이미지의 x에서부터
			, mRect.Pos.IntY() // 이미지의 y에서부터
			, mRect.Size.IntX()  // 여기까지의 x 픽셀
			, mRect.Size.IntY()
			, GetBF()
		);
	}
	else
	{
		TransparentBlt(MyTextureManager::BackDC()
			, rc.IntLeft()
			, rc.IntTop()
			, rc.Size.IntX()
			, rc.Size.IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor);
	}


}


std::wstring MyAnimationRenderer::CurImageName() 
{
	return mCurrentAnimation->mImage->Texture()->Name();
}