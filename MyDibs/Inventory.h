#pragma once

class Inventory
{
public:
	Inventory();
	~Inventory();
	static void Init();

private:
	static wchar_t** mItemBlocks;
};

