#include "BlizzardTitle.h"

#include <MyAnimationRenderer.h>
#include <MyRenderer.h>

BlizzardTitle::BlizzardTitle()
{
	Pos({ 640/2, 480/2 });

	mAnimationRenderer = CreateAniRenderer(LO_BG);
	mAnimationRenderer->SubSize({ 640.f * 1.3f , 640/2.f * 1.3f});

	mAnimationRenderer->CreateAnimation(L"Opening", L"BlizzardOpening.bmp", 0, 239, true, 0.07f);

	mAnimationRenderer->SetCurrentAnimation(L"Opening");


	mBack = CreateRenderer<MyRenderer>(LO_BBG);
	mBack->Image(L"BackBuffer.bmp");
	mBack->SubPos({ 0.f, 480 / 2.f });
	mBack->SubSize({1000, 1000});
	mBack->SetRenderStyle(RS_BIT);
}


BlizzardTitle::~BlizzardTitle()
{
}

void BlizzardTitle::Update()
{

}

void BlizzardTitle::DebugRender()
{
}

bool BlizzardTitle::IsOpeningOver()
{
	return mAnimationRenderer->IsCurrentAnimationEnd();
}
