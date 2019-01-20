#pragma once
#include <fstream>
#include "Path.h"

class Graph
{
public:
	int size,				// Ilosc miast
		**matrix;			// Macierz odleglosci

	std::string name;

	Graph(int size);
	~Graph();

	void PrintGraph();

	bool ReadLine(std::ifstream &file, int tab[], int size);				// wczytywanie linii danych z pliku
	void ReadGraph();
	void RandomGraph(int cities_min, int cities_max, int dist_min, int dist_max, bool asymmetric);
};

