#include "MyClientUpdater.h"
#include "MyInput.h"
#include "MyTime.h"
#include "MyScene.h"
#include "MyTextureManager.h"

#include "OpeningBlizzard.h"
#include "OpeningDiablo.h"
#include "MainTitle.h"
#include "CharacterSelect.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Ending.h"

MyClientUpdater::MyClientUpdater()
{
}


MyClientUpdater::~MyClientUpdater()
{
}

void MyClientUpdater::GameInit()
{
	MyTextureManager::Inst().Init();

	//////////////////////////////////////////////// Scene Setting //////////////////

	MyScene::CreateScene<OpeningBlizzard>(L"OpeningBlizzard");
	MyScene::CreateScene<OpeningDiablo>(L"OpeningDiablo");
	MyScene::CreateScene<MainTitle>(L"MainTitle");
	MyScene::CreateScene<CharacterSelect>(L"CharacterSelect");
	MyScene::CreateScene<Level1>(L"Level1");
	MyScene::CreateScene<Level2>(L"Level2");
	MyScene::CreateScene<Level3>(L"Level3");
	MyScene::CreateScene<Ending>(L"Ending");

	MyScene::SetCurScene(L"Level1");

	//////////////////////////////////////////////// Key Setting ///////////////////
	MyInput::CreateKey(L"MOVEL", L'A');
	MyInput::CreateKey(L"MOVER", L'D');
	MyInput::CreateKey(L"MOVET", L'W');
	MyInput::CreateKey(L"MOVEB", L'S');

	MyInput::CreateKey(L"MOVELT", L'A', L'W');
	MyInput::CreateKey(L"MOVERT", L'D', L'W');
	MyInput::CreateKey(L"MOVELB", L'A', L'S');
	MyInput::CreateKey(L"MOVERB", L'D', L'S');

	MyInput::CreateKey(L"FIRE", VK_SPACE);

	MyInput::CreateKey(L"LMOUSEBTN", VK_LBUTTON);

	MyInput::CreateKey(L"IMAGEINDEXPREV", L'Q');
	MyInput::CreateKey(L"IMAGEINDEXNEXT", L'E');

	/////////////////////////////////////////////

	Rect.Pos = MyVector2{ 100.f, 100.f };
	Rect.Size = MyVector2{ 100.f, 100.f };
}

static int prevFPS = MyTime::FPS();

void MyClientUpdater::GameUpdate()
{
	MyScene::Progress();

	// ¼÷Á¦

	/*
	wchar_t Arr[200] = { 0, };
	swprintf_s(Arr, L"FPS : %d\n", prevFPS);
	OutputDebugStringW(Arr);
	*/
		
	// ¼÷Á¦ ³¡
}
