#pragma once

#include<map>
#include"Effect.h"


class EffectManager
{
public:

	//���\�[�X��
	enum class EFF
	{
		NONE,
		RUN_SMOKE_01,
		RUN_SMOKE_02,
		HIT_EFFECT_01,
		HIT_EFFECT_02,
		POSE_EFFECT_01,
		POSE_EFFECT_02,
	};


	static void CreateInstance(void);//�C���X�^���X���쐬����
	
	static EffectManager& GetInstance(void);//�����̃C���X�^���X���擾����

	void Init();//������
	void Release();//�������J������
	void Destroy(void);//���\�[�X�̊��S����

	Effect Load(EFF eff);//�ǂݍ��ݏ���

	//�G�t�F�N�g���̃Z�b�g
	void SetStat(EFF eff, VECTOR pos, VECTOR rot, VECTOR scl, float speed);
	//�Đ�
	void Play(EFF eff);

	//��~
	void Stop(EFF eff);

	//�`�F�b�N
	bool IsCheckPlay(EFF eff);

private:

	static EffectManager* instance_;

	std::map<EFF, Effect>resourcesMap_;
	std::map<EFF, Effect*>loadedMap_;

	//�R���X�g���N�^
	EffectManager(void);

	//�f�X�g���N�^
	~EffectManager(void)=default;

	Effect* _Load(EFF eff);//�����ǂݍ���
};

