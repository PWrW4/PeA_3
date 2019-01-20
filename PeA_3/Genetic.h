#pragma once
#include "Algorithm.h"

class Genetic : public Algorithm
{
public:
	int population_size;
	int tournament_size;
	float GenerationAvg;
	Path ** Population;
	Path ** Parents;
	Path ** Offspring;
	Path ** Tournament;

	Genetic(Global global, Graph *graph);
	~Genetic();

	void Solve();
	void FirstPopulation();
	void TournamentSelection();
	void Mutation();
	void ApproveGeneration();			// kopiowanie z offspring do population

	void PMX();
	void NWOX();
	void Invert(int individual);
	void Insert(int individual);

	void GetNewCosts(Path ** Set);		// policz koszty w zbiorze
	float GetAvgForGen(Path ** Set, int set_size);
	void RecFind(bool * conflict_list, int Offspring_id, int position, int direction);
};
