#include "EnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Core/Factory.h"
#include "Framework/Scene.h"
#include "Engine.h"
#include "Damager.h"
#include "SpriteGame.h"

FACTORY_REGISTER(EnemyController)

void EnemyController::Start()
{
	CharacterBase::Start();

	m_physicsComponent = GetComponent<nu::PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<nu::SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void EnemyController::Update(float dt)
{

	nu::Vector2 velocity = m_physicsComponent->GetVelocity();

	if(m_attackTimer > 0.0f) {
		m_attackTimer -= dt;
	}

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		float dir = 0.0f;

		auto player = m_scene->GetActorByName("PlayerPrototype");
		if (player) {
			nu::Vector2 position = GetTransform().position;
			nu::Vector2 playerPosition = player->GetTransform().position;
			nu::Vector2 direction = playerPosition - position;

			m_rendererComponent->SetFlipH(direction.x < 0.0f);

			if (direction.Length() < 100.0f && m_attackTimer <= 0.0f) {
				m_state = State::Attack;
				m_rendererComponent->Play("attack");

				m_attackTimer = m_attackCooldown;

				auto damager = nu::Factory::Instance().Create<Damager>("DamagerPrototype");
				damager->SetDamage(1.0f);
				damager->SetPosition(GetTransform().position + nu::Vector2{ (m_rendererComponent->GetFlipH()) ? -30.f : 30.0f, 0.0f });
				damager->SetScale(1.0f);
				damager->SetTag("EnemyDamager");
				m_scene->AddActor(std::move(damager));
			}

			if (playerPosition.x < position.x) dir = -1.0f;
			else dir = 1.0f;
		}

		if (dir != 0.0f) {
			velocity.x = dir * 50.0f;
			m_rendererComponent->Play("run");
			m_rendererComponent->SetFlipH(dir < 0.0f);
		}
		else {
			m_rendererComponent->Play("idle");
		}



		
	}
		break;
	case CharacterBase::State::Attack:
		if (m_rendererComponent->IsAnimationDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	case CharacterBase::State::Hit:
		if (m_rendererComponent->IsAnimationDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	case CharacterBase::State::Death:
		break;
	default:
		break;
	}

	


	m_physicsComponent->SetVelocity(velocity);

	CharacterBase::Update(dt);
}

void EnemyController::OnCollision(nu::Actor* other)
{
	if (nu::EqualsIgnoreCase(other->GetTag(), "PlayerDamager")) {
		m_state = State::Hit;
		m_rendererComponent->Play("hit");
		Damager* damage = dynamic_cast<Damager*>(other);
		if (damage) {
			m_health -= damage->GetDamage();
		}
		if (m_health <= 0) {
			SetDestroyed();
			((SpriteGame*)m_scene->GetGame())->AddPoints(100);
			int currentEnemies = ((SpriteGame*)m_scene->GetGame())->GetEnemies() - 1;
			((SpriteGame*)m_scene->GetGame())->SetEnemies(currentEnemies);
		}
	}
}

void EnemyController::Read(const nu::json::value_t& value)
{
	CharacterBase::Read(value);
}
