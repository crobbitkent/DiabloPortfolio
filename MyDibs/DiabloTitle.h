#pragma once
#include <MyActor.h>
#include <MyGameSound.h>
class MyAnimationRenderer;

class DiabloTitle : public MyActor
{
public:
	DiabloTitle();
	~DiabloTitle();

	void DebugRender() override;

private:
	MyAnimationRenderer* mTitle;
	MyAnimationRenderer* mFire;
	MyGameSound::MyGameSoundStream* HSS;
};

