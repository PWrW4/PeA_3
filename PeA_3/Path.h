#pragma once
#include <vector>

class Path
{
public:
	std::vector<int> *path;		
	int size;					
	int cost;					
	Path();
	Path(int size, int init);
	~Path();

	void Add(int vertex);		
	void Remove();				

	void PrintCost();			
	void PrintPath();			
	Path & operator= (const Path& right);
};

