#pragma once
#include<DxLib.h>
class UserInterface
{
public:

	static constexpr int HP_MAX = 3;//HPの最大値
	static constexpr float OBSTACLE_METER_MAX = 100.0f;//お邪魔ゲージの最大値

	//コンストラクタ
	UserInterface(int hp);

	//デストラクタ
	~UserInterface(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//HPの増減
	void SubHp();
	//お邪魔ゲージの増減
	void AddObstacleMeter(float add);
	void SubObstacleMeter(float sub);

	//UI描画
	void DrawHp();
	void DrawObstacleMeter(void);
	void Release(void);

	int GetHp(void);

private:
	//UI
	//HP
	int hp_;
	//HPの画像
	int imgHp_;
	//お邪魔ゲージ
	float ObsMet_;
	//お邪魔ゲージ画像(ゲージ枠とその中身)
	int imgObsMetCase_;
	int imgObsMet_;

};

