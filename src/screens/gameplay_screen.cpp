#include "screens/gameplay_screen.h"

#include <stdlib.h>

#include "raylib.h"

#include "sprite/sprite.h"
#include "entities/nave.h"
#include "entities/projectile.h"
#include "entities/specimen.h"
#include "entities/spore.h"
#include "entities/fungus.h"
#include "collision/collisions.h"
#include "game/game.h"
#include "panels/pause_panel.h"
#include "panels/game_over_panel.h"
#include "panels/spore_panel.h"
#include "utilities/math_utils.h"
#include "game/game_constants.h"
#include "interface/button.h"
#include "audio/audio_manager.h"
#include "interface/ui_constants.h"

using namespace Collisions;
using namespace MathUtils;

namespace Gameplay
{
	static const float BACKGROUND_MOVE_SPEED_X = 1.5f;
	static const float BACKGROUND_MOVE_SPEED_Y = 1.2f;
	static const float BACKGROUND_MOVE_AMPLITUDE_X = 15.0f;
	static const float BACKGROUND_MOVE_AMPLITUDE_Y = 8.0f;
	static const Color BACKGROUND_OVERLAY_COLOR = { 0, 0, 0, 100 };

	static float backgroundTime = 0.0f;
	static float backgroundOffsetX = 0.0f;
	static float backgroundOffsetY = 0.0f;

	static Sprite::Sprite backgroundOne{};
	static Sprite::Sprite backgroundTwo{};

	static Nave::Nave nave;

	static const int MAX_PROJECTILE = 20;
	static Projectile::Projectile projectiles[MAX_PROJECTILE] = {};

	static const int DIVIDE_SPECIMENS = 2;
	static const int INITIAL_SPECIMENS = 10;
	static const int SPECIMENS_PER_SPAWN = 5;
	static const int MAX_SAFE_SPAWN = 45;
	static const int MAX_SPECIMENS = 200;
	static Specimen::Specimen specimens[MAX_SPECIMENS];

	static const int MAX_SPORES = 5;
	static Spore::Spore spores[MAX_SPORES];

	static const int MAX_FUNGI = 10;
	static Fungus::Fungus fungi[MAX_FUNGI];

	static float deltaTime;

	static bool isAccelerating;
	static float timeSinceLastShot;

	static float specimensSpawnTimer;
	static const float SPECIMEN_SPAWN_INTERVAL = 8.5f;

	static float sporesSpawnTimer;
	static const float SPORES_SPAWN_INTERVAL = 12.5f;

	static float fungiSpawnTimer;
	static const float FUNGI_SPAWN_INTERVAL = 3.5f;

	static const float BUTTON_SIZE = 65;
	static const float BUTTON_MARGIN = 15;
	static Button::Button pauseButton;

	// BACKGROUND FUNCTIONS

	static void InitBackground();
	static void UpdateBackground();
	static void DrawBackground();
	static void CloseBackground();

	// FUNCTIONS OF PROJECTILES

	static void CreateProjectile();
	static void UpdateAllProjectiles();
	static void DrawAllProjectiles();

	// FUNCTIONS OF SPECIMENS

	static void UpdateAllSpecimens();
	static void DrawAllSpecimens();

	// FUNCTIONS OF SPORES

	static void UpdateAllSpores();
	static void DrawAllSpores();

	// FUNCTIONS OF FUNGU

	static void UpdateAllFungi();
	static void DrawAllFungi();

	// FUNCTIONS TO DETECT COLLISIONS BETWEEN ENTITIES

	static void HandleProjectileSpecimenCollisions();
	static void HandleNaveSpecimenCollisions();
	static void HandleSpecimenDivision(Specimen::Specimen& specimen);
	static void SplitSpecimen(Specimen::Specimen& specimen, Specimen::Type type);
	static int GetEmptyIndexSpecimens();
	static void HandleNaveSporeCollision();
	static void HandleProjectileFungusCollisions();
	static void HandleNaveFungusCollisions();

	// SPAWNEO FUNCTIONS

	static void HandleSpawningSpecimens();
	static int GetActiveSpecimens();
	static void SpawnSpecimenWave();

	static void HandleSpawningSpores();
	static int GetActiveSpores();
	static void SpawnSpore();
	static int GetEmptyIndexSpore();

	static void HandleSpawningFungi();
	static int GetActiveFungi();
	static void SpawnFungus();
	static int GetEmptyIndexFungus();

	// FUNCTION BUTTON

	static void InitButton();
	static void UpdateButton();
	static void DrawButton();

	// INTERFACE FUNCTIONS

	static void DrawInterface();

	void Init()
	{
		InitBackground();

		isAccelerating = false;
		timeSinceLastShot = 0.0f;

		specimensSpawnTimer = 0.0f;

		Nave::Init();
		Projectile::Init();
		Specimen::Init();
		Spore::Init();
		Fungus::Init();

		InitButton();

		PausePanel::Init();
		GameOverPanel::Init();
		SporePanel::Init();

		nave = Nave::Create();

		CreateInitialSpecimen();
	}

	void Input()
	{
		if (nave.isActive)
		{
			if (IsKeyPressed(KEY_ESCAPE) && !SporePanel::isActive)
			{
				PausePanel::isActive = !PausePanel::isActive;
				AudioManager::PlaySfx(AudioManager::SfxID::SFX_PANEL);
			}

			if (!PausePanel::isActive && !SporePanel::isActive)
			{
				isAccelerating = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && timeSinceLastShot >= nave.fireRate)
				{
					CreateProjectile();
					AudioManager::PlaySfx(AudioManager::SfxID::SFX_NAVE_SHOOT);
					timeSinceLastShot = 0.0f;
				}
			}
		}
	}

	void Update()
	{
		deltaTime = GetFrameTime();

		if (!PausePanel::isActive && !SporePanel::isActive && nave.isActive)
		{
			timeSinceLastShot += deltaTime;

			UpdateBackground();

			Nave::Update(nave, deltaTime, isAccelerating);
			UpdateAllProjectiles();
			UpdateAllSpecimens();
			UpdateAllSpores();
			UpdateAllFungi();

			GameOverPanel::isActive = !nave.isActive;

			HandleProjectileSpecimenCollisions();
			HandleNaveSpecimenCollisions();
			HandleNaveSporeCollision();
			HandleProjectileFungusCollisions();
			HandleNaveFungusCollisions();

			HandleSpawningSpecimens();
			HandleSpawningSpores();
			HandleSpawningFungi();

			UpdateButton();
		}

		PausePanel::Update();
		GameOverPanel::Update();
		SporePanel::Update(nave);
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawBackground();

		Nave::Draw(nave);
		DrawAllProjectiles();
		DrawAllSpecimens();
		DrawAllSpores();
		DrawAllFungi();

		DrawButton();
		DrawInterface();

		PausePanel::Draw();
		GameOverPanel::Draw(nave);
		SporePanel::Draw();

		EndDrawing();
	}

	void Close()
	{
		CloseBackground();

		Nave::Close();
		Projectile::Close();
		Specimen::Close();
		Spore::Close();
		Fungus::Close();

		PausePanel::Close();
		GameOverPanel::Close();
		SporePanel::Close();
	}

	static void InitBackground()
	{
		backgroundOne.texture = LoadTexture("res/textures/backgrounds/gameplay_01.png");
		backgroundOne.position = { 0.0f, 0.0f };
		backgroundOne.tint = WHITE;


		backgroundTwo.texture = LoadTexture("res/textures/backgrounds/gameplay_02.png");
		backgroundTwo.position = { 0.0f, 0.0f };
		backgroundTwo.tint = WHITE;
	}

	static void UpdateBackground()
	{
		backgroundTime += deltaTime;

		backgroundOffsetX = sinf(backgroundTime * BACKGROUND_MOVE_SPEED_X) * BACKGROUND_MOVE_AMPLITUDE_X;
		backgroundOffsetY = cosf(backgroundTime * BACKGROUND_MOVE_SPEED_Y) * BACKGROUND_MOVE_AMPLITUDE_Y;
	}

	static void DrawBackground()
	{
		DrawTexture(backgroundOne.texture, 0, 0, backgroundOne.tint);

		float baseX = static_cast<float>(SCREEN_WIDTH) / 2.0f - static_cast<float>(backgroundTwo.texture.width) / 2.0f;
		float baseY = static_cast<float>(SCREEN_HEIGHT) / 2.0f - static_cast<float>(backgroundTwo.texture.height) / 2.0f;

		DrawTexture(backgroundTwo.texture, static_cast<int>(baseX + backgroundOffsetX), static_cast<int>(baseY + backgroundOffsetY), backgroundTwo.tint);

		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BACKGROUND_OVERLAY_COLOR);
	}

	static void CloseBackground()
	{
		UnloadTexture(backgroundOne.texture);
		UnloadTexture(backgroundTwo.texture);
	}

	void CreateInitialSpecimen()
	{
		for (int i = 0; i < INITIAL_SPECIMENS; i++)
		{
			int sideRandom = rand() % static_cast<int>(Specimen::SpawnSide::Left) + static_cast<int>(Specimen::SpawnSide::Top);
			int typeRandom = rand() % static_cast<int>(Specimen::Type::Small) + static_cast<int>(Specimen::Type::Big);

			Specimen::SpawnSide side = static_cast<Specimen::SpawnSide>(sideRandom);
			Specimen::Type type = static_cast<Specimen::Type>(typeRandom);

			specimens[i] = Specimen::SpawnAtSide(side, type);
		}
	}

	void Reset()
	{
		Nave::Reset(nave);

		for (int i = 0; i < MAX_PROJECTILE; i++)
		{
			Projectile::Reset(projectiles[i]);
		}

		for (int i = 0; i < MAX_SPECIMENS; i++)
		{
			Specimen::Reset(specimens[i]);
		}

		for (int i = 0; i < MAX_SPORES; i++)
		{
			Spore::Reset(spores[i]);
		}

		for (int i = 0; i < MAX_FUNGI; i++)
		{
			Fungus::Reset(fungi[i]);
		}

		CreateInitialSpecimen();
	}

	static void CreateProjectile()
	{
		for (int i = 0; i < MAX_PROJECTILE; i++)
		{
			if (!projectiles[i].isActive)
			{
				projectiles[i] = Projectile::Create(nave);
				break;
			}
		}
	}

	static void UpdateAllProjectiles()
	{
		for (int i = 0; i < MAX_PROJECTILE; i++)
		{
			Projectile::Update(projectiles[i], deltaTime);
		}
	}

	static void DrawAllProjectiles()
	{
		for (int i = 0; i < MAX_PROJECTILE; i++)
		{
			Projectile::Draw(projectiles[i]);
		}
	}

	static void UpdateAllSpecimens()
	{
		for (int i = 0; i < MAX_SPECIMENS; i++)
		{
			Specimen::Update(specimens[i], deltaTime);
		}
	}

	static void DrawAllSpecimens()
	{
		for (int i = 0; i < MAX_SPECIMENS; i++)
		{
			Specimen::Draw(specimens[i]);
		}
	}

	static void UpdateAllSpores()
	{
		for (int i = 0; i < MAX_SPORES; i++)
		{
			Spore::Update(spores[i], deltaTime);
		}
	}

	static void DrawAllSpores()
	{
		for (int i = 0; i < MAX_SPORES; i++)
		{
			Spore::Draw(spores[i]);
		}
	}

	static void UpdateAllFungi()
	{
		for (int i = 0; i < MAX_FUNGI; i++)
		{
			Fungus::Update(fungi[i], deltaTime, nave);
		}
	}

	static void DrawAllFungi()
	{
		for (int i = 0; i < MAX_FUNGI; i++)
		{
			Fungus::Draw(fungi[i]);
		}
	}

	static void HandleProjectileSpecimenCollisions()
	{
		for (int i = 0; i < MAX_PROJECTILE; i++)
		{
			if (!projectiles[i].isActive)
			{
				continue;
			}

			for (int j = 0; j < MAX_SPECIMENS; j++)
			{
				if (!specimens[j].isActive)
				{
					continue;
				}

				if (CheckCircleCollision(projectiles[i].x, projectiles[i].y, projectiles[i].radius, specimens[j].x, specimens[j].y, specimens[j].radius))
				{
					projectiles[i].isActive = false;

					Nave::AddScore(nave);

					HandleSpecimenDivision(specimens[j]);

					AudioManager::PlaySfx(AudioManager::SfxID::SFX_ENTITY_HIT);

					break;
				}
			}
		}
	}

	static void HandleNaveSpecimenCollisions()
	{
		for (int i = 0; i < MAX_SPECIMENS; i++)
		{
			if (!specimens[i].isActive)
			{
				continue;
			}

			if (CheckCircleCollision(nave.x, nave.y, nave.radius, specimens[i].x, specimens[i].y, specimens[i].radius))
			{
				specimens[i].isActive = false;

				if (!nave.isImmune)
				{
					Nave::TakeDamage(nave);

					AudioManager::PlaySfx(AudioManager::SfxID::SFX_ENTITY_HIT);
				}

				break;
			}
		}
	}

	static void HandleSpecimenDivision(Specimen::Specimen& specimen)
	{
		switch (specimen.type)
		{
		case Specimen::Type::Big:

			SplitSpecimen(specimen, Specimen::Type::Medium);

			break;
		case Specimen::Type::Medium:

			SplitSpecimen(specimen, Specimen::Type::Small);

			break;
		case Specimen::Type::Small:

			specimen.isActive = false;

			break;
		default:

			// THERE ARE NO MORE TYPES OF SPECIMENS

			break;
		}
	}

	static void SplitSpecimen(Specimen::Specimen& specimen, Specimen::Type type)
	{
		for (int i = 0; i < DIVIDE_SPECIMENS; i++)
		{
			int emptyIndex = GetEmptyIndexSpecimens();
			float velocityX = GetFloatRandomBetween(Specimen::VELOCITY_MIN, Specimen::VELOCITY_MAX);
			float velocityY = GetFloatRandomBetween(Specimen::VELOCITY_MIN, Specimen::VELOCITY_MAX);

			velocityX *= (rand() % 2 == 0) ? 1.0f : -1.0f;
			velocityY *= (rand() % 2 == 0) ? 1.0f : -1.0f;

			specimens[emptyIndex] = Specimen::Create(specimen.x, specimen.y, velocityX, velocityY, type);
		}

		specimen.isActive = false;
	}

	static int GetEmptyIndexSpecimens()
	{
		for (int i = 0; i < MAX_SPECIMENS; i++)
		{
			if (!specimens[i].isActive)
			{
				return i;
			}
		}

		return 0;
	}

	static void HandleNaveSporeCollision()
	{
		for (int i = 0; i < MAX_SPORES; i++)
		{
			if (!spores[i].isActive)
			{
				continue;
			}

			if (CheckCircleCollision(nave.x, nave.y, nave.radius, spores[i].x, spores[i].y, spores[i].radius))
			{
				spores[i].isActive = false;
				SporePanel::isActive = true;

				AudioManager::PlaySfx(AudioManager::SfxID::SFX_PANEL);
			}
		}
	}

	static void HandleProjectileFungusCollisions()
	{
		for (int i = 0; i < MAX_PROJECTILE; i++)
		{
			if (!projectiles[i].isActive)
			{
				continue;
			}

			for (int j = 0; j < MAX_FUNGI; j++)
			{
				if (!fungi[j].isActive)
				{
					continue;
				}

				if (CheckCircleCollision(projectiles[i].x, projectiles[i].y, projectiles[i].radius, fungi[j].x, fungi[j].y, fungi[j].radius))
				{
					projectiles[i].isActive = false;

					Nave::AddScore(nave);

					fungi[j].isActive = false;

					AudioManager::PlaySfx(AudioManager::SfxID::SFX_ENTITY_HIT);

					break;
				}
			}
		}
	}

	static void HandleNaveFungusCollisions()
	{
		for (int i = 0; i < MAX_FUNGI; i++)
		{
			if (!fungi[i].isActive)
			{
				continue;
			}

			if (CheckCircleCollision(nave.x, nave.y, nave.radius, fungi[i].x, fungi[i].y, fungi[i].radius))
			{
				fungi[i].isActive = false;

				if (!nave.isImmune)
				{
					Nave::TakeDamage(nave);

					AudioManager::PlaySfx(AudioManager::SfxID::SFX_ENTITY_HIT);
				}

				break;
			}
		}
	}

	void HandleSpawningSpecimens()
	{
		specimensSpawnTimer += deltaTime;

		if (specimensSpawnTimer >= SPECIMEN_SPAWN_INTERVAL && GetActiveSpecimens() <= MAX_SAFE_SPAWN)
		{
			SpawnSpecimenWave();
			specimensSpawnTimer = 0.0f;
		}
	}

	static int GetActiveSpecimens()
	{
		int counter = 0;

		for (int i = 0; i < MAX_SPECIMENS; i++)
		{
			if (specimens[i].isActive)
			{
				counter++;
			}
		}

		return counter;
	}

	static void SpawnSpecimenWave()
	{
		for (int i = 0; i < SPECIMENS_PER_SPAWN; i++)
		{
			int emptyIndex = GetEmptyIndexSpecimens();

			int sideRandom = rand() % static_cast<int>(Specimen::SpawnSide::Left) + static_cast<int>(Specimen::SpawnSide::Top);
			int typeRandom = rand() % static_cast<int>(Specimen::Type::Small) + static_cast<int>(Specimen::Type::Big);

			Specimen::SpawnSide side = static_cast<Specimen::SpawnSide>(sideRandom);
			Specimen::Type type = static_cast<Specimen::Type>(typeRandom);

			specimens[emptyIndex] = Specimen::SpawnAtSide(side, type);
		}
	}

	static void HandleSpawningSpores()
	{
		sporesSpawnTimer += deltaTime;

		if (sporesSpawnTimer >= SPORES_SPAWN_INTERVAL && GetActiveSpores() < MAX_SPORES)
		{
			SpawnSpore();
			sporesSpawnTimer = 0.0f;
		}
	}

	static int GetActiveSpores()
	{
		int counter = 0;

		for (int i = 0; i < MAX_SPORES; i++)
		{
			if (spores[i].isActive)
			{
				counter++;
			}
		}

		return counter;
	}

	static void SpawnSpore()
	{
		int emptyIndex = GetEmptyIndexSpore();

		int sideRandom = rand() % static_cast<int>(Spore::SpawnSide::Left) + static_cast<int>(Spore::SpawnSide::Top);
		Spore::SpawnSide side = static_cast<Spore::SpawnSide>(sideRandom);

		spores[emptyIndex] = Spore::SpawnAtSide(side);
	}

	static int GetEmptyIndexSpore()
	{
		for (int i = 0; i < MAX_SPORES; i++)
		{
			if (!spores[i].isActive)
			{
				return i;
			}
		}

		return 0;
	}

	static void HandleSpawningFungi()
	{
		fungiSpawnTimer += deltaTime;

		if (fungiSpawnTimer >= FUNGI_SPAWN_INTERVAL && GetActiveFungi() < MAX_FUNGI)
		{
			SpawnFungus();
			fungiSpawnTimer = 0.0f;
		}
	}

	static int GetActiveFungi()
	{
		int counter = 0;

		for (int i = 0; i < MAX_FUNGI; i++)
		{
			if (fungi[i].isActive)
			{
				counter++;
			}
		}

		return counter;
	}

	static void SpawnFungus()
	{
		int emptyIndex = GetEmptyIndexFungus();

		int sideRandom = rand() % static_cast<int>(Fungus::SpawnSide::Left) + static_cast<int>(Fungus::SpawnSide::Top);
		Fungus::SpawnSide side = static_cast<Fungus::SpawnSide>(sideRandom);

		fungi[emptyIndex] = Fungus::SpawnAtSide(side);
	}

	static int GetEmptyIndexFungus()
	{
		for (int i = 0; i < MAX_FUNGI; i++)
		{
			if (!fungi[i].isActive)
			{
				return i;
			}
		}

		return 0;
	}

	static void InitButton()
	{
		float x = static_cast<float>(SCREEN_WIDTH) - BUTTON_SIZE - BUTTON_MARGIN;
		float y = BUTTON_MARGIN;

		pauseButton = Button::Create(x, y, BUTTON_SIZE, BUTTON_SIZE, " ", Button::Type::Pause);
	}

	static void UpdateButton()
	{
		Button::Update(pauseButton);

		if (pauseButton.clicked)
		{
			PausePanel::isActive = true;
		}
	}

	static void DrawButton()
	{
		Button::Draw(pauseButton);
	}

	static void DrawInterface()
	{
		if (GameOverPanel::isActive)
		{
			return;
		}

		int screenCenterX = SCREEN_WIDTH / 2;
		int margin = 20;

		std::string scoreText = "Score: " + std::to_string(nave.score);
		Vector2 scoreSize = MeasureTextEx(SpatiumCarnis::font, scoreText.c_str(), UIConstants::USER_INTERFACE_FONT_SIZE, UIConstants::SPACING);
		float scoreX = static_cast<float>(screenCenterX) - scoreSize.x / 2.0f;
		float scoreY = static_cast<float>(margin);
		DrawTextEx(SpatiumCarnis::font, scoreText.c_str(), { scoreX, scoreY }, UIConstants::USER_INTERFACE_FONT_SIZE, UIConstants::SPACING, WHITE);

		std::string liveText = "Lives: " + std::to_string(nave.lives);
		Vector2 lifeSize = MeasureTextEx(SpatiumCarnis::font, liveText.c_str(), UIConstants::USER_INTERFACE_FONT_SIZE, UIConstants::SPACING);
		float lifeX = static_cast<float>(screenCenterX) - lifeSize.x / 2.0f;
		float lifeY = static_cast<float>(SCREEN_HEIGHT) - lifeSize.y - margin;
		DrawTextEx(SpatiumCarnis::font, liveText.c_str(), { lifeX, lifeY }, UIConstants::USER_INTERFACE_FONT_SIZE, UIConstants::SPACING, WHITE);
	}
}