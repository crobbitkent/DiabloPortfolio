#pragma once

#include <Windows.h>
#include <map>
#include <string>

#include "MyMacro.h"
#include "MyMath.h"


class MyWindowClass
{
	////////////////////////////////// 이너 클래스 ///////////////////////////
public:
	class LoopUpdater
	{
	public:
		virtual void GameInit() = 0;
		virtual void GameUpdate() = 0;
	};

private:
	class MyDefaultWindow
	{
	public:
		MyDefaultWindow();
		~MyDefaultWindow();
	};


	////////////////////////////////// 멤버 함수 /////////////////////////////
public:


	///////////////////////////// static 관리자 함수 ////////////////////////////
	static LRESULT __stdcall MsgFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool IsRegistered(const wchar_t* className);
	static void RegisterWindowClass(const WNDCLASSEXW& wcex);
	static MyWindowClass* FindWindowClass(const wchar_t* winName);
	static MyWindowClass* CreateWindowClass(const wchar_t* winName, const wchar_t* className = L"DefaultWindowClass", bool bShow = true);
	static int BaseLoop(LoopUpdater& loopObj);
	static MyWindowClass* MainWindow();
	static HDC MainDC() { return mMainDC; }
	static HWND MainHWND() { return mMainWindow->mHWnd; }


	////////////////////////////////// 일반 public 함수 ////////////////////////////////// 
	void Show();
	MyVector2 Size() { return mSize; }
	void Size(int x, int y);
	void Size(MyVector2 size);

	MyVector2 MousePos();

private:
	MyWindowClass();
	MyWindowClass(HWND hWnd);
	virtual ~MyWindowClass();

	static void DestroyWindowClass(HWND hWnd);

	////////////////////////////////// 멤버 변수 ///////////////////////////
public:
	static MyDefaultWindow Construct;
private:
	static std::map<std::wstring, WNDCLASSEXW> mClassMap;
	static std::map<std::wstring, MyWindowClass*> mWinMap;
	static std::map<HWND, MyWindowClass*> mHWndMap;
	static MyWindowClass* mMainWindow;
	static HDC mMainDC;

	std::wstring mName;
	HWND mHWnd;
	
	int mStyle;
	MyVector2 mSize;
};

