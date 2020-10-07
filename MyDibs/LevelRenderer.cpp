#include "LevelRenderer.h"

#include <MyTextureManager.h>
#include <MyImage.h>

#include "Player.h"

LevelRenderer::LevelRenderer()
{
	Image(L"NewYellowFont.bmp");

	TransColor(255, 255, 255);
}


LevelRenderer::~LevelRenderer()
{
}

void LevelRenderer::Render()
{
	if (false == mPlayer->IsStatOpen())
	{
		start = 7;


		ImageIndex(11);
		TransparentBlt(MyTextureManager::BackDC()
			, 10
			, start
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		ImageIndex(4);
		TransparentBlt(MyTextureManager::BackDC()
			, start + 12 * 1
			, start
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		ImageIndex(21);
		TransparentBlt(MyTextureManager::BackDC()
			, start + 12 * 2
			, start
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		ImageIndex(4);
		TransparentBlt(MyTextureManager::BackDC()
			, start + 12 * 3
			, start
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		ImageIndex(11);
		TransparentBlt(MyTextureManager::BackDC()
			, start + 12 * 4
			, start
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		ImageIndex(38);
		TransparentBlt(MyTextureManager::BackDC()
			, start + 12 * 5
			, start
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		if (1 == mPlayer->GetLevel())
		{
			ImageIndex(27);
			TransparentBlt(MyTextureManager::BackDC()
				, start + 12 * 6
				, start
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
		else if (2 == mPlayer->GetLevel())
		{
			ImageIndex(28);
			TransparentBlt(MyTextureManager::BackDC()
				, start + 12 * 6
				, start
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
		else if (3 == mPlayer->GetLevel())
		{
			ImageIndex(29);
			TransparentBlt(MyTextureManager::BackDC()
				, start + 12 * 6
				, start
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
		else if (4 == mPlayer->GetLevel())
		{
			ImageIndex(30);
			TransparentBlt(MyTextureManager::BackDC()
				, start + 12 * 6
				, start
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
		else
		{
			BOOM;
		}
	}
}

void LevelRenderer::SetPlayer(Player * player)
{
	mPlayer = player;
}
