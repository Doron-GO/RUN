#include <string>
#include <cmath>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include"../Manager/UserInterface.h"
#include "Player.h"

Player::Player(Camera& camera, std::vector<CheckPoint> checkPoint, int plyernum):
	camera_(camera), checkPoints_(checkPoint),playerNum_((P_NUM)plyernum)
{
	animationController_ = nullptr;
	state_ = STATE::NONE;
	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;
	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;
	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;
	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;
	imgShadow_ = -1;
	capsule_ = nullptr;
	hp_ = HP_MAX;
}

Player::~Player(void)
{
	// animationController_;
}

void Player::Init(void)
{
	float offset = 0;
	if (playerNum_== P_NUM::P2)
	{
		offset = OFFSETZ;
	}
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	initPos_.z += offset;
	transform_.pos = initPos_;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 35.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// �������
	ChangeState(STATE::PLAY);

	poseFlag_ = true;

	//UI
	UI_ = std::make_unique<UserInterface>(hp_);
	UI_->Init();

	//�f�o�b�O
	colisionCheck = false;//�����蔻��`�F�b�N(��|���S���ł��������true)
	PrevcolisionCheck = colisionCheck;//�O��̓����蔻��`�F�b�N����
									  //��xtrue�ɂȂ�ƁA�����蔻�肪���S�ɂȂ��Ȃ�܂ŕύX����Ȃ�
	victoryCheck = false;//�����`�F�b�N
	PrevvictoryCheck = victoryCheck;//�O��̏����`�F�b�N����
									//��xtrue�ɂȂ�ƁA�����蔻�肪���S�ɂȂ��Ȃ�܂ŕύX����Ȃ�

	//�G�t�F�N�g
	InitEffect();
}

//�G�t�F�N�g�̏�����
void Player::InitEffect()
{
	EffectManager::EFF effresorce;
	EffStat effstat;

	if (playerNum_ == P_NUM::P1)
	{
		//�v���C���[�X���[�N
		effresorce = EffectManager::EFF::RUN_SMOKE_01;
		EffectManager::GetInstance().Load(effresorce);
		effstat = { VAdd(transform_.pos, SMOKE_POS), Quaternion::ToEuler(transform_.quaRotLocal),{20.0f, 20.0f, 20.0f} };
		mapEff.emplace(effresorce, effstat);

		effresorce = EffectManager::EFF::HIT_EFFECT_01;
		EffectManager::GetInstance().Load(effresorce);
		effstat = { VAdd(transform_.pos, HIT_POS), Quaternion::ToEuler(transform_.quaRotLocal),{20.0f, 20.0f, 20.0f} };
		mapEff.emplace(effresorce, effstat);

		effresorce = EffectManager::EFF::POSE_EFFECT_01;
		EffectManager::GetInstance().Load(effresorce);
		effstat = { VAdd(transform_.pos, HIT_POS), Quaternion::ToEuler(transform_.quaRotLocal),{60.0f, 60.0f, 60.0f} };
		mapEff.emplace(effresorce, effstat);


		//�����ɃG�t�F�N�g���Đ�
		EffectManager::GetInstance().Play(EffectManager::EFF::RUN_SMOKE_01);
	}
	if (playerNum_ == P_NUM::P2)
	{
		//�v���C���[�X���[�N
		effresorce = EffectManager::EFF::RUN_SMOKE_02;
		EffectManager::GetInstance().Load(effresorce);
		effstat = { VAdd(transform_.pos, SMOKE_POS), Quaternion::ToEuler(transform_.quaRotLocal),{20.0f, 20.0f, 20.0f} };
		mapEff.emplace(effresorce, effstat);

		effresorce = EffectManager::EFF::HIT_EFFECT_02;
		EffectManager::GetInstance().Load(effresorce);
		effstat = { VAdd(transform_.pos, HIT_POS), Quaternion::ToEuler(transform_.quaRotLocal),{20.0f, 20.0f, 20.0f} };
		mapEff.emplace(effresorce, effstat);

		effresorce = EffectManager::EFF::POSE_EFFECT_02;
		EffectManager::GetInstance().Load(effresorce);
		effstat = { VAdd(transform_.pos, HIT_POS), Quaternion::ToEuler(transform_.quaRotLocal),{60.0f, 60.0f, 60.0f} };
		mapEff.emplace(effresorce, effstat);

		//�����ɃG�t�F�N�g���Đ�
		EffectManager::GetInstance().Play(EffectManager::EFF::RUN_SMOKE_02);
	}

}

void Player::Update(Input& input)
{

	input_.Update((int)playerNum_);
	// �X�V�X�e�b�v
	switch (state_)
	{
	case Player::STATE::NONE:
		UpdateNone();
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::CORNER:
		UpdateCurve();
		break;
	}



	// �A�j���[�V�����Đ�
	animationController_->Update();

	//UI�̍X�V
	UI_->Update();

	//�G�t�F�N�g�̍X�V
	UpdateEffect();

	// ���f������X�V
	transform_.Update();

}

//�G�t�F�N�g�̍X�V
void Player::UpdateEffect()
{
	//RunSmoke_01
	if (playerNum_ == P_NUM::P1)
	{
		EffectManager::EFF effresource = EffectManager::EFF::RUN_SMOKE_01;
		EffStat& effstat = mapEff.at(effresource);
		effstat.pos_ = VAdd(transform_.pos, SMOKE_POS);
		effstat.rot_ = Quaternion::ToEuler(transform_.quaRotLocal);
		EffectManager::GetInstance().SetStat(effresource, effstat.pos_, effstat.rot_, effstat.scl_, 0.5f);

		//�Đ����I����Ă���ꍇ������x�Đ�
		if (!EffectManager::GetInstance().IsCheckPlay(effresource))
		{
			EffectManager::GetInstance().Play(effresource);
		}
	}

	//RunSmoke_02
	if (playerNum_ == P_NUM::P2)
	{
		EffectManager::EFF effresource = EffectManager::EFF::RUN_SMOKE_02;
		EffStat& effstat = mapEff.at(effresource);
		effstat.pos_ = VAdd(transform_.pos, SMOKE_POS);
		effstat.rot_ = Quaternion::ToEuler(transform_.quaRotLocal);
		EffectManager::GetInstance().SetStat(effresource, effstat.pos_, effstat.rot_, effstat.scl_, 0.5f);

		//�Đ����I����Ă���ꍇ������x�Đ�
		if (!EffectManager::GetInstance().IsCheckPlay(effresource))
		{
			EffectManager::GetInstance().Play(effresource);
		}
	}

	//�v���C����Ă��Ȃ���Ή����N����Ȃ�
	UpdateHitEffect();//�Փ˃G�t�F�N�g�̍X�V
	UpdatePoseEffect();//�|�[�Y�G�t�F�N�g�̍X�V
}

void Player::UpdateHitEffect()
{
	VECTOR hitPos = transform_.pos;
	if (playerNum_ == P_NUM::P1)
	{
		EffectManager::EFF effresource = EffectManager::EFF::HIT_EFFECT_01;
		EffStat& effstate = mapEff.at(effresource);
		effstate.pos_ = VAdd(hitPos, HIT_POS);
		effstate.rot_ = Quaternion::ToEuler(transform_.quaRotLocal);
		EffectManager::GetInstance().SetStat(effresource, effstate.pos_, effstate.rot_, effstate.scl_, 0.5f);
	}
	if (playerNum_ == P_NUM::P2)
	{
		EffectManager::EFF effresource = EffectManager::EFF::HIT_EFFECT_02;
		EffStat& effstate = mapEff.at(effresource);
		effstate.pos_ = VAdd(hitPos, HIT_POS);
		effstate.rot_ = Quaternion::ToEuler(transform_.quaRotLocal);
		EffectManager::GetInstance().SetStat(effresource, effstate.pos_, effstate.rot_, effstate.scl_, 0.5f);
	}
}

void Player::UpdatePoseEffect(void)
{
	if (playerNum_ == P_NUM::P1)
	{
		EffectManager::EFF effresource = EffectManager::EFF::POSE_EFFECT_01;
		EffStat& effstate = mapEff.at(effresource);
		effstate.pos_ = transform_.pos;
		effstate.rot_ = Quaternion::ToEuler(transform_.quaRotLocal);
		EffectManager::GetInstance().SetStat(effresource, effstate.pos_, effstate.rot_, effstate.scl_, 0.4f);
	}
	if (playerNum_ == P_NUM::P2)
	{
		EffectManager::EFF effresource = EffectManager::EFF::POSE_EFFECT_02;
		EffStat& effstate = mapEff.at(effresource);
		effstate.pos_ = transform_.pos;
		effstate.rot_ = Quaternion::ToEuler(transform_.quaRotLocal);
		EffectManager::GetInstance().SetStat(effresource, effstate.pos_, effstate.rot_, effstate.scl_, 0.4f);
	}
}

void Player::Draw(void)
{

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �ۉe�`��
	DrawShadow();
	////DrawFormatString(0, 100, 0xffffff, "movePow_:x %f:y %f:z %f", movePow_.x, movePow_.y, movePow_.z);

	////if (playerNum_ == P_NUM::P2)
	////{
	////	DrawFormatString(0, 130, 0xffffff, "pos__:x %f:y %f:z %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	////}
	////capsule_->Draw();

	////auto a = MV1GetFrameName(colliders_[0].modelId_, stageFrame_);
	////DrawString(0, 180, a, 0xffffff);

	//UI�̕`��
	UI_->Draw();

}

void Player::AddCollider(Collider& collider)
{
	colliders_.push_back(collider);
}

void Player::ClearCollider(void)
{
	colliders_.clear();
}

void Player::SetPos(VECTOR pos)
{
	initPos_ = pos;
}

const Player::P_NUM Player::GetPlayerNum() const
{
	return playerNum_;
}

const std::weak_ptr<Capsule> Player::GetCapsule(void) const
{
	return capsule_;
}

void Player::InitAnimation(void)
{

	std::string path = "Data/Model/Player/NewAnimation/";
	animationController_ = std::make_unique< AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "HappyIdle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jumping.mv1", 50.0f);
	animationController_->Add((int)ANIM_TYPE::POSE, path + "Salute.mv1", 40.0f);

	animationController_->StartPlay((int)ANIM_TYPE::IDLE);

}

void Player::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case Player::STATE::NONE:
		ChangeStateNone();
		break;
	case Player::STATE::PLAY:
		ChangeStatePlay();
		break;
	}

}

void Player::ChangeStateNone(void)
{
}

void Player::ChangeStatePlay(void)
{
}

void Player::UpdateNone(void)
{
}

void Player::UpdatePlay()
{
	//Curve����
	Corner();

	// �ړ�����
	ProcessMove();

	// �W�����v����
	ProcessJump();

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	//�J�v�Z���̍X�V
	//�J�v�Z���̍��W���v���C���[��Ǐ]����悤�ɂ��܂���
	capsule_->Update();
	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;

	//�|�[�Y����
	ProcessPose();

}

void Player::UpdateCurve()
{

	ProcessMove();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;

}

void Player::DrawShadow(void)
{

	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3] = { VERTEX3D(), VERTEX3D(), VERTEX3D() };
	VECTOR SlideVec;
	int ModelHandle;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (const auto collider : colliders_)
	{

		// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��
		ModelHandle = collider.modelId_;

		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1,
			transform_.pos, VAdd(transform_.pos, { 0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f }), PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);

}

void Player::ProcessMove()
{

	// �ړ��ʂ��[��
	movePow_ = AsoUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = camera_.GetQuaRotOutX();

	// ��]�������p�x
	double rotRad = 0;

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	xDir_ = AsoUtility::VECTOR_ZERO;
	dir = transform_.GetForward();

	
	
	if (!MoveHorizontally())
	{
		ApproachZero(moveX_);
	}

	
	// �ړ�����
	speed_ = SPEED_MOVE;
	moveDir_ = VScale(dir, speed_);//�v���C���[�̌����Ă���x�N�g��
	xDir_ = VScale(xDir_, moveX_);//�v���C���[�̍��E�̃x�N�g��
	moveDir_ = VAdd(moveDir_, xDir_);//���̓������
	movePow_ = moveDir_;


	//// ��]����
	//SetGoalRotate(rotRad);

	if (!isJump_ && poseFlag_)
	{
		animationController_->Play(((int)ANIM_TYPE::FAST_RUN));
	}

}

void Player::ProcessJump()
{

	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	// �W�����v
	if (input_.IsTriggerd("jump") && (!isJump_))
	{

		animationController_->Play((int)ANIM_TYPE::JUMP, true, 14.0f, 25.0f);
		animationController_->SetEndLoop(23.0f, 25.0f, 3.0f);
		isJump_ = true;
	}
	if (isJump_)
	{
		stepJump_ += 0.07f;
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(AsoUtility::DIR_U, POW_JUMP);
		}
	}

	// �{�^���𗣂�����W�����v�͂ɉ��Z���Ȃ�
	//if (!isHit)
	//{
	//	stepJump_ = TIME_JUMP_IN;
	//}

}

void Player::ProcessPose()
{
	//P�{�^������������pose���Ƃ�
	if (CheckHitKey(KEY_INPUT_P)&&poseFlag_)
	{
		animationController_->Play(((int)ANIM_TYPE::POSE),false);
		Quaternion axisQua = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
		transform_.quaRotLocal = transform_.quaRotLocal.Mult(axisQua);
		//transform_.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
		//VECTOR rotPos = transform_.quaRot.PosAxis(transform_.pos);
		//transform_.pos = VAdd(transform_.pos, rotPos);
		poseFlag_ = false;

		if (playerNum_ == P_NUM::P1)
		{
			EffectManager::GetInstance().Play(EffectManager::EFF::POSE_EFFECT_01);
		}
		if (playerNum_ == P_NUM::P2)
		{
			EffectManager::GetInstance().Play(EffectManager::EFF::POSE_EFFECT_02);
		}
	}
	if (animationController_->IsEnd()&&!poseFlag_ )
	{
		poseFlag_ = true;
		Quaternion axisQua = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
		transform_.quaRotLocal = transform_.quaRotLocal.Mult(axisQua);

		//�|�[�Y���I�������^�C�~���O�ł��ז��Q�[�W�`���[�W
		//�f�o�b�O::�`���[�W���킩��₷���悤�ɁA�����đ傫�߂̐����`���[�W���Ă��܂�
		UI_->AddObstacleMeter(50.0f);

		if (playerNum_ == P_NUM::P1)
		{
			EffectManager::GetInstance().Stop(EffectManager::EFF::POSE_EFFECT_01);
		}
		if (playerNum_ == P_NUM::P2)
		{
			EffectManager::GetInstance().Stop(EffectManager::EFF::POSE_EFFECT_02);
		}
	}

}

void Player::ApproachZero(float& variable)
{
	if (!input_.IsPrassed("right") && !input_.IsPrassed("left"))
	{

		if (variable > 0.0f)
		{
			variable += -3.0f;
		}
		if (variable < 0.0f)
		{
			variable += 3.0f;
		}
	}
}

bool Player::MoveHorizontally()
{
	bool flag = false;
	// �v���C���[�̉E���ֈړ�������
	if (!(IsPrassedLeft())&&IsPrassedRight())
	{
		if (moveX_ <15)
		{
			moveX_ += 3.0f;
			xDir_ = transform_.GetRight();
			flag= true;
		}
		else
		{
			moveX_ = 15.0f;
			xDir_ = transform_.GetRight();
			flag = true;
		}
	}
	// �v���C���[�̍����ֈړ�������
	 if (!(IsPrassedRight())&&IsPrassedLeft())
	{
		if (moveX_ < 15)
		{
			moveX_ += 3.0f;
			xDir_ = transform_.GetLeft();
			flag = true;
		}
		else
		{
			moveX_ =15.0f;
			xDir_ = transform_.GetLeft();
			flag = true;
		}
	}
	return flag;
}

bool Player::IsPrassedRight()
{
	return input_.IsPrassed("right");
}

bool Player::IsPrassedLeft()
{
	return input_.IsPrassed("left");
}

void Player::Corner()
{
	for (auto& checkPoint : checkPoints_)
	{
		if (!checkPoint.passageFlag_)
		{
			if (IsCheckPoint(checkPoint))
			{
				checkPoint.passageFlag_ = true;
				Curve(checkPoint.curve_);

			}
		}
	}

}

void Player::Curve(CheckPoint::CURVE curve)
{
	double rotRad = 0;
	switch (curve)
	{
	case CheckPoint::CURVE::RIGHT:
		rotRad = AsoUtility::Deg2RadD(90);
		SetGoalRotate(rotRad);

		break;
	case CheckPoint::CURVE::LEFT:
		 rotRad = AsoUtility::Deg2RadD(-90);
		SetGoalRotate(rotRad);

		break;
	}
}

void Player::SetGoalRotate(double rotRad)
{

	VECTOR cameraRot = camera_.GetAngles();
	VECTOR forward = transform_.rot;
	//Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);
	Quaternion axis = Quaternion::AngleAxis((double)forward.y + rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;

}

void Player::Rotate(void)
{

	stepRotTime_ --;

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void Player::Collision(void)
{

	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;

}

void Player::CollisionGravity(void)
{

	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = 25.0f;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{
		if (!(c.type_== Collider::TYPE::STAGE))
		{
			continue;
		}
		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c.modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		//if (hit.HitFlag > 0)
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			stageFrame_ =  hit.FrameIndex;
			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			//�f�o�b�O
			//�S�[���ɓ����������ǂ���
			auto a = MV1GetFrameName(colliders_[0].modelId_, stageFrame_);
			TCHAR c[5] = _T("Goal");
			if (*c == *a)
			{
				victoryCheck = true;//��������
			}

			// �W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				animationController_->Play(((int)ANIM_TYPE::FAST_RUN));

			}

			isJump_ = false;
			stepJump_ = 0.0f;

		}
	}


	//�f�o�b�O
	//�S�[�����Ă���ꍇ�̗l�X�ȏ���
	if (victoryCheck && !PrevvictoryCheck)
	{
		//�O��S�[���̓����蔻�肪�Ȃ��ꍇ
		//���A���݂͓����蔻�肪����ꍇ
		PrevvictoryCheck = victoryCheck;//�X�V
	}
	else if (victoryCheck)
	{
		//�O��S�[���̓����蔻�肪�������ꍇ
		//���A���݂͓����蔻�肪����ꍇ
		//�����t���O��false�ɂ���
		victoryCheck = false;
	}
	else
	{
		//�O��S�[���̓����蔻�肪�������ꍇ
		//���A���݂͓����蔻�肪�Ȃ��ꍇ
		PrevvictoryCheck = victoryCheck;//�X�V
	}
}

void Player::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (const auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c.modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (c.type_ ==Collider::TYPE::STAGE)
				{
					if (pHit)
					{
						//colisionCheck = true;//�����蔻��`�F�b�N
						movedPos_ = VAdd(movedPos_, VScale(movePow_, -0.1f));
						float speed = SPEED_MOVE * 0.1f;
						movedPos_ = VAdd(movedPos_, VScale(transform_.GetForward(), speed));
						// �J�v�Z�����ړ�������
						trans.pos = movedPos_;
						trans.Update();
						//continue;
						break;
					}
				}
				else
				{

					colisionCheck = true;
				}
				break;
			}
		}
		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}

	//�����蔻�肪�����HP�����炷
	//��x����������I�u�W�F�N�g���痣���܂�HP�͌���Ȃ�
	if (colisionCheck && !PrevcolisionCheck)
	{
		//�O��I�u�W�F�N�g�̓����蔻�肪�Ȃ��ꍇ
		//���A���݂͓����蔻�肪����ꍇ
		//HP�����炷
		UI_->SubHp();
		if (playerNum_ == P_NUM::P1)
		{
			EffectManager::GetInstance().Play(EffectManager::EFF::HIT_EFFECT_01);
		}
		if (playerNum_ == P_NUM::P2)
		{
			EffectManager::GetInstance().Play(EffectManager::EFF::HIT_EFFECT_02);
		}

		PrevcolisionCheck = colisionCheck;
	}
	else if (colisionCheck)
	{
		//�O��I�u�W�F�N�g�̓����蔻�肪�������ꍇ
		//���A���݂͓����蔻�肪����ꍇ
		//�����t���O��false�ɂ���
		colisionCheck = false;
	}
	else
	{
		//�O��I�u�W�F�N�g�̓����蔻�肪�������ꍇ
		//���A���݂͓����蔻�肪�Ȃ��ꍇ
		PrevcolisionCheck = colisionCheck;
	}
}

bool Player::IsCheckPoint(CheckPoint c)
{
	auto& pos = transform_.pos;
	if ((pos.z<c.max_.z&& pos.z > c.min_.z)&&
		(pos.x<c.max_.x && pos.x > c.min_.x)&&
		(pos.x<c.max_.x && pos.x > c.min_.x))
	{
		return true;
	}
	return false;
}

void Player::CalcGravityPow(void)
{

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = 25.0f;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}

}

bool Player::IsEndLanding(void)
{

	bool ret = true;

	// �A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::FAST_RUN)
	{
		return ret;
	}

	// �A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;

}

//�����蔻��`�F�b�N���擾
bool Player::GetColisionCheck()
{
	return colisionCheck;
}

//�����`�F�b�N���擾
bool Player::GetVictoryCheck()
{
	return victoryCheck;
}

int Player::GetHp(void)
{
	return UI_->GetHp();
}
