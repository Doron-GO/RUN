#include "../Manager/ResourceManager.h"
#include "UserInterface.h"

UserInterface::UserInterface(void)
{
}

UserInterface::~UserInterface(void)
{
}

void UserInterface::Init(void)
{
	playerHpImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HP).handleId_;
}

void UserInterface::Update(void)
{
}

void UserInterface::DarwPlayerHp(int playerHp)
{
	playerHp_ = playerHp;
	for (int i = 0; i < playerHp_; i++)
	{
		DrawGraph(64 * i, 0, playerHpImg_, true);
	}
}

void UserInterface::DrawObstacleMeter(void)
{

}

void UserInterface::Release(void)
{
}
