#include "GameOverScene.h"
#include "VictoryScene.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/AnimationController.h"
#include"../Utility/AsoUtility.h"
#include "../Application.h"
#include<DxLib.h>
#include"../Input/Input.h"


GameOverScene::GameOverScene(void)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	imgVback = ResourceManager::GetInstance().Load(ResourceManager::SRC::VICTORY_BACK).handleId_;
	imgCongratulation = ResourceManager::GetInstance().Load(ResourceManager::SRC::OVER).handleId_;
	pleaseA_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::A).handleId_;

	victoryWaveId_ = soundMana_.Load(SoundManager::SOD::VICTORY).handleId_;//Ÿ—˜‰¹
	StopSoundMem(victoryWaveId_);
	soundFlag = false;//Ÿ—˜‰¹‚ª–Â‚Á‚½‚©

	//ŸÒ
	WinnerModel.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER).handleId_);
	WinnerModel.scl = { 2.0f, 2.0f, 2.0f };
	WinnerModel.pos = { static_cast<float>(Application::SCREEN_SIZE_X / 2),
						static_cast<float>(Application::SCREEN_SIZE_Y / 2 - 300),
							0.0f };

	WinnerModel.quaRot = Quaternion();
	WinnerModel.quaRotLocal = Quaternion();
	WinnerModel.Update();

	anime = new AnimationController(WinnerModel.modelId);
	anime->Add((int)ANIM_TYPE::POSE, "Data/Model/Player/NewAnimation/Salute.mv1", 20.0f);

	anime->StartPlay((int)ANIM_TYPE::POSE);

}

void GameOverScene::Update(Input& input)
{
	if (!CheckSoundMem(victoryWaveId_) && !soundFlag)
	{
		PlaySoundMem(victoryWaveId_, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(255, victoryWaveId_);
		soundFlag = true;
	}

	anime->Update();

	// ƒV[ƒ“‘JˆÚ
	auto& ins = InputManager::GetInstance();
	if (input.IsTriggerd("jump"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SoundStop();//‰¹‚ğ’â~‚·‚é
	}

}

void GameOverScene::Draw(void)
{
	DrawGraph(0, 0, imgVback, true);
	DrawGraph(150, 0, imgCongratulation, true);
	DrawGraph(350, 500, pleaseA_, true);

	MV1DrawModel(WinnerModel.modelId);

}
