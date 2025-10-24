#include "screens/gameplay_screen.h"

#include <stdlib.h>

#include "raylib.h"

#include "entities/nave.h"
#include "entities/projectile.h"
#include "entities/specimen.h"
#include "collision/collisions.h"
#include "game/game.h"
#include "panels/pause_panel.h"
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

	static float deltaTime;

	static bool isAccelerating;
	static float fireRate;
	static float timeSinceLastShot;

	static float specimensSpawnTimer;
	static const float SPECIMEN_SPAWN_INTERVAL = 8.5f;

	// FUNCTIONS OF PROJECTILES

	static void CreateProjectile();
	static void UpdateAllProjectiles();
	static void DrawAllProjectiles();

	// FUNCTIONS OF SPECIMENS

	static void UpdateAllSpecimens();
	static void DrawAllSpecimens();

	// FUNCTIONS TO DETECT COLLISIONS BETWEEN ENTITIES

	static void HandleProjectileSpecimenCollisions();
	static void HandleNaveSpecimenCollisions();
	static void HandleSpecimenDivision(Specimen::Specimen& specimen);
	static void SplitSpecimen(Specimen::Specimen& specimen, Specimen::Type type);
	static int GetEmptyIndexSpecimens();

	// SPAWNEO FUNCTIONS

	static void HandleSpawningSpecimens();
	static int GetActiveSpecimens();
	static void SpawnSpecimenWave();

	void Init()
	{
		isAccelerating = false;
		fireRate = 0.25f;
		timeSinceLastShot = 0.0f;

		specimensSpawnTimer = 0.0f;

		Nave::Init();
		Projectile::Init();
		Specimen::Init();

		PausePanel::Init();

		nave = Nave::Create();

		CreateInitialSpecimen();
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			PausePanel::isActive = !PausePanel::isActive;
		}

		if (!PausePanel::isActive)
		{
			isAccelerating = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && timeSinceLastShot >= fireRate)
			{
				CreateProjectile();
				timeSinceLastShot = 0.0f;
			}
		}
	}

	void Update()
	{
		deltaTime = GetFrameTime();

		if (!PausePanel::isActive)
		{
			timeSinceLastShot += deltaTime;

			Nave::Update(nave, deltaTime, isAccelerating);
			UpdateAllProjectiles();
			UpdateAllSpecimens();

			HandleProjectileSpecimenCollisions();
			HandleNaveSpecimenCollisions();

			HandleSpawningSpecimens();
		}
		else
		{
			PausePanel::Update();
		}
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Nave::Draw(nave);
		DrawAllProjectiles();
		DrawAllSpecimens();

		if (PausePanel::isActive)
		{
			PausePanel::Draw();
		}

		EndDrawing();
	}

	void Close()
	{
		Nave::Close();
		Projectile::Close();
		Specimen::Close();
		PausePanel::Close();
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

				Nave::TakeDamage(nave);

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
}