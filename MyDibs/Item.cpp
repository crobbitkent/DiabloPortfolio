
#include "Item.h"
#include "Player.h"

#include <MyRenderer.h>
#include <MyAnimationRenderer.h>
#include <MyWindowClass.h>
#include <MyInput.h>
#include <MyTextureManager.h>

#include "InvenBackRenderer.h"

bool Item::mIsChanging = false;

Item::Item()
{
	mMouseItem = nullptr;

	mInGame = CreateRenderer<MyAnimationRenderer>(LO_TILE);
	mInGame->Image(L"InGameItem.bmp");
	mInGame->TransColor(255, 255, 255);
	mInGame->SetRenderStyle(RS_TRANS);

	float speed = 0.05f;

	mInGame->CreateAnimation(L"HARMOR", L"InGameItem.bmp", 0, 14, false, speed);
	mInGame->CreateAnimation(L"AXE", L"InGameItem.bmp", 20, 32, false, speed);
	mInGame->CreateAnimation(L"CLEAVER", L"InGameItem.bmp", 80, 92, false, speed);
	mInGame->CreateAnimation(L"HPOTION", L"InGameItem.bmp", 140, 156, false, speed); // 16
	mInGame->CreateAnimation(L"MPOTION", L"InGameItem.bmp", 180, 196, false, speed);
	mInGame->CreateAnimation(L"RPOTION", L"InGameItem.bmp", 280, 296, false, speed);
	mInGame->CreateAnimation(L"MONEY", L"InGameItem.bmp", 480, 489, false, speed);
	mInGame->CreateAnimation(L"HELM", L"InGameItem.bmp", 500, 512, false, speed);
	mInGame->CreateAnimation(L"LARMOR", L"InGameItem.bmp", 520, 532, false, speed);
	mInGame->CreateAnimation(L"RING", L"InGameItem.bmp", 580, 592, false, speed);
	mInGame->CreateAnimation(L"SCROLL", L"InGameItem.bmp", 600, 612, false, speed);
	mInGame->CreateAnimation(L"SHIELD", L"InGameItem.bmp", 620, 632, false, speed);
	mInGame->CreateAnimation(L"SWORD", L"InGameItem.bmp", 660, 672, false, speed);
	mInGame->CreateAnimation(L"SSHEILD", L"InGameItem.bmp", 700, 712, false, speed);
	mInGame->SubSize({ 96, 161 });
	mInGame->Off();

	mInven = CreateRenderer<MyRenderer>(LO_UI4);
	mInven->Image(L"Items.bmp");
	mInven->SetRenderStyle(RS_TRANS);
	mInven->Off();
	mInven->SubSize({ 56.f, 84.f });
	mInven->TransColor(255, 255, 255);
	mInven->SubPos({ -100.f, -100.f });
}

Item::Item(const Item & other)
{
	mX = other.mX;
	mX = other.mY;

	mBase = other.mBase;

	mMouseItem = nullptr;

	// 애니메이션 확인용
	mIsOver = false;
	mIndex = other.mIndex;
	mAniName = other.mAniName;
	float speed = 0.05f;

	mItemData.mName = other.mItemData.mName;
	mItemData.mDamage = other.mItemData.mDamage;
	mItemData.mMaxDamage = other.mItemData.mMaxDamage;
	mItemData.mArmor = other.mItemData.mArmor;
	mItemData.mMaxArmor = other.mItemData.mMaxArmor;
	mItemData.mDurability = other.mItemData.mDurability;
	mItemData.mRequire1 = other.mItemData.mRequire1;
	mItemData.mRequire1Amount = other.mItemData.mRequire1Amount;
	mItemData.mRequire2 = other.mItemData.mRequire2;
	mItemData.mRequire2Amount = other.mItemData.mRequire2Amount;
	mItemData.mPrice = other.mItemData.mPrice;
	mItemData.mItemLevel = other.mItemData.mItemLevel;
	mItemData.mType = other.mItemData.mType;
	mItemData.X = other.mItemData.X;
	mItemData.Y = other.mItemData.Y;

	mInGame = CreateRenderer<MyAnimationRenderer>(LO_UPTILE);
	mInGame->Image(L"InGameItem.bmp");
	mInGame->TransColor(255, 255, 255);
	mInGame->SetRenderStyle(RS_TRANS);

	//mSubInGame = CreateRenderer<MyAnimationRenderer>(LO_DOWNTILE);
	//mSubInGame->Image(L"InGameItem.bmp");
	//mSubInGame->TransColor(255, 255, 255);
	//mSubInGame->SetRenderStyle(RS_TRANS);

	mInGame2 = CreateRenderer<MyAnimationRenderer>(LO_ITEM);
	mInGame2->Image(L"InGameItem.bmp");
	mInGame2->TransColor(255, 255, 255);
	mInGame2->SetRenderStyle(RS_TRANS);

	mInGameBack = CreateRenderer<MyAnimationRenderer>(LO_ITEMBACK);
	mInGameBack->Image(L"InGameItemB.bmp");
	mInGameBack->TransColor(255, 255, 255);
	mInGameBack->SetRenderStyle(RS_TRANS);

	mInGame->CreateAnimation(L"HARMOR", L"InGameItem.bmp", 0, 14, false, speed);
	mInGame->CreateAnimation(L"AXE", L"InGameItem.bmp", 20, 32, false, speed);
	mInGame->CreateAnimation(L"CLEAVER", L"InGameItem.bmp", 80, 92, false, speed);
	mInGame->CreateAnimation(L"HPOTION", L"InGameItem.bmp", 140, 156, false, speed); // 16
	mInGame->CreateAnimation(L"MPOTION", L"InGameItem.bmp", 180, 196, false, speed);
	mInGame->CreateAnimation(L"RPOTION", L"InGameItem.bmp", 280, 296, false, speed);
	mInGame->CreateAnimation(L"MONEY", L"InGameItem.bmp", 480, 489, false, speed);
	mInGame->CreateAnimation(L"HELM", L"InGameItem.bmp", 500, 512, false, speed);
	mInGame->CreateAnimation(L"LARMOR", L"InGameItem.bmp", 540, 552, false, speed);
	mInGame->CreateAnimation(L"RING", L"InGameItem.bmp", 580, 592, false, speed);
	mInGame->CreateAnimation(L"SCROLL", L"InGameItem.bmp", 600, 612, false, speed);
	mInGame->CreateAnimation(L"SHIELD", L"InGameItem.bmp", 620, 632, false, speed);
	mInGame->CreateAnimation(L"SWORD", L"InGameItem.bmp", 640, 652, false, speed);
	mInGame->CreateAnimation(L"SSHEILD", L"InGameItem.bmp", 700, 712, false, speed);
	mInGame->SubSize({ 96, 192 });
	mInGame->Off();

	mInGame2->CreateAnimation(L"HARMOR", L"InGameItem.bmp", 0, 14, false, speed);
	mInGame2->CreateAnimation(L"AXE", L"InGameItem.bmp", 20, 32, false, speed);
	mInGame2->CreateAnimation(L"CLEAVER", L"InGameItem.bmp", 80, 92, false, speed);
	mInGame2->CreateAnimation(L"HPOTION", L"InGameItem.bmp", 140, 156, false, speed); // 16
	mInGame2->CreateAnimation(L"MPOTION", L"InGameItem.bmp", 180, 196, false, speed);
	mInGame2->CreateAnimation(L"RPOTION", L"InGameItem.bmp", 280, 296, false, speed);
	mInGame2->CreateAnimation(L"MONEY", L"InGameItem.bmp", 480, 489, false, speed);
	mInGame2->CreateAnimation(L"HELM", L"InGameItem.bmp", 500, 512, false, speed);
	mInGame2->CreateAnimation(L"LARMOR", L"InGameItem.bmp", 540, 552, false, speed);
	mInGame2->CreateAnimation(L"RING", L"InGameItem.bmp", 580, 592, false, speed);
	mInGame2->CreateAnimation(L"SCROLL", L"InGameItem.bmp", 600, 612, false, speed);
	mInGame2->CreateAnimation(L"SHIELD", L"InGameItem.bmp", 620, 632, false, speed);
	mInGame2->CreateAnimation(L"SWORD", L"InGameItem.bmp", 640, 652, false, speed);
	mInGame2->CreateAnimation(L"SSHEILD", L"InGameItem.bmp", 700, 712, false, speed);
	mInGame2->SubSize({ 96, 192 });
	mInGame2->Off();

	mInGameBack->CreateAnimation(L"HARMOR", L"InGameItemB.bmp", 0, 14, false, speed);
	mInGameBack->CreateAnimation(L"AXE", L"InGameItemB.bmp", 20, 32, false, speed);
	mInGameBack->CreateAnimation(L"CLEAVER", L"InGameItemB.bmp", 80, 92, false, speed);
	mInGameBack->CreateAnimation(L"HPOTION", L"InGameItemB.bmp", 140, 156, false, speed); // 16
	mInGameBack->CreateAnimation(L"MPOTION", L"InGameItemB.bmp", 180, 196, false, speed);
	mInGameBack->CreateAnimation(L"RPOTION", L"InGameItemB.bmp", 280, 296, false, speed);
	mInGameBack->CreateAnimation(L"MONEY", L"InGameItemB.bmp", 480, 489, false, speed);
	mInGameBack->CreateAnimation(L"HELM", L"InGameItemB.bmp", 500, 512, false, speed);
	mInGameBack->CreateAnimation(L"LARMOR", L"InGameItemB.bmp", 540, 552, false, speed);
	mInGameBack->CreateAnimation(L"RING", L"InGameItemB.bmp", 580, 592, false, speed);
	mInGameBack->CreateAnimation(L"SCROLL", L"InGameItemB.bmp", 600, 612, false, speed);
	mInGameBack->CreateAnimation(L"SHIELD", L"InGameItemB.bmp", 620, 632, false, speed);
	mInGameBack->CreateAnimation(L"SWORD", L"InGameItemB.bmp", 640, 652, false, speed);
	mInGameBack->CreateAnimation(L"SSHEILD", L"InGameItemB.bmp", 700, 712, false, speed);
	mInGameBack->SubSize({ 96 , 192 });
	mInGameBack->Off();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////


	mInven = CreateRenderer<MyRenderer>(LO_UI4);

	mInven->Image(L"Items.bmp");
	mInven->SetRenderStyle(RS_TRANS);
	mInven->SubSize({ 56.f, 84.f });
	mInven->TransColor(255, 255, 255);
	mInven->Off();
	mInven->ImageIndex(mIndex);
	mInven->CamOnOff();

	if (nullptr != other.mInven)
	{
		mInven->SubPos(other.mInven->SubPos());
	}


	mInvenEdge = CreateRenderer<MyRenderer>(LO_UI5);
	mInvenEdge->Image(L"ItemsEdge.bmp");
	mInvenEdge->SetRenderStyle(RS_TRANS);
	mInvenEdge->SubSize({ 56.f, 84.f });
	mInvenEdge->TransColor(255, 255, 255);
	mInvenEdge->Off();
	mInvenEdge->ImageIndex(mIndex);
	mInvenEdge->CamOnOff();

	if (nullptr != other.mInvenEdge)
	{
		mInvenEdge->SubPos(other.mInvenEdge->SubPos());
	}
}



Item::~Item()
{
}

Item* Item::CopyItem(int level)
{
	Item* temp = new Item(*this);
	temp->mLevel = level;

	return temp;
}

void Item::SetTile(MyVector2 pos)
{

}

void Item::Update()
{
	if (true == mInGame->IsOn())
	{
		if (true == mInGame->IsCurrentAnimationEnd() && false == mIsOver)
		{
			mInGame->Off();

			mInGame2->FinishAnimation();
			mInGame2->On();
			mInGameBack->FinishAnimation();
			// mInGameBack->On();

			mIsOver = true;
		}
		else
		{
			int index = 0;

			switch (mItemData.mType)
			{
			case IT_GOLD:
				index = 24;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (5 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"gold1.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_WEAPON_AXE:
				index = 1;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipaxe.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_WEAPON_CLEAVER:
				index = 4;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipaxe.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_WEAPON_SWORD:
				index = 32;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipswor.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_WEAPON_BOW:
				index = 32;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipbow.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_ARMOR_LIGHT:
				index = 17;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"fliplarm.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_ARMOR_HEAVY:
				index = 32;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipharm.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_HELM:
				index = 25;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipcap.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_SHIELD:
				index = 31;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipshld.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_RING:
			case IT_NECK:
				index = 27;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipring.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_BOOK:
				index = 32;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipbook.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_HPOTION:
				index = 7;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flippot.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_MPOTION:
				index = 8;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flippot.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_RPOTION:
				index = 14;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flippot.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_ISCROLL:
			case IT_PSCROLL:
				index = 30;
				if (false == mIsDropped)
				{
					if (0 != mInGame->CurFrame() || 0 != mInGame2->CurFrame())
					{
						if (6 == (mInGame->CurFrame() - index * 20))
						{
							MyGameSound::SoundPlayToInst(L"flipscrl.wav");
							mIsDropped = true;
						}
					}
				}
				break;
			case IT_MAX:
				break;
			default:
				break;
			}
		}
		
	}

	// 마우스 위치에 따라서 불 들어옴
	if (mPlayer != nullptr && nullptr != mInGameBack && false == mIsTaken && true == mIsOver)
	{
		if (mTileIndex == mPlayer->GetMouseIndex())
		{
			mInGameBack->FinishAnimation();
			mInGameBack->On();

			// 아이템 이름 띄우기
			if (IT_WEAPON_CLEAVER == mItemData.mType)
			{
				mPlayer->DescYOn(mItemData.mName);
			}
			else if (IT_GOLD == mItemData.mType)
			{
				wchar_t amount[5];
				int gold = mItemData.gold;
				_itow_s(gold, amount, 5, 10);

				std::wstring temp = amount;
				temp.append(L" ");
				temp.append(mItemData.mName);
				temp.append(L" ");
				temp.append(L"pieces");
				mPlayer->DescOn(temp);
			}
			else
			{
				mPlayer->DescOn(mItemData.mName);
			}

			
		}
		else
		{
			// mPlayer->UnshowDescription();
			mInGameBack->Off();
		}
	}

	if (mItemData.mType == IT_GOLD)
	{
		if (mItemData.gold < 2500)
		{
			mInven->ImageIndex(15);
		}
		else if (mItemData.gold < 5000)
		{
			mInven->ImageIndex(16);
		}
		else
		{
			mInven->ImageIndex(17);
		}
	}

	if (nullptr != mPlayer)
	{
		if (true == mPlayer->IsInvenOpen() && true == mIsTaken)
		{
			mInven->On();
		}
		else
		{
			mInven->Off();
		}

		// 인벤 아이템 마우스에 불 들어옴
		if (true == CheckMouse() && true == mPlayer->IsInvenOpen() && true == mIsTaken && false == mIsOn)
		{
			mInvenEdge->On();
		}
		else
		{
			mInvenEdge->Off();
		}

		// 올리기
		if (true == CheckMouse() && true == mPlayer->IsInvenOpen() && true == mIsTaken
			&& true == MyInput::Down(L"LMOUSEBTN") && false == mIsOn && true == mIsSet)
		{
			Unarm();
		}
		// 내리기
		else if (true == mIsOn && false == mIsSet)
		{
			mInven->SubPos(MyWindowClass::MainWindow()->MousePos());

			if (true == MyInput::Down(L"LMOUSEBTN") && false == mWillBeSet)
			{
				mWillBeSet = true;
				// SetInven(MyWindowClass::MainWindow()->MousePos());
			}
			// 내리기 예정
			else if (true == mWillBeSet)
			{
				SetInven(MyWindowClass::MainWindow()->MousePos());
				mWillBeSet = false;
			}
		}
	}

	if (true == mWillBeOn)
	{
		mIsOn = true;
		mWillBeOn = false;
	}

	if (true == mWillBeOff)
	{
		mIsOn = false;
		mWillBeOff = false;
	}

	// if()
}

// 올릴때
void Item::Unarm()
{
	// 올리려는데 (마우스에) 이미 누가 있다! 두둥
	ITEM_TYPE type = mItemData.mType;

	// 이미 있다면
	if (nullptr != mPlayer->mMouseItem && this != mPlayer->mMouseItem)
	{
	// 	 mPlayer->mMouseItem->SetInven(MyWindowClass::MainWindow()->MousePos());
		mPlayer->mMouseItem->mWillBeSet = true;
	}

	mInvenEdge->Off();
	// mPlayer->mInvenBack->Off();
	UnsetInven();
	mInven->SubPos(MyWindowClass::MainWindow()->MousePos());
	mIsOn = true;

	mIsSet = false;

	//if (true == mPlayer->mIsItemOn)
	//{
	//	if (mPlayer->mMouseItem != this)
	//	{
	//		BOOM;
	//	}
	//}
	//else
	//{
	mPlayer->mMouseItem = this;
	mPlayer->mIsItemOn = true;
	// }

	if (true == mIsEquiped)
	{
		mIsEquiped = false;
		mPlayer->UnapplyItem(this);

		switch (type)
		{
		case IT_GOLD:
			break;
		case IT_WEAPON_AXE:
		case IT_WEAPON_CLEAVER:
			mHasShield = false;
			mPlayer->mHasShield = false;
			mPlayer->mShield = nullptr;
		case IT_WEAPON_SWORD:
		case IT_WEAPON_BOW:
			mHasWeapon = false;
			mPlayer->mHasWeapon = false;
			mPlayer->mWeapon = nullptr;
			break;
		case IT_ARMOR_LIGHT:
		case IT_ARMOR_HEAVY:
			mHasArmor = false;
			mPlayer->mHasArmor = false;
			mPlayer->mArmor = nullptr;
			break;
		case IT_HELM:
			mHasHelm = false;
			mPlayer->mHasHelm = false;
			mPlayer->mHelm = nullptr;
			break;
		case IT_SHIELD:
			mHasShield = false;
			mPlayer->mHasShield = false;
			mPlayer->mShield = nullptr;
			break;
		case IT_RING:
			if (true == mHasLeftRing)
			{
				mHasLeftRing = false;
				mPlayer->mHasLeftRing = false;
				mPlayer->mLRing = nullptr;
			}
			else if (true == mHasRightRing)
			{
				mHasRightRing = false;
				mPlayer->mHasRightRing = false;
				mPlayer->mRRing = nullptr;
			}
			break;
		case IT_NECK:
			mHasNeck = false;
			mPlayer->mHasNeck = false;
			mPlayer->mNeck = nullptr;
			break;
		default:
			BOOM;
			break;
		}
	}

	MyGameSound::SoundPlayToInst(L"invgrab.wav");
}


// 내릴때
void Item::SetInven(MyVector2 mousePos)
{
	ITEM_TYPE type = mItemData.mType;

	switch (type)
	{
	case IT_GOLD:
		MyGameSound::SoundPlayToInst(L"gold.wav");
		break;
	case IT_WEAPON_AXE:
		MyGameSound::SoundPlayToInst(L"invaxe.wav");
		break;
	case IT_WEAPON_CLEAVER:
		MyGameSound::SoundPlayToInst(L"invaxe.wav"); 
		break;
	case IT_WEAPON_SWORD:
		MyGameSound::SoundPlayToInst(L"invsword.wav");
		break;
	case IT_WEAPON_BOW:
		MyGameSound::SoundPlayToInst(L"invbow.wav");
		break;
	case IT_ARMOR_LIGHT:
		MyGameSound::SoundPlayToInst(L"invlarm.wav");
		break;
	case IT_ARMOR_HEAVY:
		MyGameSound::SoundPlayToInst(L"invharm.wav");
		break;
	case IT_HELM:
		MyGameSound::SoundPlayToInst(L"invcap.wav");
		break;
	case IT_SHIELD:
		MyGameSound::SoundPlayToInst(L"invshiel.wav");
		break;
	case IT_RING:
	case IT_NECK:
		MyGameSound::SoundPlayToInst(L"invring.wav");
		break;
	case IT_BOOK:
		MyGameSound::SoundPlayToInst(L"invbook.wav");
		break;
	case IT_HPOTION:
	case IT_MPOTION:
	case IT_RPOTION:
		MyGameSound::SoundPlayToInst(L"invpot.wav");
		break;
	case IT_ISCROLL:
	case IT_PSCROLL:
		MyGameSound::SoundPlayToInst(L"invscrol.wav");
		break;
	case IT_MAX:
		break;
	default:
		break;
	}

	int newX = mousePos.IntX();
	int newY = (int)(mousePos.Y);

	if (1 == mItemData.X && 1 == mItemData.Y)
	{
		for (int x = 0; x < 10 - mItemData.X + 1; ++x)
		{
			for (int y = 0; y < 4 - mItemData.Y + 1; ++y)
			{
				if (newX >= 366.f + (x - 1) * 29 && newX < 366.f + (x - 1) * 29 + 29)
				{
					if (newY >= 260.f + (y - 1.5) * 29 && newY < 266.f + (y - 1.5) * 29 + 29)
					{
						ResetPosition(x, y);
						mInvenEdge->On();

						mWillBeOff = true;

						mPlayer->mIsItemOn = false;

						if (this == mPlayer->mMouseItem)
						{
							mPlayer->mMouseItem = nullptr;
						}

						return;
					}
				}
			}
		}
	}
	else
	{
		for (int x = 0; x < 10 - mItemData.X + 1; ++x)
		{
			for (int y = 0; y < 4 - mItemData.Y + 1; ++y)
			{
				if (newX >= 366.f + (x - 1.5f) * 29 && newX < 366.f + (x - 1.5f) * 29 + mItemData.X * 29)
				{
					if (newY >= 260.f + (y - 1) * 29 && newY < 266.f + (y - 1) * 29 + mItemData.Y * 29 / 2)
					{
						ResetPosition(x, y);
						mInvenEdge->On();

						mWillBeOff = true;
		
						mPlayer->mIsItemOn = false;

						if (this == mPlayer->mMouseItem)
						{
							mPlayer->mMouseItem = nullptr;
						}
				
						return;
					}
				}
			}
		}
	}

	/////////////////////////////////////////////// 장착하는 부분 ///////////////////////////////////
	///////// 무기
	if (IT_WEAPON_AXE == mItemData.mType || IT_WEAPON_SWORD == mItemData.mType || IT_WEAPON_CLEAVER == mItemData.mType)
	{
		if (false == mPlayer->mHasWeapon)
		{
			if (newX >= 338.f && newX < 338.f + 29 * 2)
			{
				if (newY >= 76 && newY < 76 + 29 * 3)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasWeapon)
					{
						// mPlayer->mWeapon->Unarm();
					}

					ArmWeapon();

					return;
				}
			}
		}
		
	}

	if (IT_ARMOR_LIGHT == mItemData.mType || IT_ARMOR_HEAVY == mItemData.mType)
	{
		if (false == mPlayer->mHasArmor)
		{
			if (newX >= 338.f + 29 * 4 && newX < 338.f + 29 * 6)
			{
				if (newY >= 76 && newY < 76 + 29 * 3)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasArmor)
					{
						// mPlayer->mArmor->Unarm();
					}

					ArmArmor();
					return;
				}
			}
		}
	}

	if (IT_SHIELD == mItemData.mType)
	{
		if (false == mPlayer->mHasShield)
		{
			if (newX >= 338.f + 29 * 8 && newX < 338.f + 29 * 10)
			{
				if (newY >= 76 && newY < 76 + 29 * 3)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasShield)
					{
						// mPlayer->mShield->Unarm();
					}

					ArmShield();
					return;
				}
			}
		}
	}

	if (IT_HELM == mItemData.mType)
	{
		if (false == mPlayer->mHasHelm)
		{
			if (newX >= 338.f + 29 * 4 && newX < 338.f + 29 * 6)
			{
				if (newY >= 3 && newY < 3 + 29 * 2)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasHelm)
					{
						// mPlayer->mHelm->Unarm();
					}

					ArmHelm();
					return;
				}
			}
		}
	}

	if (IT_RING == mItemData.mType)
	{
		if (false == mPlayer->mHasLeftRing)
		{
			if (newX >= 338.f + 29 && newX < 338.f + 29 * 2)
			{
				if (newY >= 76 + 29 * 3.5 && newY < 76 + 29 * 4.5)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasLeftRing)
					{
						// mPlayer->mLRing->Unarm();
					}

					ArmLRing();
					return;
				}
			}
		}

		if (false == mPlayer->mHasRightRing)
		{
			if (newX >= 338.f + 29 * 8 && newX < 338.f + 29 * 9)
			{
				if (newY >= 76 + 29 * 3.5 && newY < 76 + 29 * 4.5)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasRightRing)
					{
						// mPlayer->mRRing->Unarm();
					}

					ArmRRing();
					return;
				}
			}
		}
	}

	if (IT_NECK == mItemData.mType)
	{
		if (false == mPlayer->mHasNeck)
		{
			if (newX >= 338.f + 29 * 6.5f && newX < 338.f + 29 * 7.5f)
			{
				if (newY >= 3 + 29 && newY < 3 + 29 * 2)
				{
					// 만약 이미 장착 중이였다면
					if (mPlayer->mHasNeck)
					{
						// mPlayer->mNeck->Unarm();
					}

					ArmNeck();
					return;
				}
			}
		}
	}	
}

void Item::UnsetInven()
{
	if (mX < 0 || mY < 0)
	{
		return;
	}

	for (int x = mX; x < mX + mItemData.X; ++x)
	{
		for (int y = mY; y < mY + mItemData.Y; ++y)
		{
			mPlayer->mInventory[x + 10 * y] = nullptr;
		}
	}

	mX = -1;
	mY = -1;
}

void Item::ArmWeapon()
{
	UnsetInven();

	mIsEquiped = true;

	if (mItemData.X == 2)
	{
		mInven->SubPos({ 338.f + 29, 76.f + 29 * 1.5f });
		mInvenEdge->SubPos({ 338.f + 29, 76.f + 29 * 1.5f });
	}
	else if (mItemData.X == 1)
	{
		mInven->SubPos({ 338.f + 29 * 1.5, 76.f + 29 * 1.5f });
		mInvenEdge->SubPos({ 338.f + 29 * 1.5f, 76.f + 29 * 1.5f });
	}


	mWillBeOff = true;
	mHasWeapon = true;

	mPlayer->mHasWeapon = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mWeapon = this;

	mIsSet = true;

	if (this == mPlayer->mMouseItem)
	{
		mPlayer->mMouseItem = nullptr;
	}
}

void Item::ArmArmor()
{
	UnsetInven();

	mIsEquiped = true;

	mInven->SubPos({ 338.f + 29 * 5, 76.f + 29 * 1.5f });
	mInvenEdge->SubPos({ 338.f + 29 * 5, 76.f + 29 * 1.5f });



	mWillBeOff = true;
	mHasArmor = true;

	mPlayer->mHasArmor = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mArmor= this;

	mIsSet = true;
}

void Item::ArmShield()
{
	UnsetInven();

	mIsEquiped = true;

	mInven->SubPos({ 338.f + 29 * 9, 76.f + 29 * 1.5f });
	mInvenEdge->SubPos({ 338.f + 29 * 9, 76.f + 29 * 1.5f });

	mWillBeOff = true;
	mHasShield = true;

	mPlayer->mHasShield = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mShield = this;

	mIsSet = true;

	if (this == mPlayer->mMouseItem)
	{
		mPlayer->mMouseItem = nullptr;
	}
}

void Item::ArmHelm()
{
	UnsetInven();

	mIsEquiped = true;

	mInven->SubPos({ 338.f + 29 * 5, 31 * 1.5f });
	mInvenEdge->SubPos({ 338.f + 29 * 5,  31 * 1.5f });

	mWillBeOff = true;
	mHasHelm = true;

	mPlayer->mHasHelm = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mHelm= this;

	mIsSet = true;

	if (this == mPlayer->mMouseItem)
	{
		mPlayer->mMouseItem = nullptr;
	}
}

void Item::ArmLRing()
{
	UnsetInven();

	mIsEquiped = true;

	mInven->SubPos({ 338.f +2+ 29 * 2.f,  76 + 29 * 5.f });
	mInvenEdge->SubPos({ 338.f +2+ 29 * 2.f,  76 + 29 * 5.f });

	mWillBeOff = true;
	mHasLeftRing = true;

	mPlayer->mHasLeftRing = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mLRing= this;

	mIsSet = true;

	if (this == mPlayer->mMouseItem)
	{
		mPlayer->mMouseItem = nullptr;
	}
}

void Item::ArmRRing()
{
	UnsetInven();

	mIsEquiped = true;

	mInven->SubPos({ 338.f + 29 * 9.f,  76 + 29 * 5.f });
	mInvenEdge->SubPos({ 338.f + 29 * 9.f,  76 + 29 * 5.f });

	mWillBeOff = true;
	mHasRightRing = true;

	mPlayer->mHasRightRing = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mRRing = this;

	mIsSet = true;

	if (this == mPlayer->mMouseItem)
	{
		mPlayer->mMouseItem = nullptr;
	}
}

void Item::ArmNeck()
{
	UnsetInven();

	mIsEquiped = true;

	mInven->SubPos({ 338.f + 29 * 7.5f, 31 * 2.5f });
	mInvenEdge->SubPos({ 338.f + 29 * 7.5f,  31 * 2.5f });

	mWillBeOff = true;
	mHasNeck = true;

	mPlayer->mHasNeck = true;
	mPlayer->ApplyItem(this);
	// mPlayer->mIsItemOn = false;
	mPlayer->mNeck = this;

	mIsSet = true;

	if (this == mPlayer->mMouseItem)
	{
		mPlayer->mMouseItem = nullptr;
	}
}

// 아이템 짚어가서 바닥에서 사라짐
void Item::AllOff()
{
	mInGameBack->Off();
	mInGame->Off();
	mInGame2->Off();
	mIsTaken = true;

	mTileIndex = { -1, -1 };
}

void Item::ResetPosition(int x, int y)
{
	mInven->SubPos({ 366.f + x * 29, 268.f + y * 29 });
	mInvenEdge->SubPos({ 366.f + x * 29, 268.f + y * 29 });

	mX = x;
	mY = y;

	int count = 0;

	// mPlayer->mItemList.push_back(this);

	for (int i = x; i < x + mItemData.X; ++i)
	{
		for (int j = y; j < y + mItemData.Y; ++j)
		{
			if (nullptr != mPlayer->mInventory[i + 10 * (j)])
			{
				++count;
				if (count == 1)
				{
					mMouseItem = mPlayer->mInventory[i + 10 * (j)];
					continue;
				}

				if (mPlayer->mInventory[i + 10 * (j)] != nullptr &&
					mMouseItem != mPlayer->mInventory[i + 10 * (j)])
				{		
					mMouseItem = nullptr;
			
					return;
				}
			}
		}
	}

	//if (nullptr != mMouseItem)
	//{
	//	mMouseItem->UnsetInven();
	//	mMouseItem->mWillBeOn = true;
	//	mMouseItem = nullptr;
	//}

	for (int i = x; i < x + mItemData.X; ++i)
	{
		for (int j = y; j < y + mItemData.Y; ++j)
		{
			mPlayer->mInventory[i + 10 * j] = this;
		}
	}

	mIsSet = true;
}

void Item::SetPosition(int x, int y)
{
	mInven->SubPos({ 366.f + x * 29, 268.f + y * 29 });
	mInvenEdge->SubPos({ 366.f + x * 29, 268.f + y * 29 });

	mX = x;
	mY = y;

	for (int i = x; i < x + mItemData.X; ++i)
	{
		for (int j = y; j < y + mItemData.Y; ++j)
		{
			mPlayer->mInventory[i + 10 * j] = this;
		}
	}

	mIsSet = true;
}

void Item::ResetAni()
{
	ITEM_TYPE type = mItemData.mType;

	switch (type)
	{
	case IT_GOLD:
		MyGameSound::SoundPlayToInst(L"gold1.wav");
		break;
	case IT_WEAPON_AXE:
		MyGameSound::SoundPlayToInst(L"invaxe.wav");
		break;
	case IT_WEAPON_CLEAVER:
		MyGameSound::SoundPlayToInst(L"invsign.wav");
		break;
	case IT_WEAPON_SWORD:
		MyGameSound::SoundPlayToInst(L"invsword.wav");
		break;
	case IT_WEAPON_BOW:
		MyGameSound::SoundPlayToInst(L"invbow.wav");
		break;
	case IT_ARMOR_LIGHT:
		MyGameSound::SoundPlayToInst(L"invlarm.wav");
		break;
	case IT_ARMOR_HEAVY:
		MyGameSound::SoundPlayToInst(L"invharm.wav");
		break;
	case IT_HELM:
		MyGameSound::SoundPlayToInst(L"invcap.wav");
		break;
	case IT_SHIELD:
		MyGameSound::SoundPlayToInst(L"invshiel.wav");
		break;
	case IT_RING:
	case IT_NECK:
		MyGameSound::SoundPlayToInst(L"invring.wav");
		break;
	case IT_BOOK:
		MyGameSound::SoundPlayToInst(L"invbook.wav");
		break;
	case IT_HPOTION:
	case IT_MPOTION:
	case IT_RPOTION:
		MyGameSound::SoundPlayToInst(L"invpot.wav");
		break;
	case IT_ISCROLL:
	case IT_PSCROLL:
		MyGameSound::SoundPlayToInst(L"invscrol.wav");
		break;
	case IT_MAX:
		break;
	default:
		break;
	}

	mInGame->On();
	mInGame2->On();
	mInGameBack->On();
	mInGame->SetCurrentAnimation(mAniName.c_str());
	mInGame2->SetCurrentAnimation(mAniName.c_str());
	mInGameBack->SetCurrentAnimation(mAniName.c_str());
}

// 올릴때 사용된다.
bool Item::CheckMouse()
{
	MyVector2 mPos = MyWindowClass::MainWindow()->MousePos();

	int newX = mPos.IntX();
	int newY = mPos.IntY();

	if (false == mIsEquiped)
	{
		if (newX >= 366.f + (mX - 1) * 29 && newX < 366.f + (mX - 1) * 29 + mItemData.X * 29)
		{
			if (newY >= 266.f + (mY - 1.5) * 29 && newY < 266.f + (mY - 1.5) * 29 + mItemData.Y * 29)
			{
				return true;
			}
		}
	}
	else if(true == mHasWeapon)
	{
		if (newX >= 338.f && newX < 338.f + 29 * 2)
		{
			if (newY >= 76.f && newY < 76.f + 29 * 3)
			{
				return true;
			}
		}
	}	

	if (IT_ARMOR_LIGHT == mItemData.mType || IT_ARMOR_HEAVY == mItemData.mType)
	{
		if (true == mPlayer->mHasArmor)
		{
			if (newX >= 338.f + 29 * 4 && newX < 338.f + 29 * 6)
			{
				if (newY >= 76 && newY < 76 + 29 * 3)
				{
					return true;
				}
			}
		}
	}

	if (IT_SHIELD == mItemData.mType)
	{
		if (true == mPlayer->mHasShield)
		{
			if (newX >= 338.f + 29 * 8 && newX < 338.f + 29 * 10)
			{
				if (newY >= 76 && newY < 76 + 29 * 3)
				{
					return true;
				}
			}
		}
	}

	if (IT_HELM == mItemData.mType)
	{
		if (true == mPlayer->mHasHelm)
		{
			if (newX >= 338.f + 29 * 4 && newX < 338.f + 29 * 6)
			{
				if (newY >= 3 && newY < 3 + 29 * 2)
				{
					return true;
				}
			}
		}
	}

	if (IT_RING == mItemData.mType)
	{
		if (true == mPlayer->mHasLeftRing)
		{
			if (newX >= 338.f + 29 && newX < 338.f + 29 * 2)
			{
				if (newY >= 76 + 29 * 3.5 && newY < 76 + 29 * 4.5)
				{
					return true;
				}
			}
		}

		if (true == mPlayer->mHasRightRing)
		{
			if (newX >= 338.f + 29 * 8 && newX < 338.f + 29 * 9)
			{
				if (newY >= 76 + 29 * 3.5 && newY < 76 + 29 * 4.5)
				{
					return true;
				}
			}
		}
	}

	if (IT_NECK == mItemData.mType)
	{
		if (true == mPlayer->mHasNeck)
		{
			if (newX >= 338.f + 29 * 6.5f && newX < 338.f + 29 * 7.5f)
			{
				if (newY >= 3 + 29 && newY < 3 + 29 * 2)
				{
					return true;
				}
			}
		}
	}

	return false;
}


void Item::DebugRender()
{
	wchar_t Arr[256] = { 0, };

	//swprintf_s(Arr, L"mX, mY : %d, %d", mX, mY);
	//TextOutW(MyTextureManager::BackDC(), 0, 45, Arr, lstrlen(Arr));

	//swprintf_s(Arr, L"mIsOn : %d", mIsOn);
	//TextOutW(MyTextureManager::BackDC(), 0, 60, Arr, lstrlen(Arr));
}

