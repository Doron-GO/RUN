#pragma once
#include <string>
#include <chrono>

#include <map>
class SceneManager;

class AnimationController
{

public:
	static constexpr float CHARA_ANIM_BLEND_SPEED = 0.01f;

	// �A�j���[�V�����f�[�^
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// �R���X�g���N�^
	AnimationController(int modelId);
	// �f�X�g���N�^
	~AnimationController(void);

	// �A�j���[�V�����ǉ�
	void Add(int type, const std::string& path, float speed);

	void StartPlay(int type, bool isLoop = true, bool isStop = false);


	// �A�j���[�V�����Đ�
	void Play(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(void);
	void Release(void);

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	void SetEndLoop(float startStep, float endStep, float speed);

	// �Đ����̃A�j���[�V����
	int GetPlayType(void) const;

	// �Đ��I��
	bool IsEnd(void) const;

	float GetTotalTime(void);
private:

	// ���f���̃n���h��ID
	int modelId_;

	// ��ޕʂ̃A�j���[�V�����f�[�^
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;
	Animation prePlayAnim_;

	//std::list<Animation> playAnimList_;

	// �A�j���[�V���������[�v���邩���Ȃ���
	bool isLoop_;

	// �A�j���[�V�������~�߂��܂܂ɂ���
	bool isStop_;

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	//�O�̃A�j���[�V�����i�[�ϐ�
	int prevAttachIndex_;
	//���݂̃A�j���[�V�����i�[�ϐ�
	int currentAttachIndex_;

	//�u�����h���[�g
	float rate_;
	float prev_blend_rate_;
	float blend_time_;
	float stepBlend_;
	float blendTime_;

	//�A�j���[�V�����e�X�g�t���O
	bool animationFlag_;

};

