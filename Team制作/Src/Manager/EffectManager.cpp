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

	//ジェットエフェクト
	res = Effect(Effect::TYPE::EFKEFC, Application::PATH_EFFECT + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(EFF::RUN_SMOKE_01, res);
	resourcesMap_.emplace(EFF::RUN_SMOKE_02, res);

	//ヒットエフェクト
	res = Effect(Effect::TYPE::EFKEFC, Application::PATH_EFFECT + "HitEffect/HitEffect.efkefc");
	resourcesMap_.emplace(EFF::HIT_EFFECT_01, res);
	resourcesMap_.emplace(EFF::HIT_EFFECT_02, res);

	//ポーズエフェクト
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
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Effect* ret = new Effect(rPair->second);
	loadedMap_.emplace(eff, ret);

	return ret;
}

//エフェクト情報のセット
void EffectManager::SetStat(EFF eff, VECTOR pos, VECTOR rot, VECTOR scl, float speed)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return;//読み込まれていない
	}

	lPair->second->SetPos(pos);
	lPair->second->SetRot(rot);
	lPair->second->SetScl(scl);
	lPair->second->SetSpeed(speed);
}

//再生
void EffectManager::Play(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return;//読み込まれていない
	}

	lPair->second->Play();
}

//停止
void EffectManager::Stop(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return;//読み込まれていない
	}

	lPair->second->Stop();
}

//チェック
bool EffectManager::IsCheckPlay(EFF eff)
{
	const auto& lPair = loadedMap_.find(eff);
	if (lPair == loadedMap_.end())
	{
		return false;//読み込まれていない
	}

	return lPair->second->IsCheckPlay();
}