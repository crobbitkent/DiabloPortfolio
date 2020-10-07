#include "OpeningBlizzard.h"

#include <MyWindowClass.h>
#include <MyInput.h>

#include "Player.h"

#include "BlizzardTitle.h"

OpeningBlizzard::OpeningBlizzard()
{
}


OpeningBlizzard::~OpeningBlizzard()
{
}

// Init °°Àº ´À³¦???
void OpeningBlizzard::Loading()
{
	mOpening = new BlizzardTitle();

	
}

void OpeningBlizzard::SceneUpdate()
{
	if (true == mOpening->IsOpeningOver())
	{
		SetCurScene(L"OpeningDiablo");
	}

	if (true == MyInput::Down(L"GoToNextScene") || true == MyInput::Down(L"LMOUSEBTN"))
	{
		SetCurScene(L"OpeningDiablo");
		HSS->Stop();
	}
}

void OpeningBlizzard::Init()
{
	HSS = MyGameSound::SoundPlay(L"dlogo.mp3");
}
