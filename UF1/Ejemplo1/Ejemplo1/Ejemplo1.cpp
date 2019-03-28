#include "pch.h"
#include "Raylib.h"
#include <iostream>
#include <fstream>
#include <string>

//void Ejemplo1() {
//	int intEdad;
//	float floatPeso;
//	//abrimos el archivo personas.csv
//	std::ifstream ip("../Ejemplo1/personas.csv");
//	//si esto da true entonces se cierra el programa y devuelve un error 
//	if (!ip.is_open()) {
//		std::cout << "ERROR: Error al abrir el archivo" << "\n";
//		return 1;
//	}
//
//	//creamos  variables 
//	std::string nombre;
//	std::string apellido;
//	std::string edad;
//	std::string peso;
//
//	while (ip.good()/*te envia true si esta bien abierto*/)
//	{
//		//dentro de ip cogeremos todos los caracteresesy lo gardaremos en algun sitio, asta llegar asta el primer ;
//		std::getline(ip, nombre, ';');
//		std::getline(ip, apellido, ';');
//		std::getline(ip, edad, ';');
//		std::getline(ip, peso, '\n');
//
//		std::cout << "Nombre: " << nombre << " " << apellido << '\n';
//		std::cout << "Edad: " << edad << '\n';
//		std::cout << "Peso: " << peso << '\n';
//		std::cout << "===============" << '\n';
//
//		intEdad = std::stoi(edad);
//		floatPeso = std::stof(peso);
//
//	}
//
//	ip.close();
//}

//td::ofstream archivo;
//
//archivo.open("datos.csv");
//
//archivo << "a;b;c;\n";
//archivo << "d;f;g;\n";
//archivo << "1;2;3.456;\n";
//archivo.close();
//
//return 0;

std::string* textures;
int levels;
struct StructLevel
{
	int level;
	std::string nameLevel;
	int width;
	int high;
	int **background;
	std::string **collisions;
	int **foreground;
};

StructLevel structLevel[100];


int LoadTextures(std::ifstream& game_conf, int num)
{
	std::string word;

	textures = new std::string[num + 1];
	textures[0] = "none";

	int counter = 1;
	while (counter < num + 1)
	{
		std::getline(game_conf, word, ';');
		std::getline(game_conf, word, ';');
		textures[counter] = word;
		std::cout << word << '\n';
		std::getline(game_conf, word, '\n');

		counter++;
	}

	return 0;
}

int LoadLevels(std::ifstream& game_conf, int num)
{
	std::string word;
	std::getline(game_conf, word, ';');
	structLevel[num].nameLevel = word;
	
	std::cout << "Name Level: " << word << '\n';

	std::getline(game_conf, word, ';');
	structLevel[num].high = std::stoi(word);
	int high = std::stoi(word);
	std::cout << "High Level: " << word << '\n';

	std::getline(game_conf, word, ';');
	structLevel[num].width = std::stoi(word);
	int width = std::stoi(word);
	std::cout << "Width Level: " << word << '\n';
	std::getline(game_conf, word, '\n');

	std::getline(game_conf, word, ';');


		structLevel[num].background = new int*[width];
		structLevel[num].collisions = new std::string*[width];
		structLevel[num].foreground = new int*[width];
		for (int h = 0; h < width; h++)
		{
			structLevel[num].background[h] = new int[high];
			structLevel[num].collisions[h] = new std::string[high];
			structLevel[num].foreground[h] = new int[high];
		}

		if (word.compare("BACKGROUND") == 0)
		{
			std::getline(game_conf, word, '\n');

			int value;
			for (int w = 0; w < width; w++)
			{
				for (int h = 0; h < high; h++)
				{
					std::getline(game_conf, word, ';');
					value = std::stoi(word);
					
					structLevel[num].background[w][h] = value;
					std::cout << structLevel[num].background[w][h];
				}
				std::getline(game_conf, word, '\n');
				std::cout << '\n';
			}			
		}

		std::getline(game_conf, word, ';');

		if (word.compare("COLLISIONS") == 0)
		{
			std::getline(game_conf, word, '\n');

			for (int w = 0; w < width; w++)
			{
				for (int h = 0; h < high; h++)
				{
					std::getline(game_conf, word, ';');
					structLevel[num].collisions[w][h] = word;
					std::cout << structLevel[num].collisions[w][h];
				}
				std::getline(game_conf, word, '\n');
				std::cout << '\n';
			}
		}

		std::getline(game_conf, word, ';');

		if (word.compare("FOREGROUND") == 0)
		{
			std::getline(game_conf, word, '\n');

			int value;
			for (int w = 0; w < width; w++)
			{
				for (int h = 0; h < high; h++)
				{
					std::getline(game_conf, word, ';');
					value = std::stoi(word);
					structLevel[num].foreground[w][h] = value;
					std::cout << structLevel[num].foreground[w][h];
				}
				std::getline(game_conf, word, '\n');
				std::cout << '\n';
			}
		}
	return 0;
}


int main(int agrc, char* argv[])
{
	std::ifstream game_conf("../Ejemplo1/game.csv");
	if (!game_conf.is_open()) {
		std::cout << "ERROR: Error al abrir el archivo" << "\n";
		return 1;
	}

	while (game_conf.good()/*te envia true si esta bien abierto*/)
	{
		std::string word;
		std::string title;
		int textures_n = 0;
		int numLevel = 0;

		std::getline(game_conf, word, ';');

		if (word.compare("TEXTURES") == 0)
		{
			std::getline(game_conf, word, ';');
			textures_n = std::stoi(word);
			std::getline(game_conf, word, '\n');

			LoadTextures(game_conf, textures_n);

		}
		else if (word.compare("TITLE") == 0)
		{
			std::getline(game_conf, title, ';');
			std::getline(game_conf, word, '\n');

			std::cout << title << std::endl;
		}
		else if (word.compare("LEVELS") == 0)
		{
			std::getline(game_conf, word, ';');
			levels = std::stoi(word);

			std::getline(game_conf, word, '\n');
		}
		else if (word.compare("LEVEL") == 0)
		{
			std::getline(game_conf, word, ';');
			numLevel = std::stoi(word);
			structLevel[numLevel].level = std::stoi(word);
			std::cout << "Number Level: " << word << '\n';
			LoadLevels(game_conf, numLevel);
		}
	}

	// Initialization
   //--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 1000;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);
		Texture2D suelo= LoadTexture("../Ejemplo1/suelo_verde.png");
		DrawTexture(suelo,50, 50,GREEN);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
	
}


