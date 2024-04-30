#include "Stoper.h"
#include"../../Manager/SceneManager.h"

Stoper::Stoper()
{
	Init();
}

void Stoper::Init()
{
	
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::STOPER));
	transform_.scl = { 2.0f,2.0f ,2.0f };
	transform_.pos = { 4500.0f,2.0f,6860.0f };
	transform_.rot = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		{ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.quaRotLocal = Quaternion();
	transform_.MakeCollider(Collider::TYPE::OBSTACLE);
	transform_.Update();

}

void Stoper::Update(Input& input)
{
}

void Stoper::Draw()
{
 	MV1DrawModel(transform_.modelId);
}


