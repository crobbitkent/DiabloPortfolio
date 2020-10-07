#include "MyTexture.h"

#include "MyMacro.h"
#include "MyFile.h"
#include "MyWindowClass.h"

MyTexture::MyTexture()
	: mImage(nullptr)
	, mHBITMAP(nullptr)
	, mOldHBITMAP(nullptr)
{
}


MyTexture::~MyTexture()
{
	DeleteObject(mOldHBITMAP);
	DeleteObject(mImage);
}

void MyTexture::TextureSize(const std::wstring & path)
{
	MyFile file = MyFile(path.c_str(), L"rb");

	file.Read(mBitFileHeader);
	file.Read(mBitInfo);
}

int MyTexture::GetTexturePixel(int x, int y)
{
	return GetPixel(mImage, x, y);
}

bool MyTexture::Create(int x, int y)
{
	mImage = CreateCompatibleDC(nullptr);
	mHBITMAP = CreateCompatibleBitmap(MyWindowClass::MainDC(), x, y);

	mOldHBITMAP = (HBITMAP)SelectObject(mImage, mHBITMAP);

	if (nullptr == mImage)
	{
		BOOM;
		return false;
	}

	return true;
}

bool MyTexture::Load(const MyFile & file)
{
	return Load(file.GetPath());
}

bool MyTexture::Load(const std::wstring & path)
{
	mHBITMAP = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (nullptr == mHBITMAP)
	{
		BOOM;
		return false;
	}

	// ����ִ� HDC�� ������ִ� �Լ�
	mImage = CreateCompatibleDC(nullptr);

	// ����ִ� HDC�� �ε��� �̹��� HDC�� �ٲ۴�. 
	mOldHBITMAP = (HBITMAP)SelectObject(mImage, mHBITMAP);

	if (nullptr == mImage)
	{
		BOOM;
		return false;
	}

	TextureSize(path);

	return true;
}
