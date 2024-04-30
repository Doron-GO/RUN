#pragma once
#include<map>
#include<string>
#include"Sound.h"

class SoundManager
{
public:
	
	//リソース名
	enum class SOD
	{
		TITLE_BGM,//タイトルBGM
		BATTLE_BGM,//バトルBGM
		VICTORY,//勝利BGM
		AFF_FALL,//落下中
		BEF_FALL,//落下後
		COLL,//当たり判定
	};


	static void CreateInstance();//インスタンスの生成

	static SoundManager& GetInstance();//インスタンスの取得

	void Init();//初期化

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Sound& Load(SOD src);

	void SetPlaySoundFlag(bool flg);

private:

	static SoundManager* instance_;//静的インスタンス

	std::map<SOD, Sound*>soundsMap_;//リソース管理の対象
	std::map<SOD, Sound&>loadedMap_;//読み込み済みリソース

	Sound dummy_;//空のリソース

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SoundManager();
	// コピーコンストラクタも同様
	SoundManager(const SoundManager& manager) = default;
	// デストラクタも同様
	~SoundManager(void) = default;

	// 内部ロード
	Sound& _Load(SOD sod);

	//音を適用するかのフラグ
	//true : 適用, false : 適用しない
	bool playSoundFlag;
};

