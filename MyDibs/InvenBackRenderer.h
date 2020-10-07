#pragma once
#include <MyRenderer.h>

class Player;

class InvenBackRenderer : public MyRenderer
{
public:
	InvenBackRenderer();
	~InvenBackRenderer();

	void SetPlayer(Player* player) { mPlayer = player; }
	void Render() override;

private:
	MyRenderer* mBack;
	Player* mPlayer;

};

