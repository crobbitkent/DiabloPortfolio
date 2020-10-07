#pragma once
#include <MyRenderer.h>

class MyTexture;
class Player;

class StatRenderer : public MyRenderer
{
public:
	StatRenderer();
	~StatRenderer();

	void Render() override;

	void SetPlayer(Player* player);

	void ClassRender();
	void StatRender();
	void LevelUpRender();

private:
	Player* mPlayer;
	int space;

	MyActor* mRed;
	MyRenderer* mRedRenderer;

};

