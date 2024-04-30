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

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("�t�H�[���E�u���b�N�ERUN��RUN");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		TRACE("DxLib �̏��������s");
		isInitFail_ = true;
		return;
	}

	// Effekseer�̏�����
	InitEffekseer();

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// �f�o�b�O�@�\������
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);
	Measure::CreateInstance();

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	//�@�T�E���h�Ǘ�������
	SoundManager::CreateInstance();
	//���L�̃T�E���h�}�l�[�W���[�N���X�֐����R�����g�A�E�g�����
	//������Ȃ��悤�ɂł��܂�
	SoundManager::GetInstance().SetPlaySoundFlag(true);

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

	//�G�t�F�N�g�Ǘ�������
	EffectManager::CreateInstance();
}

void Application::Run(void)
{

	auto& intpuManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	Input input;
	// �Q�[�����[�v
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

	// Effekseer���I������B
	Effkseer_End();

	// DxLib�I��
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
