#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"

class SkyDome : public ActorBase
{

public:

	static constexpr float SCALE =300.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// �R���X�g���N�^
	SkyDome(VECTOR size);

	// �f�X�g���N�^
	~SkyDome(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* target);
private:
	//�Ǐ]�ΏۂƂ���Transform
	const Transform* target_;

	// ���
	STATE state_;

	//�T�C�Y
	VECTOR size_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateStay(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateStay(void);

};
