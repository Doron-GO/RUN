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


	// �R���X�g���N�^
	CheckPoint(VECTOR min, VECTOR max, CURVE curve);

	// �f�X�g���N�^
	~CheckPoint(void);


	// ���f���̃n���h��ID
	VECTOR min_;
	VECTOR max_;
	CURVE curve_;
	bool passageFlag_;

};

