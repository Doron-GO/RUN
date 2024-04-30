#include<cmath>
#include<DxLib.h>
#include "Car.h"
#include"../../Manager/SceneManager.h"

Car::Car()
{
	Init();
}

void Car::Init()
{
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CAR));
	transform_.scl = SIZE;
	transform_.pos = {-50.0f,0.0f,4500.0f};
	transform_.rot = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		{ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.quaRotLocal = Quaternion();
	transform_.MakeCollider(Collider::TYPE::OBSTACLE);
	transform_.Update();
	speed_ = SPEED;
	time_ = TIME;
}

void Car::Update(Input& input)
{
	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), speed_));
	
	
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	if (time_ <= 0.0f)
	{
		speed_ = -speed_;
		time_ = TIME;
	}
	transform_.Update();

}

void Car::Draw()
{
	MV1DrawModel(transform_.modelId);
	//DrawFormatString(0, 200, 0xff0000, "car.x=%f,car.y=%f,car.z=%f", transform_.pos.x
	//		, transform_.pos.y, transform_.pos.z);
	//

}


