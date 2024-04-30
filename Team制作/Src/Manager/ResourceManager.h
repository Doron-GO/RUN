#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		START_LOGO,
		END_LOGO,
		TITLE,
		PUSH_SPACE,
		PLAYER,
		PLAYER_SHADOW,
		SKY_DOME,
		STGECHIP,
		STAGE,
		STGECORNER,
		FOOT_SMOKE,
		TANK_BODY,
		TANK_BARREL,
		TANK_WHEEL,

		PLAYER_HP,//�v���C���[HP�摜�̔ԍ�
		OJAMAGAUGE_CASE,//���ז��Q�[�W�g
		OJAMAGAUGE,//���ז��Q�[�W�g�̒��g
		VICTORY_BACK,//������ʂ̔w�i
		OVER,//���s��ʂ̔w�i
		CONGRATULATIONS,//�R���O���`�����[�V����
		A,

		CAR,
		FALLING_OBJ,
		STOPER,
		SPHERE
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource*> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
