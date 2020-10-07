#pragma once
#include <MyRenderer.h>

class Player;

class YFontRenderer : public MyRenderer
{
public:
	YFontRenderer();
	~YFontRenderer();

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
	int mSizeX = 0;
	int mIndex = 0;
};