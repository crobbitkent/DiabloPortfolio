#include "MyClientUpdater.h"

#include <MyInput.h>
#include <MyTime.h>
#include <MyScene.h>
#include <MyTextureManager.h>
#include <RandomNumber.h>

#include "OpeningBlizzard.h"
#include "OpeningDiablo.h"
#include "MainTitle.h"
#include "CharacterSelect.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Ending.h"
#include "Dungeon2.h"
#include "Dungeon15.h"
#include "Dungeon18.h"

#include "TestScene.h"
#include "TestPlayerScene.h"

MyClientUpdater::MyClientUpdater()
{
}


MyClientUpdater::~MyClientUpdater()
{
}

void MyClientUpdater::GameInit()
{
	MyTextureManager::Inst().Init();
	RandomNumber::Init();
	//////////////////////////////////////////////// Scene Setting //////////////////

	MyScene::CreateScene<OpeningBlizzard>(L"OpeningBlizzard");
	OpeningDiablo* dia = (OpeningDiablo*)MyScene::CreateScene<OpeningDiablo>(L"OpeningDiablo");
	MainTitle* main = (MainTitle*)MyScene::CreateScene<MainTitle>(L"MainTitle");

	main->SetOpening(dia);



	MyScene::CreateScene<CharacterSelect>(L"CharacterSelect");
	MyScene::CreateScene<Level1>(L"Level1");
	MyScene::CreateScene<Level2>(L"Level2");
	MyScene::CreateScene<Level3>(L"Level3");
	MyScene::CreateScene<Ending>(L"Ending");

	MyScene::CreateScene<TestPlayerScene>(L"TestPlayerScene");

	TestScene* test = (TestScene*)MyScene::CreateScene<TestScene>(L"TestScene");
	Dungeon15* d15 = (Dungeon15*)MyScene::CreateScene<Dungeon15>(L"Dungeon15");
	Dungeon18* d18 = (Dungeon18*)MyScene::CreateScene<Dungeon18>(L"Dungeon18");
	Dungeon2* d2 = (Dungeon2*)MyScene::CreateScene<Dungeon2>(L"Dungeon2");




	test->SetDungeon15(d15);
// 이전
	d15->SetTestScene(test);
	d15->SetDungeon18(d18);
// 이전
	d18->SetDungeon15(d15);
	d18->SetDungeon2(d2);

	d2->SetDungeon18(d18);

	MyScene::SetCurScene(L"TestScene");

	//////////////////////////////////////////////// Key Setting ///////////////////
	MyInput::CreateKey(L"MOVEL", L'A');
	MyInput::CreateKey(L"MOVER", L'D');
	MyInput::CreateKey(L"MOVET", L'W');
	MyInput::CreateKey(L"MOVEB", L'X');

	MyInput::CreateKey(L"MOVELT", L'Q');
	MyInput::CreateKey(L"MOVERT", L'E');
	MyInput::CreateKey(L"MOVELB", L'Z');
	MyInput::CreateKey(L"MOVERB", L'C');

	MyInput::CreateKey(L"ATTACK", L'R');
	MyInput::CreateKey(L"GETHIT", L'T');
	MyInput::CreateKey(L"BLUESKILL", L'Y');
	MyInput::CreateKey(L"REDSKILL", L'U');
	MyInput::CreateKey(L"BLOCK", L'I');
	MyInput::CreateKey(L"DIE", L'O');

	MyInput::CreateKey(L"GoToNextScene", VK_SPACE);

	MyInput::CreateKey(L"SHIFT", VK_SHIFT);
	MyInput::CreateKey(L"LMOUSEBTN", VK_LBUTTON);
	MyInput::CreateKey(L"RMOUSEBTN", VK_RBUTTON);

	MyInput::CreateKey(L"OUTPUT", VK_RETURN);
	MyInput::CreateKey(L"DEBUG", VK_F1);
	MyInput::CreateKey(L"COLDEBUG", VK_F2);

	MyInput::CreateKey(L"MakeARoom", L'P');

	MyInput::CreateKey(L"EXPUP", VK_F4);
	MyInput::CreateKey(L"MONEY", VK_F6);
	MyInput::CreateKey(L"ITEM", VK_F7);
	/////////////////////////////////////////////

	Rect.Pos = MyVector2{ 100.f, 100.f };
	Rect.Size = MyVector2{ 100.f, 100.f };
}

static int prevFPS = MyTime::FPS();

void MyClientUpdater::GameUpdate()
{
	MyScene::Progress();
}
