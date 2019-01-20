#pragma once
#include "Global.h"
#include "Graph.h"
#include "Time.h"
#include "Path.h"

class Algorithm
{
public:
	int iterations;				
	Path *actual_path;			
	Path *next_path;			
	Path *best_path;			
	Global *global;				
	Graph *graph;				
	Time *time;					

	Algorithm(Global global, Graph *graph);
	virtual ~Algorithm();


	void virtual Solve();
	void PrintData();
	void FirstResult(Path *path);			
	void FirstRandomResult(Path *path);		


	void GetCost(Path *path);				
};

