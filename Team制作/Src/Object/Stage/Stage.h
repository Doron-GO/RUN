#pragma once
#include<DxLib.h>
#include<vector>
#include "../Common/Transform.h"
#include"../Common/CheckPoint.h"
#include"../ActorBase.h"


class Stage : public ActorBase
{
public:

	enum class PLAYERNUM
	{
		MIN,
		P1,
		P2
	};
	Stage(int playernum);
	~Stage();
	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;
	const std::vector<Collider> Getclliders();
	const std::vector<CheckPoint> GetCheckPoint();

private:
	std::vector<Collider> colliders_;
	std::vector<CheckPoint> checkPoint_;
	PLAYERNUM playerNum_;
	Transform transform_;

	void DebugCheckPointDraw(CheckPoint checPoint);
	void CheckPointDrawUp(VECTOR min,VECTOR max);
	void CheckPointDrawRight(VECTOR min,VECTOR max);
	void CheckPointDrawLeft(VECTOR min, VECTOR max);
	void CheckPointDrawForward(VECTOR min, VECTOR max);
	void CheckPointDrawBack(VECTOR min, VECTOR max);
};

