#pragma once
#include<DxLib.h>
#include<string>
#include<vector>

class Effect
{
public:

	enum class TYPE
	{
		NONE,
		EFKEFC,
		PNG,
	};

	//�R���X�g���N�^
	Effect(void);
	Effect(TYPE type, const std::string& path);
	
	//�f�X�g���N�^
	~Effect(void);

	void Load(void);//�ǂݍ���

	void Release(void);//�������J������

	//�G�t�F�N�g�̏����Z�b�g
	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetScl(VECTOR scl);
	void SetSpeed(float speed);

	//�Đ�
	void Play();
	//��~
	void Stop();

	//�`�F�b�N
	bool IsCheckPlay();

	TYPE resType_;//���\�[�X�̎��

	std::string path_;//�f�[�^�̃p�X
	int handleId_;

	int playId_;//�G�t�F�N�g�̃v���CID
};

