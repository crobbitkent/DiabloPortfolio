#include "MyGameDebug.h"

void MyGameDebug::LeakCheckSetting()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

MyGameDebug::MyGameDebug()
{
}


MyGameDebug::~MyGameDebug()
{
}
