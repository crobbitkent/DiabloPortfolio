#pragma once

#include <windows.h>

class MyTime
{
private:
	//////////////////// Inner class /////////////////////
	class MyTimer
	{
	public:
		friend MyTime;

	public:
		float Update();
		void Reset();

	private:
		MyTimer();
		~MyTimer();

	private:
		LARGE_INTEGER mCPUCount;
		LARGE_INTEGER mTotalCPUCount;
		LARGE_INTEGER mPrevCPUCount;
		double mDoubleDeltaTime;
		float mFloatDeltaTime;	

		int mFPS;
		int mNewFPS;
		int mTime;
	};

public:
	static float Update()
	{
		return mMainTimer.Update();
	}

	static float DeltaTime()
	{
		return mMainTimer.mFloatDeltaTime;
	}

	static int FPS()
	{
		return mMainTimer.mNewFPS;
	}
private:
	MyTime();
	~MyTime();

private:
	static MyTimer mMainTimer;

};

