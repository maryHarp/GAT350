#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"
#include "Resources/Resource.h"

class SpriteGame : public nu::Game
{
public:
	enum GameState {
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver,
		Win
	};
		

public:
	SpriteGame() = default;
	SpriteGame(nu::Scene* scene) : nu::Game(scene)
	{ }

	bool Initialize() override;

	void Update(float dt) override;
	void Draw(class nu::Renderer& renderer) override;

	void OnPlayerDead();

	void AddPoints(int points) { m_score += points; }
	
	int GetScore() const { return m_score; }

	int GetEnemies() const { return m_enemiesLeft; }
	void SetEnemies(int enemies) { m_enemiesLeft = enemies;  }

	int GetPlayerHealth() const { return m_playerHealth; }
	void SetPlayerHealth(int health) { m_playerHealth = health;  }




private:
	void SpawnPlayer();
	void SpawnEnemy();


	int m_score{ 0 };
	int m_playerHealth{ 0 };
	int m_enemiesToSpawn = 5;
	int m_enemiesLeft = 0;
	//int m_maxEnemies = 0;
	int m_coinsToSpawn = 10;
	int m_coinsLeft = 0;

	float m_stateTimer = 0.0f;

	float m_spawnTimer = 0.0f; 
	float m_spawnTime = 5.0f;
	int m_spawnCount = 0;

	

	GameState m_gameState = Title;

	nu::res_t<nu::Font> m_titleFont;
	nu::res_t<nu::Font> m_gameFont;
	nu::res_t<nu::Font> m_gameOverFont;

	//nu::Font* m_titleFont{ nullptr };
	nu::Text* m_titleText{ nullptr };

	//nu::Font* m_gameOverFont{ nullptr };
	nu::Text* m_gameOverText{ nullptr };
	nu::Text* m_gameText{ nullptr };

	//nu::Font* m_gameFont{ nullptr };

	//nu::Text* m_scoreText{ nullptr };
	//nu::Text* m_livesText{ nullptr };
	//nu::Text* m_roundsText{ nullptr };
};
