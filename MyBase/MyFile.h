#pragma once
#include <iostream>
#include "MyPath.h"
#include <assert.h>

class MyFile : public MyPath
{
private:
	std::wstring mFileName;
	std::wstring mExt;

	std::wstring mOpenMode;

	FILE* mFile;

public:
	template<typename T>
	void Write(const T& _Data) 
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsWrite())
		{
			assert(false);
		}

		int size = sizeof(T);

		fwrite(&_Data, sizeof(T), 1, mFile);
	}



	template<typename T>
	void Read(T& _Data)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsRead())
		{
			assert(false);
		}

		fread_s(&_Data, sizeof(T), sizeof(T), 1, mFile);
	}

	template<typename T>
	void Read(T& _Data, int size)
	{
		if (false == IsOpen())
		{
			assert(false);
		}

		if (false == IsRead())
		{
			assert(false);
		}
		//                30, 22 / 2, 2
		fread_s(&_Data, size * 2, size, sizeof(wchar_t), mFile);
	}

	// 템플릿 특수화를 써서 string과 wstring을 저장할수 있다.
	//template<>
	//void Read(std::wstring& _Data)	{	}


public:
	FILE* GetFile() { return mFile; }
	// const wchar_t* GetPath() const { return mPath; }

	bool IsOpen() 
	{
		return nullptr != mFile;
	}

	bool IsWrite() 
	{
		// 쓰기로 열렸나요?
		// 일기로 열렸나요?
		if (false == IsOpen())
		{
			return false;
		}

		if ((size_t)-1 == mOpenMode.find_first_of(L'w'))
		{
			return false;
		}

		return true;
	}

	bool IsRead()
	{
		// 쓰기로 열렸나요?
		// 일기로 열렸나요?
		if (false == IsOpen())
		{
			return false;
		}

		if ((size_t)-1 == mOpenMode.find_first_of(L'r'))
		{
			return false;
		}

		return true;
	}

	bool IsExist() 
	{
		return MyPath::IsExist(mPath.c_str());
	}

public:
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);
	void PathSetting(const std::wstring& path);
	bool Open();
	bool Open(const wchar_t* _Path, const wchar_t* _Mode);
	void Close();
	void Copy(const MyFile& other);

	// 확장자 리턴하는 함수
	inline const std::wstring Ext() const { return mExt; }
	inline const std::wstring FileName() const { return mFileName; }


public:
	MyFile();
	MyFile(const MyFile& other);
	MyFile(const wchar_t* _FileName);
	MyFile(const wchar_t* _Path, const wchar_t* _Mode);
	~MyFile();
};

