#include "OpeningDiablo.h"

#include <MyInput.h>

#include "DiabloTitle.h"


OpeningDiablo::OpeningDiablo()
{
}


OpeningDiablo::~OpeningDiablo()
{
}

void OpeningDiablo::Loading()
{
	mDiablo = new DiabloTitle();
}

void OpeningDiablo::SceneUpdate()
{
	if (true == MyInput::Down(L"GoToNextScene") || true == MyInput::Down(L"LMOUSEBTN"))
	{
		// HSS->Stop();
		SetCurScene(L"MainTitle");
	}
}

void OpeningDiablo::Init()
{
	HSS = MyGameSound::SoundPlay(L"dintro.mp3");
}
