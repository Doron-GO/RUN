#include <string>
#include "Sphere.h"
#include"../../Manager/SceneManager.h"
#include"../Stage/Stage.h"
//#include"../Player.h"

Sphere::Sphere()
{
	Init();
}

void Sphere::Init()
{
	//stage_ = std::make_shared<Stage>();
	//transform_.SetModel(resourceManager_.LoadModelDuplicate(
	//	ResourceManager::SRC::SPHERE));
	//transform_.scl = { 50.0f,50.0f ,50.0f };//Curve5 5番
	//auto name = MV1GetFrameName(stage_->GetTransform().modelId, 5);
	//int frame = MV1SearchFrame(stage_->GetTransform().modelId, name);
	//VECTOR framePos= MV1GetFramePosition(stage_->GetTransform().modelId, frame);
	//transform_.pos = framePos;
	//transform_.rot = AsoUtility::VECTOR_ZERO;
	//transform_.quaRot = Quaternion();
	//transform_.quaRotLocal = Quaternion::Euler(
	//	{ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	//transform_.Update();
	//time_ = 14.5f;
}

void Sphere::Update(Input& input)
{
	//一個前のカーブはCurve4
	//int frame = MV1SearchFrame(stage_->GetTransform().modelId, "Curve4");
	//VECTOR curvePos = MV1GetFramePosition(stage_->GetTransform().modelId, frame);

	//VECTOR pos=player_->GetTransform().pos;
	//float size = VSize(VSub(pos, curvePos));
	// 
	//if (size<=0.0f)
	//{
	//	transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), 5.0f));
	//}
	//transform_.Update();

	time_ -= SceneManager::GetInstance().GetDeltaTime();
	if (time_ <= 0.0f)
	{
		transform_.pos = VAdd(transform_.pos, VScale(transform_.GetBack(), 10.0f));
	}

	transform_.Update();
}

void Sphere::Draw()
{
	MV1DrawModel(transform_.modelId);
	DrawFormatString(0, 300, 0xff0000, "sp.x=%f,sp.y=%f,sp.z=%f", transform_.pos.x
		, transform_.pos.y, transform_.pos.z);

}

