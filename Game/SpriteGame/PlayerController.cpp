#include "PlayerController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Core/Factory.h"
#include "Framework/Scene.h"
#include "Damager.h"
#include "Engine.h"
#include "SpriteGame.h"

FACTORY_REGISTER(PlayerController)

void PlayerController::Start()
{
	CharacterBase::Start();

	m_physicsComponent = GetComponent<nu::PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<nu::SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void PlayerController::Update(float dt)
{
	
	nu::Vector2 velocity = m_physicsComponent->GetVelocity();

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		float dir = 0.0f;
		if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir = -1.0f;
		if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir = +1.0f;
		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
			velocity.y = -800.0f;
		}

		if (dir != 0.0f) {
			velocity.x = dir * 100.0f;
			m_rendererComponent->Play("run");
			m_rendererComponent->SetFlipH(dir < 0.0f);
		}
		else {
			m_rendererComponent->Play("idle");
		}

		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_RSHIFT)) {
			m_state = State::Attack;
			m_rendererComponent->Play("attack");
			auto damager = nu::Factory::Instance().Create<nu::Actor>("DamagerPrototype");
			damager->SetPosition(GetTransform().position + nu::Vector2{(m_rendererComponent->GetFlipH()) ? -40.f : 40.0f, 0.0f});
			damager->SetTag("PlayerDamager");
			m_scene->AddActor(std::move(damager));
		}

	}
		break;
	case CharacterBase::State::Attack:
	{
		/*if (m_rendererComponent->GetFrame() == 4) {

		}*/

		if (m_rendererComponent->IsAnimationDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
	}
		break;
	case CharacterBase::State::Hit:
		break;
	case CharacterBase::State::Death:
		break;
	default:
		break;

	}

	

	m_physicsComponent->SetVelocity(velocity);


	nu::Engine::Get().GetRenderer().SetCamera(m_physicsComponent->GetPosition());

	CharacterBase::Update(dt);
}

void PlayerController::OnCollision(nu::Actor* other)
{
	if (nu::EqualsIgnoreCase(other->GetTag(), "EnemyDamager")) {
		Damager* damage = dynamic_cast<Damager*>(other);
		if (damage) {
			m_health -= damage->GetDamage();
			//((SpriteGame*)m_scene->GetGame())->SetPlayerHealth();
			int currentHealth = ((SpriteGame*)m_scene->GetGame())->GetPlayerHealth();
			((SpriteGame*)m_scene->GetGame())->SetPlayerHealth(currentHealth - damage->GetDamage());
		}
		if (m_health <= 0) {
			SetDestroyed();
			((SpriteGame*)m_scene->GetGame())->OnPlayerDead();
		}
	}
}

void PlayerController::Read(const nu::json::value_t& value)
{
	CharacterBase::Read(value);
}
