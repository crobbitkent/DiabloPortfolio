#include "MyImage.h"

#include "MyMacro.h"

MyImage::MyImage()
{
}


MyImage::~MyImage()
{
}

void MyImage::Cut(MyTexture * texture, int x, int y)
{
	if (nullptr == texture ||
		1 > x ||
		1 > y)
	{
		BOOM;
	}

	mTexture = texture;
	mDC = mTexture->TextureDC();

	MyVector2 cutSize = mTexture->Size();

	cutSize.X = cutSize.X / x;
	cutSize.Y = cutSize.Y / y;

	MyVector2 startPos = MyVector2::ZERO;

	for (int i = 0; i < y; ++i)
	{
		for (int j = 0;  j < x; ++j)
		{
			MyRect rc;

			rc.Pos = { startPos.X, startPos.Y };
			rc.Size = cutSize;

			mAllCutData.push_back(rc);
		
			startPos.X += cutSize.X;
		}
		startPos.X = 0.0f;
		startPos.Y += cutSize.Y;
	}
}
