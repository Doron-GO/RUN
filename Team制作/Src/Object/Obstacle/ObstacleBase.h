#pragma once
#include<DxLib.h>
#include"../../Manager/ResourceManager.h"
#include "../ActorBase.h"
#include"../../Application.h"
#include "../../Input/Input.h"
#include"../../Manager/ResourceManager.h"
#include"../../Utility/AsoUtility.h"

class ResourceManager;

class ObstacleBase : public ActorBase
{
public:

	static constexpr VECTOR SIZE = { 0.4f,0.4f,0.4f };

	ObstacleBase();
	virtual ~ObstacleBase() {};
	virtual void Init()override=0;
	virtual void Update(Input& input)override = 0;
	virtual void Draw()override = 0;
	virtual void SetPos(VECTOR pos);
	virtual void SetRot(VECTOR Rot);

private:


protected:
};

