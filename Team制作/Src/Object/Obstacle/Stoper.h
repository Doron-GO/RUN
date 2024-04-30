#pragma once
#include "ObstacleBase.h"


class Stoper :
    public ObstacleBase
{
public:
    Stoper();

    virtual void Init()override;
    virtual void Update(Input& input)override;
    virtual void Draw()override;
private:
    std::vector<Transform>stopers_;
    std::vector<Collider> colliders_;
};

