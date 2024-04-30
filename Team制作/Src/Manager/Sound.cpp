#include<DxLib.h>
#include "Sound.h"

//コンストラクタ
Sound::Sound(void)
{
	type_ = TYPE::NONE;
	path_ = "";
}

Sound::Sound(TYPE type, std::string path)
{
	type_ = type;
	path_ = path;
}

//読み込み処理
void Sound::Load(void)
{
	switch (type_) 
	{
	case Sound::TYPE::MUSIC:
	{
		handleId_ = LoadMusicMem(path_.c_str());
	}
		break;
	case Sound::TYPE::WAVE:
	{
		handleId_ = LoadSoundMem(path_.c_str());
	}
		break;
	}
}

//開放処理
void Sound::Release(void)
{
	switch (type_)
	{
	case Sound::TYPE::MUSIC:
		StopMusicMem(handleId_);
		DeleteMusicMem(handleId_);
		break;
	case Sound::TYPE::WAVE:
		StopSoundMem(handleId_);
		DeleteSoundMem(handleId_);
		break;
	}
}