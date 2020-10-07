#include "FontRenderer.h"


#include <MyTextureManager.h>
#include <MyImage.h>
#include <MyTexture.h>

FontRenderer::FontRenderer()
{
	Image(L"NewFont.bmp");
	SetRenderStyle(RS_TRANS);
	SubPos({ 0.f, 425.f });
	SubSize({ 12.f, 12.f });
	CamOnOff();
	TransColor(255, 255, 255);
}


FontRenderer::~FontRenderer()
{
}

void FontRenderer::Render()
{
	if (1 == mSizeY)
	{
		int start = 320 - mSizeX1 * 6;

		for (int i = 0; i < mSizeX1; ++i)
		{
			mIndex = (int)mString1[i] - 97;

			if (-65 == mIndex)
			{
				continue;
			}
			else if (mIndex < 0)
			{
				mIndex += 49 + 26;
			}
			else if (mIndex == 36)
			{
				mIndex = 38;
			}

			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 12
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
	else if (2 == mSizeY)
	{
		int start = 320 - mSizeX1 * 6;

		for (int i = 0; i < mSizeX1; ++i)
		{
			mIndex = (int)mString1[i] - 97;

			if (-65 == mIndex)
			{
				continue;
			}
			else if (mIndex < 0)
			{
				mIndex += 49 + 26;
			}
			else if (mIndex == 36)
			{
				mIndex = 38;
			}

			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 12
				, SubPos().IntY() - 8
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

		start = 320 - mSizeX2 * 6;
		for (int i = 0; i < mSizeX2; ++i)
		{
			mIndex = (int)mString2[i] - 97;

			if (-65 == mIndex)
			{
				continue;
			}
			else if (mIndex < 0)
			{
				mIndex += 49 + 26;
			}
			else if (mIndex == 36)
			{
				mIndex = 38;
			}

			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 12
				, SubPos().IntY() + 8
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
	else if (3 == mSizeY)
	{
		int start = 320 - mSizeX1 * 6;

		for (int i = 0; i < mSizeX1; ++i)
		{
			mIndex = (int)mString1[i] - 97;

			if (-65 == mIndex)
			{
				continue;
			}
			else if (mIndex < 0)
			{
				mIndex += 49 + 26;
			}
			else if (mIndex == 36)
			{
				mIndex = 38;
			}

			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 12
				, SubPos().IntY() - 16
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

		start = 320 - mSizeX2 * 6;
		for (int i = 0; i < mSizeX2; ++i)
		{
			mIndex = (int)mString2[i] - 97;

			if (-65 == mIndex)
			{
				continue;
			}
			else if (mIndex < 0)
			{
				mIndex += 49 + 26;
			}

			if (mIndex == 36)
			{
				mIndex = 38;
			}

			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 12
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

		start = 320 - mSizeX3 * 6;
		for (int i = 0; i < mSizeX3; ++i)
		{
			mIndex = (int)mString3[i] - 97;

			if (-65 == mIndex)
			{
				continue;
			}
			else if (mIndex < 0)
			{
				mIndex += 49 + 26;
			}
			else if (mIndex == 36)
			{
				mIndex = 38;
			}

			ImageIndex(mIndex);
			TransparentBlt(MyTextureManager::BackDC()
				, start + i * 12
				, SubPos().IntY() + 16
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

void FontRenderer::SetString(std::wstring string1)
{
	mString1 = string1;
	mSizeY = 1;
	mSizeX1 = string1.size();
}

void FontRenderer::SetString(std::wstring string1, std::wstring string2)
{
	mString1 = string1;
	mString2 = string2;
	mSizeY = 2;
	mSizeX1 = string1.size();
	mSizeX2 = string2.size();

}

void FontRenderer::SetString(std::wstring string1, std::wstring string2, std::wstring string3)
{
	mString1 = string1;
	mString2 = string2;
	mString3 = string3;
	mSizeY = 3;
	mSizeX1 = string1.size();
	mSizeX2 = string2.size();
	mSizeX3 = string3.size();
}
