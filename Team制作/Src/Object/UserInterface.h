#pragma once
#include<DxLib.h>
class UserInterface
{
public:
	UserInterface(void);
	~UserInterface(void);

	void Init(void);
	void Update(void);
	//vC[HP
	void DarwPlayerHp(int playerHp);
	void DrawObstacleMeter(void);
	void Release(void);

private:
	//vC[HPĚć
	int playerHpImg_;
	//vC[ĚHpi[Ď
	int playerHp_;
};

