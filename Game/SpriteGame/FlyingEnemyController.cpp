#include "FlyingEnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Core/Factory.h"
#include "Framework/Scene.h"
#include "Engine.h"
#include "Damager.h"

FACTORY_REGISTER(FlyingEnemyController)

void FlyingEnemyController::Start()
{
	CharacterBase::Start();

	m_physicsComponent = GetComponent<nu::PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<nu::SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void FlyingEnemyController::Update(float dt)
{

	nu::Vector2 velocity = m_physicsComponent->GetVelocity();

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

			if (direction.Length() < 100.0f) {
				m_state = State::Attack;
				m_rendererComponent->Play("attack");

				auto damager = nu::Factory::Instance().Create<Damager>("DamagerPrototype");
				damager->SetDamage(3.0f);
				damager->SetPosition(GetTransform().position);
				damager->SetScale(3.0f);
				damager->SetTag("EnemyDamager");
				m_scene->AddActor(std::move(damager));
			}

			m_physicsComponent->ApplyForce(direction.Normalized() * 800.0f);
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


	

	CharacterBase::Update(dt);
}

void FlyingEnemyController::OnCollision(nu::Actor* other)
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
		}

		other->SetDestroyed();
	}
}

void FlyingEnemyController::Read(const nu::json::value_t& value)
{
	CharacterBase::Read(value);
}
