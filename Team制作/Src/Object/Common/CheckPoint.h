#pragma once
#include<DxLib.h>
class CheckPoint
{
public:

	enum class CURVE
	{
		RIGHT,
		LEFT
	};


	// コンストラクタ
	CheckPoint(VECTOR min, VECTOR max, CURVE curve);

	// デストラクタ
	~CheckPoint(void);


	// モデルのハンドルID
	VECTOR min_;
	VECTOR max_;
	CURVE curve_;
	bool passageFlag_;

};

