#include "pch.h"
#include "Menu.h"
#include <ctime>
#include <iostream>
#include "Genetic.h"
#include "Algorithm.h"

using namespace std;

Menu::Menu()
{
	graph = new Graph(0);
	global = new Global();
	srand(unsigned int(time(NULL)));
}


Menu::~Menu()
{
	delete global;
	delete graph;
}





void Menu::IAmTheProgram()
{
	while (1)
	{
		MainMenu();
		char decision;
		do
		{
			printf("czy chcesz opuscic program? T/N\n");
			cout << "\n\n>";
			cin >> decision;
			if (decision == 'T' || decision == 't')
				return;
			else if (decision == 'N' || decision == 'n')
			{
				system("cls");
				break;
			}
			system("cls");
		}
		while (decision != 'T' || 't' || 'N' || 'n');
	}
}

void Menu::MainMenu()
{
	while (1)
	{
		printf("Projektowanie Efektywnych Algorytmow - Projekt\nZadanie projektowe nr 3\n\n");
		cout << "MENU glowne:\n"
			<< "1 - Testy manualne\n"
			<< "2 - Testy efektywnosci\n"
			<< "3 - Parametry\n"
			<< "4 - Graf\n"
			<< "5 - Zakoncz\n"
			<< endl;
		int x = 0;
		cout << "\n\n>";
		cin >> x;
		switch (x)
		{
		case 1:
			ManualTest();
			break;
		case 2:
			Test();
			break;
		case 3:
			global->Params();
			break;
		case 4:
			GraphSetup();
			break;
		case 5:
			return;
		default:
			cout << "zly rozkaz sterujacy" << endl;
			system("pause");
			system("cls");
			break;
		}
	}

	return;
}

void Menu::ManualTest()
{
	while (1)
	{
		system("cls");
		cout << "MENU Testow manualnych\n\n"
			<< "1 - Genetic Algorithm\n"
			<< "2 - Powrot\n"
			<< endl;
		cout << "\n\n>";
		int x;
		cin >> x;
		switch (x)
		{
		case 1:
			{
				Algorithm* algorithm = new Genetic(*global, graph);
				algorithm->Solve();
				algorithm->PrintData();
				delete algorithm;

				system("pause");
				system("cls");
				break;
			}
		case 2:
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

void Menu::GraphSetup()
{
	system("cls");

	while (1)
	{
		int from_file = -1, x = -1;

		system("cls");
		cout << "MENU Grafu\n\n"
			<< "wczytano graf: " << graph->name.c_str()
			<< "\n1 - Wczytaj\n"
			<< "2 - Wyswietl\n"
			<< "3 - Powrot\n"
			<< endl;
		cout << "\n\n>";
		cin >> x;
		switch (x)
		{
		case 1:
			{
				graph->ReadGraph();
				system("cls");
				break;
			}
		case 2:
			{
				graph->PrintGraph();
				system("pause");
				system("cls");
				break;
			}
		case 3:
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

void Menu::Test()
{
	while (1)
	{
		system("cls");
		int x;
		cout << "MENU Testow efektywnosci\n\n"
			<< "1 - Genetic Algorithm\n"
			<< "2 - Powrot\n"
			<< endl;
		cout << "\n\n>";
		cin >> x;
		switch (x)
		{
		case 1:
			{
				int n = 0;
				printf("podaj liczbe iteracji: ");
				cin >> n;

#if PRINT_TO_FILE==1
			fstream histo;
			histo.open("histo_genetic_crossing.txt", ios_base::out);
			histo << "histo\n";
#endif
				for (int i = 0; i < n; i++)
				{
					Algorithm* algorithm = new Genetic(*global, graph);
					algorithm->Solve();
					algorithm->PrintData();
#if PRINT_TO_FILE==1
				histo << algorithm->best_path->cost << "\n";
#endif
					delete algorithm;
				}
#if PRINT_TO_FILE==1
			histo.close();
#endif

				system("pause");
				system("cls");
				break;
			}
		case 2:
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
