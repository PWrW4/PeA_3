#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"

using namespace std;

Graph::Graph(int size)
{
	name = "brak grafu";
	this->size = size;
	matrix = new int *[size];
	for (int i = 0; i < size; i++)
	{
		matrix[i] = new int[size];
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[j][i] = 0;
		}
	}

};


Graph::~Graph()
{
	for (int i = 0; i < size; i++)
		delete matrix[i];
	delete matrix;
}

void Graph::PrintGraph()
{

	printf("   ");
	for (int i = 0; i < size; i++)
	{

		printf("%3d.", i);
	}
	printf("\n");
	for (int i = 0; i < size; i++)
	{
		printf("%2d.", i);
		for (int j = 0; j < size; j++)
		{
			printf("%4d", matrix[j][i]);
		}
		printf("\n");
	}
}


bool Graph::ReadLine(ifstream &file, int tab[], int size)				
{
	string s;
	getline(file, s);

	if (file.fail() || s.empty())
		return(false);

	istringstream in_ss(s);

	for (int i = 0; i < size; i++)
	{
		in_ss >> tab[i];
		if (in_ss.fail())
			return(false);
	}
	return(true);
}
void Graph::ReadGraph()		
{
	string file_name;
	int check = 0;
	int size;
	ifstream file;
	do
	{
		printf("podaj nazwe pliku: ");
		cin >> file_name;
		name = file_name;
		file.open(file_name.c_str());

		if (!file.good())
			printf("bledna nazwa pliku lub plik nie istnieje!\n\n");
	} while (!file.good());				

	if (file.is_open())
	{
		if (ReadLine(file, &size, 1))
		{
			this->size = size;
			delete matrix;
			this->matrix = new int *[size];
			for (int i = 0; i < size; i++)
			{
				this->matrix[i] = new int[size];
			}

			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					this->matrix[j][i] = 0;
				}
			}

			

			int *tab = new int[size];

			for (int i = 0; i < size; i++)
				if (ReadLine(file, tab, size))
				{
					for (int j = 0; j < size; j++)
					{
						matrix[j][i] = tab[j];
					}
				}
				else
				{
					printf("File error - READ EDGE\n");
					break;
				}
			delete tab;
			file.close();
		}
		else
			printf("File error - READ INFO\n");
		file.close();
	}
	else
		printf("File error - OPEN\n");

}

void Graph::RandomGraph(int cities_min, int cities_max, int dist_min, int dist_max, bool asymmetric)
{
	if (cities_min > cities_max || dist_min > dist_max)
	{
		printf("Bad data");
	}

	if (cities_min == cities_max)
		size = cities_min;
	else
	{
		size = (rand() % (cities_max - cities_min + 1) + cities_min);
	}

	delete matrix;
	this->matrix = new int *[size];
	for (int i = 0; i < size; i++)
	{
		this->matrix[i] = new int[size];
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			this->matrix[j][i] = 0;
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{

			if (!asymmetric && j < i)
			{
				matrix[j][i] = matrix[i][j];
			}
			else
			{
				if (dist_max == dist_min)
					matrix[j][i] = dist_max;
				else
					matrix[j][i] = rand() % (dist_max - dist_min + 1) + dist_min;
			}
		}
	}

}

