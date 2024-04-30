#pragma once

#include<map>
#include"Effect.h"


class EffectManager
{
public:

	//リソース名
	enum class EFF
	{
		NONE,
		RUN_SMOKE_01,
		RUN_SMOKE_02,
		HIT_EFFECT_01,
		HIT_EFFECT_02,
		POSE_EFFECT_01,
		POSE_EFFECT_02,
	};


	static void CreateInstance(void);//インスタンスを作成する
	
	static EffectManager& GetInstance(void);//自分のインスタンスを取得する

	void Init();//初期化
	void Release();//メモリ開放処理
	void Destroy(void);//リソースの完全放棄

	Effect Load(EFF eff);//読み込み処理

	//エフェクト情報のセット
	void SetStat(EFF eff, VECTOR pos, VECTOR rot, VECTOR scl, float speed);
	//再生
	void Play(EFF eff);

	//停止
	void Stop(EFF eff);

	//チェック
	bool IsCheckPlay(EFF eff);

private:

	static EffectManager* instance_;

	std::map<EFF, Effect>resourcesMap_;
	std::map<EFF, Effect*>loadedMap_;

	//コンストラクタ
	EffectManager(void);

	//デストラクタ
	~EffectManager(void)=default;

	Effect* _Load(EFF eff);//内部読み込み
};

