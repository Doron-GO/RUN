#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	//モデル本体
	modelId_ = modelId;
	//アニメーションタイプ
	playType_ = -1;
	//ループフラグ
	isLoop_ = false;
	//アニメーションストップフラグ
	isStop_ = false;
	//終わった後ある一部のアニメーションをループさせるときのアニメーションスピード
	endLoopSpeed_ = 0.0f;
	//どの部分～どの部分までかの指定
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;

	//前のアニメーション格納変数
	prevAttachIndex_ = -1;
	//現在のアニメーション格納変数
	currentAttachIndex_ = -1;

	rate_ = 0.0f;

	blendTime_ = 0.1f;

	animationFlag_ = true;

}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(int type, const std::string& path, float speed)
{
	Animation anim;
	//ロード
	anim.model = MV1LoadModel(path.c_str());
	//アニメーションタイプをインデックス（新たな要素を末尾に）に追加
	anim.animIndex = type;
	//スピード
	anim.speed = speed;

	//アニメーションタイプが入ってない場合？ここ確認
	if (animations_.count(type) == 0)
	{
		// 入れ替え
		animations_.emplace(type, anim);
	}
	else
	{
		// 追加
		animations_[type].model = anim.model;
		animations_[type].animIndex = anim.animIndex;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}

}

void AnimationController::StartPlay(int type, bool isLoop, bool isStop)
{
	// アニメーション種別を変更
	//スタートだけどここでの指定は二つのアニメーションが合成されないように
	playType_ = type;
	playAnim_ = animations_[type];


	// モデルにアニメーションを付ける
	int animIdx = 0;
	playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);

	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	// アニメーションループ
	isLoop_ = isLoop;

	// アニメーションしない
	isStop_ = isStop;

	animationFlag_ = false;

}

void AnimationController::Play(int type, bool isLoop,
	float startStep, float endStep, bool isStop, bool isForce)
{
	//アニメーションタイプが違ったら
	if (playType_ != type || isForce) {

		//アニメーションタイプが入っていたら
		if (playType_ != -1)
		{
			if (!animationFlag_)
			{
				//ここで前のアニメーションと現在のアニメーションをとる
				prevAttachIndex_ = playAnim_.attachNo;
				// アニメーション種別を変更
				playType_ = type;
				//プレイするアニメーションを指定する
				playAnim_ = animations_[type];
				//int animIdx = 0;
				// モデルからアニメーションを外す
				playAnim_.attachNo = MV1AttachAnim(modelId_, 0, playAnim_.model);
				animationFlag_ = true;
			}
		}

		// 初期化
		playAnim_.step = startStep;
		// アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			playAnim_.totalTime = endStep;
		}
		else
		{
			playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
		}

		// アニメーションループ
		isLoop_ = isLoop;

		// アニメーションしない
		isStop_ = isStop;

		stepEndLoopStart_ = -1.0f;
		stepEndLoopEnd_ = -1.0f;
	}
}

void AnimationController::Update(void)
{
	if (animationFlag_)
	{
		if (rate_ < 1.0f)
		{
			stepBlend_ += SceneManager::GetInstance().GetDeltaTime();
			MV1SetAttachAnimBlendRate(modelId_, prevAttachIndex_, 1.0f - rate_);
			MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, rate_);
			rate_ = stepBlend_ / blendTime_;
		}
		if (rate_ >= 1.0f)
		{
			MV1DetachAnim(modelId_, prevAttachIndex_);
			animationFlag_ = false;
			stepBlend_ = 0.0f;
			rate_ = 0.0f;
		}
	}
	else
	{
		// 経過時間の取得
		float deltaTime = SceneManager::GetInstance().GetDeltaTime();
		//ストップフラグがfalse
		if (!isStop_)
		{
			// 再生
			playAnim_.step += (deltaTime * playAnim_.speed);

			// アニメーション終了判定
			bool isEnd = false;
			//アニメーション進行時間がトータル時間を超えたら
			if (playAnim_.step > playAnim_.totalTime)
			{
				isEnd = true;
			}

			if (isEnd)
			{
				// アニメーションが終了したら
				if (isLoop_)
				{
					// ループ再生
					if (stepEndLoopStart_ > 0.0f)
					{
						// アニメーション終了後の指定フレーム再生
						playAnim_.speed = endLoopSpeed_;
					}
					else
					{
						// 通常のループ再生
						playAnim_.step = 0.0f;
					}
				}
				else
				{
					// ループしない
					playAnim_.step = playAnim_.totalTime;
				}
			}
		}
		// アニメーション設定
		MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
	}
}

void AnimationController::Release(void)
{
	for (const auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(void) const
{
	bool ret = false;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return ret;

}

float AnimationController::GetTotalTime(void)
{
	return playAnim_.totalTime;
}
