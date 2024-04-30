#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"

class SkyDome : public ActorBase
{

public:

	static constexpr float SCALE =300.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// 状態
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// コンストラクタ
	SkyDome(VECTOR size);

	// デストラクタ
	~SkyDome(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;

	// 追従対象の設定
	void SetFollow(const Transform* target);
private:
	//追従対象とするTransform
	const Transform* target_;

	// 状態
	STATE state_;

	//サイズ
	VECTOR size_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateStay(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateStay(void);

};
