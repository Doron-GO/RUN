#include "ObstacleBase.h"

ObstacleBase::ObstacleBase()
{
}

void ObstacleBase::SetPos(VECTOR pos)
{
	transform_.pos = pos;
	transform_.Update();

}

void ObstacleBase::SetRot(VECTOR Rot)
{
	transform_.quaRot = Quaternion::Euler(
		{ AsoUtility::Deg2RadF(Rot.x),
			AsoUtility::Deg2RadF(Rot.y),
			 AsoUtility::Deg2RadF(Rot.z)});
	transform_.Update();
}
