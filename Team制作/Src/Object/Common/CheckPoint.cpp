#include "CheckPoint.h"

CheckPoint::CheckPoint(VECTOR min, VECTOR max, CURVE curve):
	min_(min),max_(max),curve_(curve), passageFlag_(false)
{
}

CheckPoint::~CheckPoint(void)
{
}
