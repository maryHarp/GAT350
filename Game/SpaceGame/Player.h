#pragma once
#include "Framework/Actor.h"



struct PlayerDesc : public nu::ActorDesc {
    float speed = 0.0f;

};

class Player : public nu::Actor{
public:
    Player() = default;
    Player(const PlayerDesc& playerDesc) :
        Actor{ playerDesc },
        m_speed{ playerDesc.speed }

    {}

    CLASS_PROTOTYPE(Player)


    void Update(float dt) override;
    
    void OnCollision(Actor* other)override;

    bool IsInvincible() const { return m_invinibleTimer > 0.0f; }

    float GetSpeed() const { return m_speed;  }

    void Read(const nu::json::value_t& value) override;

private:
    float m_speed = 4000.0f;
    float m_invinibleTimer = 1.0f;
    float m_invinibleDuration = 2.0f;



    

};