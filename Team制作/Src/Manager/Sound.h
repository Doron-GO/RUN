#pragma once
#include<string>
#include<vector>

class Sound
{
public:

	enum class TYPE
	{
		NONE,
		MUSIC,//BGM
		WAVE,//���ʉ�
	};


	Sound(void);//�R���X�g���N�^
	Sound(TYPE type, std::string path);//�R���X�g���N�^

	void Load(void);//�ǂݍ��ݏ���

	void Release(void);//�J������

	TYPE type_;//�T�E���h�̎��

	std::string path_;//�T�E���h�̃p�X

	int handleId_;//�T�E���h�̃n���h��
};

