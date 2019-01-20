#pragma once
#include "Graph.h"
#include "Global.h"

class Menu
{
public:
	Menu();
	~Menu();


	Global *global;
	Graph *graph;

	void IAmTheProgram();
	void MainMenu();
	void ManualTest();
	void GraphSetup();
	void Test();
};

