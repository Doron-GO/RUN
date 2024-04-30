#pragma once
#include <vector>
#include<memory>
#include "../Manager/InputManager.h"
#include "SceneBase.h"

class Player;
class Stage;
class SkyDome;
class Camera;
class Vector2;
class ObstacleManager;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;
	void Release(void) override;

	void DrawGame(int num);

	void SoundPlay()override;//条件付きの音を再生
	void SoundStop();//音を停止する
	void VolumeRegulation()override;//音量調整



private:
	std::vector<std::unique_ptr<Player>> players_;
	std::vector<std::unique_ptr<Stage>> stages_;
	std::vector<std::unique_ptr<Camera>> cameras_;
	std::unique_ptr<Stage>stage_;
	std::unique_ptr<ObstacleManager>obstacleManager_;

	std::vector<VECTOR> vecPos;
	std::vector<InputManager::JOYPAD_NO> vecPad;
	//画面
	std::vector<int> cameraScreens_;
	std::vector<Vector2> screenPos_;
	std::unique_ptr<SkyDome> skyDome_;


	bool CollisionPlayerToCar(void);
	void AddCollider(void);
	void AddObstacleCollider(void);

	//プレイヤークラスからGetしてくる
	//プレイヤーのpos(仮)
	VECTOR playerPos_;
	//プレイヤーのカプセル上のPos(仮)
	VECTOR playerCupsuleTopPos_;
	//プレイヤーのカプセル下のPos(仮)
	VECTOR playerCupsuleBottomPos_;
	//プレイヤーの半径(仮)
	float playerRadius_;


	//車のpos(仮)
	VECTOR carPos_;
	//車のカプセル上のPos(仮)
	VECTOR carCupsuleTopPos_;
	//車のカプセル下のPos(仮)
	VECTOR carCupsuleBottomPos_;
	//車の半径
	float carRadius_;

	//サウンド変数
	int sodBattId_;//バトルBGMのハンドル番号を格納

	//プレイヤー人数
	int playerNum_;

	//デバッグ用
	int colisionWaveId_;//当たり判定の効果音
	int victoryWaveId_;//勝利音
};
