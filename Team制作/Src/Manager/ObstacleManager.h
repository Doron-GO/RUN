#pragma once
#include<DxLib.h>
#include<fstream>
#include<stdlib.h>
#include<memory>
#include<vector>
#include<string>
#include<map>
#include"../../Data/Json/json.hpp"
#include"../Object/Obstacle/Stoper.h"

enum class P_NUM
{
	MIN,
	P1,
	P2
};

class Input;
class Car;
class ObstacleBase;
class Sphere;
using Obstacles = std::vector<std::shared_ptr<ObstacleBase>>;
using ObstacleMap = std::map<P_NUM, Obstacles>;
using Json = nlohmann::json;

class ObstacleManager
{
public:

	enum class OBSTACLE_TYPE
	{
		WALL,
		CAR,
		MOVE
	};

	ObstacleManager(void);
	~ObstacleManager(void);
	void Init(void);
	void Update(Input& input);
	void Draw(void);
	void PushVector(std::shared_ptr<ObstacleBase> obst);
	const ObstacleMap& GetObstacleMap(void)const;
private:

	std::shared_ptr<ObstacleBase> ObstacleInstance(OBSTACLE_TYPE type);
	ObstacleMap obstaclesMap_;
	Json json_;

	void InitObject(std::string filename);
	void LoadObstacle(Json& stage,Json&obstaclename,OBSTACLE_TYPE type,Obstacles& obstacleList);

};

