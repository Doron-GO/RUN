#include "../Manager/ResourceManager.h"
#include "UserInterface.h"

UserInterface::UserInterface(int hp)
{
	hp_ = hp;
}

UserInterface::~UserInterface(void)
{
}

void UserInterface::Init(void)
{
	//UI
	//HP
	//HPは1につき1つのハート画像を表示することで表す
	//お邪魔ゲージ
	//お邪魔ゲージは残り残量をゲージで表す
	ObsMet_ = 0;

	//プレイヤーHP画像の読み込み
	imgHp_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HP).handleId_;
	//お邪魔ゲージ画像
	imgObsMetCase_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OJAMAGAUGE_CASE).handleId_;
	imgObsMet_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OJAMAGAUGE).handleId_;
}

void UserInterface::Update(void)
{
}

//プレイヤーHPの増減
void UserInterface::SubHp()
{
	//呼び出すとHPを1減らす
	hp_--;

	//減りすぎないようにする
	if (hp_ < 0)
	{
		hp_ = 0;
	}
}

//お邪魔ゲージの増減
void UserInterface::AddObstacleMeter(float add)
{
	//呼び出すとお邪魔ゲージを引数分増やす
	ObsMet_ += add;

	//最大値を超えないようにする
	if (ObsMet_ > OBSTACLE_METER_MAX)
	{
		ObsMet_ = OBSTACLE_METER_MAX;
	}
}

void UserInterface::SubObstacleMeter(float sub)
{
	//呼び出すとお邪魔ゲージを引数分減らす
	ObsMet_ -= sub;

	//減りすぎないようにする
	if (ObsMet_ < 0)
	{
		ObsMet_ = 0;
	}
}

//描画
void UserInterface::Draw(void)
{
	//HPの描画
	//画像の大きさは64×64です
	DrawHp();

	//お邪魔ゲージの描画
	DrawObstacleMeter();
}

void UserInterface::DrawHp()
{
	//HP画像の描画
	for (int i = 0;i < hp_;i++) {
		DrawGraph(64 * i, 0, imgHp_, true);
	}
}

void UserInterface::DrawObstacleMeter(void)
{
	//お邪魔ゲージ枠
	DrawGraph(0, 80, imgObsMetCase_, true);

	//お邪魔ゲージの中身
	//残りゲージで長さが変動する
	int xx = static_cast<int>(192.0f * (ObsMet_ / 100.0f));
	DrawRectGraph(0, 80, 0, 0, xx, 16, imgObsMet_, true);
}

void UserInterface::Release(void)
{
}

int UserInterface::GetHp(void)
{
	return hp_;
}
