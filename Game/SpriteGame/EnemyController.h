#pragma once
#include "CharacterBase.h"

namespace nu {
	class PhysicsComponent;
	class SpriteAnimatorRendererComponent;
}

class EnemyController : public CharacterBase
{
public:
	CLASS_PROTOTYPE(EnemyController)

		void Start() override;
	void Update(float dt) override;

	void OnCollision(nu::Actor* other) override;

	void Read(const nu::json::value_t& value) override;

protected:
	class nu::PhysicsComponent* m_physicsComponent = nullptr;
	class nu::SpriteAnimatorRendererComponent* m_rendererComponent = nullptr;

private:
	float m_attackTimer = 0.0f;
	float m_attackCooldown = 3.0f;
};

