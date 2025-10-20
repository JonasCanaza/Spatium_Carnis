#include "screens/gameplay_screen.h"

#include "raylib.h"

#include "entities/nave.h"
#include "entities/projectile.h"
#include "game/game.h"

namespace Gameplay
{
	static const int MAX_PROJECTILE = 20;

	static Nave::Nave nave;
	static Projectile::Projectile projectiles[MAX_PROJECTILE] = {};

	static float deltaTime;

	static float fireRate = 0.25f;
	static float timeSinceLastShot = 0.0f;

	static void CreateProjectile();
	static void UpdateAllProjectiles();
	static void DrawAllProjectiles();

	void Init()
	{
		Nave::Init();
		Projectile::Init();

		nave = Nave::Create();
	}

	void Input()
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			SpatiumCarnis::currentScene = SpatiumCarnis::Scenes::MainMenu;
		}

		Nave::Input();

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

		Nave::Update(nave, deltaTime);
		UpdateAllProjectiles();
	}

	void Draw()
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Nave::Draw(nave);
		DrawAllProjectiles();

		EndDrawing();
	}

	void Close()
	{
		Nave::Close();
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
}