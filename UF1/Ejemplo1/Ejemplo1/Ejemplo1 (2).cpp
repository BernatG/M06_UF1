#include <iostream>
#include <fstream>
#include <string>

#include <raylib.h>

std::string* textures;
std::string title;
int textures_num = 0;
int levels_num = 0;

//LEVEL; 1; Prado del MAL; 12; 8;;

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

int loadBackground (std::ifstream& file, int level_num, int w, int h) {
	//1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1;;
	std::string word;
	int value = -1;

	for (int i = 0; i < h; ++i){
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


int loadForeground (std::ifstream& file, int level_num, int w, int h) {
	//1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1;;
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


int loadCollisions (std::ifstream& file, int level_num, int w, int h) {
	std::string word;	

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			std::getline(file, word, ';');
			levels[level_num].collisions[i][j] = word;

			std::cout << word;
		}
		std::cout << '\n';

		std::getline(file, word, '\n');
	}

	return 0;
}


int loadLevels (std::ifstream& file, int num) {
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
		w = std::stoi(word);

		std::getline(file, word, ';');
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

int loadTextures (std::ifstream& file, int num) {
	std::string word;

	textures = new std::string[num + 1];
	textures[0] = "none";

	int counter = 1;
	while (counter < num + 1) {
		//1; suelo_verde.png;;
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

	Vector3 playerPosition = { 0.0f, 1.0f, 2.0f };
	Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
	Color playerColor = GREEN;

	Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
	Vector3 enemyBoxSize = { 2.0f, 2.0f, 2.0f };

	Vector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };
	float enemySphereSize = 1.5f;

	bool collision = false;

	Image img_list[10];
	Texture2D textures2d[10];

	for (size_t i = 1; i < 4; i++)
	{
		std::string tmp = "../Ejemplo1/" + textures[i];
		img_list[i] = LoadImage(tmp.c_str());
		textures2d[i] = LoadTextureFromImage(img_list[i]);
	}

	Image background = LoadImage("../Project1/suelo_verde.png");     // Load image in CPU memory (RAM)
	Texture2D texture = LoadTextureFromImage(background);

	SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------

		// Move player
		if (IsKeyDown(KEY_RIGHT)) playerPosition.x += 0.2f;
		else if (IsKeyDown(KEY_LEFT)) playerPosition.x -= 0.2f;
		else if (IsKeyDown(KEY_DOWN)) playerPosition.z += 0.2f;
		else if (IsKeyDown(KEY_UP)) playerPosition.z -= 0.2f;

		collision = false;

		/*// Check collisions player vs enemy-box
		if (CheckCollisionBoxes(
			(BoundingBox) {(Vector3) {
			playerPosition.x - playerSize.x / 2,
				playerPosition.y - playerSize.y / 2,
				playerPosition.z - playerSize.z / 2
		},
				(Vector3) {
				playerPosition.x + playerSize.x / 2,
					playerPosition.y + playerSize.y / 2,
					playerPosition.z + playerSize.z / 2
			}
		},
			(BoundingBox) {
			(Vector3) {
			enemyBoxPos.x - enemyBoxSize.x / 2,
				enemyBoxPos.y - enemyBoxSize.y / 2,
				enemyBoxPos.z - enemyBoxSize.z / 2
		},
				(Vector3) {
				enemyBoxPos.x + enemyBoxSize.x / 2,
					enemyBoxPos.y + enemyBoxSize.y / 2,
					enemyBoxPos.z + enemyBoxSize.z / 2
			}
		})) collision = true;

		// Check collisions player vs enemy-sphere
		if (CheckCollisionBoxSphere(
			(BoundingBox) {
			(Vector3) {
			playerPosition.x - playerSize.x / 2,
				playerPosition.y - playerSize.y / 2,
				playerPosition.z - playerSize.z / 2
		},
				(Vector3) {
				playerPosition.x + playerSize.x / 2,
					playerPosition.y + playerSize.y / 2,
					playerPosition.z + playerSize.z / 2
			}
		},
			enemySpherePos, enemySphereSize)) collision = true;

		if (collision) playerColor = RED;
		else playerColor = GREEN;
		*/
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		// Draw enemy-box
		DrawCubeTexture(texture, enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
		DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);

		// Draw enemy-sphere
		DrawSphere(enemySpherePos, enemySphereSize, GRAY);
		DrawSphereWires(enemySpherePos, enemySphereSize, 16, 16, DARKGRAY);

		// Draw player
		DrawCubeV(playerPosition, playerSize, playerColor);

		DrawGrid(10, 1.0f);        // Draw a grid

		EndMode3D();

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
	//Matrix snipet, guardadlo
	/*
	int w = 32, h = 8;
	int** matrix;
	matrix = new int*[h];
	for (int i = 0; i < h; ++i) {		
		matrix[i] = new int[w];
	}
	*/








	std::ifstream game_conf("../Project1/game.csv");

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


	//doRaylibMagic();

	return 0;
}