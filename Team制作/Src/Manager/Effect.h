#pragma once
#include<DxLib.h>
#include<string>
#include<vector>

class Effect
{
public:

	enum class TYPE
	{
		NONE,
		EFKEFC,
		PNG,
	};

	//コンストラクタ
	Effect(void);
	Effect(TYPE type, const std::string& path);
	
	//デストラクタ
	~Effect(void);

	void Load(void);//読み込み

	void Release(void);//メモリ開放処理

	//エフェクトの情報をセット
	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetScl(VECTOR scl);
	void SetSpeed(float speed);

	//再生
	void Play();
	//停止
	void Stop();

	//チェック
	bool IsCheckPlay();

	TYPE resType_;//リソースの種類

	std::string path_;//データのパス
	int handleId_;

	int playId_;//エフェクトのプレイID
};

