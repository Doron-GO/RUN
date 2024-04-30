#pragma once
#include <string>
#include <chrono>

#include <map>
class SceneManager;

class AnimationController
{

public:
	static constexpr float CHARA_ANIM_BLEND_SPEED = 0.01f;

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	void Add(int type, const std::string& path, float speed);

	void StartPlay(int type, bool isLoop = true, bool isStop = false);


	// アニメーション再生
	void Play(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(void);
	void Release(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

	float GetTotalTime(void);
private:

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;
	Animation prePlayAnim_;

	//std::list<Animation> playAnimList_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	//前のアニメーション格納変数
	int prevAttachIndex_;
	//現在のアニメーション格納変数
	int currentAttachIndex_;

	//ブレンドレート
	float rate_;
	float prev_blend_rate_;
	float blend_time_;
	float stepBlend_;
	float blendTime_;

	//アニメーションテストフラグ
	bool animationFlag_;

};

