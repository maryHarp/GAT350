#include "pch.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"


namespace nu {
	bool ParticleSystem::Initialize(size_t size)
	{
		// reserve # particles in container
		m_particles.resize(size);
		m_texture = Resources().Get<Texture>("textures/SpaceCat.png", Engine::Get().GetRenderer());

		return true;
	}

	void ParticleSystem::Shutdown()
	{
		// clear all particles
		// TODO: clear() particles
		m_particles.clear();
	}

	void ParticleSystem::Update(float dt)
	{
		// update active particles
		for (auto& particle : m_particles)
		{
			if (!particle.active) continue;

			// reduce particle.lifespan by subtracting delta time
			// TODO: particle.lifespan -= delta time

			// set active if (particle.lifespan > 0);
			// TODO: particle.active = particle lifespan > 0

			// update position with velocity (multiply by dt)
			// TODO: update position += with particle velocity * delta time

			particle.lifespan -= dt;
			particle.active = (particle.lifespan > 0.0f);
			particle.position += particle.velocity * dt;
		}
	}

	void ParticleSystem::Draw(const Renderer& renderer)
	{
		// draw all active particlee
		for (auto& particle : m_particles)
		{
			if (particle.active)
			{
				// set particle color and draw point at current position
				// TODO: set color with particle color
				// TODO: draw point with particle position

				renderer.setColor(particle.color.r, particle.color.g, particle.color.b);
				renderer.DrawTexture(*m_texture, particle.position.x, particle.position.y, m_transform.rotation, 0.25f);

			}
		}
	}

	void ParticleSystem::AddParticle(const Particle& particle)
	{
		// get free particle
		Particle* freeParticle = GetFreeParticle();
		// check if free particle is not nullptr
		if (freeParticle)
		{

			*freeParticle = particle;
			freeParticle->active = true;
		}
	}

	Particle* ParticleSystem::GetFreeParticle()
	{
		// find and return inactive particle 
		for (auto& particle : m_particles)
		{
			// return pointer to inactive particle
			if (!particle.active)
				return &particle;
		}

		// no free particles, return nullptr
		return nullptr;
	}
}