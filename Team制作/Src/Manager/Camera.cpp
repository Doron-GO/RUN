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

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
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

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
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

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
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

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;
	// 注視点
	targetPos_ = AsoUtility::VECTOR_ZERO;
	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{
	//// 同期先の位置
	VECTOR pos = target_->pos;

	//// 重力の方向制御に従う
	//// 正面から設定されたY軸分、回転させる
	//rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	//// 正面から設定されたX軸分、回転させる
	//rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis({ -100.0f,100.0f,300.0f });
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis({ 100.0f,200.0f,-300.0f });
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::ProcessRot(void)
{

	auto& ins = InputManager::GetInstance();

	float movePow = 5.0f;

	// カメラ回転
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		// 右回転
		angles_.y += AsoUtility::Deg2RadF(1.0f);
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		// 左回転
		angles_.y += AsoUtility::Deg2RadF(-1.0f);
	}

	// 上回転
	if (ins.IsNew(KEY_INPUT_UP))
	{
		angles_.x += AsoUtility::Deg2RadF(1.0f);
		if (angles_.x > LIMIT_X_UP_RAD)
		{
			angles_.x = LIMIT_X_UP_RAD;
		}
	}

	// 下回転
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
	// 何もしない
}

void Camera::SetBeforeDrawFollow(void)
{

	// カメラ操作
	//ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();

}

void Camera::SetBeforeDrawStartFollowSpring(void)
{
	//ターゲット（プレイヤー）の情報
	VECTOR targetPos = target_->pos;
	Quaternion targetRot = target_->quaRot;

	//カメラ座標
	VECTOR relativeCameraPos = targetRot.PosAxis(LOCAL_F2C_POS);

	//ばねの定数（固定値移動予定）
	float SPRING_POW = 30.0f;

	//減衰 (抵抗)
	float attenuate = 1.0f * sqrtf(SPRING_POW);

	//進行時間
	auto stepTime = SceneManager::GetInstance().GetDeltaTime();

	//理想の位置
	VECTOR idealPos = VAdd(targetPos, relativeCameraPos);

	//理想と実際の位置差分
	VECTOR difference = VSub(pos_, idealPos);

	//力＝-バネの強さ　×　バネの伸び - 抵抗　×　カメラの速度
	VECTOR force = VScale(difference, -SPRING_POW);		//-バネの強さ×バネの伸び
	force = VSub(force, VScale(velocity_, attenuate));	//カメラの速度×抵抗

	//速度更新
	velocity_ = VAdd(velocity_, VScale(force, stepTime));

	//カメラの位置更新
	pos_ = VAdd(pos_, VScale(velocity_, stepTime));

	//注視点
	VECTOR localTargetPos = targetRot.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localTargetPos);

}

void Camera::SetBeforeDrawStartFollowSpring1(void)
{
	//ターゲット（プレイヤー）の情報
	VECTOR targetPos = target_->pos;
	Quaternion targetRot = target_->quaRot;

	//カメラ座標
	VECTOR relativeCameraPos = targetRot.PosAxis(LOCAL_F2C_POS);

	//ばねの定数（固定値移動予定）
	float SPRING_POW = 30.0f;

	//減衰 (抵抗)
	float attenuate = 1.0f * sqrtf(SPRING_POW);

	//進行時間
	auto stepTime = SceneManager::GetInstance().GetDeltaTime();

	//理想の位置
	VECTOR idealPos = VAdd(targetPos, relativeCameraPos);

	//理想と実際の位置差分
	VECTOR difference = VSub(pos_, idealPos);

	//力＝-バネの強さ　×　バネの伸び - 抵抗　×　カメラの速度
	VECTOR force = VScale(difference, -SPRING_POW);		//-バネの強さ×バネの伸び
	force = VSub(force, VScale(velocity1_, attenuate));	//カメラの速度×抵抗

	//速度更新
	velocity1_ = VAdd(velocity1_, VScale(force, stepTime));

	//カメラの位置更新
	pos_ = VAdd(pos_, VScale(velocity1_, stepTime));

	//注視点
	VECTOR localTargetPos = targetRot.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localTargetPos);
}

void Camera::SetBeforeDrawStartFollowSpring2(void)
{
	//ターゲット（プレイヤー）の情報
	VECTOR targetPos = target_->pos;
	Quaternion targetRot = target_->quaRot;

	//カメラ座標
	VECTOR relativeCameraPos = targetRot.PosAxis(LOCAL_F2C_POS);

	//ばねの定数（固定値移動予定）
	float SPRING_POW = 30.0f;

	//減衰 (抵抗)
	float attenuate = 1.0f * sqrtf(SPRING_POW);

	//進行時間
	auto stepTime = SceneManager::GetInstance().GetDeltaTime();

	//理想の位置
	VECTOR idealPos = VAdd(targetPos, relativeCameraPos);

	//理想と実際の位置差分
	VECTOR difference = VSub(pos_, idealPos);

	//力＝-バネの強さ　×　バネの伸び - 抵抗　×　カメラの速度
	VECTOR force = VScale(difference, -SPRING_POW);		//-バネの強さ×バネの伸び
	force = VSub(force, VScale(velocity2_, attenuate));	//カメラの速度×抵抗

	//速度更新
	velocity2_ = VAdd(velocity2_, VScale(force, stepTime));

	//カメラの位置更新
	pos_ = VAdd(pos_, VScale(velocity2_, stepTime));

	//注視点
	VECTOR localTargetPos = targetRot.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localTargetPos);
}
