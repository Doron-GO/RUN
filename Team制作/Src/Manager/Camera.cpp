#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	//mode_ = MODE::NONE;
	pos_ = AsoUtility::VECTOR_ZERO;
	pos_ = {0.0f, 0.0f, 10000.0f};
	
	targetPos_ = AsoUtility::VECTOR_ZERO;
	velocity_ = AsoUtility::VECTOR_ZERO;
	velocity1_ = AsoUtility::VECTOR_ZERO;
	velocity2_ = AsoUtility::VECTOR_ZERO;
	//stepTime_ = 0.0f;
	//followTransform_ = nullptr;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawStartFollowSpring();
		break;
	case Camera::MODE::FOLLOW_SPRING_1:
		SetBeforeDrawStartFollowSpring1();
		break;
	case Camera::MODE::FOLLOW_SPRING_2:
		SetBeforeDrawStartFollowSpring2();
		break;
	}

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
}

void Camera::SetFollow(const Transform* target)
{
	target_ = target;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::FOLLOW_SPRING_1:
		break;
	case Camera::MODE::FOLLOW_SPRING_2:
		pos_ = { 0.0f, 0.0f, 9400.0f };
		targetPos_ = AsoUtility::VECTOR_ZERO;

		break;
	}

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;
	// �����_
	targetPos_ = AsoUtility::VECTOR_ZERO;
	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{
	//// ������̈ʒu
	VECTOR pos = target_->pos;

	//// �d�͂̕�������ɏ]��
	//// ���ʂ���ݒ肳�ꂽY�����A��]������
	//rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	//// ���ʂ���ݒ肳�ꂽX�����A��]������
	//rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis({ -100.0f,100.0f,300.0f });
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = rot_.PosAxis({ 100.0f,200.0f,-300.0f });
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::ProcessRot(void)
{

	auto& ins = InputManager::GetInstance();

	float movePow = 5.0f;

	// �J������]
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		// �E��]
		angles_.y += AsoUtility::Deg2RadF(1.0f);
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		// ����]
		angles_.y += AsoUtility::Deg2RadF(-1.0f);
	}

	// ���]
	if (ins.IsNew(KEY_INPUT_UP))
	{
		angles_.x += AsoUtility::Deg2RadF(1.0f);
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}

	// ����]
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		angles_.x += AsoUtility::Deg2RadF(-1.0f);
		if (angles_.x < -LIMIT_X_DW_RAD)
		{
			angles_.x = -LIMIT_X_DW_RAD;
		}
	}

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// �������Ȃ�
}

void Camera::SetBeforeDrawFollow(void)
{

	// �J��������
	//ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollow();

}

void Camera::SetBeforeDrawStartFollowSpring(void)
{
	//�^�[�Q�b�g�i�v���C���[�j�̏��
	VECTOR targetPos = target_->pos;
	Quaternion targetRot = target_->quaRot;

	//�J�������W
	VECTOR relativeCameraPos = targetRot.PosAxis(LOCAL_F2C_POS);

	//�΂˂̒萔�i�Œ�l�ړ��\��j
	float SPRING_POW = 30.0f;

	//���� (��R)
	float attenuate = 1.0f * sqrtf(SPRING_POW);

	//�i�s����
	auto stepTime = SceneManager::GetInstance().GetDeltaTime();

	//���z�̈ʒu
	VECTOR idealPos = VAdd(targetPos, relativeCameraPos);

	//���z�Ǝ��ۂ̈ʒu����
	VECTOR difference = VSub(pos_, idealPos);

	//�́�-�o�l�̋����@�~�@�o�l�̐L�� - ��R�@�~�@�J�����̑��x
	VECTOR force = VScale(difference, -SPRING_POW);		//-�o�l�̋����~�o�l�̐L��
	force = VSub(force, VScale(velocity_, attenuate));	//�J�����̑��x�~��R

	//���x�X�V
	velocity_ = VAdd(velocity_, VScale(force, stepTime));

	//�J�����̈ʒu�X�V
	pos_ = VAdd(pos_, VScale(velocity_, stepTime));

	//�����_
	VECTOR localTargetPos = targetRot.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localTargetPos);

}

void Camera::SetBeforeDrawStartFollowSpring1(void)
{
	//�^�[�Q�b�g�i�v���C���[�j�̏��
	VECTOR targetPos = target_->pos;
	Quaternion targetRot = target_->quaRot;

	//�J�������W
	VECTOR relativeCameraPos = targetRot.PosAxis(LOCAL_F2C_POS);

	//�΂˂̒萔�i�Œ�l�ړ��\��j
	float SPRING_POW = 30.0f;

	//���� (��R)
	float attenuate = 1.0f * sqrtf(SPRING_POW);

	//�i�s����
	auto stepTime = SceneManager::GetInstance().GetDeltaTime();

	//���z�̈ʒu
	VECTOR idealPos = VAdd(targetPos, relativeCameraPos);

	//���z�Ǝ��ۂ̈ʒu����
	VECTOR difference = VSub(pos_, idealPos);

	//�́�-�o�l�̋����@�~�@�o�l�̐L�� - ��R�@�~�@�J�����̑��x
	VECTOR force = VScale(difference, -SPRING_POW);		//-�o�l�̋����~�o�l�̐L��
	force = VSub(force, VScale(velocity1_, attenuate));	//�J�����̑��x�~��R

	//���x�X�V
	velocity1_ = VAdd(velocity1_, VScale(force, stepTime));

	//�J�����̈ʒu�X�V
	pos_ = VAdd(pos_, VScale(velocity1_, stepTime));

	//�����_
	VECTOR localTargetPos = targetRot.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localTargetPos);
}

void Camera::SetBeforeDrawStartFollowSpring2(void)
{
	//�^�[�Q�b�g�i�v���C���[�j�̏��
	VECTOR targetPos = target_->pos;
	Quaternion targetRot = target_->quaRot;

	//�J�������W
	VECTOR relativeCameraPos = targetRot.PosAxis(LOCAL_F2C_POS);

	//�΂˂̒萔�i�Œ�l�ړ��\��j
	float SPRING_POW = 30.0f;

	//���� (��R)
	float attenuate = 1.0f * sqrtf(SPRING_POW);

	//�i�s����
	auto stepTime = SceneManager::GetInstance().GetDeltaTime();

	//���z�̈ʒu
	VECTOR idealPos = VAdd(targetPos, relativeCameraPos);

	//���z�Ǝ��ۂ̈ʒu����
	VECTOR difference = VSub(pos_, idealPos);

	//�́�-�o�l�̋����@�~�@�o�l�̐L�� - ��R�@�~�@�J�����̑��x
	VECTOR force = VScale(difference, -SPRING_POW);		//-�o�l�̋����~�o�l�̐L��
	force = VSub(force, VScale(velocity2_, attenuate));	//�J�����̑��x�~��R

	//���x�X�V
	velocity2_ = VAdd(velocity2_, VScale(force, stepTime));

	//�J�����̈ʒu�X�V
	pos_ = VAdd(pos_, VScale(velocity2_, stepTime));

	//�����_
	VECTOR localTargetPos = targetRot.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localTargetPos);
}
