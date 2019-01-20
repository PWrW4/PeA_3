#include "pch.h"
#include "Global.h"


#include <windows.h>
#include <iostream>
#include <string>
#include "Global.h"

using namespace std;

Global::Global()
	: iterations(1000),
	execution_time(0),
	starting_population(300),
	mutation_method(1),
	mutation_ratio(0.2),
	crossover_method(2),
	crossover_ratio(0.7),
	tournament_size(5)
{}
Global::~Global() {}

int Global::GetIterations()
{
	return iterations;
}

float Global::GetExecution_time()
{
	return execution_time;
}

int Global::GetStarting_population()
{
	return starting_population;
}

int Global::GetMutation_method()
{
	return mutation_method;
}

float Global::GetMutation_ratio()
{
	return mutation_ratio;
}

int Global::GetCrossover_method()
{
	return crossover_method;
}

float Global::GetCrossover_ratio()
{
	return crossover_ratio;
}

int Global::GetTournament_size()
{
	return tournament_size;
}

void Global::SetIterations(int iterations)
{
	this->iterations = iterations;
}

void Global::SetExecution_time(float execution_time)
{
	this->execution_time = execution_time;
}

void Global::SetStarting_population(int starting_population)
{
	this->starting_population = starting_population;
}

void Global::SetMutation_method(int mutation_method)
{
	this->mutation_method = mutation_method;
}

void Global::SetMutation_ratio(float mutation_ratio)
{
	this->mutation_ratio = mutation_ratio;
}

void Global::SetCrossover_method(int crossing_method)
{
	this->crossover_method = crossing_method;
}

void Global::SetCrossover_ratio(float crossing_ratio)
{
	this->crossover_ratio = crossing_ratio;
}

void Global::SetTournament_size(int tournament_size)
{
	this->tournament_size = tournament_size;
}

void Global::Params()
{
	system("cls");

	while (1)
	{
		int from_file = -1, x = -1;
		system("cls");
		cout << "Menu Parametrow\n\n"
			<< "1 - liczba iteracji: " << GetIterations() << "\n"
			<< "2 - Czas stopu [s]: " << GetExecution_time() << "\n"
			<< "3 - Populacja startowa: " << GetStarting_population() << "\n"
			<< "4 - Sposob mutacji: ";
		if (GetMutation_method() == 2)
			cout << "Invert" << "\n";
		else if (GetMutation_method() == 1)
			cout << "Insert" << "\n";
		else
			cout << "?" << "\n";
		cout << "5 - Wsp. mutacji: " << GetMutation_ratio() << "\n"
			<< "6 - Sposob krzyzowania: ";
		if (GetCrossover_method() == 1)
			cout << "PMX" << "\n";
		else if (GetCrossover_method() == 2)
			cout << "NWOX" << "\n";
		else
			cout << "?" << "\n";
		cout << "7 - Wsp. Krzyzowania: " << GetCrossover_ratio() << "\n"
			<< "8 - Rozmiar turnieju: " << GetTournament_size() << "\n"
			<< "0 - Powrot\n"
			<< "\n\n> ";
		cin >> x;
		switch (x)
		{
		case 1:
		{
			int iterations;
			printf("> ");
			cin >> iterations;
			SetIterations(iterations);


			system("cls");
			break;
		}
		case 2:
		{
			float execution_time;
			printf("> ");
			cin >> execution_time;
			SetExecution_time(execution_time);


			system("cls");
			break;
		}
		case 3:
		{
			int starting_population;
			printf("> ");
			cin >> starting_population;
			SetStarting_population(starting_population);


			system("cls");
			break;
		}
		case 4:
		{
			int mutation_method;

			do
			{
				cout << "MENU wyboru sposobu mutacji:\n"
					<< "1 - Insert\n"
					<< "2 - Invert\n\n> ";
				cin >> mutation_method;
				if (mutation_method >= 1 && mutation_method <= 2)
					break;

			} while (1);
			SetMutation_method(mutation_method);


			system("cls");
			break;
		}
		case 5:
		{
			float mutation_ratio;
			printf("> ");
			cin >> mutation_ratio;
			SetMutation_ratio(mutation_ratio);


			system("cls");
			break;
		}
		case 6:
		{
			int crossing_method;
			do
			{
				cout << "MENU wyboru sposobu krzyzowania:\n"
					<< "1 - PMX\n"
					<< "2 - NWOX\n\n> ";
				cin >> crossing_method;
				if (crossing_method >= 1 && crossing_method <= 2)
					break;

			} while (1);
			SetCrossover_method(crossing_method);


			system("cls");
			break;
		}
		case 7:
		{
			float crossing_ratio;
			printf("> ");
			cin >> crossing_ratio;
			SetCrossover_ratio(crossing_ratio);


			system("cls");
			break;
		}
		case 8:
		{
			int tournament_size;
			printf("> ");
			cin >> tournament_size;
			SetTournament_size(tournament_size);

			system("cls");
			break;
		}
		case 9:
		{

			system("cls");
			break;
		}
		case 10:
		{

			system("cls");
			break;
		}
		case 11:
		{

			system("cls");
			break;
		}
		case 0:
		{
			system("cls");
			return;
		}
		default:
		{
			cout << "zly rozkaz sterujacy" << endl;
			system("pause");
			system("cls");
			break;
		}
		}
	}
}
