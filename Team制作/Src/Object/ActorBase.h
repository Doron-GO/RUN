#pragma once
#include "Common/Transform.h"
class ResourceManager;
class SceneManager;
class GravityManager;
class Input;

constexpr float OFFSETZ =10000.0f;

class ActorBase
{

public:

	// �R���X�g���N�^
	ActorBase(void);

	// �f�X�g���N�^
	virtual ~ActorBase(void);

	virtual void Init(void) = 0;
	virtual void Update(Input& input) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

protected:

	// �V���O���g���Q��
	ResourceManager& resMng_;

	// ���f������̊�{���
	Transform transform_;

};
