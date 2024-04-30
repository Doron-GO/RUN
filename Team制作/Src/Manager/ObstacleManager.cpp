#include "ObstacleManager.h"
#include"../Input/Input.h"
#include"../Object/Obstacle/Car.h"
#include"../Object/Obstacle/Sphere.h"

ObstacleManager::ObstacleManager()
{
	InitObject("Data/Json/obstacleList_.json");
}

ObstacleManager::~ObstacleManager()
{
}

void ObstacleManager::Init()
{
	//PushVector(std::make_shared<Car>());
	//PushVector(std::make_shared<Stoper>());
	//PushVector(std::make_shared<Sphere>());
}

void ObstacleManager::Update(Input& input)
{
	for (auto& obstacles : obstaclesMap_)
	{
		for (auto& obstacle : obstacles.second)
		{
			obstacle->Update(input);
		}
	}

}

void ObstacleManager::Draw()
{
	for (auto& obstacles : obstaclesMap_)
	{
		for (auto& obstacle  :obstacles.second)
		{
 			obstacle->Draw();
		}
	}
}


void ObstacleManager::PushVector(std::shared_ptr<ObstacleBase> obst)
{
}

const ObstacleMap& ObstacleManager::GetObstacleMap(void) const
{
	return obstaclesMap_;
}

std::shared_ptr<ObstacleBase> ObstacleManager::ObstacleInstance(OBSTACLE_TYPE type)
{
	if (type== OBSTACLE_TYPE::WALL)
	{
		std::shared_ptr<Stoper> stopper = std::make_shared<Stoper>();
		return stopper;
	}
	else if (type == OBSTACLE_TYPE::CAR)
	{
		std::shared_ptr<Car> car = std::make_shared<Car>();
		return car;
	}
}



void ObstacleManager::LoadObstacle(Json& stage,
	Json& obstaclename,OBSTACLE_TYPE type,Obstacles& obstacleList)
{
	int num = (int)type;
	std::string name = obstaclename["name"];
	for (auto& obstacle : stage[name.c_str()])
	{
		auto ins= ObstacleInstance(type);
		float x = obstacle["x"].get<float>();
		float y = obstacle["y"].get<float>();
		float z = obstacle["z"].get<float>();
		float rotx = obstacle["rotx"].get<float>();
		float roty = obstacle["roty"].get<float>();
		float rotz = obstacle["rotz"].get<float>();
		ins->SetPos(VECTOR{ x,y,z });
		ins->SetRot(VECTOR{ rotx,roty,rotz });
		obstacleList.push_back(ins);
	}
}

void ObstacleManager::InitObject(std::string filename)
{
	std::ifstream f(filename.c_str());
	json_ = Json::parse(f);
	auto& stages = json_["stage"];
	Obstacles obstacle;
	auto Load=[this](Json& stages, Obstacles obstacle, ObstacleMap& obstaclemap)
	{
		int stageNum = 0;
		for(auto& stage : stages)
		{
			int typeNum = 0;
			auto& obstaclenames = stage["obstaclename"];
			for (auto& obstaclename: obstaclenames)
			{
				stageNum = stage["num"].get<int>();
				LoadObstacle(stage, obstaclename,(OBSTACLE_TYPE)typeNum, obstacle);
				typeNum++;
			}
			obstaclemap[(P_NUM)stageNum] = (obstacle);
		}
	};
	Load(stages, obstacle, obstaclesMap_);
}
