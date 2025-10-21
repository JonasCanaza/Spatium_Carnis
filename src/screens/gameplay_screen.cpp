#include "screens/gameplay_screen.h"

#include "raylib.h"

#include "entities/nave.h"
#include "entities/projectile.h"
#include "entities/specimen.h"
#include "game/game.h"

namespace Gameplay
{
	static Nave::Nave nave;

	static const int MAX_PROJECTILE = 20;
	static Projectile::Projectile projectiles[MAX_PROJECTILE] = {};

	static const int MAX_SPECIMENS = 10;
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

	void Init()
	{
		isAccelerating = false;
		fireRate = 0.25f;
		timeSinceLastShot = 0.0f;

		Nave::Init();
		Projectile::Init();
		Specimen::Init();

		nave = Nave::Create();

		CreateInitialSpecimen();
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
		}

		isAccelerating = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && timeSinceLastShot >= fireRate)
		{
			CreateProjectile();
			timeSinceLastShot = 0.0f;
		}
	}

	void Update()
	{
		deltaTime = GetFrameTime();
		timeSinceLastShot += deltaTime;

		Nave::Update(nave, deltaTime, isAccelerating);
		UpdateAllProjectiles();
		UpdateAllSpecimens();
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Nave::Draw(nave);
		DrawAllProjectiles();
		DrawAllSpecimens();

		EndDrawing();
	}

	void Close()
	{
		Nave::Close();
	}

	void CreateInitialSpecimen()
	{
		specimens[0] = Specimen::SpawnAtSide(Specimen::SpawnSide::Left, Specimen::Type::Big);
		specimens[1] = Specimen::SpawnAtSide(Specimen::SpawnSide::Top, Specimen::Type::Medium);
		specimens[2] = Specimen::SpawnAtSide(Specimen::SpawnSide::Right, Specimen::Type::Small);
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
}