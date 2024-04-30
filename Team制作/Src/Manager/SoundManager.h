#pragma once
#include<map>
#include<string>
#include"Sound.h"

class SoundManager
{
public:
	
	//���\�[�X��
	enum class SOD
	{
		TITLE_BGM,//�^�C�g��BGM
		BATTLE_BGM,//�o�g��BGM
		VICTORY,//����BGM
		AFF_FALL,//������
		BEF_FALL,//������
		COLL,//�����蔻��
	};


	static void CreateInstance();//�C���X�^���X�̐���

	static SoundManager& GetInstance();//�C���X�^���X�̎擾

	void Init();//������

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Sound& Load(SOD src);

	void SetPlaySoundFlag(bool flg);

private:

	static SoundManager* instance_;//�ÓI�C���X�^���X

	std::map<SOD, Sound*>soundsMap_;//���\�[�X�Ǘ��̑Ώ�
	std::map<SOD, Sound&>loadedMap_;//�ǂݍ��ݍς݃��\�[�X

	Sound dummy_;//��̃��\�[�X

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SoundManager();
	// �R�s�[�R���X�g���N�^�����l
	SoundManager(const SoundManager& manager) = default;
	// �f�X�g���N�^�����l
	~SoundManager(void) = default;

	// �������[�h
	Sound& _Load(SOD sod);

	//����K�p���邩�̃t���O
	//true : �K�p, false : �K�p���Ȃ�
	bool playSoundFlag;
};

