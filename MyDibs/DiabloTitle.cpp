#include "DiabloTitle.h"

#include <MyAnimationRenderer.h>

DiabloTitle::DiabloTitle()
{
	Pos({ 640/2.f, 480/2.f });

	mTitle = CreateAniRenderer(LO_BG);
	mTitle->SubSize({ 640.f , 480.f });

	mTitle->CreateAnimation(L"Diablo", L"Diablo.bmp", 0, 0, true);
	mTitle->SetCurrentAnimation(L"Diablo");

	mFire = CreateAniRenderer(LO_TILE);
	mFire->CreateAnimation(L"DiabloFire", L"DiabloFire.bmp", 0, 14, true);
	mFire->SubSize({ 550.f, 216.f });
	mFire->SubPos({ 0, 60 });
	mFire->SetCurrentAnimation(L"DiabloFire");	
}


DiabloTitle::~DiabloTitle()
{
}

void DiabloTitle::DebugRender()
{
}
