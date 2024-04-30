#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
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

		PLAYER_HP,//プレイヤーHP画像の番号
		OJAMAGAUGE_CASE,//お邪魔ゲージ枠
		OJAMAGAUGE,//お邪魔ゲージ枠の中身
		VICTORY_BACK,//勝利画面の背景
		OVER,//失敗画面の背景
		CONGRATULATIONS,//コングラチュレーション
		A,

		CAR,
		FALLING_OBJ,
		STOPER,
		SPHERE
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
