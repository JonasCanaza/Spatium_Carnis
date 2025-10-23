#include "screens/gameplay_screen.h"

#include <iostream>
#include <stdlib.h>

#include "raylib.h"

#include "entities/nave.h"
#include "entities/projectile.h"
#include "entities/specimen.h"
#include "collision/collisions.h"
#include "game/game.h"
#include "panels/pause_panel.h"

using namespace Collisions;

namespace Gameplay
{
	static Nave::Nave nave;

	static const int MAX_PROJECTILE = 20;
	static Projectile::Projectile projectiles[MAX_PROJECTILE] = {};

	static const int INITIAL_SPECIMENS = 10;
	static const int MAX_SPECIMENS = 50;
	static Specimen::Specimen specimens[MAX_SPECIMENS];

	static float deltaTime;

	static bool isAccelerating;
	static float fireRate;
	static float timeSinceLastShot;

	// FUNCTIONS OF PROJECTILES

	static void CreateProjectile();
	static void UpdateAllProjectiles();
	static void DrawAllProjectiles();

	// FUNCTIONS OF SPECIMENS

	static void UpdateAllSpecimens();
	static void DrawAllSpecimens();

	// FUNCTIONS TO DETECT COLLISIONS BETWEEN ENTITIES

	static void HandleProjectileSpecimenCollisions();

	void Init()
	{
		isAccelerating = false;
		fireRate = 0.25f;
		timeSinceLastShot = 0.0f;

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
					specimens[j].isActive = false;

					break;
				}
			}
		}
	}
}