#pragma once
#include <memory>
#include "SceneBase.h"

class TitlePlayer;
class Camera;
class SkyDome;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;
	void DrawLogo(void);
	void Release(void) override;

	void SoundPlay()override;//条件付きの音を再生
	void SoundStop();//音を停止する
	void VolumeRegulation()override;//音量調整

private:
	std::unique_ptr<TitlePlayer> player_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<SkyDome> skyDome_;

	// タイトルロゴ
	int titleLogoImg_;
	int  pleaseA_ ;

	int sodTitleBgm;
};

