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
	//�v���C���[������
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	//�G�t�F�N�g������
	InitEffect();

	//�A�j���[�V�����X�V
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
	//�v���C���[�X�V
	transform_.Update();

	//�G�t�F�N�g�X�V
	UpdateEffect();

	//�A�j���[�V�����X�V
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
