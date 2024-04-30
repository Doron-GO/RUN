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

	// コンストラクタ
	ActorBase(void);

	// デストラクタ
	virtual ~ActorBase(void);

	virtual void Init(void) = 0;
	virtual void Update(Input& input) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

protected:

	// シングルトン参照
	ResourceManager& resMng_;

	// モデル制御の基本情報
	Transform transform_;

};
