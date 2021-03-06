#include "pch.h"
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>

std::string* textures;
std::string title;
int textures_num = 0;
int levels_num = 0;
int bombasGastadas = 0;
Vector3 posicionPlayer1;

struct level {
	std::string title;
	int number;
	int width;
	int height;
	int** background;
	std::string** collisions;
	int** foreground;
};

level* levels;

struct bomba {

	bool active = false;

	Vector3 posicion = { .0f, .0f, .0f };
	float time_limit = 7.0f;
	float time_cur = 0.0f;
	float current_time = 0.0f;

	float expansion_time = 2.0f;
	float expamsion_time_cur = 0.0f;

	float size = 1.0f;
	float size_max = 2.0f;
	float size_cur = size;

	Color color = BLACK;
	Color color_hot = PINK;
	Color color_exploding = RED;
};

bomba bombas[5];


void CreateBomba() {
	float t = GetFrameTime();
	for (int i = 0; i < (sizeof(bombas) / sizeof(*bombas)); i++)
	{
		if (bombas[i].active) {
			bombas[i].time_cur += t;
			Color c = bombas[i].color;

			if (bombas[i].time_cur > bombas[i].time_limit / 2)
			{
				c = bombas[i].color_hot;
			}
			if (bombas[i].time_cur > bombas[i].time_limit - 1)
			{
				bombas[i].size = bombas[i].size_max;
			}
			if (bombas[i].time_cur > bombas[i].time_limit)
			{
				/*for (int i = -1; i < 1; i++)
				{
					for (int i = -1; i < 1; i++)
					{
						if(levels[1].collisions[][])
					}
				}*/
				bombas[i].active = false;
			}
			DrawSphereEx(bombas[i].posicion, bombas[i].size, 6, 6, c);
			DrawSphereWires(bombas[i].posicion, bombas[i].size, 6, 6, WHITE);
		}
	}
}

int loadBackground(std::ifstream& file, int level_num, int w, int h) {
	std::string word;
	int value = -1;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			std::getline(file, word, ';');
			value = std::stoi(word);

			levels[level_num].background[i][j] = value;

			std::cout << value;
		}
		std::cout << '\n';
		std::getline(file, word, '\n');
	}
	return 0;
}

int loadForeground(std::ifstream& file, int level_num, int w, int h) {
	std::string word;
	int value = -1;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			std::getline(file, word, ';');
			value = std::stoi(word);
			levels[level_num].foreground[i][j] = value;
			std::cout << value;
		}
		std::cout << '\n';
		std::getline(file, word, '\n');
	}
	return 0;
}

int loadCollisions(std::ifstream& file, int level_num, int w, int h) {
	std::string word;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			std::getline(file, word, ';');
			levels[level_num].collisions[i][j] = word;
			if (levels[level_num].collisions[i][j] == "p1")
			{
				posicionPlayer1 = { -levels[levels_num].height / 2 + (float)i, 1.0f, -levels[levels_num].width / 2 + (float)j };
				word = "n";
			}
			std::cout << word;
		}
		std::cout << '\n';

		std::getline(file, word, '\n');
	}

	return 0;
}

int loadLevels(std::ifstream& file, int num) {
	int w;
	int h;
	int num_level;
	std::string title;

	std::string word;

	levels = new level[num + 1];

	levels[0].title = "none";
	for (int i = 1; i < num + 1; ++i) {
		std::getline(file, word, ';');
		if (word.compare("LEVEL") != 0)
			return 1;

		std::getline(file, word, ';');
		num_level = std::stoi(word);

		std::getline(file, word, ';');
		title = word;

		std::getline(file, word, ';');
		levels[i].width = std::stoi(word);
		w = std::stoi(word);

		std::getline(file, word, ';');
		levels[i].height = std::stoi(word);
		h = std::stoi(word);

		std::getline(file, word, '\n');

		levels[i].background = new int*[h];
		levels[i].collisions = new std::string*[h];
		levels[i].foreground = new int*[h];
		for (int j = 0; j < h; ++j) {
			levels[i].background[j] = new int[w];
			levels[i].collisions[j] = new std::string[w];
			levels[i].foreground[j] = new int[w];
		}

		for (int k = 0; k < 3; ++k) {
			std::getline(file, word, ';');
			std::string block = word;
			std::getline(file, word, '\n');

			std::cout << block << '\n';

			if (block.compare("BACKGROUND") == 0)
				loadBackground(file, i, w, h);
			else if (block.compare("FOREGROUND") == 0)
				loadForeground(file, i, w, h);
			else if (block.compare("COLLISION") == 0)
				loadCollisions(file, i, w, h);
		}

	}

	return 0;
}

int loadTextures(std::ifstream& file, int num) {
	std::string word;

	textures = new std::string[num + 1];
	textures[0] = "none";

	int counter = 1;
	while (counter < num + 1) {
		
		std::getline(file, word, ';');
		std::getline(file, word, ';');
		textures[counter] = word;
		std::cout << textures[counter] << '\n';
		std::getline(file, word, '\n');

		counter++;
	}

	return 0;
}

int doRaylibMagic() {
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [models] example - box collisions");

	// Define the camera to look into our 3d world
	Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

	Vector3 playerPosition = posicionPlayer1;
	Vector3 playerPosition_pre = playerPosition;
	Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
	Color playerColor = GREEN;

	Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
	Vector3 enemyBoxSize = { 1.0f, 1.0f, 1.0f };

	Vector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };

	bool collision = false;
	bool bombCollision = false;

	Image img_list[10];
	Texture2D textures2d[10];

	for (size_t i = 1; i < 4; i++)
	{
		std::string tmp = "../bomberman/" + textures[i];
		img_list[i] = LoadImage(tmp.c_str());
		textures2d[i] = LoadTextureFromImage(img_list[i]);
	}

	Image background = LoadImage("../bomberman/suelo_verde.png");     // Load image in CPU memory (RAM)
	Texture2D texture = LoadTextureFromImage(background);


	SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		playerPosition_pre = playerPosition;
		// Move player
		if (IsKeyDown(KEY_RIGHT)) playerPosition.x += 0.2f;
		else if (IsKeyDown(KEY_LEFT)) playerPosition.x -= 0.2f;
		else if (IsKeyDown(KEY_DOWN)) playerPosition.z += 0.2f;
		else if (IsKeyDown(KEY_UP)) playerPosition.z -= 0.2f;

		collision = false;
		bombCollision = false;

		// Check collisions player vs enemy-box

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		int cur_level = 1;
		// Draw enemy-box
		for (int i = 0; i < levels[cur_level].height; i++)
		{
			for (int j = 0; j < levels[cur_level].width; j++)
			{
				int n = levels[cur_level].background[i][j];
				Vector3 enemyBoxPos = {
					-levels[cur_level].height / 2 + i, 0.5f, -levels[cur_level].width / 2 + j
				};

				DrawCubeTexture(textures2d[n], enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
				DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);

			}
		}

		for (int i = 0; i < levels[cur_level].height; i++)
		{
			for (int j = 0; j < levels[cur_level].width; j++)
			{
				int n = levels[cur_level].foreground[i][j];
				Vector3 enemyBoxPos = {
					-levels[cur_level].height / 2 + i, 1.5f, -levels[cur_level].width / 2 + j
				};

				if (n != 0)
				{
					DrawCubeTexture(textures2d[n], enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
					DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);
				}

			}
		}

		for (int i = 0; i < levels[cur_level].height; i++)
		{
			for (int j = 0; j < levels[cur_level].width; j++)
			{
				Vector3 enemyBoxPos = {
					-levels[cur_level].height / 2 + i, 1.5f, -levels[cur_level].width / 2 + j
				};
				std::string n = levels[cur_level].collisions[i][j];

				if ((n == "y" || n == "w") && CheckCollisionBoxes(

					(BoundingBox(
						{
							(Vector3({
								(float)(playerPosition.x - playerSize.x / 2),
								(float)(playerPosition.y - playerSize.y / 2),
								(float)(playerPosition.z - playerSize.z / 2)
								})),
							(Vector3({
								(float)(playerPosition.x + playerSize.x / 2),
								(float)(playerPosition.y + playerSize.y / 2),
								(float)(playerPosition.z + playerSize.z / 2)
								}))
						})),
						(BoundingBox(
							{
								(Vector3({
									(float)(enemyBoxPos.x - enemyBoxSize.x / 2),
									(float)(enemyBoxPos.y - enemyBoxSize.y / 2),
									(float)(enemyBoxPos.z - enemyBoxSize.z / 2)
									})),
								(Vector3({
									(float)(enemyBoxPos.x + enemyBoxSize.x / 2),
									(float)(enemyBoxPos.y + enemyBoxSize.y / 2),
									(float)(enemyBoxPos.z + enemyBoxSize.z / 2)
									}))
							}))
						)
					) collision = true;

				if (n == "w" && CheckCollisionBoxSphere(
					(BoundingBox({ (Vector3({ (float)(enemyBoxPos.x - enemyBoxSize.x / 2),
												(float)(enemyBoxPos.y - enemyBoxSize.y / 2),
												(float)(enemyBoxPos.z - enemyBoxSize.z / 2)})),
									(Vector3({    (float)(enemyBoxPos.x + enemyBoxSize.x / 2.5f),
												(float)(enemyBoxPos.y + enemyBoxSize.y / 2.5f),
												(float)(enemyBoxPos.z + enemyBoxSize.z / 2.5f)    }))
						})),
					bombas[i].posicion, bombas[i].size
				)) {
					bombCollision = true;
					levels[cur_level].collisions[i][j] = "n";
					levels[cur_level].foreground[i][j] = 0;
				}

				if (collision) {
					playerColor = RED;
					playerPosition = playerPosition_pre;
				}
				else playerColor = GREEN;
			}
		}

		if (IsKeyPressed(KEY_SPACE))
		{
			if (bombasGastadas < 5)
			{
				bombas[bombasGastadas].posicion = playerPosition;
				bombas[bombasGastadas].active = true;
				bombasGastadas++;
			}
		}


		CreateBomba();

		// Draw player
		DrawCubeV(playerPosition, playerSize, playerColor);

		DrawGrid(10, 1.0f);        // Draw a grid

		EndMode3D();

		DrawText(FormatText("Bombas: %i/5", bombasGastadas), 10, 40, 20, GRAY);
		DrawText("Move player with cursors to collide", 220, 40, 20, GRAY);

		DrawFPS(10, 10);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();

	return 0;
}


int main(int argc, char* argv[]) {

	std::ifstream game_conf("../bomberman/game.csv");

	if (!game_conf.is_open()) {
		std::cout << "ERROR: Error al abrir el archivo" << '\n';
		return 1;
	}

	std::string word;

	while (game_conf.good()) {
		std::getline(game_conf, word, ';');

		if (word.compare("TEXTURES") == 0) {
			std::getline(game_conf, word, ';');
			textures_num = std::stoi(word);
			std::getline(game_conf, word, '\n');

			loadTextures(game_conf, textures_num);
		}
		else if (word.compare("TITLE") == 0) {
			std::getline(game_conf, word, ';');
			title = word;
			std::cout << title << std::endl;
			std::getline(game_conf, word, '\n');
		}
		else if (word.compare("LEVELS") == 0) {
			std::getline(game_conf, word, ';');
			levels_num = std::stoi(word);
			std::getline(game_conf, word, '\n');
			loadLevels(game_conf, levels_num);
		}
	}


	game_conf.close();

	doRaylibMagic();

	return 0;
}

