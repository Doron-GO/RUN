#include "Stage.h"
#include"../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"


Stage::Stage(int playernum):playerNum_((Stage::PLAYERNUM)playernum)
{
	Init();
}

Stage::~Stage()
{
}

void Stage::Init(void)
{	// ÉÇÉfÉãÇÃäÓñ{ê›íË
	float offset = 0;
	if(playerNum_== Stage::PLAYERNUM::P2)
	{
		offset = OFFSETZ;
	}
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::STAGE));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f , 0.0f, 0.0f+ offset };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	colliders_.emplace_back(Collider::TYPE::STAGE, transform_.modelId);
	checkPoint_.emplace_back(VECTOR{ -296  ,2,6496+ offset }, VECTOR{ 779 ,500,6800+ offset }, CheckPoint::CURVE::RIGHT);
	checkPoint_.emplace_back(VECTOR{ 7725 ,2,6300+ offset }, VECTOR{ 7790 ,500,7072+ offset },CheckPoint::CURVE::RIGHT);
	checkPoint_.emplace_back(VECTOR{  7388 ,2,-1479+ offset }, VECTOR{  8187 ,500,-825+ offset },CheckPoint::CURVE::LEFT);
	checkPoint_.emplace_back(VECTOR{ 15209 ,2,-1479+ offset }, VECTOR{ 15809 ,500,-425 + offset}, CheckPoint::CURVE::LEFT);
	checkPoint_.emplace_back(VECTOR{ 15009 ,2,6203+ offset }, VECTOR{ 15809 ,500,6874+ offset }, CheckPoint::CURVE::RIGHT);
	transform_.Update();
}


void Stage::Update(Input& input)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	//for (auto& c : checkPoint_)
	//{
	//	DebugCheckPointDraw(c);
	//}
}

const std::vector<Collider> Stage::Getclliders()
{
	return colliders_;
}

const std::vector<CheckPoint> Stage::GetCheckPoint()
{
	return checkPoint_;
}

void Stage::DebugCheckPointDraw(CheckPoint checPoint)
{
	CheckPointDrawForward(checPoint.min_, checPoint.max_ );
	CheckPointDrawBack(checPoint.min_, checPoint.max_);
	CheckPointDrawRight(checPoint.min_, checPoint.max_);
	CheckPointDrawLeft(checPoint.min_, checPoint.max_);
}

void Stage::CheckPointDrawUp(VECTOR min, VECTOR max)
{

}

void Stage::CheckPointDrawRight(VECTOR min, VECTOR max)
{
	DrawLine3D(VGet(max.x, max.y, min.z), VGet(max.x, max.y, max.z), 0xff0000);
	DrawLine3D(VGet(max.x, min.y, min.z), VGet(max.x, min.y, max.z), 0xff0000);
}

void Stage::CheckPointDrawLeft(VECTOR min, VECTOR max)
{
	DrawLine3D(VGet(min.x, max.y, min.z), VGet(min.x, max.y, max.z), 0xff0000);
	DrawLine3D(VGet(min.x, min.y, min.z), VGet(min.x, min.y, max.z), 0xff0000);
}

void Stage::CheckPointDrawForward(VECTOR min, VECTOR max)
{
	DrawLine3D(VGet(min.x, min.y, max.z), VGet(max.x, min.y, max.z), 0xff0000);
	DrawLine3D(VGet(min.x, max.y, max.z), VGet(max.x, max.y, max.z), 0xff0000);
	DrawLine3D(VGet(min.x, min.y, max.z), VGet(min.x, max.y, max.z), 0xff0000);
	DrawLine3D(VGet(max.x, min.y, max.z), VGet(max.x, max.y, max.z), 0xff0000);
}

void Stage::CheckPointDrawBack(VECTOR min, VECTOR max)
{
	DrawLine3D(VGet(min.x, min.y, min.z), VGet(max.x, min.y, min.z), 0xff0000);
	DrawLine3D(VGet(min.x, max.y, min.z), VGet(max.x, max.y, min.z), 0xff0000);
	DrawLine3D(VGet(min.x, min.y, min.z), VGet(min.x, max.y, min.z), 0xff0000);
	DrawLine3D(VGet(max.x, min.y, min.z), VGet(max.x, max.y, min.z), 0xff0000);
}

