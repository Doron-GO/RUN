#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class AnimationController;

class VictoryScene :public SceneBase
{
public:
	// �R���X�g���N�^
	VictoryScene(void);

	// �f�X�g���N�^
	~VictoryScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;
	void Release(void) override;


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
	Transform WinnerModel;//����
	AnimationController* anime;
	int pleaseA_;

	int victoryWaveId_;//������
	bool soundFlag;//��������������
};

