#pragma once

#include <vector>

#include <MyRenderer.h>

class Player;

class FogRenderer :	public MyRenderer
{
	class FogData
	{
	public:
		TileIndex mFogIndex;
		MyVector2 mPos;
		MyVector2 mSize;
		int mBF;
	};



public:
	FogRenderer();
	~FogRenderer();

	void Init(Player* player);

	void Render() override;

private:
	std::vector<FogData*> mAllFog1;
	std::vector<FogData*> mAllFog2;
	
	Player* mPlayer;
	
	int mRadius;
};

