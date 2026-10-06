#include "pch.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "Bullet.h"
#include "Renderer/Model.h"

#include "Resources/ResourceManager.h"
#include "Components/PhysicsComponent.h"

#include "SpaceGame.h"

FACTORY_REGISTER(Player)

void Player::Update(float dt) {
    
    //invincibility 
    if (m_invinibleTimer > 0.0f) {
        m_invinibleTimer -= dt;
    }

    //movement
    float thrust = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) {
        thrust = m_speed;
    }
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) {
        thrust = -m_speed;
    }

    float rotate = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate = -40.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate = +40.0f;

    auto physicsComponent = GetComponent<nu::PhysicsComponent>();
    if (physicsComponent) {
        nu::Vector2 forward{1, 0}; // -> 
        nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;

        physicsComponent->ApplyForce(force);
        physicsComponent->ApplyTorque(rotate);

        nu::Vector2 position = physicsComponent->GetPosition();
        /*position.x = nu::Wrap(0.0f, 1280.0f, position.x);
        position.y = nu::Wrap(0.0f, 1024.0f, position.y);
        physicsComponent->SetPosition(position);*/

        nu::Engine::Get().GetRenderer().SetCamera(position);

    }

    //particle
    if (thrust) {
        nu::Particle particle;
        particle.position = m_transform.position;
        //particle.color = { 1.0f, 1.0f, 1.0f };
        particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
        particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-200.0f, 200.0f) };

        nu::Engine::Get().GetPS().AddParticle(particle);
    }

    //fire
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) {

        auto bullet = nu::Factory::Instance().Create<Bullet>("BulletPrototype");
        bullet->SetTransform(m_transform);
        bullet->SetScale(2.0f);
        bullet->SetTag("PlayerBullet");

        m_scene->AddActor(std::move(bullet));

        

        
    }

    //bullet time
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_X)) {
        nu::Engine::Get().GetTime().SetTimeScale(0.5f);
    }
    else {
        nu::Engine::Get().GetTime().SetTimeScale(1.0f);
    }



    Actor::Update(dt);
}

void Player::OnCollision(Actor* other)
{
    return; //dont die!!

    if (IsInvincible()) {
        return;
    }


    if (other->GetTag() == "Enemy") {
        SetDestroyed();
        ((SpaceGame*)m_scene->GetGame())->OnPlayerDead();
    }
}

void Player::Read(const nu::json::value_t& value)
{
    Actor::Read(value);

    JSON_READ_NAME(value, "speed", m_speed);
}
