#include"Chromosome.h"
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<algorithm>

static const int POPSIZE = 6500; // change this as suited.

/* The response rate for boards till 10x10 is about 1-10s without any misses for the given POPSIZE.
11 and 12 still generated solutions within 10-20s.
13 onwards requires restarts, with 13 at 80%, 14 at 60% and 15 at 40% solution rate.
Time is approx a minute for 15x15.

Based on piazza and class discussions, I have kept 15x15 as a soft upper limit, though this could theoretically work for higher values as well. Time and RNG would play bigger roles in determining the outcome.
*/
int size; // size of the board
std::vector<Chromosome> population(POPSIZE,Chromosome()); //initializes vector to store population
//std::vector<double> matingProbs(POPSIZE/2);

void generatePopulation();
void printSol();
void crossOver();
void mutation();


void geneticAlgorithm()
{
	std::cout << "Enter board size: ";
	std::cin >> size;
	if (size == 3 || size == 2) // no solutions possible for these boards, hence they are excluded.
	{
		std::cout << "No solution possible\n";
		std::cout << "Terminal input, Enter any digit to end: ";
		int x;
		std::cin >> x;
		return;
	}
	std::cout << "\n\n";
	for (int i = 0; i < POPSIZE; i++)
	{
		population.at(i).setSize(size);
	}
	srand(time(NULL)); //initalizes random seed
	generatePopulation(); // generates a random population
	
	for (int i = 0; i < POPSIZE; i++)
	{
		population.at(i).generateFitness();// for each chromosome, we calculate its fitness
	}

	std::sort(population.rbegin(),population.rend()); // vector is sorted from highest to lowest fitness

	//double maxfit = population.at(0).getFitness(), minfit = population.at(0).getFitness();
	double avfit = 0.0; // stores average fitness
	int genNo = 1; // current gen number

	for (int i = 0; i < POPSIZE; i++)
	{
		//std::vector<int> chromes = population.at(i).getGenetics(); // gets current chromosome
		double fit = population.at(i).getFitness(); // gets current chromosome's fitness
		/*for (int j = 0; j < size; j++)
		{
			std::cout << chromes.at(j) << " ";
		}
		std::cout << " fitness: " << fit << "\n";
		if (maxfit < fit)
			maxfit = fit;
		if (minfit > fit)
			minfit = fit;*/
		avfit += fit; 
	}
	avfit /= POPSIZE; 
	
	//std::cout << "Maximum fitness: " << maxfit << "\n";
	//std::cout << "Minimum fitness: " << minfit << "\n";
	std::cout << "Generation no: " << genNo << ", Average fitness: " << avfit << ", Max: " << population[0].getFitness() << ", Min: " << population[POPSIZE-1].getFitness() << "\n";
	

	while (true) // keeps running until we arrive at solution
	{
		if (population[0].getFitness() == 100) //since array is sorted, the first element will be the solution, if it is present
		{
			printSol();
			break;
		}

		population.erase(population.begin() + (POPSIZE / 2), population.end()); // bottom half of the population is castrated
		
		/*for (int i = 0; i < 100; i++)
		{
			matingProbs[i] = population[i].getFitness() / avfit - 1;
		}*/

		for (int i = 0; i < POPSIZE/4; i++) // top half is allowed to breed
		{
			crossOver();
		}
		mutation();
		std::sort(population.rbegin(), population.rend()); // once again, sorted by fitness
		genNo++; // generation is updated
		avfit = 0;
		for (int i = 0; i < POPSIZE; i++)
		{
			avfit += population[i].getFitness();
		}
		avfit /= POPSIZE; // average fitness is calculated
		std::cout << "Generation no: " << genNo << ", Average fitness: " << avfit << ", Max: " << population[0].getFitness() << ", Min: " << population[POPSIZE - 1].getFitness() << "\n";
	}
	
	std::cout << "Terminal input, Enter any digit to end: ";
	int x;
	std::cin >> x;
}

void generatePopulation() // generates a random popluation 
{
	std::vector<int> generatedChromes(size); // stores generated chromosome
	//srand(time(NULL));
	int r;
	for (int i = 0; i < POPSIZE; i++)
	{
		generatedChromes.clear();
		for (int j = 0; j < size; j++)
		{
			r = rand() % size + 1; // random gene generated
			generatedChromes.push_back(r); // pushed into chromosome
		}
		population.at(i).setGenetics(generatedChromes); // complete chromosome is added into the array
		
	}
}

void crossOver() // two rand chormosomes are bred
{
	int r1 = rand() % (POPSIZE/2); // random indices are obtain
	int r2 = rand() % (POPSIZE/2);
	while (r1 == r2) // if the inidices are the same, we reobtain one
	{
		r2 = rand() % (POPSIZE / 2);
	}
	std::vector<int> par1 = population[r1].getGenetics(); // get the parent chromosome info
	std::vector<int> par2 = population[r2].getGenetics();
	while (par1 == par2) // if parents are identical, we obtain new parents
	{
		r2 = rand() % (POPSIZE / 2); 
		par2 = population[r2].getGenetics();
	}
	int k = rand() % (size-1) + 1; // domination gene factor is chosen at random
	std::vector<int> ch1(size); // create child chromosome vectors to store genes
	std::vector<int> ch2(size);
	for (int i = 0; i < size; i++)
	{
		if (i < k) // depending on domination factor, ch1 could be vastly similar to p1 or vice versa. ch2 will be just the opposite.
		{
			ch1[i] = par1[i];
			ch2[i] = par2[i];
		}
		else
		{
			ch1[i] = par2[i];
			ch2[i] = par1[i];
		}
	}
	Chromosome c1, c2; // creating the Chromosome objects
	c1.setSize(ch1.size()); // updating the object info
	c1.setGenetics(ch1);
	c1.generateFitness();
	c2.setSize(ch2.size());
	c2.setGenetics(ch2);
	c2.generateFitness();

	population.push_back(c1); // adding new children to the population
	population.push_back(c2);
}

void mutation() // mutates the genes in chromosomes by +1/-1 @ 2%
{
	//srand(time(NULL));
	int r;
	for (int i = POPSIZE / 2; i < POPSIZE; i++)
	{
		std::vector<int> chr = population[i].getGenetics(); // gets current chromosome
		for (int j = 0; j < size; j++)
		{
			r = rand() % 200 + 1; // using rand, we check if mutation occurs
			if(r == 2 || r == 3) // 1% chance to mutate upwards
			{
				if (chr[j] == size)
				{
					chr[j] = 1;
				}
				else
					chr[j]++;
			}
			else if (r == 1 || r == 4) // 1% chance to mutate downwards
			{
				if (chr[j] == 1)
				{
					chr[j] == size;
				}
				else
					chr[j]--;
			}
			else;
		}
	}
}

void printSol() // prints the squared board - empty positions as dots and Q as queen
{
	std::vector<int> winner = population[0].getGenetics(); // grabs winning board
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (winner[i] == (j+1))
				std::cout << " Q ";
			else
				std::cout << " . ";
		}
		std::cout << "\n";
	}
}

void main() // main function to start genetic Algorithm
{
	geneticAlgorithm();
}