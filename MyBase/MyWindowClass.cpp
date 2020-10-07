#include "MyWindowClass.h"

#include "MyTime.h"
#include "MyInput.h"
#include "MyGameSound.h"

std::map<std::wstring, WNDCLASSEXW> MyWindowClass::mClassMap;
std::map<std::wstring, MyWindowClass*> MyWindowClass::mWinMap;
std::map<HWND, MyWindowClass*> MyWindowClass::mHWndMap;
MyWindowClass* MyWindowClass::mMainWindow = nullptr;
HDC MyWindowClass::mMainDC;

MyWindowClass::MyDefaultWindow MyWindowClass::Construct = MyWindowClass::MyDefaultWindow();




MyWindowClass::MyWindowClass()
{
}

MyWindowClass::MyWindowClass(HWND hWnd) : mHWnd(hWnd)
{
	mMainDC = GetDC(hWnd);
}

MyWindowClass::~MyWindowClass()
{
}


void MyWindowClass::DestroyWindowClass(HWND hWnd)
{
	std::map<HWND, MyWindowClass*>::iterator FindIter = mHWndMap.find(hWnd);

	if (mHWndMap.end() == FindIter)
	{
		BOOM;
	}

	std::map<std::wstring, MyWindowClass*>::iterator NameFindIter = mWinMap.find(FindIter->second->mName);

	delete FindIter->second;
	mWinMap.erase(NameFindIter);
	mHWndMap.erase(FindIter);
}

LRESULT __stdcall MyWindowClass::MsgFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		DestroyWindowClass(hWnd);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool MyWindowClass::IsRegistered(const wchar_t* className)
{
	std::map<std::wstring, WNDCLASSEXW>::iterator FindIter = mClassMap.find(className);

	if (mClassMap.end() == FindIter)
	{
		return false;
	}

	return true;
}

void MyWindowClass::RegisterWindowClass(const WNDCLASSEXW& wcex)
{
	if (true == IsRegistered(wcex.lpszClassName) || 0 == RegisterClassExW(&wcex))
	{
		BOOM;
	}

	mClassMap.insert(std::map<std::wstring, WNDCLASSEXW>::value_type(wcex.lpszClassName, wcex));
}

MyWindowClass* MyWindowClass::FindWindowClass(const wchar_t* winName)
{
	std::map<std::wstring, MyWindowClass*>::iterator FindIter = mWinMap.find(winName);

	if (mWinMap.end() == FindIter)
	{
		return nullptr;
	}

	return FindIter->second;
}

MyWindowClass* MyWindowClass::CreateWindowClass(const wchar_t * winName
	, const wchar_t * className /*= L"DefaultWindowClass"*/, bool bShow /* = true*/)
{
	if (nullptr != FindWindowClass(winName) || false == IsRegistered(className))
	{
		BOOM;
	}

	HWND hWnd = CreateWindowW(className, winName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, 0, nullptr);

	if (0 == hWnd)
	{
		return FALSE;
	}

	MyWindowClass* newWin = new MyWindowClass(hWnd);
	newWin->mStyle = WS_OVERLAPPEDWINDOW;
	newWin->mName = winName;
	mWinMap.insert(std::map<std::wstring, MyWindowClass*>::value_type(winName, newWin));
	mHWndMap.insert(std::map<HWND, MyWindowClass*>::value_type(hWnd, newWin));

	if (1 == mWinMap.size())
	{
		mMainWindow = newWin;
	}

	if (true == bShow)
	{
		newWin->Show();
	}

	return newWin;
}

MyWindowClass* MyWindowClass::MainWindow()
{
	return mMainWindow;
}

////////////////////////////////////// MESSAGE /////////////////////////////////////////////////////

int MyWindowClass::BaseLoop(LoopUpdater& loopObj)
{
	MSG msg;

	loopObj.GameInit();

	while (0 != mWinMap.size())
	{
		if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 게임 로직 삽입
			MyTime::Update();
			MyVector2::DirUpdate();
			MyInput::Update();
			MyGameSound::SoundUpdate();
			loopObj.GameUpdate();
		}

	}

	MyGameSound::SoundEnd();

	return (int)msg.wParam;
}


//////////////////////////////// 일반 public 함수 ///////////////////////////////////


void MyWindowClass::Show()
{
	ShowWindow(mHWnd, SW_SHOW);
}

void MyWindowClass::Size(int x, int y)
{
	mSize = { (float)x, (float)y };

	RECT rc = { 0, 0, mSize.IntX(), mSize.IntY() };

	HMENU menu = GetMenu(mHWnd);

	if (nullptr == menu)
	{
		AdjustWindowRect(&rc, mStyle, FALSE);
	}
	else
	{
		AdjustWindowRect(&rc, mStyle, TRUE);
	}

	SetWindowPos(mHWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void MyWindowClass::Size(MyVector2 size)
{
	mSize = size;

	RECT rc = { 0, 0, mSize.IntX(), mSize.IntY() };

	HMENU menu = GetMenu(mHWnd);

	if (nullptr == menu)
	{
		AdjustWindowRect(&rc, mStyle, FALSE);
	}
	else
	{
		AdjustWindowRect(&rc, mStyle, TRUE);
	}

	SetWindowPos(mHWnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

}

MyVector2 MyWindowClass::MousePos()
{
	POINT Point;

	GetCursorPos(&Point);
	ScreenToClient(mHWnd, &Point);

	MyVector2 ReturnPos;

	ReturnPos.X = (float)Point.x;
	ReturnPos.Y = (float)Point.y;

	return ReturnPos;
}



///////////////////////////////// 이너 클래스 구현부 /////////////////////////////////////



MyWindowClass::MyDefaultWindow::MyDefaultWindow()
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MyWindowClass::MsgFunc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = 0;
	wcex.hIcon = 0;/*LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPI))*/;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 4);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"DefaultWindowClass";
	wcex.hIconSm = 0;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterWindowClass(wcex);
}

MyWindowClass::MyDefaultWindow::~MyDefaultWindow()
{
}
