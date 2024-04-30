#pragma once
#include<array>
#include<vector>
#include "ObstacleBase.h"
#include"../Common/Transform.h"

class Car :
    public ObstacleBase
{
public:

    static constexpr float CAR_MOVE_MAX = 100.0f;
    static constexpr float CAR_MOVE_MIN = -100.0f;
    static constexpr VECTOR CAR_MOVE_MAXV = {0.0f,0.0f,500.0f };
    static constexpr VECTOR CAR_MOVE_MINV = { 0.0f,0.0f,-500.0f };
    static constexpr float SPEED = 2.5f;
    static constexpr float TIME = 0.6f;


    Car();

    virtual void Init()override;
    virtual void Update(Input& input)override;
    virtual void Draw()override;
private:
    float speed_;
    float time_;
    std::vector<Transform>cars_;
    std::vector<Collider> colliders_;
    std::array<VECTOR,3>carsPos_;

};

