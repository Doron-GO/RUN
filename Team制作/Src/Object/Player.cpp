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
	// 衝突チェック
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
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_.scl = AsoUtility::VECTOR_ONE;
	initPos_.z += offset;
	transform_.pos = initPos_;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// アニメーションの設定
	InitAnimation();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 35.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// 初期状態
	ChangeState(STATE::PLAY);

	poseFlag_ = true;

	//UI
	UI_ = std::make_unique<UserInterface>(hp_);
	UI_->Init();

	//デバッグ
	colisionCheck = false;//当たり判定チェック(一ポリゴンでもあたるとtrue)
	PrevcolisionCheck = colisionCheck;//前回の当たり判定チェック結果
									  //一度trueになると、当たり判定が完全になくなるまで変更されない
	victoryCheck = false;//勝利チェック
	PrevvictoryCheck = victoryCheck;//前回の勝利チェック結果
									//一度trueになると、当たり判定が完全になくなるまで変更されない

	//エフェクト
	InitEffect();
}

//エフェクトの初期化
void Player::InitEffect()
{
	EffectManager::EFF effresorce;
	EffStat effstat;

	if (playerNum_ == P_NUM::P1)
	{
		//プレイヤースモーク
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


		//初期にエフェクトを再生
		EffectManager::GetInstance().Play(EffectManager::EFF::RUN_SMOKE_01);
	}
	if (playerNum_ == P_NUM::P2)
	{
		//プレイヤースモーク
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

		//初期にエフェクトを再生
		EffectManager::GetInstance().Play(EffectManager::EFF::RUN_SMOKE_02);
	}

}

void Player::Update(Input& input)
{

	input_.Update((int)playerNum_);
	// 更新ステップ
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



	// アニメーション再生
	animationController_->Update();

	//UIの更新
	UI_->Update();

	//エフェクトの更新
	UpdateEffect();

	// モデル制御更新
	transform_.Update();

}

//エフェクトの更新
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

		//再生が終わっている場合もう一度再生
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

		//再生が終わっている場合もう一度再生
		if (!EffectManager::GetInstance().IsCheckPlay(effresource))
		{
			EffectManager::GetInstance().Play(effresource);
		}
	}

	//プレイされていなければ何も起こらない
	UpdateHitEffect();//衝突エフェクトの更新
	UpdatePoseEffect();//ポーズエフェクトの更新
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

	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// 丸影描画
	DrawShadow();
	////DrawFormatString(0, 100, 0xffffff, "movePow_:x %f:y %f:z %f", movePow_.x, movePow_.y, movePow_.z);

	////if (playerNum_ == P_NUM::P2)
	////{
	////	DrawFormatString(0, 130, 0xffffff, "pos__:x %f:y %f:z %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	////}
	////capsule_->Draw();

	////auto a = MV1GetFrameName(colliders_[0].modelId_, stageFrame_);
	////DrawString(0, 180, a, 0xffffff);

	//UIの描画
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

	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
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
	//Curve処理
	Corner();

	// 移動処理
	ProcessMove();

	// ジャンプ処理
	ProcessJump();

	// 移動方向に応じた回転
	Rotate();

	// 重力による移動量
	CalcGravityPow();

	//カプセルの更新
	//カプセルの座標がプレイヤーを追従するようにしました
	capsule_->Update();
	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = playerRotY_;

	//ポーズ処理
	ProcessPose();

}

void Player::UpdateCurve()
{

	ProcessMove();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
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

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// 影を落とすモデルの数だけ繰り返し
	for (const auto collider : colliders_)
	{

		// チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル
		ModelHandle = collider.modelId_;

		// プレイヤーの直下に存在する地面のポリゴンを取得
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1,
			transform_.pos, VAdd(transform_.pos, { 0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f }), PLAYER_SHADOW_SIZE);

		// 頂点データで変化が無い部分をセット
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// 球の直下に存在するポリゴンの数だけ繰り返し
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// ポリゴンの座標は地面ポリゴンの座標
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ちょっと持ち上げて重ならないようにする
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// ポリゴンの不透明度を設定する
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// 影ポリゴンを描画
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);

}

void Player::ProcessMove()
{

	// 移動量をゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = camera_.GetQuaRotOutX();

	// 回転したい角度
	double rotRad = 0;

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	xDir_ = AsoUtility::VECTOR_ZERO;
	dir = transform_.GetForward();

	
	
	if (!MoveHorizontally())
	{
		ApproachZero(moveX_);
	}

	
	// 移動処理
	speed_ = SPEED_MOVE;
	moveDir_ = VScale(dir, speed_);//プレイヤーの向いているベクトル
	xDir_ = VScale(xDir_, moveX_);//プレイヤーの左右のベクトル
	moveDir_ = VAdd(moveDir_, xDir_);//その二つを合成
	movePow_ = moveDir_;


	//// 回転処理
	//SetGoalRotate(rotRad);

	if (!isJump_ && poseFlag_)
	{
		animationController_->Play(((int)ANIM_TYPE::FAST_RUN));
	}

}

void Player::ProcessJump()
{

	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	// ジャンプ
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

	// ボタンを離したらジャンプ力に加算しない
	//if (!isHit)
	//{
	//	stepJump_ = TIME_JUMP_IN;
	//}

}

void Player::ProcessPose()
{
	//Pボタンを押したらposeをとる
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

		//ポーズが終了したタイミングでお邪魔ゲージチャージ
		//デバッグ::チャージがわかりやすいように、あえて大きめの数をチャージしています
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
	// プレイヤーの右側へ移動したい
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
	// プレイヤーの左側へ移動したい
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

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;

}

void Player::Rotate(void)
{

	stepRotTime_ --;

	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void Player::Collision(void)
{

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;

}

void Player::CollisionGravity(void)
{

	// ジャンプ量を加算
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// 重力の強さ
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
		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c.modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
		//if (hit.HitFlag > 0)
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			stageFrame_ =  hit.FrameIndex;
			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			//デバッグ
			//ゴールに当たったかどうか
			auto a = MV1GetFrameName(colliders_[0].modelId_, stageFrame_);
			TCHAR c[5] = _T("Goal");
			if (*c == *a)
			{
				victoryCheck = true;//当たった
			}

			// ジャンプリセット
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


	//デバッグ
	//ゴールしている場合の様々な処理
	if (victoryCheck && !PrevvictoryCheck)
	{
		//前回ゴールの当たり判定がない場合
		//かつ、現在は当たり判定がある場合
		PrevvictoryCheck = victoryCheck;//更新
	}
	else if (victoryCheck)
	{
		//前回ゴールの当たり判定があった場合
		//かつ、現在は当たり判定がある場合
		//勝利フラグをfalseにする
		victoryCheck = false;
	}
	else
	{
		//前回ゴールの当たり判定があった場合
		//かつ、現在は当たり判定がない場合
		PrevvictoryCheck = victoryCheck;//更新
	}
}

void Player::CollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
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
						//colisionCheck = true;//当たり判定チェック
						movedPos_ = VAdd(movedPos_, VScale(movePow_, -0.1f));
						float speed = SPEED_MOVE * 0.1f;
						movedPos_ = VAdd(movedPos_, VScale(transform_.GetForward(), speed));
						// カプセルを移動させる
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
		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	}

	//当たり判定があるとHPを減らす
	//一度当たったらオブジェクトから離れるまでHPは減らない
	if (colisionCheck && !PrevcolisionCheck)
	{
		//前回オブジェクトの当たり判定がない場合
		//かつ、現在は当たり判定がある場合
		//HPを減らす
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
		//前回オブジェクトの当たり判定があった場合
		//かつ、現在は当たり判定がある場合
		//勝利フラグをfalseにする
		colisionCheck = false;
	}
	else
	{
		//前回オブジェクトの当たり判定があった場合
		//かつ、現在は当たり判定がない場合
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

	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = 25.0f;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow_ = gravity;
	}

}

bool Player::IsEndLanding(void)
{

	bool ret = true;

	// アニメーションがジャンプではない
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::FAST_RUN)
	{
		return ret;
	}

	// アニメーションが終了しているか
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;

}

//当たり判定チェックを取得
bool Player::GetColisionCheck()
{
	return colisionCheck;
}

//勝利チェックを取得
bool Player::GetVictoryCheck()
{
	return victoryCheck;
}

int Player::GetHp(void)
{
	return UI_->GetHp();
}
