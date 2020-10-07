#include "TestPlayerScene.h"

#include "Player.h"
#include <MyActor.h>
#include "Monster.h"
#include "RandomMap.h"

TestPlayerScene::TestPlayerScene()
{
}


TestPlayerScene::~TestPlayerScene()
{
	delete mMap;
}

void TestPlayerScene::Loading()
{
	/*MyScene::Loading();

	mMap = new RandomMap(1);

	MakeMap();*/
}

void TestPlayerScene::SceneUpdate()
{
}

void TestPlayerScene::MakeMap()
{
	Make3RoomsY();

	// �߰��� ����ġ��
	mMap->MakeRandomRooms({ 15, 14 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap->ResetCount();
	mMap->MakeRandomRooms({ 15, 14 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
	mMap->ResetCount();


	// ���� ����ġ��
	mMap->MakeRandomRooms({ 15, 0 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap->ResetCount();
	mMap->MakeRandomRooms({ 15, 0 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
	mMap->ResetCount();



	// ���� ����ġ��
	mMap->MakeRandomRooms({ 15, 28 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap->ResetCount();
	mMap->MakeRandomRooms({ 15, 28 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
	mMap->ResetCount();

	// ������ (�ݵ�� ȣ��)
	mMap->FinishMap();

	// ������ ���
	if (false == mMap->HasDownstair())
	{
		MyVector2 temp = MyVector2(15, 14);

		mMap->MakeDownstair(temp);
	}
}

void TestPlayerScene::Make3RoomsY()
{
	// ��
	mMap->MakeARoom({ 15, 0 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap->MakeTopEleven({ 17, 11 }, { 6, 2 }, { 15, 0 }, { 10, 10 });
	// ��
	mMap->MakeARoom({ 15, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap->MakeAConnection({ 15, 14 }, { 10, 10 }, { 17, 11 }, { 6, 2 }, RT_BOTTOM);
	mMap->MakeTopEleven({ 17, 25 }, { 6, 2 }, { 15, 14 }, { 10, 10 });
	// ��
	mMap->MakeARoom({ 15, 28 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap->MakeAConnection({ 15, 28 }, { 10, 10 }, { 17, 25 }, { 6, 2 }, RT_BOTTOM);

	mMap->MakeArchY({ 17, 11 }, { 6, 2 }, { 15, 0 }, { 10, 10 });


	mMap->SetColumnAt(19, 4);
	mMap->SetColumnAt(22, 4);

	mMap->SetColumnAt(19, 7);
	mMap->SetColumnAt(22, 7);

	mMap->SetColumnAt(19, 18);
	mMap->SetColumnAt(22, 18);

	mMap->SetColumnAt(19, 21);
	mMap->SetColumnAt(22, 21);

	mMap->SetColumnAt(19, 32);
	mMap->SetColumnAt(22, 32);

	mMap->SetColumnAt(19, 35);
	mMap->SetColumnAt(22, 35);
}

void TestPlayerScene::Make3RoomsX()
{
	mMap->MakeARoom({ 0, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap->MakeARoom({ 11, 16 }, { 2, 6 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	mMap->MakeARoom({ 14, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap->MakeARoom({ 25, 16 }, { 2, 6 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	mMap->MakeARoom({ 28, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	mMap->SetColumnAt(4, 18);
	mMap->SetColumnAt(8, 18);

	mMap->SetColumnAt(4, 21);
	mMap->SetColumnAt(8, 21);

	mMap->SetColumnAt(18, 18);
	mMap->SetColumnAt(21, 18);

	mMap->SetColumnAt(18, 21);
	mMap->SetColumnAt(21, 21);

	mMap->SetColumnAt(32, 18);
	mMap->SetColumnAt(35, 18);

	mMap->SetColumnAt(32, 21);
	mMap->SetColumnAt(35, 21);
}



