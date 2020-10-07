#include "MyTextureManager.h"

#include "MyTexture.h"
#include "MyImage.h"
#include "MyFile.h"
#include "MyWindowClass.h"

MyVector2 MyTextureManager::mBackDCSize;
HDC MyTextureManager::mBackDC = nullptr;

MyTextureManager::MyTextureManager()
{
}


MyTextureManager::~MyTextureManager()
{
	// 모든 텍스쳐 삭제
	std::map<std::wstring, MyTexture*>::iterator iter = mAllTexture.begin();
	std::map<std::wstring, MyTexture*>::iterator endIter = mAllTexture.end();

	for (; iter != endIter; ++iter)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	mAllTexture.clear();

	// 모든 이미지 삭제
	std::map<std::wstring, MyImage*>::iterator ImgStart = mAllImage.begin();
	std::map<std::wstring, MyImage*>::iterator ImgEnd = mAllImage.end();

	for (; ImgStart != ImgEnd; ++ImgStart)
	{
		if (nullptr != ImgStart->second)
		{
			delete ImgStart->second;
			ImgStart->second = nullptr;
		}
	}
	mAllImage.clear();

	// 백버퍼 삭제
	if (nullptr != mBackBufferTexture)
	{
		delete mBackBufferTexture;
		mBackBufferTexture = nullptr;
	}
}

void MyTextureManager::Init()
{
	mBackBufferTexture = new MyTexture();

	// 백버퍼 만들기 실패
	if (false == mBackBufferTexture->Create(
		MyWindowClass::MainWindow()->Size().IntX(), MyWindowClass::MainWindow()->Size().IntY()))
	{
		BOOM;
	}

	mBackDC = mBackBufferTexture->TextureDC();
	mBackDCSize = MyWindowClass::MainWindow()->Size();
}

MyTexture * MyTextureManager::LoadTexture(const MyFile & file)
{
	MyTexture* newTex = new MyTexture();

	if (false == newTex->Load(file.GetPath()))
	{
		BOOM;
	}

	newTex->Name(file.FileName().c_str());
	mAllTexture.insert(std::map<std::wstring, MyTexture*>::value_type(file.FileName(), newTex));

	return newTex;
}

MyTexture * MyTextureManager::CreateTexture(const wchar_t * name, int x, int y)
{
	MyTexture* newTex = new MyTexture();

	if (false == newTex->Create(x, y))
	{
		BOOM;
	}

	newTex->Name(name);
	mAllTexture.insert(std::map<std::wstring, MyTexture*>::value_type(name, newTex));

	return newTex;
}

MyTexture * MyTextureManager::FindTexture(const std::wstring & key)
{
	std::map<std::wstring, MyTexture*>::iterator texIter = mAllTexture.find(key);

	if (mAllTexture.end() == texIter)
	{
		return nullptr;
	}

	if(nullptr == texIter->second)
	{
		BOOM;
	}

	return texIter->second;
}

MyImage * MyTextureManager::CreateImage(const wchar_t* texName, unsigned int x, unsigned int y)
{
	MyTexture* findTex = FindTexture(texName);

	// 찾는 Texture가 없거나
	// 만들 Image가 이미 있으면 터짐
	if (nullptr == findTex ||
		nullptr != FindImage(findTex->Name()))
	{
		BOOM;
		return nullptr;
	}

	MyImage* newImage = new MyImage();

	newImage->Cut(findTex, x, y);

	mAllImage.insert(std::map<std::wstring, MyImage*>::value_type(findTex->Name(), newImage));

	return newImage;
}

MyImage * MyTextureManager::FindImage(const std::wstring & key)
{
	std::map<std::wstring, MyImage*>::iterator findImage = mAllImage.find(key);

	if (mAllImage.end() == findImage)
	{
		return nullptr;
	}

	return findImage->second;
}

