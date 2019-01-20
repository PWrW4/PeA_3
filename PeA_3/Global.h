#pragma once
#define PRINT_TO_FILE 0
#define TIME 1
#include <string>

class Global
{
private:
	// Wspolne
	int iterations;
	float execution_time;
	// Genetic
	int starting_population;
	int mutation_method;
	float mutation_ratio;
	int crossover_method;
	float crossover_ratio;
	int tournament_size;

public:
	Global();
	~Global();

	int GetIterations();
	float GetExecution_time();

	int GetStarting_population();
	int GetMutation_method();
	float GetMutation_ratio();
	int GetCrossover_method();
	float GetCrossover_ratio();
	int GetTournament_size();


	void SetIterations(int iterations);
	void SetExecution_time(float execution_time);

	void SetStarting_population(int starting_population);
	void SetMutation_method(int mutation_method);
	void SetMutation_ratio(float mutation_ratio);
	void SetCrossover_method(int crossing_method);
	void SetCrossover_ratio(float crossing_ratio);
	void SetTournament_size(int tournament_size);

	void Params();
};

