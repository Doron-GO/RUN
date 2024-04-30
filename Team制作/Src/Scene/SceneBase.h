#pragma once
#include"../Manager/SoundManager.h"

class Input;
class Soundmanager;

class SceneBase
{
public:

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void);

	// 初期化処理
	virtual void Init(void);

	// 更新ステップ
	virtual void Update(Input& input);

	// 描画処理
	virtual void Draw(void);

	// 解放処理
	virtual void Release(void);

	virtual void SoundPlay();//条件付きの音を再生
	virtual void SoundStop();//音を停止する
	virtual void VolumeRegulation();//音量調整

protected:

	//シングルトン参照
	SoundManager& soundMana_;
};

