#include<DxLib.h>
#include"Sound.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

//�C���X�^���X�̐���
void SoundManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}

	instance_->Init();

}

//�C���X�^���X�̎擾
SoundManager& SoundManager::GetInstance()
{
	return *instance_;
}

//������
void SoundManager::Init()
{
	std::string PATH_MUSIC = "Data/Sound/Music/";
	std::string PATH_WAVE = "Data/Sound/Wave/";

	Sound* res;

	//�o�g��BGM
	res = new Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Battle_1.mp3");
 	soundsMap_.emplace(SOD::BATTLE_BGM, res);

	//�^�C�g��BGM
	res = new Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Title_2.mp3");
	soundsMap_.emplace(SOD::TITLE_BGM, res);

	//������
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "after_fall.mp3");
	soundsMap_.emplace(SOD::AFF_FALL, res);

	//�������ďՓ�
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "befor_fall.mp3");
	soundsMap_.emplace(SOD::BEF_FALL, res);

	//�����蔻�莞�̌��ʉ�
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "Collision.mp3");
	soundsMap_.emplace(SOD::COLL, res);

	//������
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "Victory.mp3");
	soundsMap_.emplace(SOD::VICTORY, res);

	//����K�p���邩�̃t���O
	playSoundFlag = false;
}

void SoundManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void SoundManager::Destroy(void)
{
	Release();
	for (auto& res : soundsMap_)
	{
		res.second->Release();
		delete res.second;
	}
	soundsMap_.clear();
	delete instance_;
}

const Sound& SoundManager::Load(SOD sod)
{
	if (playSoundFlag)
	{
		Sound& res = _Load(sod);
		if (res.type_ == Sound::TYPE::NONE)
		{
			return dummy_;
		}
		return res;
	}
	return dummy_;
}

Sound& SoundManager::_Load(SOD sod)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(sod);
	if (lPair != loadedMap_.end())
	{
		return *soundsMap_.find(sod)->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& sPair = soundsMap_.find(sod);
	if (sPair == soundsMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	sPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(sod, *sPair->second);

	return *sPair->second;

}

//�R���X�g���N�^
SoundManager::SoundManager()
{

}

void SoundManager::SetPlaySoundFlag(bool flg)
{
	playSoundFlag = flg;
}
