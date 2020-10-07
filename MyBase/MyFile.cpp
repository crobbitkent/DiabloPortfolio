#include "MyFile.h"

#include <atlstr.h>


MyFile::MyFile(const wchar_t* _Path, const wchar_t* _Mode) : mFile(nullptr)
{
	Open(_Path, _Mode);
}

MyFile::MyFile() : mFile(nullptr)
{
}

MyFile::MyFile(const MyFile & other)
{
	Copy(other);
}

MyFile::MyFile(const wchar_t* _FileName) : mFile(nullptr)
{
	// mFileName = _FileName;

	PathSetting(_FileName);
}

MyFile::~MyFile()
{
	Close();
}

void MyFile::PathSetting(const char* _Path) 
{
	PathSetting(CA2W(_Path).m_psz);
}

void MyFile::PathSetting(const wchar_t* _Path)
{
	mPath = _Path;
	mFileName = mPath;
	size_t LastFolderIndex = mFileName.find_last_of(L"\\");
	mFileName = mFileName.replace(0, LastFolderIndex + 1, L"");

	mExt = mFileName;
	LastFolderIndex = mExt.find_last_of(L".");
	mExt = mExt.replace(0, LastFolderIndex + 1, L"");
}

void MyFile::PathSetting(const std::wstring & path)
{
	PathSetting(path.c_str());
}

bool MyFile::Open()
{
	return Open(mPath.c_str(), mOpenMode.c_str());
}

bool MyFile::Open(const wchar_t* _Path, const wchar_t* _Mode) 
{
	PathSetting(_Path);
	mOpenMode = _Mode;
	_wfopen_s(&mFile, mPath.c_str(), mOpenMode.c_str());

	if (nullptr == mFile)
	{
		assert(false);
		return false;
	}

	return true;
}

void MyFile::Close() {
	if (nullptr != mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}
}

void MyFile::Copy(const MyFile & other)
{
	PathSetting(other.mPath);
	mFile = other.mFile;
}

