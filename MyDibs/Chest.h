#pragma once
#include <MyActor.h>



class Chest : public MyActor
{
public:
	Chest();
	~Chest();

	void DebugRender() override;
private:
	
	// Skeleton : 20 %
	// Gold : 22.5%
	// Item : 7.5%
	// Nothing : 50 %
	float mOpenRate;
};

