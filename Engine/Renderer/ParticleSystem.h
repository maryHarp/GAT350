#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Resources/ResourceManager.h"
#include "Math/Transform.h"
#include "Resources/Resource.h"

#include <vector>
#include <memory>


namespace nu {
	class Texture;

	struct Particle
	{
		bool active{ false };
		float lifespan{ 1 };

		Vector2 position{ 0, 0 };
		Vector2 velocity{ 0, 0 };
		Color color{ 0, 0, 0 };
		//res_t<Texture> 
	};

	class ParticleSystem
	{
	public:
		ParticleSystem() = default;

		bool Initialize(size_t poolSize = 1000);
		void Shutdown();

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		void AddParticle(const Particle& particle);

	private:
		Particle* GetFreeParticle();

	private:
		// store particles in particle pool
		res_t<Texture> m_texture;

		Transform m_transform;

		std::vector<Particle> m_particles;
	};

	
}