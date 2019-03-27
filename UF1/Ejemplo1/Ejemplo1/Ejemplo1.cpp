#include "pch.h"
#include <stdio.h>
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
std::string* levels;
std::string* level;
std::string* nameLevel;
std::string* width;
std::string* high;

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
	nameLevel[num] = word;
	std::cout << "Name Level: " << word << '\n';

	std::getline(game_conf, word, ';');
	width[num] = word;
	std::cout << "Width Level: " << word << '\n';

	std::getline(game_conf, word, ';');
	high[num] = word;
	int high = std::stoi(word);
	std::cout << "High Level: " << word << '\n';
	std::getline(game_conf, word, '\n');

	std::getline(game_conf, word, ';');

	if (word.compare("BACKGROUND") == 0)
	{
		std::getline(game_conf, word, '\n');
		std::getline(game_conf, word, '\n');

		int counter = 0;
		while (high - 1 != counter)
		{
			std::getline(game_conf, word, '\n');
			counter++;
		}		
	}

	std::getline(game_conf, word, ';');

	if (word.compare("COLLISIONS") == 0)
	{
		std::getline(game_conf, word, '\n');
		std::getline(game_conf, word, '\n');

		int counter = 0;
		while (high - 1 != counter)
		{
			std::getline(game_conf, word, '\n');
			counter++;
		}
	}

	std::getline(game_conf, word, ';');

	if (word.compare("FOREGROUND") == 0)
	{
		std::getline(game_conf, word, '\n');
		std::getline(game_conf, word, '\n');

		int counter = 0;
		while (high - 1 != counter)
		{
			std::getline(game_conf, word, '\n');
			counter++;
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
		int levels_n = 0;
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
			levels_n = std::stoi(word);

			level = new std::string[levels_n + 1];
			level[0] = "none";
			nameLevel = new std::string[levels_n + 1];
			nameLevel[0] = "none";
			width = new std::string[levels_n + 1];
			width[0] = "none";
			high = new std::string[levels_n + 1];
			high[0] = "none";

			std::getline(game_conf, word, '\n');
		}
		else if (word.compare("LEVEL") == 0)
		{
			std::getline(game_conf, word, ';');
			numLevel = std::stoi(word);
			level[numLevel] = word;
			std::cout << "Number Level: " << word << '\n';
			LoadLevels(game_conf, numLevel);
		}
	}
	
}


