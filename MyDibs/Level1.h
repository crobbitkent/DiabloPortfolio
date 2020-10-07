#pragma once


#include <MyScene.h>

class Level1 : public MyScene
{
public:
	Level1();
	~Level1();

	void Loading() override;
	void SceneUpdate() override;


	void MakeMap() {}
};

