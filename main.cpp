#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm> 
#include <string>
Vector2 windowSize = {600, 1000};

Vector3 playerSize = { 60, 100, 20};
Rectangle player = { 0, 300, 60, 100};
Color playerColor = BLACK;
float transitionSpeed = 0;
float score = 0;

struct Enemy {
	Rectangle rect;
	float speed;
	Texture2D txt;
};

std::vector<Texture2D> cars;

Enemy spawnEnemy();

Texture2D carTexture;

int main() {
	
	bool collision = false;
	InitWindow(600, 1000, "Game");
	windowSize = { float(GetScreenWidth()), float(GetScreenHeight()) };

	Texture2D bg = LoadTexture("road.png");
	
	cars.push_back(LoadTexture("car.png"));
	cars.push_back(LoadTexture("car1.png"));
	cars.push_back(LoadTexture("car2.png"));
	cars.push_back(LoadTexture("car3.png"));
	cars.push_back(LoadTexture("car4.png"));

	Camera2D camera = { 0 };
	camera.zoom = 1;
	camera.target = { 0,0 };
	camera.offset = { windowSize.x / 2, windowSize.y/2};
	
	std::vector <Enemy> enemies;

	//movement vars
	float moveX = 0, moveY = 0;
	float speed = 200;

	float timeSinceLastSpawn = 0;
	SetTargetFPS(60);
	float spawnTime = 1;



	while (!WindowShouldClose()) {

		timeSinceLastSpawn += GetFrameTime();

		if (timeSinceLastSpawn >= spawnTime) {
			spawnTime = GetRandomValue(1, 2);
			timeSinceLastSpawn = 0;
			enemies.push_back(spawnEnemy());
		}

		///Player Movement
		if (IsKeyDown(KEY_A))moveX = -1;
		if (IsKeyDown(KEY_D))moveX = 1;
		if (IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) moveX = 0;
		if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) moveX = 0;


		///Shrink and Span
		
		transitionSpeed = speed * 2 * GetFrameTime();
		
		if (IsKeyDown(KEY_W) && player.width > playerSize.z) { player.width-=transitionSpeed; player.height+=transitionSpeed; player.x += transitionSpeed/2; player.y -= transitionSpeed/2; }

		if (IsKeyDown(KEY_S) && player.height > playerSize.z) { player.width += transitionSpeed; player.height -= transitionSpeed; player.x -= transitionSpeed / 2; player.y += transitionSpeed / 2;  }

		player.x += moveX * speed * GetFrameTime();

		///Block Movement 
		if (player.x > (windowSize.x / 2) - 20 - player.width)player.x = (windowSize.x / 2) - 20 - player.width;
		if (player.x < (-windowSize.x / 2) + 20) player.x = (-windowSize.x / 2) + 20;

		if (collision == true) {
			
			player.width = playerSize.x;
			player.height = playerSize.y;
			player.x = 0;
			player.y = 300;
			enemies.clear();
			collision = false;
			score = 0;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTexture(bg, windowSize.x / 2 - bg.width / 2, windowSize.y / 2 - bg.height / 2, WHITE);



		BeginMode2D(camera);

			DrawRectangleRec(player, playerColor);
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i].rect.y += enemies[i].speed * GetFrameTime();
				
				if (enemies[i].rect.y >= windowSize.y / 2) { enemies.erase(enemies.begin() + i); }
				DrawTexture(enemies[i].txt, enemies[i].rect.x, enemies[i].rect.y, WHITE);
				if (CheckCollisionRecs(enemies[i].rect, player)) { collision = true; }

			}

			DrawFPS(-windowSize.x / 2 + 20, (-windowSize.y / 2) + 20);

		EndMode2D();

		std::string scoreString = "Survival Time: ";
		scoreString += std::to_string(static_cast<int>(score));

		//DrawText("LyncDev", 0, 0, 20, BLACK);
		DrawText(scoreString.c_str(), 20,0, 20, WHITE);
		EndDrawing();

		score += GetFrameTime();
		
	}
	UnloadTexture(bg);
	for (Texture2D t : cars) {
		UnloadTexture(t);
	}
	CloseWindow();
	
	return 0;

}

Enemy spawnEnemy() {
	float xLimit = windowSize.x / 2;
	float enemyX = player.x;

	Rectangle e = { enemyX, -windowSize.y/2 + 60, 60, 100 };
	Enemy enemy = { e, GetRandomValue(100, 400), cars[GetRandomValue(0,4)]};
	
	std::cout << "Hello Enemiy";
	return enemy;
}