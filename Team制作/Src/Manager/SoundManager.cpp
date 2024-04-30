#include<DxLib.h>
#include"Sound.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

//インスタンスの生成
void SoundManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}

	instance_->Init();

}

//インスタンスの取得
SoundManager& SoundManager::GetInstance()
{
	return *instance_;
}

//初期化
void SoundManager::Init()
{
	std::string PATH_MUSIC = "Data/Sound/Music/";
	std::string PATH_WAVE = "Data/Sound/Wave/";

	Sound* res;

	//バトルBGM
	res = new Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Battle_1.mp3");
 	soundsMap_.emplace(SOD::BATTLE_BGM, res);

	//タイトルBGM
	res = new Sound(Sound::TYPE::MUSIC, PATH_MUSIC + "Title_2.mp3");
	soundsMap_.emplace(SOD::TITLE_BGM, res);

	//落下中
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "after_fall.mp3");
	soundsMap_.emplace(SOD::AFF_FALL, res);

	//落下して衝突
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "befor_fall.mp3");
	soundsMap_.emplace(SOD::BEF_FALL, res);

	//当たり判定時の効果音
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "Collision.mp3");
	soundsMap_.emplace(SOD::COLL, res);

	//勝利音
	res = new Sound(Sound::TYPE::WAVE, PATH_WAVE + "Victory.mp3");
	soundsMap_.emplace(SOD::VICTORY, res);

	//音を適用するかのフラグ
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

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(sod);
	if (lPair != loadedMap_.end())
	{
		return *soundsMap_.find(sod)->second;
	}

	// リソース登録チェック
	const auto& sPair = soundsMap_.find(sod);
	if (sPair == soundsMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	sPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(sod, *sPair->second);

	return *sPair->second;

}

//コンストラクタ
SoundManager::SoundManager()
{

}

void SoundManager::SetPlaySoundFlag(bool flg)
{
	playSoundFlag = flg;
}
