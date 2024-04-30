#pragma once
#include<memory>
#include "ObstacleBase.h"

class Stage;
//class Player;

class Sphere :
    public ObstacleBase
{
public:

    Sphere();
    virtual void Init()override;
    virtual void Update(Input& input)override;
    virtual void Draw()override;
private:
    Transform transform_;
    float time_;
   // std::shared_ptr<Player> player_;
   // std::shared_ptr<Stage> stage_;
    std::vector<Collider> colliders_;
    const VECTOR relativePos = {0.0f,100.0f,0.0f};
protected:

};

