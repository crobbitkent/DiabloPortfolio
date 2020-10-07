#pragma once


#include <MyScene.h>
#include <MyActor.h>
#include <MyGameSound.h>

class OpeningDiablo;

class MainTitle : public MyScene
{
public:
	MainTitle();
	~MainTitle();

	void Loading() override;
	void SceneUpdate() override;
	void Init();

	void MakeMap() {}

	void SetOpening(OpeningDiablo* dia) { mOpening = dia; }
private:
	MyActor* mMain;
	MyRenderer* mMainRenderer;
	MyAnimationRenderer* mFire;
	MyAnimationRenderer* mStar1;
	MyAnimationRenderer* mStar2;


	MyGameSound::MyGameSoundStream* SSselect;
	MyGameSound::MyGameSoundStream* SSmove;
	MyGameSound::MyGameSoundStream* HSS;

	bool mSelectOn = false;;
	bool mMoveOn1 = false;
	bool mMoveOn2 = false;
	bool mMoveOn3 = false;
	bool mMoveOn4 = false;
	bool mMoveOn5 = false;

	int mStartX;
	int mStartY;
	int mGapY = 40;
	int mGapX = 360;

	int mStartGap = 40;

	MyVector2 mMousePos;

	OpeningDiablo* mOpening;
};

