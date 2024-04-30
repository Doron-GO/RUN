#include"../Application.h"
#include"Effect.h"
#include "EffectManager.h"
#include<DxLib.h>

EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new EffectManager();
	}
	instance_->Init();
}

EffectManager& EffectManager::GetInstance(void)
{
	return *instance_;
}

void EffectManager::Init(void)
{

	Effect res;

	//�W�F�b�g�G�t�F�N�g
	res = Effect(Effect::TYPE::EFKEFC, Application::PATH_EFFECT + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(EFF::RUN_SMOKE_01, res);
	resourcesMap_.emplace(EFF::RUN_SMOKE_02, res);

	//�q�b�g�G�t�F�N�g
	res = Effect(Effect::TYPE::EFKEFC, Application::PATH_EFFECT + "HitEffect/HitEffect.efkefc");
	resourcesMap_.emplace(EFF::HIT_EFFECT_01, res);
	resourcesMap_.emplace(EFF::HIT_EFFECT_02, res);

	//�|�[�Y�G�t�F�N�g
	res = Effect(Effect::TYPE::EFKEFC, Application::PATH_EFFECT + "PoseEffect/PoseEffect.efkefc");
	resourcesMap_.emplace(EFF::POSE_EFFECT_01, res);
	resourcesMap_.emplace(EFF::POSE_EFFECT_02, res);

}

void EffectManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void EffectManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Effect EffectManager::Load(EFF eff)
{
	Effect* res = _Load(eff);
	if (res == nullptr)
	{
		return Effect();
	}
	Effect ret = *res;
	return *res;
}


EffectManager::EffectManager(void)
{
}

Effect* EffectManager::_Load(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(eff);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Effect* ret = new Effect(rPair->second);
	loadedMap_.emplace(eff, ret);

	return ret;
}

//�G�t�F�N�g���̃Z�b�g
void EffectManager::SetStat(EFF eff, VECTOR pos, VECTOR rot, VECTOR scl, float speed)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return;//�ǂݍ��܂�Ă��Ȃ�
	}

	lPair->second->SetPos(pos);
	lPair->second->SetRot(rot);
	lPair->second->SetScl(scl);
	lPair->second->SetSpeed(speed);
}

//�Đ�
void EffectManager::Play(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return;//�ǂݍ��܂�Ă��Ȃ�
	}

	lPair->second->Play();
}

//��~
void EffectManager::Stop(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return;//�ǂݍ��܂�Ă��Ȃ�
	}

	lPair->second->Stop();
}

//�`�F�b�N
bool EffectManager::IsCheckPlay(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return false;//�ǂݍ��܂�Ă��Ȃ�
	}

	return lPair->second->IsCheckPlay();
}