#pragma once
#include<DxLib.h>
class UserInterface
{
public:

	static constexpr int HP_MAX = 3;//HPÌÅål
	static constexpr float OBSTACLE_METER_MAX = 100.0f;//¨×Q[WÌÅål

	//RXgN^
	UserInterface(int hp);

	//fXgN^
	~UserInterface(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//HPÌ¸
	void SubHp();
	//¨×Q[WÌ¸
	void AddObstacleMeter(float add);
	void SubObstacleMeter(float sub);

	//UI`æ
	void DrawHp();
	void DrawObstacleMeter(void);
	void Release(void);

	int GetHp(void);

private:
	//UI
	//HP
	int hp_;
	//HPÌæ
	int imgHp_;
	//¨×Q[W
	float ObsMet_;
	//¨×Q[Wæ(Q[WgÆ»Ìg)
	int imgObsMetCase_;
	int imgObsMet_;

};

