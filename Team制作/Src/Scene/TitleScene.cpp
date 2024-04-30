#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/TitlePlayer.h"
#include "../Object/SkyDome.h"
#include"../Input/Input.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	titleLogoImg_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	float skyDomeSize = 100.0f;
	skyDome_ = std::make_unique<SkyDome>(VECTOR{ skyDomeSize ,skyDomeSize ,skyDomeSize });
	skyDome_->Init();

	player_ = std::make_unique<TitlePlayer>();
	player_->Init();
	

	camera_ = std::make_unique<Camera>();
	camera_->SetFollow(&player_->GetTransform());
	camera_->ChangeMode(Camera::MODE::FOLLOW);

	pleaseA_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::A).handleId_;
	// タイトルロゴ
	titleLogoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE).handleId_;
	sodTitleBgm = SoundManager::GetInstance().Load(SoundManager::SOD::TITLE_BGM).handleId_;
	SoundPlay();
}

void TitleScene::Update(Input& input)
{
	SoundPlay();
	skyDome_->Update(input);

	camera_->Update();

	player_->Update(input);


	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (input.IsTriggerd("jump"))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		SoundStop();
	}

}

void TitleScene::Draw(void)
{
	skyDome_->Draw();

	camera_->SetBeforeDraw();

	player_->Draw();

	// ロゴ描画
	DrawLogo();
	DrawGraph(350, 500, pleaseA_, true);

}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, titleLogoImg_, true);

	//// Pushメッセージ
	//std::string msg = "Push Space";
	//SetFontSize(28);
	//int len = (int)strlen(msg.c_str());
	//int width = GetDrawStringWidth(msg.c_str(), len);
	//DrawFormatString(cx - (width / 2), 200, 0x87cefa, msg.c_str());
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//SetFontSize(16);

}

void TitleScene::Release(void)
{

	DeleteGraph(titleLogoImg_);

}

//条件付きの効果音・音楽を再生
void TitleScene::SoundPlay()
{
	//BGMが鳴っていないなら曲をプレイ
	if (!CheckMusicMem(sodTitleBgm))
	{
		PlayMusicMem(sodTitleBgm, DX_PLAYTYPE_BACK);//バックグラウンドで再生
		VolumeRegulation();
	}
}

//音を停止する
void TitleScene::SoundStop()
{
	//BGMが鳴っているなら曲を停止
	if (CheckMusicMem(sodTitleBgm))
	{
		StopMusicMem(sodTitleBgm);
	}
}

//音量調整
void TitleScene::VolumeRegulation()
{
	ChangeVolumeSoundMem(230, sodTitleBgm);
}