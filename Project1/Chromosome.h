#pragma once
#include<vector>
#include<math.h>

class Chromosome // defines the chromosome 
{
private:
	std::vector<int> pos; // stores the genes
	double fitness; // stores fitness
public:
	Chromosome();
	void setGenetics(std::vector<int>);
	void setFitness(double);
	std::vector<int> getGenetics();
	void generateFitness();
	double getFitness();
	void setSize(int s);

	bool operator < (const Chromosome& c) const //overloads the < operator for use during sorting function call
	{
		return (fitness < c.fitness);
	}
};

Chromosome::Chromosome() // constructor
{
	pos.reserve(15);
	fitness = 0.0;
}
void Chromosome::setGenetics(std::vector<int> a) //setter for genes
{
	pos = a;
}
void Chromosome::setFitness(double f) // setter for fitness
{
	fitness = f;
}
std::vector<int> Chromosome::getGenetics() // getter for genes
{
	return pos;
}
double Chromosome::getFitness() // getter for fitness
{
	return fitness;
}

void Chromosome::setSize(int s) // resizes chromosome vector depending on board size
{
	pos.resize(s);
}
void Chromosome::generateFitness() // calculates fitness
{
	int atkingQs = 0;
	int bSize = pos.size();
	int Q1, Q2;
	for (int i = 0; i < bSize; i++)
	{
		Q1 = pos.at(i);
		for (int j = 0; j < bSize; j++)
		{
			if (j == i)
				continue;
			Q2 = pos.at(j);
			if (Q1 == Q2 || abs(Q1-Q2) == abs(i-j)) // checks for column and diagonal conflicts. The random population generator will not generate any row conflicts.
				atkingQs--;
		}
	}
	fitness = 100+atkingQs; // 100 is perfect fitness, since atkingQs is a negative value, anything less means we do not have a solution.
}
