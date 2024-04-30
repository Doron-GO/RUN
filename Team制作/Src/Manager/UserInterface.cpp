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
	//HP��1�ɂ�1�̃n�[�g�摜��\�����邱�Ƃŕ\��
	//���ז��Q�[�W
	//���ז��Q�[�W�͎c��c�ʂ��Q�[�W�ŕ\��
	ObsMet_ = 0;

	//�v���C���[HP�摜�̓ǂݍ���
	imgHp_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_HP).handleId_;
	//���ז��Q�[�W�摜
	imgObsMetCase_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OJAMAGAUGE_CASE).handleId_;
	imgObsMet_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OJAMAGAUGE).handleId_;
}

void UserInterface::Update(void)
{
}

//�v���C���[HP�̑���
void UserInterface::SubHp()
{
	//�Ăяo����HP��1���炷
	hp_--;

	//���肷���Ȃ��悤�ɂ���
	if (hp_ < 0)
	{
		hp_ = 0;
	}
}

//���ז��Q�[�W�̑���
void UserInterface::AddObstacleMeter(float add)
{
	//�Ăяo���Ƃ��ז��Q�[�W�����������₷
	ObsMet_ += add;

	//�ő�l�𒴂��Ȃ��悤�ɂ���
	if (ObsMet_ > OBSTACLE_METER_MAX)
	{
		ObsMet_ = OBSTACLE_METER_MAX;
	}
}

void UserInterface::SubObstacleMeter(float sub)
{
	//�Ăяo���Ƃ��ז��Q�[�W�����������炷
	ObsMet_ -= sub;

	//���肷���Ȃ��悤�ɂ���
	if (ObsMet_ < 0)
	{
		ObsMet_ = 0;
	}
}

//�`��
void UserInterface::Draw(void)
{
	//HP�̕`��
	//�摜�̑傫����64�~64�ł�
	DrawHp();

	//���ז��Q�[�W�̕`��
	DrawObstacleMeter();
}

void UserInterface::DrawHp()
{
	//HP�摜�̕`��
	for (int i = 0;i < hp_;i++) {
		DrawGraph(64 * i, 0, imgHp_, true);
	}
}

void UserInterface::DrawObstacleMeter(void)
{
	//���ז��Q�[�W�g
	DrawGraph(0, 80, imgObsMetCase_, true);

	//���ז��Q�[�W�̒��g
	//�c��Q�[�W�Œ������ϓ�����
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
