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
		WAVE,//効果音
	};


	Sound(void);//コンストラクタ
	Sound(TYPE type, std::string path);//コンストラクタ

	void Load(void);//読み込み処理

	void Release(void);//開放処理

	TYPE type_;//サウンドの種類

	std::string path_;//サウンドのパス

	int handleId_;//サウンドのハンドル
};

