#pragma once
#include <MyRenderer.h>

class Player;

class FontRenderer : public MyRenderer
{
public:
	FontRenderer();
	~FontRenderer();

	void SetPlayer(Player* player) { mPlayer = player; }
	void Render() override;
	void SetString(std::wstring string1);
	void SetString(std::wstring string1, std::wstring string2);
	void SetString(std::wstring string1, std::wstring string2, std::wstring string3);
private:
	MyRenderer* mFont;
	Player* mPlayer;

	std::wstring mString1;
	std::wstring mString2;
	std::wstring mString3;

	int mSizeY = 0;
	int mSizeX1 = 0;
	int mSizeX2 = 0;
	int mSizeX3 = 0;
	int mIndex = 0;
};