#include "../Manager/ResourceManager.h"
#include "ActorBase.h"

ActorBase::ActorBase(void): resMng_(ResourceManager::GetInstance())
{
}

ActorBase::~ActorBase(void)
{
	transform_.Release();
}

const Transform& ActorBase::GetTransform(void) const
{
	return transform_;
}
