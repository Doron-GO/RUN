#pragma once
#include "ActorBase.h"

class AnimationController;

class TitlePlayer : public ActorBase
{
public:

	enum class ANIM_TYPE
	{
		RUN,
	};

	// �R���X�g���N�^
	TitlePlayer(void);

	// �f�X�g���N�^
	~TitlePlayer(void);

	void Init(void) override;
	void InitEffect();
	void InitAnimaiton(void);
	void Update(Input& input) override;
	void UpdateEffect();
	void UpdateAnimation(void);
	void Draw(void) override;
private:
	std::unique_ptr<AnimationController> animationController_;

};

