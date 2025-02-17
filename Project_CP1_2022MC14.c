/*
Written by Takreem Naveed (2022-MC-14) on 06-Jan-2024.
IDE: Visual Studio Code 1.60.0
C Compiler: GCC (Rev. 5, Built by MSYS2 Project) 10.3.0 */

// Built-in Libraries--------------------------------------------------------
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Pre-defined Macros---------------------------------------------------------
#define boardSize 8
#define PopulationSize 40
#define maxGenerations 1000
#define CrossoverPoint 4

// Structure to represent a chromosome (solution)
typedef struct
{
    int knights[boardSize][boardSize];
    int fitness;
} Chromosome;

// 1. Function to generate random chromosomes for the initial population
void RandomBoards(Chromosome population[])
{
    for (int i = 0; i < PopulationSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            for (int k = 0; k < boardSize; k++)
            { 
                population[i].knights[j][k] = rand() % 2; // Randomly place knights
            }
        }
    }
}

// 2. Function to calculate the fitness value of each chromosome
void fitnessfunction(Chromosome *chromosome)
{
    int attackedSquares[boardSize][boardSize] = {0};

    for (int j = 0; j < boardSize; j++)
    {
        for (int k = 0; k < boardSize; k++)
        {
            if (chromosome->knights[j][k])
            {
                int moves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                   {1, -2},  {1, 2},  {2, -1},  {2, 1}};
                for (int m = 0; m < 8; m++)
                {
                    int newRow = j + moves[m][0];
                    int newColumn = k + moves[m][1];
                    if (newRow >= 0 && newRow < boardSize && newColumn >= 0 && newColumn < boardSize)
                    {
                        attackedSquares[newRow][newColumn] = 1;
                    }
                }
            }
        }
    }

    int fitnessvalue = 0;
    for (int j = 0; j < boardSize; j++)
    {
        for (int k = 0; k < boardSize; k++)
        {
            if (!attackedSquares[j][k])
            {
                fitnessvalue++;
            }
        }
    }
    chromosome->fitness = fitnessvalue;
}

// 3. Function to sort the population based on fitness
void NextGeneration(Chromosome population[])
{
    for (int i = 0; i < PopulationSize - 1; i++)
    {
        for (int j = 0; j < PopulationSize - i - 1; j++)
        {
            if (population[j].fitness > population[j + 1].fitness)
            {
                Chromosome temp = population[j];
                population[j] = population[j + 1];
                population[j + 1] = temp;
            }
        }
    }
}

// 4. Function to perform crossover between two parents
void crossover(Chromosome *parent1, Chromosome *parent2, Chromosome *child1, Chromosome *child2)
{
    for (int j = 0; j < boardSize; j++)
    {
        for (int k = 0; k < boardSize; k++)
        {
            if (j < CrossoverPoint)
            {
                child1->knights[j][k] = parent1->knights[j][k];
                child2->knights[j][k] = parent2->knights[j][k];
            }
            else
            {
                child1->knights[j][k] = parent2->knights[j][k];
                child2->knights[j][k] = parent1->knights[j][k];
            }
        }
    }
}

// 5. Function to perform mutation on a chromosome
void mutation(Chromosome *chromosome)
{
    float mutationRate = 0.01; // Lower mutation rate
    for (int j = 0; j < boardSize; j++)
    {
        for (int k = 0; k < boardSize; k++)
        {
            if ((float)rand() / RAND_MAX < mutationRate)
            {
                chromosome->knights[j][k] ^= 1; // Toggle the knight
            }
        }
    }
}

// 6. Function to print the chessboard
void printBoard(Chromosome *chromosome)
{
    printf("  ");
    for (int k = 0; k < boardSize; k++)
        printf("%2d ", k + 1);
    printf("\n");

    for (int j = 0; j < boardSize; j++)
    {
        printf("%2d|", j + 1);
        for (int k = 0; k < boardSize; k++)
        {
            printf("%s ", chromosome->knights[j][k] ? "O" : "X");
        }
        printf("\n");
    }
}

int main()
{
    srand(time(NULL));

    Chromosome population[PopulationSize];
    Chromosome offspring[PopulationSize];

    RandomBoards(population);

    int generation;
    for (generation = 0; generation < maxGenerations; generation++)
    {
        // Calculate fitness
        for (int i = 0; i < PopulationSize; i++)
            fitnessfunction(&population[i]);

        // Sort population by fitness
        NextGeneration(population);

        // Generate offspring
        for (int i = 0; i < PopulationSize; i += 2)
        {
            crossover(&population[i], &population[i + 1], &offspring[i], &offspring[i + 1]);
            mutation(&offspring[i]);
            mutation(&offspring[i + 1]);
        }

        // Replace old population with offspring
        for (int i = 0; i < PopulationSize; i++)
            population[i] = offspring[i];
    }

    // Find the best solution
    Chromosome *bestSolution = &population[0];
    for (int i = 1; i < PopulationSize; i++)
    {
        if (population[i].fitness < bestSolution->fitness)
            bestSolution = &population[i];
    }

    printf("Best solution found (Fitness: %d):\n", bestSolution->fitness);
    printBoard(bestSolution);

    return 0;
}
