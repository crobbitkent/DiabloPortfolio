#pragma once
#include <MyScene.h>

class Player;
class RandomMap;

class TestPlayerScene : public MyScene
{
public:
	TestPlayerScene();
	~TestPlayerScene();

	void Loading() override;
	void SceneUpdate() override;
	void MakeMap();
	void Make3RoomsX();
	void Make3RoomsY();


private:
	Player* mPlayer;
	RandomMap* mMap;
};

