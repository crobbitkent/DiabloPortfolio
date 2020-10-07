#pragma once

#include <crtdbg.h>

class MyGameDebug
{
public:
	static void LeakCheckSetting();

private:
	MyGameDebug();
	~MyGameDebug();
};

