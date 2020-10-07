#include "YFontRenderer.h"


#include <MyTextureManager.h>
#include <MyImage.h>
#include <MyTexture.h>

YFontRenderer::YFontRenderer()
{
	Image(L"NewYellowFont.bmp");
	SetRenderStyle(RS_TRANS);
	SubPos({ 0.f, 425.f });
	SubSize({ 14.f, 14.f });
	CamOnOff();
	TransColor(255, 255, 255);
}


YFontRenderer::~YFontRenderer()
{
}

void YFontRenderer::Render()
{
	if (1 == mSizeY)
	{
		int start = 320 - mSizeX * 7;

		for (int i = 0; i < mSizeX; ++i)
		{
			mIndex = (int)mString1[i] - 97;
			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 14
				, SubPos().IntY()
				, SubSize().IntX()
				, SubSize().IntY()
				, mImage->ImageHDC()
				, mRect.Pos.IntX()
				, mRect.Pos.IntY()
				, mRect.Size.IntX()
				, mRect.Size.IntY()
				, mTransColor
			);
		}
	}
}

void YFontRenderer::SetString(std::wstring string1)
{
	mString1 = string1;
	mSizeY = 1;
	mSizeX = string1.size();
}

void YFontRenderer::SetString(std::wstring string1, std::wstring string2)
{
}

void YFontRenderer::SetString(std::wstring string1, std::wstring string2, std::wstring string3)
{
}
