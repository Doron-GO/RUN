#include "VictoryScene.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/AnimationController.h"
#include"../Utility/AsoUtility.h"
#include "../Application.h"
#include"../Input/Input.h"

#include<DxLib.h>

VictoryScene::VictoryScene(void)
{
	anime = nullptr;
}

VictoryScene::~VictoryScene(void)
{
}

void VictoryScene::Init(void)
{
	imgVback = ResourceManager::GetInstance().Load(ResourceManager::SRC::VICTORY_BACK).handleId_;
	imgCongratulation = ResourceManager::GetInstance().Load(ResourceManager::SRC::CONGRATULATIONS).handleId_;
	pleaseA_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::A).handleId_;

	victoryWaveId_ = soundMana_.Load(SoundManager::SOD::VICTORY).handleId_;//èüóòâπ
	StopSoundMem(victoryWaveId_);
	soundFlag = false;//èüóòâπÇ™ñ¬Ç¡ÇΩÇ©

	//èüé“
	WinnerModel.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER).handleId_);
	WinnerModel.scl = {2.0f, 2.0f, 2.0f};
	WinnerModel.pos = {static_cast<float>(Application::SCREEN_SIZE_X / 2), 
						static_cast<float>(Application::SCREEN_SIZE_Y / 2 - 300),
							0.0f};

	WinnerModel.quaRot = Quaternion();
	WinnerModel.quaRotLocal = Quaternion();
	WinnerModel.Update();

	anime = new AnimationController(WinnerModel.modelId);
	anime->Add((int)ANIM_TYPE::POSE,  "Data/Model/Player/NewAnimation/Salute.mv1", 20.0f);

	anime->StartPlay((int)ANIM_TYPE::POSE);
}

void VictoryScene::Update(Input& input)
{
	if (!CheckSoundMem(victoryWaveId_) && !soundFlag)
	{
		PlaySoundMem(victoryWaveId_, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(255,victoryWaveId_);
		soundFlag = true;
	}

	anime->Update();

	// ÉVÅ[ÉìëJà⁄
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		SoundStop();//âπÇí‚é~Ç∑ÇÈ
	}
}

void VictoryScene::Draw(void)
{
	DrawGraph(0, 0, imgVback, true);
	DrawGraph(150, 0, imgCongratulation, true);
	DrawGraph(350, 500, pleaseA_, true);

	MV1DrawModel(WinnerModel.modelId);
}


void VictoryScene::Release(void)
{


}
