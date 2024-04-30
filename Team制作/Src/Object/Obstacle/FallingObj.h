#pragma once
#include "ObstacleBase.h"

class ResourceManager;

class FallingObj :
    public ObstacleBase
{
public:
    FallingObj();

    virtual void Init()override;
    virtual void Update(Input& input)override;
    virtual void Draw()override;
private:
    void VectorTrans(VECTOR pos);
    Transform transform_;
    //std::vector<Transform>cars_;
    //std::array<VECTOR, 3>carsPos_;
    ResourceManager& resourceManager_;
};

