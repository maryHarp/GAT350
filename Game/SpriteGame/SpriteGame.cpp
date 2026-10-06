#include "SpriteGame.h"
#include "Engine.h"
#include "Resources/ResourceManager.h"
#include <memory>
#include "Framework/Scene.h"


using namespace nu;
class Scene;

bool SpriteGame::Initialize() {

	SetWorkingDirectory("Defend the Village"); 

	Game::Initialize();

	m_scene = std::make_unique<nu::Scene>();
	m_scene->SetGame(this);
	m_scene->Load("scenes/scene.json");


	m_titleText = new Text(Resources().Get<Font>("fonts/font.ttf", 50.0f));
	m_titleText->Create(Engine::Get().GetRenderer(), "SpriteGame", Color{ 1, 1, 1 });

	m_gameText = new Text(Resources().Get<Font>("fonts/font.ttf", 10.0f));

	m_gameOverText = new Text(Resources().Get<Font>("fonts/font.ttf", 50.0f));

	//Engine::Get().GetAudio().AddSound("alert", "audio/alert.mp3");

	return true;
}


void SpriteGame::Update(float dt){
	switch (m_gameState) 
	{
	case SpriteGame::Title:
		//draw title

		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
			m_gameState = SpriteGame::StartGame;
		}
		break;
	case SpriteGame::StartGame:
		m_score = 0;
		m_playerHealth = 5;
		m_enemiesToSpawn = 3;
		m_enemiesLeft = 3;
		m_spawnTimer = 5.0f;
		m_stateTimer = 0.5f;
		m_gameState = SpriteGame::StartLevel;
		break;
	case SpriteGame::StartLevel:

		m_scene->RemoveAllActors();

		m_scene->Load("scenes/level.json");

		SpawnPlayer();
		SpawnEnemy();

		
		m_gameState = SpriteGame::Game;
		break;
		
	case SpriteGame::Game:
		//draw score / lives
		
		if (m_enemiesLeft == 0) {
			
			m_gameState = SpriteGame::Win;
			


		}





		break;
	case SpriteGame::GameOver:
		//draw game over
		if (m_stateTimer <= 0) {
			m_scene->RemoveAllActors();


		}
		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
			m_gameState = SpriteGame::Title;
		}

		break;
	case SpriteGame::Win:
		//draw game over
		if (m_stateTimer <= 0) {
			m_scene->RemoveAllActors();

		}
		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
			m_gameState = SpriteGame::Title;
		}

		break;

	}
	Game::Update(dt);
}

void SpriteGame::Draw(nu::Renderer& renderer) {

	renderer.EnableCamera(false);
	renderer.DrawTexture(*nu::Resources().Get<Texture>("textures/bg03.png", Engine::Get().GetRenderer()), 500, 500, 0, 1.0f);
	switch (m_gameState)
	{
	case SpriteGame::Title:
		m_titleText->Draw(renderer, 450, 450);
		break;
	case SpriteGame::StartGame:
		break;
	case SpriteGame::StartLevel:

		break;
	case SpriteGame::Game:
		m_gameText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
		m_gameText->Draw(renderer, 350, 30);
		m_gameText->Create(renderer, "Health: " + std::to_string(m_playerHealth), { 1.0f, 1.0f, 1.0f });
		m_gameText->Draw(renderer, 650, 30);
		
		break;
	case SpriteGame::GameOver:
		m_gameOverText->Create(renderer, "YOU DIED", { 1.0f, 0.0f, 0.0f });
		m_gameOverText->Draw(renderer, 450, 450);
		break;
	case SpriteGame::Win:
		m_gameOverText->Create(renderer, "YOU WIN!", { 0.0f, 1.0f, 0.0f });
		m_gameOverText->Draw(renderer, 450, 450);
		break;
	default:
		break;

	}

	renderer.EnableCamera(true);
	Game::Draw(renderer);
}

void SpriteGame::OnPlayerDead()

{

		m_gameState = SpriteGame::GameOver;
	

}

void SpriteGame::SpawnPlayer() {
	auto actor = Factory::Instance().Create<Actor>("PlayerPrototype");


	m_scene->AddActor(std::move(actor));
	


}

void SpriteGame::SpawnEnemy()
{
	for(int i = 0; i < m_enemiesToSpawn; i++)
	{
		auto actor = Factory::Instance().Create<Actor>("EnemyPrototype");
	actor->SetPosition({nu::RandomFloat(1024.0f), nu::RandomFloat(800.0f) });
	

	m_scene->AddActor(std::move(actor));
	}
	
	
	



}
