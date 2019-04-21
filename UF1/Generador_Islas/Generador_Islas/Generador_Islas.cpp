#include "pch.h"
#include "Raylib.h"
#include <iostream>
#include <fstream>
#include <string>

int buscarArchivo;
std::string nombreIsla;
int ancho;
int alto;
int** mapa;

int Generador(std::ifstream& file)
{
	std::string island;

	//Creamos un array bidimensional
	mapa = new int*[alto];
	for (int i = 0; i < alto; i++)
	{
		mapa[i] = new int[ancho];
	}

	//Le anyadimos los datos del csv en el array bidimensional
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			std::getline(file, island, ';');
			mapa[i][j] = std::stoi(island);
			std::cout << island;
		}
		std::getline(file, island, '\n');
		std::cout << '\n';
	}	
	return 0;
}

int AbrirArchivo(std::ifstream& file)
{
	//Si no se puede abrir da error
	if (!file.is_open()) {
		std::cout << "ERROR: Error al abrir el archivo" << '\n';
		return 1;
	}

	std::string island;

	//Si puede abrir el archivo
	while (file.good()) {
		std::getline(file, island, ';');

		if (island.compare("Nombre") == 0) {
			std::getline(file, island, ';');
			//Nos guardamos el nombre en una string
			nombreIsla = island;
			std::cout << "Nombre:" << nombreIsla << '\n';
			std::getline(file, island, '\n');
		}
		else if (island.compare("Ancho") == 0)
		{
			std::getline(file, island, ';');
			//Nos guardamos el ancho en un int 
			ancho = std::stoi(island);
			std::cout << "Ancho:" << ancho << '\n';
			std::getline(file, island, '\n');
		}
		else if (island.compare("Alto") == 0)
		{
			std::getline(file, island, ';');
			//Nos guardamos el alto en un int 
			alto = std::stoi(island);
			std::cout << "Alto:" << alto << '\n';
			std::getline(file, island, '\n');
		}
		else if (island.compare("Mapa") == 0)
		{
			std::getline(file, island, '\n');
			Generador(file);
		}
	}

	//Cerramos el archivo
	file.close();
	return 0;
}

int GenerarMapa()
{
	//Nos recorremos todo el array
	for (int i = 0; i < alto; i++)
	{
		for (int j = 0; j < ancho; j++)
		{
			//Las dimensiones que tendran los cubos
			Vector3 cubo = { 1.0f, 1.0f, 1.0f };

			//Las posiciones que tendran los cubos 
			Vector3 cero{ -ancho / 2 + i, 0.5f, -alto / 2 + j };
			Vector3 uno_dos{ -ancho / 2 + i, 1.f, -alto / 2 + j };
			Vector3 tres_cinco{ -ancho / 2 + i, 1.5f, -alto / 2 + j };
			Vector3 seis_siete{ -ancho / 2 + i, 2.f, -alto / 2 + j };
			Vector3 ocho_once{ -ancho / 2 + i, 2.5f, -alto / 2 + j };
			Vector3 doce_catorce{ -ancho / 2 + i, 3.f, -alto / 2 + j };
			Vector3 quince_dieciseis{ -ancho / 2 + i, 3.5f, -alto / 2 + j };

			//Asi comparamos que numeros son y depende del numero
			//se dibujara en una posicion diferente y en un color diferente 
			if (mapa[i][j] == 0)
			{
				DrawCube(cero, cubo.x, cero.y + 0.5f, cubo.z, BLUE);
			}
			if (mapa[i][j] == 1 || mapa[i][j] == 2)
			{
				DrawCube(uno_dos, cubo.x, uno_dos.y + 1.f, cubo.z, YELLOW);
			}
			if (mapa[i][j] == 3 || mapa[i][j] == 4 || mapa[i][j] == 5)
			{
				DrawCube(tres_cinco, cubo.x, tres_cinco.y + 1.5f, cubo.z, GREEN);
			}
			if (mapa[i][j] == 6 || mapa[i][j] == 7)
			{
				DrawCube(seis_siete, cubo.x, seis_siete.y + 2.f, cubo.z, DARKGREEN);
			}
			if (mapa[i][j] == 8 || mapa[i][j] == 9 || mapa[i][j] == 10 || mapa[i][j] == 11)
			{
				DrawCube(ocho_once, cubo.x, ocho_once.y + 2.5f, cubo.z, BROWN);
			}
			if (mapa[i][j] == 12 || mapa[i][j] == 13 || mapa[i][j] == 14)
			{
				DrawCube(doce_catorce, cubo.x, doce_catorce.y + 3.f, cubo.z, DARKBROWN);
			}
			if (mapa[i][j] == 15 || mapa[i][j] == 16)
			{
				DrawCube(quince_dieciseis, cubo.x, quince_dieciseis.y + 3.5f, cubo.z, WHITE);
			}			
		}
	}
	return 0;
}

int Raylib_magic()
{
	// Initialization
	   //--------------------------------------------------------------------------------------
	//Tamanyo de la ventana 
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Generador de Islas");

	// Definimos la camara 3D
	Camera3D camera;
	camera.position = (Vector3{ 10.0f, (float)ancho + 2, (float)alto + 2}); // Camera position
	camera.target = (Vector3{ 0.0f, 0.0f, 0.0f });      // Camera looking at point
	camera.up = (Vector3 { 0.0f, 1.0f, 0.0f });          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	//Camara orbital
	SetCameraMode(camera, CAMERA_ORBITAL); // Set a free camera mode
	
	SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera);          // Update camera

		// Move player
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		//Camara 3D
		BeginMode3D(camera);

		//Dibuja toda la isal
		GenerarMapa();       

		//Finaliza la camara 3d
		EndMode3D();
		
		//Dibujar nombre de la isla 
		const char *cstr = nombreIsla.c_str();
		DrawText(FormatText("%s", cstr), 10, 40, 20, GRAY);
		
		//Dibujar los FPS
		DrawFPS(10, 10);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

int main(int argc, char* argv[])
{

	//Elegir la dirrecion para abrir el archivo
	do
	{
		std::cout << "0.- Salir.\n";
		std::cout << "1.- Generar una isla ya creada.\n";
		std::cout << "2.- Generar la isla que tu has creado en Cube_Island_Simulator_3000.\n";
		std::cin >> buscarArchivo;

		if (buscarArchivo >= 3 || buscarArchivo <= -1)
		{
			std::cout << "Ha habido un error. Repitelo otra vez\n";
		}
		if (buscarArchivo == 0)
		{
			return 0;
		}

	} while (buscarArchivo >= 3 || buscarArchivo <= -1);
	
	//Busca la opcion 1
	if (buscarArchivo == 1)
	{
		//Abrimos el archivo
		std::ifstream wold("/island_PorDefecto.csv");
		AbrirArchivo(wold);
	}

	//Busca la opcion 2
	if (buscarArchivo == 2)
	{
		//Abrimos el archivo
		std::ifstream wold("/island.csv");
		AbrirArchivo(wold);
	}
	
	//Ejecuta la Raylib-
	Raylib_magic();
}


