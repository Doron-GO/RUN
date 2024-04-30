#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"
#include "TitlePlayer.h"

TitlePlayer::TitlePlayer(void)
{
}

TitlePlayer::~TitlePlayer(void)
{
}

void TitlePlayer::Init(void)
{
	//プレイヤー初期化
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	//エフェクト初期化
	InitEffect();

	//アニメーション更新
	InitAnimaiton();



}

void TitlePlayer::InitEffect()
{

}

void TitlePlayer::InitAnimaiton(void)
{
	std::string path = "Data/Model/Player/NewAnimation/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "FastRun.mv1", 20.0f);

	animationController_->StartPlay((int)ANIM_TYPE::RUN);
}

void TitlePlayer::Update(Input& input)
{
	//プレイヤー更新
	transform_.Update();

	//エフェクト更新
	UpdateEffect();

	//アニメーション更新
	UpdateAnimation();

}

void TitlePlayer::UpdateEffect()
{
}

void TitlePlayer::UpdateAnimation(void)
{
	animationController_->Update();
}

void TitlePlayer::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}
