#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class AnimationController;

class GameOverScene :
    public SceneBase
{
public:
	// �R���X�g���N�^
	GameOverScene(void);

	// �f�X�g���N�^
	~GameOverScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;


private:

	// �A�j���[�V�������
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

	int imgVback;//������ʂ̔w�i
	int imgCongratulation;//�R���O���`�����[�V����
	int pleaseA_;
	Transform WinnerModel;//����
	AnimationController* anime;

	int victoryWaveId_;//������
	bool soundFlag;//��������������

};

