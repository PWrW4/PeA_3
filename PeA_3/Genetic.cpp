#include "pch.h"
#include "Genetic.h"
#include <vector>
#include <random>
using namespace std;

Genetic::Genetic(Global global, Graph *graph)
	: Algorithm(global, graph)
{
	population_size = this->global->GetStarting_population();
	tournament_size = this->global->GetTournament_size();
	Population = new Path*[population_size];
	Parents = new Path*[population_size];
	Offspring = new Path*[population_size];
	Tournament = new Path*[this->global->GetTournament_size()];

	for (int i = 0; i < population_size; i++)
	{
		Population[i] = new Path();
		Parents[i] = new Path();
		Offspring[i] = new Path(this->graph->size, -5000);
		Offspring[i]->path->at(0) = 0;
	}

	for (int i = 0; i < this->global->GetTournament_size(); i++)
	{
		Tournament[i] = new Path();
	}

}

Genetic::~Genetic()
{
	for (int i = 0; i < population_size; i++)
	{
		delete Population[i];
		delete Parents[i];
		delete Offspring[i];
	}
	for (int i = 0; i < this->global->GetTournament_size(); i++)
	{
		delete Tournament[i];
	}
	delete Population;
	delete Parents;
	delete Offspring;
	delete Tournament;
}

void Genetic::Solve()
{
#if TIME ==1
	time->Start();
#endif

#if PRINT_TO_FILE==1
	fstream costs;
	costs.open("koszta_genetic_crossing.txt", ios_base::out);
	costs << "t koszta avg\n";
#endif

	FirstPopulation();
	GetNewCosts(Population);

	do
	{

		//printf("GEN %d AVG: %lf\n", iterations, GenerationAvg=GetAvgForGen(Population, population_size));
		//printf("GEN %d BEST:\n", iterations);
		//actual_path->PrintAll();
		//printf("GLOBAL BEST:\n");
		//best_path->PrintAll();
		//actual_path->PrintCost();
		//printf("GLOBAL BEST:\n");
		//best_path->PrintCost();


		iterations++;

		TournamentSelection();

		if (global->GetCrossover_method() == 1)
			PMX();
		else if (global->GetCrossover_method() == 2)
			NWOX();

		Mutation();
		GetNewCosts(Offspring);
		ApproveGeneration();

		GenerationAvg = GetAvgForGen(Population, population_size);
#if TIME ==1
		time->Stop();
		//	time->Print();
#endif
#if PRINT_TO_FILE==1
		costs << time->time_ms << " " << best_path->cost << " " << GenerationAvg << "\n";
#endif
	} while ((time->time_ms < (global->GetExecution_time() * 1000) || global->GetExecution_time() == 0) && (iterations < global->GetIterations() || global->GetIterations() == 0));

#if TIME ==1
	time->Stop();
#endif
#if PRINT_TO_FILE==1
	costs.close();
#endif

}

void Genetic::FirstPopulation()
{
	for (int i = 0; i < population_size; i++)
	{
		FirstRandomResult(Population[i]);
		//		printf("gen. 0 osobnik %d.\n", i);
		//		Population[i]->PrintAll();
	}
	//	printf("\n");
}

void Genetic::TournamentSelection()
{
	//	printf("Selekcja turniejowa, wybrano:\n");
	int individual,
		best_individual;
	bool *selected = new bool[population_size];									// aby nie wybiera? tego samego osobnika do turnieju

	for (int k = 0; k < population_size; k++)
	{
		//		printf("turniej %d\n",k);
		for (int j = 0; j < population_size; j++)
		{
			selected[j] = false;
		}
		best_individual = 0;
		for (int i = 0; i < tournament_size; i++)
		{
			do
			{
				individual = rand() % population_size;
			} while (selected[individual]);

			selected[individual] = true;
			*Tournament[i] = *Population[individual];
			//			printf("osobnik %d.\n", i);
			//			Tournament[i]->PrintAll();
			//		printf("best individual:\n");
			//		Tournament[best_individual]->PrintAll();
			if (Tournament[i]->cost < Tournament[best_individual]->cost)
				best_individual = i;											// Aktualnie najlepszy w turnieju
		}

		*Parents[k] = *Tournament[best_individual];								// Po turnieju, nowy rodzic

//		printf("rodzic %d\n",k);
//		Parents[k]->PrintAll();
//		printf("\n");
	}
	//	printf("\n");
	delete selected;

}

void Genetic::Mutation()
{
	float probability;
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dist(0.0, 1.0);
	for (int individual = 0; individual < population_size; individual++)
	{
		probability = (float)dist(gen);

		if (probability < global->GetMutation_ratio())							// mutuj zgodnie z prawdopodobie?stwem
		{
			//			printf("mutacja %d\n", individual);
			if (global->GetMutation_method() == 1)
				Insert(individual);
			else
				Invert(individual);
			continue;															// kolejna iteracja
		}
	}
}

void Genetic::ApproveGeneration()
{
	for (int i = 0; i < population_size; i++)
	{
		*Population[i] = *Offspring[i];
		//		printf("gen. 1 osobnik %d.\n", i);
		//		Population[i]->PrintAll();
	}
}

void Genetic::PMX()																// Operator krzy?owania Partialy Mapped Crossover
{
	int position_1,
		position_2,
		parent_1,
		parent_2;

	float probability;

	vector<int> *map_1 = new vector<int>;										// tablica odwzorowa? 1
	vector<int> *map_2 = new vector<int>;										// tablica odwzorowa? 2

	bool *conflict_list_1 = new bool[graph->size],
		*conflict_list_2 = new bool[graph->size];

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dist(0.0, 1.0);

	for (int k = 0; k < population_size / 2; k++)
	{
		probability = (float)dist(gen);

		parent_1 = rand() % population_size;									// Wybranie nowych rodzic?w
		do
		{
			parent_2 = rand() % population_size;
		} while (parent_1 == parent_2);

		//		printf("parent_1 = %d parent_2 = %d\n", parent_1, parent_2);
		if (probability > global->GetCrossover_ratio())							// je?li prawdopodobie?stwo ma?e -  nie krzy?uj
		{																		// do nowej populacji trafiaj? kandydaci na rodzic?w bez potomk?w
//			printf("BEZ KRZYZOWANIA\n");
			*Offspring[2 * k] = *Parents[parent_1];
			*Offspring[2 * k + 1] = *Parents[parent_2];
			continue;															// kolejna iteracja
		}
		// krzy?owanie - iteracja zaliczona, dodane zostan? 2 osobniki potomne
		for (int i = 0; i < graph->size; i++)
		{
			conflict_list_1[i] = false;
			conflict_list_2[i] = false;
		}

		if (graph->size % 2)													// Nowe pozycje krzy?owania
		{
			position_1 = 1 + rand() % (graph->size / 2 - 2);
			position_2 = ((graph->size / 2) + 2) + rand() % (graph->size / 2 - 2);
		}
		else
		{
			position_1 = 1 + rand() % (graph->size / 2 - 2);
			position_2 = ((graph->size / 2) + 1) + rand() % (graph->size / 2 - 2);
		}
		//		printf("pos1: %d, pos2: %d\n", position_1, position_2);


				// ?rodek
		for (int i = position_1, map = 0; i <= position_2; i++, map++)
		{
			Offspring[2 * k]->path->at(i) = Parents[parent_2]->path->at(i);		// Dziecko 1
			conflict_list_1[Parents[parent_2]->path->at(i)] = true;				// Wpisanie u?ytych wierzcho?k?w na list? konflikt?w z rodzicem 2 dla dziecka 1

			Offspring[2 * k + 1]->path->at(i) = Parents[parent_1]->path->at(i);	// Dziecko 2
			conflict_list_2[Parents[parent_1]->path->at(i)] = true;				// Wpisanie u?ytych wierzcho?k?w na list? konflikt?w z rodzicem 1 dla dziecka 2
			map_1->push_back(Parents[parent_1]->path->at(i));
			map_2->push_back(Parents[parent_2]->path->at(i));
		}
		//printf("srodek\n");
		//printf("potomek %d.\n", 2 * k);
		//Offspring[2 * k]->Print();
		//printf("potomek %d.\n", 2 * k + 1);
		//Offspring[2 * k + 1]->Print();
		//printf("map1, map2\n");
		//for (int i = 0; i <= position_2 - position_1; i++)
		//{
		//	printf("%d. %d, %d\n", i, map_1->at(i), map_2->at(i));
		//}

		// Lewa strona
		for (int i = 0; i < position_1; i++)
		{
			if (!conflict_list_1[Parents[parent_1]->path->at(i)])				// je?li w dziecku 1 nie ma warto?ci z indeksu i u rodzica 1
			{
				//				printf("L brak konfilktu1\n");
				Offspring[2 * k]->path->at(i) = Parents[parent_1]->path->at(i);	// przypisz dziecku 1 t? warto?? na tym indeksie
				conflict_list_1[Parents[parent_1]->path->at(i)] = true;
			}
			else
			{
				int map_pos = 0;
				int looking_for = Parents[parent_1]->path->at(i);
				bool found = false;
				do
				{
					for (map_pos = 0; map_pos < map_2->size(); map_pos++)
						if (map_2->at(map_pos) == looking_for)					// znajd? odwzorowanie
							break;

					if (!conflict_list_1[map_1->at(map_pos)])					// je?li nie jest w konflikcie- wstaw
					{
						Offspring[2 * k]->path->at(i) = map_1->at(map_pos);
						conflict_list_1[map_1->at(map_pos)] = true;
						found = true;
					}
					else														// w przeciwnym wypadku szukaj kolejnego odwzorowania
						looking_for = map_1->at(map_pos);
				} while (!found);
			}


			if (!conflict_list_2[Parents[parent_2]->path->at(i)])				// je?li w dziecku 2 nie ma warto?ci z indeksu i u rodzica 2	
			{
				//				printf("L brak konfilktu2\n");
				Offspring[2 * k + 1]->path->at(i) = Parents[parent_2]->path->at(i);	// przypisz dziecku 2 t? warto?? na tym indeksie
				conflict_list_2[Parents[parent_2]->path->at(i)] = true;
			}
			else
			{
				int map_pos = 0;
				int looking_for = Parents[parent_2]->path->at(i);
				bool found = false;
				do
				{
					for (map_pos = 0; map_pos < map_1->size(); map_pos++)
						if (map_1->at(map_pos) == looking_for)					// znajd? odwzorowanie
							break;

					if (!conflict_list_2[map_2->at(map_pos)])					// je?li nie jest w konflikcie- wstaw
					{
						Offspring[2 * k + 1]->path->at(i) = map_2->at(map_pos);
						conflict_list_2[map_2->at(map_pos)] = true;
						found = true;
					}
					else														// w przeciwnym wypadku szukaj kolejnego odwzorowania
						looking_for = map_2->at(map_pos);
				} while (!found);
			}
		}
		//printf("lewa\n");
		//printf("potomek %d.\n", 2 * k);
		//Offspring[2 * k]->Print();
		//printf("potomek %d.\n", 2 * k + 1);
		//Offspring[2 * k + 1]->Print();

		// Prawa strona
		for (int i = position_2 + 1; i < graph->size; i++)
		{
			if (!conflict_list_1[Parents[parent_1]->path->at(i)])				// je?li w dziecku 1 nie ma warto?ci z indeksu i u rodzica 1
			{
				//				printf("L brak konfilktu1\n");
				Offspring[2 * k]->path->at(i) = Parents[parent_1]->path->at(i);	// przypisz dziecku 1 t? warto?? na tym indeksie
				conflict_list_1[Parents[parent_1]->path->at(i)] = true;
			}
			else
			{
				int map_pos = 0;
				int looking_for = Parents[parent_1]->path->at(i);
				bool found = false;
				do
				{
					for (map_pos = 0; map_pos < map_2->size(); map_pos++)
						if (map_2->at(map_pos) == looking_for)					// znajd? odwzorowanie
							break;

					if (!conflict_list_1[map_1->at(map_pos)])					// je?li nie jest w konflikcie- wstaw
					{
						Offspring[2 * k]->path->at(i) = map_1->at(map_pos);
						conflict_list_1[map_1->at(map_pos)] = true;
						found = true;
					}
					else														// w przeciwnym wypadku szukaj kolejnego odwzorowania
						looking_for = map_1->at(map_pos);
				} while (!found);
			}


			if (!conflict_list_2[Parents[parent_2]->path->at(i)])				// je?li w dziecku 2 nie ma warto?ci z indeksu i u rodzica 2	
			{
				//				printf("L brak konfilktu2\n");
				Offspring[2 * k + 1]->path->at(i) = Parents[parent_2]->path->at(i);	// przypisz dziecku 2 t? warto?? na tym indeksie
				conflict_list_2[Parents[parent_2]->path->at(i)] = true;
			}
			else
			{
				int map_pos = 0;
				int looking_for = Parents[parent_2]->path->at(i);
				bool found = false;
				do
				{
					for (map_pos = 0; map_pos < map_1->size(); map_pos++)
						if (map_1->at(map_pos) == looking_for)					// znajd? odwzorowanie
							break;

					if (!conflict_list_2[map_2->at(map_pos)])					// je?li nie jest w konflikcie- wstaw
					{
						Offspring[2 * k + 1]->path->at(i) = map_2->at(map_pos);
						conflict_list_2[map_2->at(map_pos)] = true;
						found = true;
					}
					else														// w przeciwnym wypadku szukaj kolejnego odwzorowania
						looking_for = map_2->at(map_pos);
				} while (!found);
			}
		}
		//printf("prawa-koniec\n");

//		printf("potomek %d.\n", 2 * k);
//		Offspring[2 * k]->PrintAll();
//		printf("potomek %d.\n", 2 * k+1);
//		Offspring[2 * k + 1]->PrintAll();

		//printf("c1, c2\n");
		//for (int i = 0; i < graph->size; i++)
		//{
		//	printf("%d. %s, %s\n", i, conflict_list_1 ? "true" : "false", conflict_list_2 ? "true" : "false");

		//}
		map_1->clear();
		map_2->clear();
	}

	delete conflict_list_1;
	delete conflict_list_2;
	delete map_1;
	delete map_2;
}

void Genetic::NWOX()
{
	int position_1,
		position_2,
		parent_1,
		parent_2,
		k = 0;

	float probability;

	bool *conflict_list_1 = new bool[graph->size],
		*conflict_list_2 = new bool[graph->size];

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dist(0.0, 1.0);

	for (int k = 0; k < population_size / 2; k++)
	{
		probability = (float)dist(gen);

		parent_1 = rand() % population_size;									// Wybranie nowych rodzic?w
		do
		{
			parent_2 = rand() % population_size;
		} while (parent_1 == parent_2);

		//		printf("parent_1 = %d parent_2 = %d\n", parent_1, parent_2);
		if (probability > global->GetCrossover_ratio())							// je?li prawdopodobie?stwo ma?e -  nie krzy?uj
		{																		// do nowej populacji trafiaj? kandydaci na rodzic?w bez potomk?w
																				//			printf("BEZ KRZYZOWANIA\n");
			*Offspring[2 * k] = *Parents[parent_1];
			*Offspring[2 * k + 1] = *Parents[parent_2];
			continue;															// kolejna iteracja
		}

		for (int i = 0; i < graph->size; i++)									// restart tablicy
		{
			conflict_list_1[i] = false;											// u dziecka 1
			conflict_list_2[i] = false;											// u dziecka 2
		}



		if (graph->size % 2)													// Nowe pozycje krzy?owania
		{
			position_1 = 1 + rand() % (graph->size / 2 - 2);
			position_2 = ((graph->size / 2) + 2) + rand() % (graph->size / 2 - 2);
		}
		else
		{
			position_1 = 1 + rand() % (graph->size / 2 - 2);
			position_2 = ((graph->size / 2) + 1) + rand() % (graph->size / 2 - 2);
		}
		//		printf("pos1: %d, pos2: %d\n", position_1, position_2);

		*Offspring[2 * k] = *Parents[parent_1];									// skopiuj rodzic?w
		*Offspring[2 * k + 1] = *Parents[parent_2];

		for (int i = position_1; i <= position_2; i++)							// znalezienie element?w do odrzucenia
		{
			conflict_list_1[Parents[parent_2]->path->at(i)] = true;				// u dziecka 1
			conflict_list_2[Parents[parent_1]->path->at(i)] = true;				// u dziecka 2
		}

		// Lewa strona
		for (int i = 0; i < position_1; i++)
		{
			if (conflict_list_1[Offspring[2 * k]->path->at(i)])					// Je?li wierzcho?ek u dziecka 1 na pozycji i jest w konflikcie
			{
				RecFind(conflict_list_1, 2 * k, i + 1, 1);						// przesuwaj
			}

			if (conflict_list_2[Offspring[2 * k + 1]->path->at(i)])				// Je?li wierzcho?ek u dziecka 2 na pozycji i jest w konflikcie
			{
				RecFind(conflict_list_2, 2 * k + 1, i + 1, 1);					// przesuwaj
			}
		}

		// Prawa strona
		for (int i = graph->size - 1; i > position_2; i--)
		{
			if (conflict_list_1[Offspring[2 * k]->path->at(i)])					// Je?li wierzcho?ek u dziecka 1 na pozycji i jest w konflikcie
			{
				RecFind(conflict_list_1, 2 * k, i - 1, -1);						// przesuwaj
			}
			if (conflict_list_2[Offspring[2 * k + 1]->path->at(i)])				// Je?li wierzcho?ek u dziecka 2 na pozycji i jest w konflikcie
			{
				RecFind(conflict_list_2, 2 * k + 1, i - 1, -1);					// przesuwaj
			}
		}

		// ?rodek
		for (int i = position_1, map = 0; i <= position_2; i++)
		{
			Offspring[2 * k]->path->at(i) = Parents[parent_2]->path->at(i);		// Dziecko 1 - ?rodek z rodzica 2

			Offspring[2 * k + 1]->path->at(i) = Parents[parent_1]->path->at(i);	// Dziecko 2 - ?rodek z rodzica 1
		}

		//		printf("potomek %d.\n", 2 * k);
		//		Offspring[2 * k]->PrintAll();
		//		printf("potomek %d.\n", 2 * k + 1);
		//		Offspring[2 * k + 1]->PrintAll();
	}
}

void Genetic::RecFind(bool * conflict_list, int Offspring_id, int position, int direction)
{

	if (conflict_list[Offspring[Offspring_id]->path->at(position)])				// Je?li wierzcho?ek u dziecka jest w konflikcie
		RecFind(conflict_list, Offspring_id, position + direction, direction);	// dziura - szukaj g??biej
	swap(Offspring[Offspring_id]->path->at(position), Offspring[Offspring_id]->path->at(position - direction));		// po znalezieniu zamie? miejscami
}

void Genetic::Invert(int individual)
{
	//	Offspring[individual]->Print();

	int position_1, position_2;
	position_1 = rand() % (graph->size - 1) + 1;

	//printf("pos1: %d \n", position_1);

	do
	{
		position_2 = rand() % (graph->size - 1) + 1;
	} while (position_2 == position_1);

	//printf("pos2: %d \n", position_2);

	if (position_1 < position_2)
		reverse(Offspring[individual]->path->begin() + position_1, Offspring[individual]->path->begin() + position_2 + 1);
	else
		reverse(Offspring[individual]->path->begin() + position_2, Offspring[individual]->path->begin() + position_1 + 1);

}

void Genetic::Insert(int individual)
{

	//	Offspring[individual]->Print();


	vector<int>::iterator position = Offspring[individual]->path->begin();
	int position_1, position_2, vertex;
	position_1 = rand() % (graph->size - 1) + 1;

	//	printf("pos1: %d \n", position_1);

	vertex = Offspring[individual]->path->at(position_1);
	do
	{
		position_2 = rand() % (graph->size - 1) + 1;
	} while (position_2 == position_1);

	//	printf("pos2: %d \n", position_2);
	//	printf("insert %d na pozycje %d\n", vertex, position_2);

	Offspring[individual]->path->erase(position + position_1);
	Offspring[individual]->path->insert(position + position_2, vertex);

	//	Offspring[individual]->Print();

}

void Genetic::GetNewCosts(Path ** Set)
{
	actual_path->cost = INT_MAX;
	for (int i = 0; i < population_size; i++)
	{
		GetCost(Set[i]);
		if (Set[i]->cost < actual_path->cost)
		{
			*actual_path = *Set[i];

			if (Set[i]->cost < best_path->cost)
			{
				time->Stop();
				*best_path = *Set[i];
			}
		}
		//		printf("policzone %d\n",i);
		//		Set[i]->Print();
	}
	//	printf("\n");
}

float Genetic::GetAvgForGen(Path ** Set, int set_size)
{
	float avarage_cost = 0;
	for (int i = 0; i < set_size; i++)
	{
		avarage_cost += Set[i]->cost;
	}
	avarage_cost /= set_size;
	return avarage_cost;
}
