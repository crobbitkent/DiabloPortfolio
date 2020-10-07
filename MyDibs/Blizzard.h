#pragma once

#include <MyActor.h>

class Blizzard : public MyActor
{
public:
	Blizzard();
	~Blizzard();

	void Update() override;
	void DebugRender() override;
};

