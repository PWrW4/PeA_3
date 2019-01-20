#include "pch.h"
#include "Path.h"

Path::Path()
{
	path = new std::vector<int>;
	size = 0;
	cost = INT_MAX;
}

Path::Path(int size, int init)
{
	path = new std::vector<int>(size, init);
	this->size = size;
	cost = 0;
}

Path::~Path()
{
	delete path;
}

void Path::Add(int vertex)
{
	path->push_back(vertex);
	size++;
}


void Path::Remove()
{
	path->pop_back();
	size--;
}

void Path::PrintCost()
{
	printf("Koszt trasy: %d\n", cost);
}

void Path::PrintPath()
{
	for (std::vector<int>::const_iterator it = path->begin(); it != path->end(); ++it)
	{
		printf("%d->", *it);
	}
	printf("0\n");
}

Path &Path::operator =(const Path& right)
{
	*path = *right.path;
	size = right.size;
	cost = right.cost;
	return *this;
}