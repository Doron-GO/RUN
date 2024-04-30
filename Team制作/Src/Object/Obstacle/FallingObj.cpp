#include "FallingObj.h"
#include"../../Manager/ResourceManager.h"

FallingObj::FallingObj() :resourceManager_(ResourceManager::GetInstance())
{
	Init();
}

void FallingObj::Init()
{
}

void FallingObj::Update(Input& input)
{
}

void FallingObj::Draw()
{
}

void FallingObj::VectorTrans(VECTOR pos)
{
}
