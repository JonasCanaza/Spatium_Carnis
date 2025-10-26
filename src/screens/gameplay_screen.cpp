#include "screens/gameplay_screen.h"

#include <stdlib.h>

#include "raylib.h"

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

using namespace Collisions;
using namespace MathUtils;

namespace Gameplay
{
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

	void Init()
	{
		isAccelerating = false;
		timeSinceLastShot = 0.0f;

		specimensSpawnTimer = 0.0f;

		Nave::Init();
		Projectile::Init();
		Specimen::Init();
		Spore::Init();
		Fungus::Init();

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
			}

			if (!PausePanel::isActive && !SporePanel::isActive)
			{
				isAccelerating = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && timeSinceLastShot >= nave.fireRate)
				{
					CreateProjectile();
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
		}

		PausePanel::Update();
		GameOverPanel::Update();
		SporePanel::Update(nave);
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Nave::Draw(nave);
		DrawAllProjectiles();
		DrawAllSpecimens();
		DrawAllSpores();
		DrawAllFungi();

		PausePanel::Draw();
		GameOverPanel::Draw();
		SporePanel::Draw();

		EndDrawing();
	}

	void Close()
	{
		Nave::Close();
		Projectile::Close();
		Specimen::Close();
		Spore::Close();
		Fungus::Close();

		PausePanel::Close();
		GameOverPanel::Close();
		SporePanel::Close();
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
}