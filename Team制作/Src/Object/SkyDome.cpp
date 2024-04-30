#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "SkyDome.h"

SkyDome::SkyDome(VECTOR size)
{

	state_ = STATE::NONE;
	size_ = size;

}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
	transform_.scl = size_;
	transform_.pos = { 0.0f,0.0f,0.0f};
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(-90.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// Z�o�b�t�@����(�˂������΍�)
	//MV1SetUseZBuffer(transform_.modelId, false);
	//MV1SetWriteZBuffer(transform_.modelId, false);

	// ��ԑJ��
	ChangeState(STATE::STAY);


}

void SkyDome::Update(Input& input)
{

	// �X�V�X�e�b�v
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		UpdateNone();
		break;
	case SkyDome::STATE::STAY:
		UpdateStay();
		break;
	}

	//transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::SetFollow(const Transform* target)
{
	target_ = target;
}

void SkyDome::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		ChangeStateNone();
		break;
	case SkyDome::STATE::STAY:
		ChangeStateStay();
		break;
	}

}

void SkyDome::ChangeStateNone(void)
{
}

void SkyDome::ChangeStateStay(void)
{
}


void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateStay(void)
{
}
