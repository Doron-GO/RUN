#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// 推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	//static std::string PATH_IMG = Application::PATH_IMAGE;
	//static std::string PATH_MDL = Application::PATH_MODEL;
	//static std::string PATH_EFF = Application::PATH_EFFECT;
	const std::string PATH_IMG = "Data/Image/";
	const std::string PATH_MDL = "Data/Model/";
	const std::string PATH_EFF = "Data/Effect/";
	const std::string PATH_SHADER = "Data/Shader/";

	Resource* res;

	// タイトル画像
	res = new RES(RES_T::IMG, PATH_IMG + "TitleImage.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	//勝利画面
	res = new RES(RES_T::IMG, PATH_IMG + "VictoryBack.png");
	resourcesMap_.emplace(SRC::VICTORY_BACK, res);

	//Congratulations
	res = new RES(RES_T::IMG, PATH_IMG + "Congratulations.png");
	resourcesMap_.emplace(SRC::CONGRATULATIONS, res);

	//over
	res = new RES(RES_T::IMG, PATH_IMG + "over.png");
	resourcesMap_.emplace(SRC::OVER, res);

	//A
	res = new RES(RES_T::IMG, PATH_IMG + "A.png");
	resourcesMap_.emplace(SRC::A, res);

	// PushSpace
 	res = new RES (RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, res);

	// プレイヤー
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	// プレイヤー影
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	// スカイドーム
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);
	
	// ステージチップ
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/StageTip.mv1");
	resourcesMap_.emplace(SRC::STGECHIP, res);

	// ステージ	
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);

	// 足煙
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, res);


	// タンク
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Body.mv1");
	resourcesMap_.emplace(SRC::TANK_BODY, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Wheel.mv1");
	resourcesMap_.emplace(SRC::TANK_WHEEL, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Barrel.mv1");
	resourcesMap_.emplace(SRC::TANK_BARREL, res);

	//プレイヤーのHP
	res = new RES(Resource::TYPE::IMG, Application::PATH_IMAGE + "Heart.png");
	resourcesMap_.emplace(SRC::PLAYER_HP, res);

	//お邪魔ゲージ枠
	res = new RES(Resource::TYPE::IMG, Application::PATH_IMAGE + "OjamaGauge_case.png");
	resourcesMap_.emplace(SRC::OJAMAGAUGE_CASE, res);

	//お邪魔ゲージ
	res = new RES(Resource::TYPE::IMG, Application::PATH_IMAGE + "OjamaGauge.png");
	resourcesMap_.emplace(SRC::OJAMAGAUGE, res);

	
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/Obstacle/taxi.mv1");
	resourcesMap_.emplace(SRC::CAR, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/Obstacle/Stoper.mv1");
	resourcesMap_.emplace(SRC::STOPER, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/Obstacle/Rock.mv1");
	resourcesMap_.emplace(SRC::FALLING_OBJ, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Stage/Obstacle/Sphere/wooden_sphere.mv1");
	resourcesMap_.emplace(SRC::SPHERE, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
