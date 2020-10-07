#pragma once


#include <MyScene.h>
#include <MyGameSound.h>

class DiabloTitle;

class OpeningDiablo : public MyScene
{
public:
	OpeningDiablo();
	~OpeningDiablo();

	void Loading() override;
	void SceneUpdate() override;

	void MakeMap() {}

	void Init();

	MyGameSound::MyGameSoundStream* GetSound() { return HSS; }
private:
	DiabloTitle* mDiablo;
	MyGameSound::MyGameSoundStream* HSS;
};

