#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "Core/Factory.h"
#include "Renderer/Renderer.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Renderer/Light.h"

namespace nu {

	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{ 
		actor->m_scene = this;
		m_pendingActors.push_back(std::move(actor)); 
	}

	void Scene::RemoveAllActors(bool force)
	{
		
		std::erase_if(m_actors, [force](auto& actor) {return !actor->GetPersistent() || force; });

	}

	bool Scene::Load(const std::string& sceneName)
	{
		json::document_t document;
	if (json::Load(sceneName, document)) {

		if (JSON_HAS_NAME(document, "actors")) {
			for (auto& actorValue : JSON_GET_NAME(document, "actors").GetArray()) {
				std::string typeName;
				JSON_READ_NAME(actorValue, "type", typeName);

				auto actor = Factory::Instance().Create<Actor>(typeName);
				actor->Read(actorValue);

				bool prototype = false;
				JSON_READ(actorValue, prototype);


				if (prototype) {
					std::string name;
					JSON_READ(actorValue, name);
					Factory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
				}
				else {
					AddActor(std::move(actor));
				}
			}
		}
		//std::string type;
		//JSON_READ(document, type);
		//auto actor = Factory::Instance().Create<Actor>("Player");
		//actor->Read(document);
		//Factory::Instance().RegisterPrototype<Actor>("PlayerPrototype", std::move(actor));
	}
	else {
		return false;
		}
		return true;
	}

	void Scene::Update(float dt) {
		for (auto& actor : m_actors) {
			actor->Update(dt);
		}

		//UpdateCollisions();

		//remove destoryed actors
		for (auto& actor : m_pendingActors) {
			if (actor->m_destroyed) actor->OnDestroy();
		}
		std::erase_if(m_actors, [](auto& actor) {return actor->m_destroyed; });


		//add pending actors
		for (auto& actor : m_pendingActors) {
			actor->Start();
			m_actors.push_back(std::move(actor));
		}
		m_pendingActors.clear();
	}

	void Scene::Draw(const Renderer& renderer) {
		FrameUniform frameUniform{};

		auto camera = GetActorByName("Camera");
		if (camera) {
			auto cameraComponent = camera->GetComponent<CameraComponent>();
			if (cameraComponent) {
				frameUniform.cameraPosition = camera->GetTransform().position;
				frameUniform.view = cameraComponent->GetView();
				frameUniform.projection = cameraComponent->GetProjection();
				frameUniform.viewProjection = frameUniform.projection * frameUniform.view;

				renderer.setVertexUniform(UniformSlot::Frame, &frameUniform, sizeof(frameUniform));
				renderer.setFragmentUniform(UniformSlot::Frame, &frameUniform, sizeof(frameUniform));
			}
		}

		LightUniform lightUniform{};
		lightUniform.ambient = glm::vec3{ 0.2f };

		auto lights = GetActorByTag("Light");
		for (auto light : lights)
		{
			auto lightComponent = light->GetComponent<LightComponent>();
			if (lightComponent)
			{
				lightUniform.lights[lightUniform.numLights++] = lightComponent->GetLightData();
			}
		}

		renderer.setVertexUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));
		renderer.setFragmentUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));


		for (const auto& actor : m_actors) {
			if(actor && actor->IsActive())
			actor->Draw(renderer);
		}
	}
	
}