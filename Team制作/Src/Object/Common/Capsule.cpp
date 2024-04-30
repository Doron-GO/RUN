#include <DxLib.h>
#include "Transform.h"
#include "Capsule.h"

Capsule::Capsule(const Transform& parent) : transformParent_(parent)
{
	radius_ = 0.0f;
	localPosTop_ = { 0.0f, 0.0f, 0.0f };
	localPosDown_ = { 0.0f, 0.0f, 0.0f };
	PosTop_ = { 0.0f, 0.0f ,0.0f };//上
	PosDown_ = { 0.0f, 0.0f, 0.0f };//下
}

Capsule::Capsule(const Capsule& base, const Transform& parent) : transformParent_(parent)
{
	radius_ = base.GetRadius();
	localPosTop_ = base.GetLocalPosTop();
	localPosDown_ = base.GetLocalPosDown();
	PosTop_ = VAdd(transformParent_.pos, localPosTop_);//上
	PosDown_ = VAdd(transformParent_.pos, localPosDown_);//下
}

Capsule::~Capsule(void)
{
}

//更新
void Capsule::Update()
{
	PosTop_ = VAdd(transformParent_.pos, localPosTop_);//上
	PosDown_ = VAdd(transformParent_.pos, localPosDown_);//下
}

void Capsule::Draw(void)
{

	// 上の球体
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, COLOR, COLOR, false);

	// 下の球体
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// 球体を繋ぐ線(X+)
	dir = transformParent_.GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// 球体を繋ぐ線(X-)
	dir = transformParent_.GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// 球体を繋ぐ線(Z+)
	dir = transformParent_.GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// 球体を繋ぐ線(Z-)
	dir = transformParent_.GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, COLOR);

	// カプセルの中心
	DrawSphere3D(GetCenter(), 5.0f, 10, COLOR, COLOR, true);

}

VECTOR Capsule::GetLocalPosTop(void) const
{
	return localPosTop_;
}

VECTOR Capsule::GetLocalPosDown(void) const
{
	return localPosDown_;
}

void Capsule::SetLocalPosTop(const VECTOR& pos)
{
	localPosTop_ = pos;
}

void Capsule::SetLocalPosDown(const VECTOR& pos)
{
	localPosDown_ = pos;
}


VECTOR Capsule::GetPosTop(void) const
{
	return PosTop_;
}

VECTOR Capsule::GetPosDown(void) const
{
	return PosDown_;
}

VECTOR Capsule::GetRotPos(const VECTOR& localPos) const
{
	VECTOR localRotPos = transformParent_.quaRot.PosAxis(localPos);
	return VAdd(transformParent_.pos, localRotPos);
}

float Capsule::GetRadius(void) const
{
	return radius_;
}

void Capsule::SetRadius(float radius)
{
	radius_ = radius;
}

float Capsule::GetHeight(void) const
{
	return localPosTop_.y;
}

VECTOR Capsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}
