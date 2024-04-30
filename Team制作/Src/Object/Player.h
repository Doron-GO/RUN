#pragma once
#include <map>
#include <list>
#include <DxLib.h>
#include "ActorBase.h"
#include "../Manager/Camera.h"
#include"../Manager/EffectManager.h"
#include "../Input/Input.h"
#include"Common/CheckPoint.h"
#include<memory>
#include<vector>
class AnimationController;
class Collider;
class Capsule;
class Camera;
class UserInterface;

class Player : public ActorBase
{
public:

	// スピード
	static constexpr float SPEED_MOVE = 15.0f;
	static constexpr float SPEED_MOVE_X = 10.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	// ジャンプ力
	static constexpr float POW_JUMP = 35.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 1.0f;

	static constexpr VECTOR SMOKE_POS = { 0.0f, 0.0f, 0.0f };


	static constexpr VECTOR HIT_POS = { 0.0f, 60.0f, 0.0f };

	static constexpr int HP_MAX = 3;//HPの最大値

	// 状態
	enum class P_NUM
	{
		MIN,
		P1,
		P2
	};
	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		CORNER,
		DEAD,
		VICTORY,
		DAMEGE,
		END
	};
	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		POSE
	};

	// コンストラクタ
	Player(Camera& camera,std::vector<CheckPoint> checkPoint,int plyernum);

	// デストラクタ
	~Player();

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider& collider);
	void ClearCollider(void);

	//位置指定
	void SetPos(VECTOR pos);

	const P_NUM GetPlayerNum()const;

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const;
	const P_NUM playerNum_;

	//デバッグ用関数
	bool GetColisionCheck();//当たり判定チェックを取得
	bool GetVictoryCheck();//勝利チェックを取得

	int GetHp(void);

private:

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;

	// 状態管理
	STATE state_;

	// 移動スピード
	float speed_;

	float moveX_ = 0.0f;

	// 移動方向
	VECTOR moveDir_;

	//左右方向
	VECTOR xDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプ判定
	bool isJump_;

	// ジャンプの入力受付時間
	float stepJump_;

	// 衝突判定に用いられるコライダ
	std::vector<Collider> colliders_;
	std::shared_ptr< Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 丸影
	int imgShadow_;

	//自分がステージのどこに立っているかを表すフレーム番号
	int stageFrame_;

	VECTOR initPos_;

	//ポーズフラグ
	bool poseFlag_;

	//HP
	int hp_;

	//アニメーションの初期化
	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// 更新ステップ{
	void UpdateNone(void);
	void UpdatePlay();
	void UpdateCurve();
	//}
	
	
	// 描画系{
	void DrawShadow(void);
	//}
	
	// 操作系{
	void ProcessMove();
	void ProcessJump();
	void ProcessPose();
	void ApproachZero(float& variable);
	//左右移動
	bool MoveHorizontally();
	//矢印右ボタンが押されているか
	bool IsPrassedRight();
	//矢印左ボタンが押されているか
	bool IsPrassedLeft();
	//}

	//カーブ関係
	void Corner();
	void Curve(CheckPoint::CURVE curve);

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	//チェックポイント系
	bool IsCheckPoint(CheckPoint c);

	// 移動量の計算
	void CalcGravityPow(void);

	// 着地モーション終了
	bool IsEndLanding(void);

	Input input_;
	Camera& camera_;
	std::vector<CheckPoint> checkPoints_;
	std::list<int>const stageModelIds_;

	std::unique_ptr<UserInterface>UI_;

	//デバッグ用
	bool colisionCheck;//当たり判定チェック(一ポリゴンでもあたるとtrue)
	bool PrevcolisionCheck;//前回の当たり判定チェック結果

	bool victoryCheck;//勝利チェック
	bool PrevvictoryCheck;//前回の勝利チェック結果


	//エフェクト関連
	typedef struct
	{
		VECTOR pos_;
		VECTOR rot_;
		VECTOR scl_;

	}EffStat;

	//エフェクトの情報
	std::map<EffectManager::EFF, EffStat> mapEff;
	//エフェクトの初期化
	void InitEffect(void);
	//エフェクトの更新
	void UpdateEffect(void);

	void UpdateHitEffect();

	void UpdatePoseEffect(void);
};
