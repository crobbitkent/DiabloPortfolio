#pragma once

#include <MyScene.h>
#include <MyGameSound.h>

class BlizzardTitle;

class OpeningBlizzard : public MyScene
{
public:
	OpeningBlizzard();
	~OpeningBlizzard();

	void Loading() override;
	void SceneUpdate() override;

	void MakeMap() {}

	void Init();
private:
	BlizzardTitle* mOpening;
	MyGameSound::MyGameSoundStream* HSS;

};

