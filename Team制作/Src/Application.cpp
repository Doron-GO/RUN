#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Manager/InputManager.h"
#include"Input/Input.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include"Manager/SoundManager.h"
#include"Manager/EffectManager.h"
#include "Utility/Measure.h"
#include "Application.h"

Application* Application::mInstance = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";

void Application::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}
	mInstance->Init();
}

Application& Application::GetInstance(void)
{
	return *mInstance;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("フォール・ブロック・RUN＆RUN");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		TRACE("DxLib の初期化失敗");
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// デバッグ機能初期化
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);
	Measure::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	//　サウンド管理初期化
	SoundManager::CreateInstance();
	//下記のサウンドマネージャークラス関数をコメントアウトすると
	//音が鳴らないようにできます
	SoundManager::GetInstance().SetPlaySoundFlag(true);

	// シーン管理初期化
	SceneManager::CreateInstance();

	//エフェクト管理初期化
	EffectManager::CreateInstance();
}

void Application::Run(void)
{

	auto& intpuManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	Input input;
	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		Measure::GetInstance().Start();

		_dbgStartDraw();

		intpuManager.Update();
		input.Update(1);
		sceneManager.Update(input);

		sceneManager.Draw();

		_dbgAddDraw();

#ifdef _DEBUG
		Measure::GetInstance().Draw();
#endif

		ScreenFlip();

	}

}

void Application::Release(void)
{

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

Application::~Application(void)
{
	delete mInstance;
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
