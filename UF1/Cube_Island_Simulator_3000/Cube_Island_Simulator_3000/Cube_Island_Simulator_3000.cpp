#include "pch.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <Windows.h>

int main()
{
	std::string nombreIsla;
	int ancho;
	int alto;
	char opciones;
	int generarIsla;

	//Elegir formato para crear isla
	do
	{
		std::cout << "0.- Salir.\n";
		std::cout << "1.- Generar isla con 0.\n";
		std::cout << "2.- Generar isla de manera aleatoria.\n";
		std::cin >> generarIsla;

		if (generarIsla >= 3 || generarIsla <= -1)
		{
			std::cout << "Ha habido un error. Repitelo otra vez\n";
		}

	} while (generarIsla >= 3 || generarIsla <= -1);
	
	//Opcion 0 de generarIsla
	if (generarIsla == 0)
	{
		return 0;
	}

	//Escribir el nombre
	std::cout << "1.- Nombre de la isla.\n";
	std::cin >> nombreIsla;

	//Escribir el ancho y el alto
    do
    {
		std::cout << "2.- Ancho.\n";
		std::cin >> ancho;
		std::cout << "3.- Alto.\n";
		std::cin >> alto;

		//Comprovaciones
		if (ancho >= 256 || alto >= 256)
		{
			std::cout << "El ancho y el alto estan limitado a 256. Porfabor repita otra vez el ancho y el alto\n";
		}
		else if (ancho <= 0 || ancho <= 0)
		{
			std::cout << "El ancho y el alto no pueden tener valores 0 o menores que 0. Porfabor repita otra vez el ancho y el alto\n";
		}
		else if (ancho > 18 || alto > 18)
		{
			std::cout << "El ancho y el alto que has indicadio puede dar problemas\n";
		}

    } while (ancho >= 256 || alto >= 256 && ancho <= 0 || ancho <= 0);

	//Creamos un csv y introducimos los datos en el csv
    std::ofstream fs("/island.csv");
    fs << "Nombre;" << nombreIsla << ";;\n";
    fs << "Ancho;" << ancho << ";;\n";
    fs << "Alto;" << alto << ";;\n";
    fs << "Mapa;;\n";
    
	//Generar isla con 0
    if (generarIsla == 1)
    {
	   for (int i = 0; i < alto; i++)
	   {
		   for (int j = 0; j < ancho; j++)
		   {
			   fs << "0;";
		   }
		   fs << ";\n";
	   }
    }

	//Generar isla de forma aleatoria
    if (generarIsla == 2)
	{
	   for (int i = 0; i < alto; i++)
	   {
		   for (int j = 0; j < ancho; j++)
		   {
			   fs << rand() % 17 << ";";
		   }
		   fs << ";\n";
	   }
	}
   
	//Cerramos el csv
	fs.close();
}

