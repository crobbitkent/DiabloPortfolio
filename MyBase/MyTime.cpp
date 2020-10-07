#include "MyTime.h"

#include <Windows.h>
#include <time.h>
#include <iostream>

MyTime::MyTimer MyTime::mMainTimer;

MyTime::MyTime()
{
}


MyTime::~MyTime()
{
}

float MyTime::MyTimer::Update()
{
	QueryPerformanceCounter(&mTotalCPUCount);

	mDoubleDeltaTime = ((double)(mTotalCPUCount.QuadPart - mPrevCPUCount.QuadPart)) / ((double)mCPUCount.QuadPart);
	mFloatDeltaTime = (float)mDoubleDeltaTime;

	mPrevCPUCount = mTotalCPUCount;

	if (mTime != time(nullptr))
	{
		// 시간을 Output에 띄우기
		//wchar_t Arr[200] = {0,};
		//swprintf_s(Arr, L"FPS : %d\n", mFPS);
		//OutputDebugStringW(Arr);

		// FPS 초기화
		mNewFPS = mFPS;
		mFPS = 0;

	}

	++mFPS;
	mTime = (int)time(nullptr);

	return mFloatDeltaTime;
}

void MyTime::MyTimer::Reset()
{
	QueryPerformanceFrequency(&mCPUCount);
	QueryPerformanceCounter(&mTotalCPUCount);
	QueryPerformanceCounter(&mPrevCPUCount);

	mFPS = 0;
	mTime = (int)time(nullptr);
}

MyTime::MyTimer::MyTimer()
{
	Reset();
}

MyTime::MyTimer::~MyTimer()
{
}
