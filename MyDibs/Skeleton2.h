#pragma once
#include "Monster.h"

class Player;

class Skeleton2 : public Monster
{
public:
	Skeleton2(ColliderMap* colmap, RandomMap* randomMap, Player* player, TileIndex index);
	virtual ~Skeleton2();

	void DebugRender() override;
	void Update() override;
	void StateUpdate();

	void FindPlayer();


	void CheckDirChange();
	bool CheckDir();

	void WalkTask();

	void Idle();
	void Walk();
	void Attack();
	void Die();
	void GetHit();
	void Special();
	void Rest();

	void SetCurrentState(MONSTER_STATE state, bool noneDir = false);

	void GetDamage(int damage) override;

	// void GoingToMoveInTheMiddle();
	// void GoingToMoveFromIdle();

	void SetAnimation();
	void AutoAnimation(const wchar_t* name, int index, float frameSpeed = 0.05f);
	void AutoAnimation(const wchar_t* name, int index, bool loop, float frameSpeed = 0.05f);

	void GoingToMove(MyVector2 mousePos);
	void GoingToAttack(MyVector2 mousePos);
	void PreMove();

private:
	MyAnimationRenderer* mAnimationRenderer;
	MyAnimationRenderer* mSubRenderer;
	MyRenderer* mBlackRenderer;

	MONSTER_STATE mState;

	Player* mPlayer;
	Monster* mMonster;

	bool mIsPlayerAround;
	bool mIsExpDone;

	static int mCount;
};

