#pragma once
#include <MyRenderer.h>

class MyTexture;
class Player;


class LevelRenderer : public MyRenderer
{
public:
	LevelRenderer();
	~LevelRenderer();

	void Render() override;
	void SetPlayer(Player* player);

private:
	Player* mPlayer;
	int start = 0;
};

