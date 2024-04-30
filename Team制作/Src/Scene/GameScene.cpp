#include <cmath>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include"../Input/Input.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include"../Object/Player.h"
#include"../Object/Stage/Stage.h"
#include"../Object/SkyDome.h"
#include"../Manager/ObstacleManager.h"

#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//������Get�ŖႢ����
	playerNum_ = 2;
	//player_ = std::make_unique<Player>(SceneManager::GetInstance().GetCamera());

	float skyDomeSize = 300.0f;
	skyDome_ = std::make_unique<SkyDome>(VECTOR{ skyDomeSize ,skyDomeSize ,skyDomeSize });
	skyDome_->Init();

	for (int num = 0; num < playerNum_; num++)
	{
		stages_.push_back(std::make_unique<Stage>(num+1));
	}
	obstacleManager_ = std::make_unique<ObstacleManager>();
	//��ʕ���
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));
	screenPos_.emplace_back(Vector2(0, 0));
	screenPos_.emplace_back(Vector2(Application::SCREEN_SIZE_X / 2, 0));
	vecPos.emplace_back(VECTOR{ 0.0f, 0.0f, 0.0f });
	vecPos.emplace_back(VECTOR{ 0.0f, 0.0f, 0.0f });
	vecPad.emplace_back(InputManager::JOYPAD_NO::PAD1);
	vecPad.emplace_back(InputManager::JOYPAD_NO::PAD2);

	for (int num = 0; num < playerNum_; num++)
	{
		cameras_.push_back(std::make_unique<Camera>());
		players_ .push_back(std::make_unique<Player>(*cameras_[num],stages_[num]->GetCheckPoint(), num+1));
		if (Player::P_NUM::P1 == players_[num]->GetPlayerNum())
		{
			cameras_[num]->ChangeMode(Camera::MODE::FOLLOW_SPRING_1);
		}
		if (Player::P_NUM::P2 == players_[num]->GetPlayerNum())
		{
			cameras_[num]->ChangeMode(Camera::MODE::FOLLOW_SPRING_2);
		}
		players_[num]->SetPos(vecPos[num]);
		players_[num]->Init();
		cameras_[num]->SetFollow(&players_[num]->GetTransform());
	}
	AddCollider();
	AddObstacleCollider();
	//SceneManager::GetInstance().GetCamera().SetFollow(&player_->GetTransform());
	//SceneManager::GetInstance().GetCamera().ChangeMode((Camera::MODE::FOLLOW_SPRING));
	//player_->Init();
	
	//�T�E���h
	//�f�o�b�O�p::������̂ŁA���ʒ��ӂł��I�I�I�I
	sodBattId_ = soundMana_.Load(SoundManager::SOD::BATTLE_BGM).handleId_;
	StopMusicMem(sodBattId_);

	colisionWaveId_ = soundMana_.Load(SoundManager::SOD::COLL).handleId_;//�����蔻��̌��ʉ�
	victoryWaveId_ = soundMana_.Load(SoundManager::SOD::VICTORY).handleId_;//������

	//�T�E���h�n�̍X�V
	SoundPlay();//�����𖞂��������Đ�
}

void GameScene::Update(Input& input)
{
	//�T�E���h�n�̍X�V
	SoundPlay();//�����𖞂��������Đ�

	skyDome_->Update(input);

	obstacleManager_->Update(input);
	//�J�����̐����񂵂܂�
	for (auto& camera : cameras_)
	{
		camera->Update();
	}

	//�v���C���[�̐����񂵂܂�
	for (auto& player : players_)
	{
		player->Update(input);
		if (player->GetVictoryCheck())
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::VICTORY);
			SoundStop();//�����~����
			return;
		}


		if (player->GetHp() <= 0)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
			SoundStop();//�����~����
		}

	}

	if (CollisionPlayerToCar())
	{
		//���������炱���ɗ���
	}

	//player_->Update();
	// �V�[���J��
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SoundStop();//�����~����
	}
	for (auto& stage : stages_)
	{
		stage->Update(input);
	}
	//player_->Update(input);
}

void GameScene::Draw(void)
{
	for (int i = 0; i < playerNum_; i++)
	{
		SetDrawScreen(cameraScreens_[i]);

		// ��ʂ�������
		ClearDrawScreen();

		cameras_[i]->SetBeforeDraw();

		DrawGame(i);

		//�ǉ�
		// �����ŃX�N���[������̃G�t�F�N�g�̍X�V�E�`����s��
		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();
		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();
	}
	SetDrawScreen(DX_SCREEN_BACK);
	// ��ʂ�������
	ClearDrawScreen();

	for (int i = 0; i < playerNum_; i++)
	{
		DrawGraph(screenPos_[i].x, screenPos_[i].y, cameraScreens_[i], false);//���ߏ����͌�Ō�����
	}
}

void GameScene::Release(void)
{

}

void GameScene::DrawGame(int num)
{
	//skyDome_->Draw();
	players_[num]->Draw();
	stages_[num]->Draw();
	obstacleManager_->Draw();

	//for (int i = 0; i < playerNum_; i++)
	//{
	//	players_[i]->Draw();
	//	stages_[i]->Draw();
	//	obstacleManager_->Draw();
	//}

}

bool GameScene::CollisionPlayerToCar(void)
{
	bool result = false;

	if (AsoUtility::IsHitCapsuleToSphere(playerCupsuleBottomPos_, playerCupsuleTopPos_, playerRadius_, carCupsuleTopPos_, carRadius_))
	{
		result = true;
	}
	if (AsoUtility::IsHitCapsuleToSphere(playerCupsuleBottomPos_, playerCupsuleTopPos_, playerRadius_, carCupsuleBottomPos_, carRadius_))
	{
		result = true;
	}

	return result;
}

void GameScene::AddCollider(void)
{
	//�v���C���[�̐����񂵂܂�
	for (auto& player : players_)
	{
		for (auto stage : stages_[((int)player->GetPlayerNum()) - 1]->Getclliders())
		{
			player->AddCollider(stage);
		}		
	}
}

void GameScene::AddObstacleCollider(void)
{
	//�v���C���[�̐����񂵂܂�
	for (auto& player : players_)
	{
		for (auto& obstacles : obstacleManager_->GetObstacleMap())
		{
			if (static_cast<int>(player->GetPlayerNum())== static_cast<int>(obstacles.first));
			{
				for (auto& obsacle : obstacles.second)
				{
					player->AddCollider(*obsacle->GetTransform().collider);
				}
			}
		}
	}
}

//�f�o�b�O�p�֐�

//�����t���̌��ʉ��E���y���Đ�
void GameScene::SoundPlay()
{
	for (int num = 0; num < playerNum_; num++)
	{
		if (players_[num]->GetColisionCheck())
		{
			PlaySoundMem(colisionWaveId_, DX_PLAYTYPE_BACK);
			//	VolumeRegulation();//�{�����[���𒲐�
		
		}
		//if (players_[num]->GetVictoryCheck())
		//{
		//	StopMusicMem(sodBattId_);
		//	PlaySoundMem(victoryWaveId_, DX_PLAYTYPE_BACK);
		//	VolumeRegulation();//�{�����[���𒲐�
		//}
	}

	//BGM�����Ă��Ȃ��Ȃ�Ȃ��v���C
	if (!CheckMusicMem(sodBattId_))
	{
		PlayMusicMem(sodBattId_, DX_PLAYTYPE_BACK);//�o�b�N�O���E���h�ōĐ�
		VolumeRegulation();
	}
}

//�����~����
void GameScene::SoundStop()
{
	//�����蔻�莞�̌��ʉ�
	if (CheckSoundMem(colisionWaveId_))
	{
		StopSoundMem(colisionWaveId_);
	}
	//������
	if (CheckSoundMem(victoryWaveId_))
	{
		StopSoundMem(victoryWaveId_);
	}
	//BGM�����Ă���Ȃ�Ȃ��~
	if (CheckMusicMem(sodBattId_))
	{
		StopMusicMem(sodBattId_);
	}
}

//���ʒ���
void GameScene::VolumeRegulation()
{
	ChangeVolumeSoundMem(230, sodBattId_);
}