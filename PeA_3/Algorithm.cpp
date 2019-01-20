#include "pch.h"
#include <random>
#include "Algorithm.h"

using namespace std;

Algorithm::Algorithm(Global global, Graph *graph)
{
	this->global = new Global();
	*this->global = global;
	this->graph = graph;
	time = new Time();
	best_path = new Path();
	next_path = new Path();
	actual_path = new Path();

	iterations = 0;
}

Algorithm::~Algorithm()
{
	delete global;
	delete time;
	delete actual_path;
	delete next_path;
	delete best_path;
}

void Algorithm::Solve() {};
void Algorithm::PrintData()
{
	printf("Wyniki:\nKoszt trasy: %d\nGeneracja: %d\n", best_path->cost, iterations);
#if TIME ==1
	time->Print();
#endif
	printf("Najlepsza odnaleziona sciezka: ");
	best_path->PrintPath();
}

void Algorithm::FirstResult(Path *path)			// Pierwsze rozwi?zanie
{
	for (int vertex = 0; vertex < graph->size; vertex++)
		path->Add(vertex);
}

void Algorithm::FirstRandomResult(Path *path)		// Pierwsze losowe rozwi?zanie
{
	FirstResult(path);
	random_shuffle(path->path->begin() + 1, path->path->end());
}

void Algorithm::GetCost(Path *path)				// licz koszty ?cie?ki 
{
	vector<int>::const_iterator it = path->path->begin();
	path->cost = 0;
	for (; it != path->path->end() - 1; ++it)
	{
		path->cost += graph->matrix[*(it + 1)][*it];
	}
	path->cost += graph->matrix[0][*(it)];
}
