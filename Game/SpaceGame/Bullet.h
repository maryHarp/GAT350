#pragma once
#include "Framework/Actor.h"


struct BulletDesc : public nu::ActorDesc {
    float speed = 400.0F;

};


class Bullet : public nu::Actor {
public:
    Bullet() = default;
    Bullet(const BulletDesc& BulletDesc) :
        Actor(BulletDesc),
        m_speed{BulletDesc.speed}
    {
    }

    CLASS_PROTOTYPE(Bullet)
    
    void Update(float dt) override;

    void Read(const nu::json::value_t& value) override;

private:
    int m_ammo = 0;
    float m_speed = 800.0f;

}; 
