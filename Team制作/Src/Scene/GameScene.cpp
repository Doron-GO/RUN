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
	//ここはGetで貰いたい
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
	//画面分割
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
	
	//サウンド
	//デバッグ用::音が鳴るので、音量注意です！！！！
	sodBattId_ = soundMana_.Load(SoundManager::SOD::BATTLE_BGM).handleId_;
	StopMusicMem(sodBattId_);

	colisionWaveId_ = soundMana_.Load(SoundManager::SOD::COLL).handleId_;//当たり判定の効果音
	victoryWaveId_ = soundMana_.Load(SoundManager::SOD::VICTORY).handleId_;//勝利音

	//サウンド系の更新
	SoundPlay();//条件を満たす音を再生
}

void GameScene::Update(Input& input)
{
	//サウンド系の更新
	SoundPlay();//条件を満たす音を再生

	skyDome_->Update(input);

	obstacleManager_->Update(input);
	//カメラの数分回します
	for (auto& camera : cameras_)
	{
		camera->Update();
	}

	//プレイヤーの数分回します
	for (auto& player : players_)
	{
		player->Update(input);
		if (player->GetVictoryCheck())
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::VICTORY);
			SoundStop();//音を停止する
			return;
		}


		if (player->GetHp() <= 0)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::OVER);
			SoundStop();//音を停止する
		}

	}

	if (CollisionPlayerToCar())
	{
		//当たったらここに来る
	}

	//player_->Update();
	// シーン遷移
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SoundStop();//音を停止する
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

		// 画面を初期化
		ClearDrawScreen();

		cameras_[i]->SetBeforeDraw();

		DrawGame(i);

		//追加
		// ここでスクリーン二つ分のエフェクトの更新・描画を行う
		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();
		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();
	}
	SetDrawScreen(DX_SCREEN_BACK);
	// 画面を初期化
	ClearDrawScreen();

	for (int i = 0; i < playerNum_; i++)
	{
		DrawGraph(screenPos_[i].x, screenPos_[i].y, cameraScreens_[i], false);//透過処理は後で見直す
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
	//プレイヤーの数分回します
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
	//プレイヤーの数分回します
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

//デバッグ用関数

//条件付きの効果音・音楽を再生
void GameScene::SoundPlay()
{
	for (int num = 0; num < playerNum_; num++)
	{
		if (players_[num]->GetColisionCheck())
		{
			PlaySoundMem(colisionWaveId_, DX_PLAYTYPE_BACK);
			//	VolumeRegulation();//ボリュームを調節
		
		}
		//if (players_[num]->GetVictoryCheck())
		//{
		//	StopMusicMem(sodBattId_);
		//	PlaySoundMem(victoryWaveId_, DX_PLAYTYPE_BACK);
		//	VolumeRegulation();//ボリュームを調節
		//}
	}

	//BGMが鳴っていないなら曲をプレイ
	if (!CheckMusicMem(sodBattId_))
	{
		PlayMusicMem(sodBattId_, DX_PLAYTYPE_BACK);//バックグラウンドで再生
		VolumeRegulation();
	}
}

//音を停止する
void GameScene::SoundStop()
{
	//当たり判定時の効果音
	if (CheckSoundMem(colisionWaveId_))
	{
		StopSoundMem(colisionWaveId_);
	}
	//勝利音
	if (CheckSoundMem(victoryWaveId_))
	{
		StopSoundMem(victoryWaveId_);
	}
	//BGMが鳴っているなら曲を停止
	if (CheckMusicMem(sodBattId_))
	{
		StopMusicMem(sodBattId_);
	}
}

//音量調整
void GameScene::VolumeRegulation()
{
	ChangeVolumeSoundMem(230, sodBattId_);
}