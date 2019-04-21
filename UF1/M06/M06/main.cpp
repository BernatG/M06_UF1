#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>

#include "raylib.h"


std::string* textures;
std::string title;
int textures_num = 0;
int levels_num = 0;
int bombasCreadas = 0;
Vector3  playerPosition;

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

struct Bomb{
	Vector3 pos = { .0f, .0f, .0f };

	bool active = false;
	int pos_w = 0;
	int pos_h = 0;

	float time_limit = 5.0f;
	float time_cur = 0.0f;

	float expansion_time = 2.0f;
	float expansion_time_cur = 0.0f;

	float size = 0.7f;
	float size_max = 2.0f;
	float size_cur = size;

	Color color = BLACK;
	Color color_hot = YELLOW;
	Color color_explosion = RED;
};

Bomb bomb[1000];

void doBombsActions() {
	float t = GetFrameTime();
	for (int i = 0; i < (sizeof(bomb) / sizeof(*bomb)); i++)
	{
		if (bomb[i].active) {
			bomb[i].time_cur += t;
			Color c = bomb[i].color;
			if (bomb[i].time_cur > bomb[i].time_limit / 2) {
				c = bomb[i].color_hot;				
			}
			if (bomb[i].time_cur >= 4.8f )
			{
				bomb[i].size += 0.030f;
			}
			if (bomb[i].time_cur >  bomb[i].time_limit)
			{
				bomb[i].active = false;
			}
			DrawSphere(bomb[i].pos, bomb[i].size, c );
			//  DrawSphereWires(bomb[i].pos, bomb[i].size, WHITE);
		}
	}
}

//esto cargará el Background
int loadBackground(std::ifstream& file, int level_num, int w, int h) {
	//1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1; 1;;
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
		//por cada \n hará un salto de linea
		std::getline(file, word, '\n');
	}

	return 0;
}

//esto cargará el Foreground
int loadForeground(std::ifstream& file, int level_num, int w, int h) {
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
		//por cada \n hará un salto de linea
		std::getline(file, word, '\n');
	}

	return 0;
}

//esto cargará las colisiones
int loadCollisions(std::ifstream& file, int level_num, int w, int h) {
	std::string word;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			std::getline(file, word, ';');
			levels[level_num].collisions[i][j] = word;

			std::cout << word;
			if (levels[level_num].collisions[i][j] == "p1")
			{
				if (i == 1)
				{
					playerPosition = { -levels[1].height / 2 + (float)j - 1, 2.5f, (float)i - 3.5f };
				}
				else
				{
					playerPosition = { -levels[1].height / 2 + (float)j - 2.f, 2.5f, (float)i - 4.5f };
				}
				levels[level_num].collisions[i][j] = "n";
				//playerPosition = { 2.f, 2.5f, 1.f };
			}


		}
		std::cout << '\n';
		//por cada \n hará un salto de linea
		std::getline(file, word, '\n');
	}

	return 0;
}

//esto cargará los niveles
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
		//por cada ; que encuentre pasara a la siguiente
		std::getline(file, word, ';');
		num_level = std::stoi(word);

		std::getline(file, word, ';');
		title = word;

		std::getline(file, word, ';');
		w = std::stoi(word);
		levels[1].width = w;

		std::getline(file, word, ';');
		h = std::stoi(word);
		levels[1].height = h;

		//por cada \n hará un salto de linea
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
//esto cargará las texturas
int loadTextures(std::ifstream& file, int num) {
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
	int screenWidth = 1024;
	int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "raylib [models] example - box collisions");

	// Define the camera to look into our 3d world
	Camera camera = { { 0.0f, 30.0f, 10.0f }, 
					{ 0.0f, 0.0f, 0.0f }, 
					{ 0.0f, 1.0f, 0.0f }, 
						45.0f, 0 };

	//Vector3 playerPosition = { 0.0f, 1.0f, 2.0f };
	Vector3 playerPosition_prev = playerPosition;

	Vector3 bombPosition = playerPosition;
	//esto es el tamaño del rectangulo verde y la posicion

	Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
	Color playerColor = GREEN;

	//esto es el tamaño del cuadrado y la posicion
	Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
	Vector3 enemyBoxSize = { 1.0f, 1.0f, 1.0f };
	//esto es el tamaño del posicion y la posicion
	Vector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };
	float enemySphereSize = 1.5f;

	bool collision = false;
	bool bombCollision = false;

	Image img_list[10];
	Texture2D textures2d[10];
	
	//img_list[0] = LoadImage("../M06/suelo_marte");
	//esto hara cargará las texturas, tendremos que darle la ruta de donde se encuentra la imagen
	//esto es un array, que te carga las imagens / textures
	//lo que tengas dentro del archivo game.csv y como hayas configurado el array te lo mostrar, tiene que tneer el mismo nombre
	for (int i = 1; i <= textures->length(); i++)
	{
		std::string tmp = "../M06/" + textures[i];
		img_list[i] = LoadImage(tmp.c_str());
		textures2d[i] = LoadTextureFromImage(img_list[i]);

	
	}
	//esto cargará la imagen del background
	//Image background = LoadImage("../M06/suelo_marte.png");// Load image in CPU memory (RAM)
	//Texture2D texture = LoadTextureFromImage(background);
	


	SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------

		playerPosition_prev = playerPosition;

		// Move player
		if (IsKeyDown(KEY_RIGHT)) playerPosition.x += 0.2f;
		else if (IsKeyDown(KEY_LEFT)) playerPosition.x -= 0.2f;
		else if (IsKeyDown(KEY_DOWN)) playerPosition.z += 0.2f;
		else if (IsKeyDown(KEY_UP)) playerPosition.z -= 0.2f;
		/*if (IsKeyDown(KEY_SPACE))
		{
			bomb[0].pos = playerPosition;
			bombPosition.z, bombPosition.x, bombPosition.y -= 0.2f;
		}*/

		collision = false;
		bombCollision = false;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		int cur_level = 1;

		int offset_x = -levels[cur_level].height / 2;
		int offset_z = -levels[cur_level].width / 2;

		// Draw enemy-box
		//DrawCubeTexture(texture, enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
		for (int  i = 0; i < levels[cur_level].height; i++)
		{
			for (int j = 0; j < levels[cur_level].width; j++)
			{
				int n = levels[cur_level].background[i][j];
				Vector3 enemyBoxPos = {
					offset_z + j,
					1.0f,
					offset_x + i
				};
				
				std::cout << n;
				DrawCubeTexture(textures2d[n],enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
				DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);
			}

		}

		for (int i = 0; i < levels[cur_level].height; i++)
		{
			for (int j = 0; j < levels[cur_level].width; j++)
			{
				int n = levels[cur_level].foreground[i][j];
				Vector3 enemyBoxPos = {
					offset_z + j,
					2.0f,
					offset_x + i
				};
				if (n != 0)
				{
					DrawCubeTexture(textures2d[n], enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
					DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);
				}
				
			}

		}
		if (IsKeyPressed(KEY_SPACE))
		{
			bomb[0].pos = playerPosition;
			bomb[bombasCreadas].pos = playerPosition;
			bomb[bombasCreadas].active = true;
			bombasCreadas++;

		}

		doBombsActions();

		for (int i = 0; i < levels[cur_level].height; i++)
		{
			for (int j = 0; j < levels[cur_level].width; j++)
			{
				Vector3 enemyBoxPos = {
					offset_z + j,
					2.0f,
					offset_x + i
				};
				std::string t_s = levels[cur_level].collisions[i][j];

				if (levels[1].collisions[i][j] == "p1")
				{
					playerPosition = { playerPosition.x, playerPosition.y, playerPosition.z };
					levels[1].collisions[i][j] = "n";
				}

				if ((t_s == "y" || t_s == "w") && CheckCollisionBoxes (
					(BoundingBox({ (Vector3({ (float)(playerPosition.x - playerSize.x / 2),
												(float)(playerPosition.y - playerSize.y / 2),
												(float)(playerPosition.z - playerSize.z / 2)})),
									(Vector3({	(float)(playerPosition.x + playerSize.x / 2.5f),
												(float)(playerPosition.y + playerSize.y / 2.5f),
												(float)(playerPosition.z + playerSize.z / 2.5f)	}))
						})),
						(BoundingBox({ (Vector3({(float)(enemyBoxPos.x - enemyBoxSize.x / 2),
													(float)(enemyBoxPos.y - enemyBoxSize.y / 2),
													(float)(enemyBoxPos.z - enemyBoxSize.z / 2)})),
										(Vector3({ (float)(enemyBoxPos.x + enemyBoxSize.x / 2.5f),
													(float)(enemyBoxPos.y + enemyBoxSize.y / 2.5f),
													(float)(enemyBoxPos.z + enemyBoxSize.z / 2.5f)}))
							})
							))) {
					collision = true;
				}	
				if (t_s == "w" && CheckCollisionBoxSphere(
					(BoundingBox({ (Vector3({ (float)(enemyBoxPos.x - enemyBoxSize.x / 2),
												(float)(enemyBoxPos.y - enemyBoxSize.y / 2),
												(float)(enemyBoxPos.z - enemyBoxSize.z / 2)})),
									(Vector3({	(float)(enemyBoxPos.x + enemyBoxSize.x / 2.5f),
												(float)(enemyBoxPos.y + enemyBoxSize.y / 2.5f),
												(float)(enemyBoxPos.z + enemyBoxSize.z / 2.5f)	}))
						})),
					bomb[i].pos, bomb[i].size
						)) {
					bombCollision = true;
					levels[cur_level].collisions[i][j] = "n";
					levels[cur_level].foreground[i][j] = 0;
				}
			}
		}

		if (collision == true)
		{
			playerColor = RED;
			playerPosition = playerPosition_prev;
		}
		else if (bombCollision == true)
		{
			playerColor = RED;
		}
		else
		{
			playerColor = GREEN;
		}


	
		// Draw enemy-sphere
	/*	DrawSphereEx(bomb[0].pos, bomb[0].size, 6, 6, DARKPURPLE);
		DrawSphereWires(bomb[0].pos, bomb[0].size, 6, 6, WHITE);*/

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
	std::ifstream game_conf("../M06/game.csv");

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