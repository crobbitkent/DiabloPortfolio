#pragma once

#include <MyActor.h>

class MyAnimationRenderer;

class BlizzardTitle : public MyActor
{
public:
	BlizzardTitle();
	~BlizzardTitle();

	void Update() override;
	void DebugRender() override;

	bool IsOpeningOver();

private:
	MyAnimationRenderer* mAnimationRenderer;
	MyRenderer* mBack;
};

