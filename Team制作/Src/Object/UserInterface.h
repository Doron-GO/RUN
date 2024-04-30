#pragma once
#include<DxLib.h>
class UserInterface
{
public:
	UserInterface(void);
	~UserInterface(void);

	void Init(void);
	void Update(void);
	//プレイヤーHP
	void DarwPlayerHp(int playerHp);
	void DrawObstacleMeter(void);
	void Release(void);

private:
	//プレイヤーHPの画像
	int playerHpImg_;
	//プレイヤーのHp格納変数
	int playerHp_;
};

