#pragma once
#include "MyPath.h"

#include <vector>
#include <filesystem>

class MyFile;

// ���丮�� ����ȭ�� Ŭ����
class MyDirectory : public MyPath
{
public:
	MyDirectory();
	~MyDirectory();


	void SetCurrentPath();

	// ���� ��θ� �־��൵ ������ ������ ������
	// �� ���丮�� �ڽ��� �н��� �����ϰ� �Ѵ�.
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

	MyFile CreateMyFile(const wchar_t* _FileName);

	// �� ���丮�� ���� BOOM
	// Move(L"\\HiSuite");
	// C:\Users\JHK\Documents
	// ������ //  C:\Users\JHK\Documents\HiSuite
	// ���丮�� �̵��� �����ϴ�.
	bool Move(const wchar_t* _Name);

	// �̳༮�� ��ΰ� �ڱ� �θ� ��η� ����ȴ�.
	// C:\Users\JHK\Documents\HiSuite
	// ������ // C:\Users\JHK\Documents
	void MoveParent();

	// �� ���丮 ���ο� �� ������ �ִ��� ������ �˾Ƴ���.
	bool IsFile(const wchar_t* _FileName);

	// �� ���丮 ���ο� �ִ� ���ϵ��� ����� ��� �˾Ƴ���.
	std::vector<MyFile> DirToAllFile();

	// *.*
	// *.png
	// *.txt
	std::vector<MyFile> DirToAllFile(const wchar_t* _Delimiter);


public:
private:

};

