#pragma once

#include <string>
#include <Windows.h>

#include "MyMath.h"

class MyFile;

// 이미지 파일 하나
class MyTexture
{
public:
	MyTexture();
	~MyTexture();

	void TextureSize(const std::wstring& path);
	int GetTexturePixel(int x, int y);

	bool Create(int x, int y);

	bool Load(const MyFile& file);
	bool Load(const std::wstring& path);

	HDC TextureDC() { return mImage; }

	MyVector2 Size() {	
		return { (float)mBitInfo.biWidth, (float)mBitInfo.biHeight };
	}

	void Name(const wchar_t* name) { mName = name; }
	std::wstring Name() { return mName; }

private:
	std::wstring mName;
	HDC mImage;
	HBITMAP mHBITMAP;
	HBITMAP mOldHBITMAP;
	BITMAPFILEHEADER mBitFileHeader;
	BITMAPINFOHEADER mBitInfo;

};

