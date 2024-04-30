#pragma once
#include<DxLib.h>
class UserInterface
{
public:

	static constexpr int HP_MAX = 3;//HP�̍ő�l
	static constexpr float OBSTACLE_METER_MAX = 100.0f;//���ז��Q�[�W�̍ő�l

	//�R���X�g���N�^
	UserInterface(int hp);

	//�f�X�g���N�^
	~UserInterface(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//HP�̑���
	void SubHp();
	//���ז��Q�[�W�̑���
	void AddObstacleMeter(float add);
	void SubObstacleMeter(float sub);

	//UI�`��
	void DrawHp();
	void DrawObstacleMeter(void);
	void Release(void);

	int GetHp(void);

private:
	//UI
	//HP
	int hp_;
	//HP�̉摜
	int imgHp_;
	//���ז��Q�[�W
	float ObsMet_;
	//���ז��Q�[�W�摜(�Q�[�W�g�Ƃ��̒��g)
	int imgObsMetCase_;
	int imgObsMet_;

};

