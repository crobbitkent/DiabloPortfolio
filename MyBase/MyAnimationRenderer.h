#pragma once
#include "MyRenderer.h"

#include <map>

class MyAnimationRenderer :	public MyRenderer
{
	/////////////// inner class //////////
	class MyAnimation
	{
	public:
		MyImage* mImage;

		unsigned int mStartFrame;
		unsigned int mEndFrame;
		float mFrameTimeGap; // ( = m_fFrameTime)
		float mCurFrameTime;
		int mCurFrame;

		bool mIsLoop;
		bool mIsEnd;
	};

public:
	bool CreateAnimation(const wchar_t* name, const wchar_t* imageName, unsigned int start, unsigned int end, bool loop, float frameTimeGap = 0.1f);
	bool SetCurrentAnimation(const wchar_t* name);

	bool IsCurrentAnimationEnd()
	{
		return mCurrentAnimation->mIsEnd;
	}

	MyAnimationRenderer();
	~MyAnimationRenderer();


	std::wstring CurImageName();
	int CurFrame() { return mCurrentAnimation->mCurFrame; }

	void FinishAnimation() { mCurrentAnimation->mCurFrame = mCurrentAnimation->mEndFrame; }
	void SetAlpha() { mIsAlpha = true; }
protected:



private:
	MyAnimation* FindAnimation(const wchar_t* name);
	void Render();

private:
	std::map<std::wstring, MyAnimation*>::iterator mFindIter;
	std::map<std::wstring, MyAnimation*>::iterator mAniIter;
	std::map<std::wstring, MyAnimation*> mAnimationMap;
	MyAnimation* mCurrentAnimation;

	bool mIsAlpha;
};

