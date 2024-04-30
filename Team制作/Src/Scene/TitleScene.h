#pragma once
#include <memory>
#include "SceneBase.h"

class TitlePlayer;
class Camera;
class SkyDome;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;
	void DrawLogo(void);
	void Release(void) override;

	void SoundPlay()override;//�����t���̉����Đ�
	void SoundStop();//�����~����
	void VolumeRegulation()override;//���ʒ���

private:
	std::unique_ptr<TitlePlayer> player_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<SkyDome> skyDome_;

	// �^�C�g�����S
	int titleLogoImg_;
	int  pleaseA_ ;

	int sodTitleBgm;
};

