#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	//���f���{��
	modelId_ = modelId;
	//�A�j���[�V�����^�C�v
	playType_ = -1;
	//���[�v�t���O
	isLoop_ = false;
	//�A�j���[�V�����X�g�b�v�t���O
	isStop_ = false;
	//�I������゠��ꕔ�̃A�j���[�V���������[�v������Ƃ��̃A�j���[�V�����X�s�[�h
	endLoopSpeed_ = 0.0f;
	//�ǂ̕����`�ǂ̕����܂ł��̎w��
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;

	//�O�̃A�j���[�V�����i�[�ϐ�
	prevAttachIndex_ = -1;
	//���݂̃A�j���[�V�����i�[�ϐ�
	currentAttachIndex_ = -1;

	rate_ = 0.0f;

	blendTime_ = 0.1f;

	animationFlag_ = true;

}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(int type, const std::string& path, float speed)
{
	Animation anim;
	//���[�h
	anim.model = MV1LoadModel(path.c_str());
	//�A�j���[�V�����^�C�v���C���f�b�N�X�i�V���ȗv�f�𖖔��Ɂj�ɒǉ�
	anim.animIndex = type;
	//�X�s�[�h
	anim.speed = speed;

	//�A�j���[�V�����^�C�v�������ĂȂ��ꍇ�H�����m�F
	if (animations_.count(type) == 0)
	{
		// ����ւ�
		animations_.emplace(type, anim);
	}
	else
	{
		// �ǉ�
		animations_[type].model = anim.model;
		animations_[type].animIndex = anim.animIndex;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}

}

void AnimationController::StartPlay(int type, bool isLoop, bool isStop)
{
	// �A�j���[�V������ʂ�ύX
	//�X�^�[�g�����ǂ����ł̎w��͓�̃A�j���[�V��������������Ȃ��悤��
	playType_ = type;
	playAnim_ = animations_[type];


	// ���f���ɃA�j���[�V������t����
	int animIdx = 0;
	playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);

	// �A�j���[�V���������Ԃ̎擾
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	// �A�j���[�V�������[�v
	isLoop_ = isLoop;

	// �A�j���[�V�������Ȃ�
	isStop_ = isStop;

	animationFlag_ = false;

}

void AnimationController::Play(int type, bool isLoop,
	float startStep, float endStep, bool isStop, bool isForce)
{
	//�A�j���[�V�����^�C�v���������
	if (playType_ != type || isForce) {

		//�A�j���[�V�����^�C�v�������Ă�����
		if (playType_ != -1)
		{
			if (!animationFlag_)
			{
				//�����őO�̃A�j���[�V�����ƌ��݂̃A�j���[�V�������Ƃ�
				prevAttachIndex_ = playAnim_.attachNo;
				// �A�j���[�V������ʂ�ύX
				playType_ = type;
				//�v���C����A�j���[�V�������w�肷��
				playAnim_ = animations_[type];
				//int animIdx = 0;
				// ���f������A�j���[�V�������O��
				playAnim_.attachNo = MV1AttachAnim(modelId_, 0, playAnim_.model);
				animationFlag_ = true;
			}
		}

		// ������
		playAnim_.step = startStep;
		// �A�j���[�V���������Ԃ̎擾
		if (endStep > 0.0f)
		{
			playAnim_.totalTime = endStep;
		}
		else
		{
			playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);
		}

		// �A�j���[�V�������[�v
		isLoop_ = isLoop;

		// �A�j���[�V�������Ȃ�
		isStop_ = isStop;

		stepEndLoopStart_ = -1.0f;
		stepEndLoopEnd_ = -1.0f;
	}
}

void AnimationController::Update(void)
{
	if (animationFlag_)
	{
		if (rate_ < 1.0f)
		{
			stepBlend_ += SceneManager::GetInstance().GetDeltaTime();
			MV1SetAttachAnimBlendRate(modelId_, prevAttachIndex_, 1.0f - rate_);
			MV1SetAttachAnimBlendRate(modelId_, playAnim_.attachNo, rate_);
			rate_ = stepBlend_ / blendTime_;
		}
		if (rate_ >= 1.0f)
		{
			MV1DetachAnim(modelId_, prevAttachIndex_);
			animationFlag_ = false;
			stepBlend_ = 0.0f;
			rate_ = 0.0f;
		}
	}
	else
	{
		// �o�ߎ��Ԃ̎擾
		float deltaTime = SceneManager::GetInstance().GetDeltaTime();
		//�X�g�b�v�t���O��false
		if (!isStop_)
		{
			// �Đ�
			playAnim_.step += (deltaTime * playAnim_.speed);

			// �A�j���[�V�����I������
			bool isEnd = false;
			//�A�j���[�V�����i�s���Ԃ��g�[�^�����Ԃ𒴂�����
			if (playAnim_.step > playAnim_.totalTime)
			{
				isEnd = true;
			}

			if (isEnd)
			{
				// �A�j���[�V�������I��������
				if (isLoop_)
				{
					// ���[�v�Đ�
					if (stepEndLoopStart_ > 0.0f)
					{
						// �A�j���[�V�����I����̎w��t���[���Đ�
						playAnim_.speed = endLoopSpeed_;
					}
					else
					{
						// �ʏ�̃��[�v�Đ�
						playAnim_.step = 0.0f;
					}
				}
				else
				{
					// ���[�v���Ȃ�
					playAnim_.step = playAnim_.totalTime;
				}
			}
		}
		// �A�j���[�V�����ݒ�
		MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
	}
}

void AnimationController::Release(void)
{
	for (const auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(void) const
{
	bool ret = false;

	if (isLoop_)
	{
		// ���[�v�ݒ肳��Ă���Ȃ�A
		// �������ŏI�����Ȃ���Ԃ�
		return ret;
	}

	if (playAnim_.step >= playAnim_.totalTime)
	{
		// �Đ����Ԃ��߂�����true
		return true;
	}

	return ret;

}

float AnimationController::GetTotalTime(void)
{
	return playAnim_.totalTime;
}
