#pragma once
#include"../Manager/SoundManager.h"

class Input;
class Soundmanager;

class SceneBase
{
public:

	// �R���X�g���N�^
	SceneBase(void);

	// �f�X�g���N�^
	virtual ~SceneBase(void);

	// ����������
	virtual void Init(void);

	// �X�V�X�e�b�v
	virtual void Update(Input& input);

	// �`�揈��
	virtual void Draw(void);

	// �������
	virtual void Release(void);

	virtual void SoundPlay();//�����t���̉����Đ�
	virtual void SoundStop();//�����~����
	virtual void VolumeRegulation();//���ʒ���

protected:

	//�V���O���g���Q��
	SoundManager& soundMana_;
};

