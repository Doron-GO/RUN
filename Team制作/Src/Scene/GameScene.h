#pragma once
#include <vector>
#include<memory>
#include "../Manager/InputManager.h"
#include "SceneBase.h"

class Player;
class Stage;
class SkyDome;
class Camera;
class Vector2;
class ObstacleManager;

class GameScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;
	void Release(void) override;

	void DrawGame(int num);

	void SoundPlay()override;//�����t���̉����Đ�
	void SoundStop();//�����~����
	void VolumeRegulation()override;//���ʒ���



private:
	std::vector<std::unique_ptr<Player>> players_;
	std::vector<std::unique_ptr<Stage>> stages_;
	std::vector<std::unique_ptr<Camera>> cameras_;
	std::unique_ptr<Stage>stage_;
	std::unique_ptr<ObstacleManager>obstacleManager_;

	std::vector<VECTOR> vecPos;
	std::vector<InputManager::JOYPAD_NO> vecPad;
	//���
	std::vector<int> cameraScreens_;
	std::vector<Vector2> screenPos_;
	std::unique_ptr<SkyDome> skyDome_;


	bool CollisionPlayerToCar(void);
	void AddCollider(void);
	void AddObstacleCollider(void);

	//�v���C���[�N���X����Get���Ă���
	//�v���C���[��pos(��)
	VECTOR playerPos_;
	//�v���C���[�̃J�v�Z�����Pos(��)
	VECTOR playerCupsuleTopPos_;
	//�v���C���[�̃J�v�Z������Pos(��)
	VECTOR playerCupsuleBottomPos_;
	//�v���C���[�̔��a(��)
	float playerRadius_;


	//�Ԃ�pos(��)
	VECTOR carPos_;
	//�Ԃ̃J�v�Z�����Pos(��)
	VECTOR carCupsuleTopPos_;
	//�Ԃ̃J�v�Z������Pos(��)
	VECTOR carCupsuleBottomPos_;
	//�Ԃ̔��a
	float carRadius_;

	//�T�E���h�ϐ�
	int sodBattId_;//�o�g��BGM�̃n���h���ԍ����i�[

	//�v���C���[�l��
	int playerNum_;

	//�f�o�b�O�p
	int colisionWaveId_;//�����蔻��̌��ʉ�
	int victoryWaveId_;//������
};
