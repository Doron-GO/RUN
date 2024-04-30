#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class AnimationController;

class GameOverScene :
    public SceneBase
{
public:
	// コンストラクタ
	GameOverScene(void);

	// デストラクタ
	~GameOverScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;


private:

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

	int imgVback;//勝利画面の背景
	int imgCongratulation;//コングラチュレーション
	int pleaseA_;
	Transform WinnerModel;//勝者
	AnimationController* anime;

	int victoryWaveId_;//勝利音
	bool soundFlag;//勝利音が鳴ったか

};

