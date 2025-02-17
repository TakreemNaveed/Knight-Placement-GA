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
// Randomly places knights on the chessboard for each chromosome.
void RandomBoards(Chromosome population[])
{
    srand(time(0));

    /*I used variable i to loop through the population size throughout the program
    and variable j and k for rows and columns respectively*/
    for (int i = 0; i < PopulationSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            for (int k = 0; k < boardSize; k++)
            { 
                population[i].knights[j][k] = 0;
            }
        }
    }
}

// 2. Function to calculate the fitness value of each chromosome in the population
// Checks for empty spaces in the rows and columns of the chessboard.
void fitnessfunction(Chromosome *chromosome)
{
    int attackedSquares[boardSize][boardSize] = {0}; // initialization for attacked squares array

    for (int j = 0; j < boardSize; j++)
    {
        for (int k = 0; k < boardSize; k++)
        {
            if (chromosome->knights[j][k])
            {
                for (int a = -2; a <= 2; a++)
                {
                    for (int b = -2; b <= 2; b++)
                    {
                        int newRow = j + a;    // row which is attacked by the knight
                        int newColumn = j + b; // column which is attacked by the knight

                        if (newRow >= 0 && newRow < boardSize && newColumn >= 0 && newColumn < boardSize)
                        {
                            attackedSquares[newRow][newColumn] = 1; // sqaures attacked by the knight
                        }
                    }
                }
            }
        }
    }

    // check if any square is not attacked by the knight
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

// 3. Function to generate the next generation of chromosomes
// Sorts chromosomes based on fitness in ascending order.
void NextGeneration(Chromosome population[])
{
    Chromosome temp; // temporary variable

    for (int i = 0; i < PopulationSize - 1; i++)
    {
        for (int j = i + 1; j < PopulationSize - i - 1; j++)
        {
            if (population[j].fitness > population[j + 1].fitness)
            {

                // swap which has high fitness
                temp = population[j];
                population[j] = population[j + 1];
                population[j + 1] = temp;
            }
        }
    }
}

// 4. Function to perform crossover between two parent chromosomes to produce two child chromosomes
// Uses a crossover point and exchanges genes between parents.
void crossover(Chromosome *parent1, Chromosome *parent2, Chromosome *child1, Chromosome *child2)
{

    for (int j = 0; j < boardSize; j++)
    {

        for (int k = 0; k < boardSize; k++)
        {
            // Check if the current column is outside the crossover point
            if (j < CrossoverPoint || j >= CrossoverPoint)
            {
                // If outside the crossover point, copy genes from parents to children
                child1->knights[j][k] = parent1->knights[j][k];
                child2->knights[j][k] = parent2->knights[j][k];
            }
            else
            {
                // If inside the crossover point, exchange genes between parents for children
                child1->knights[j][k] = parent2->knights[j][k];
                child2->knights[j][k] = parent1->knights[j][k];
            }
        }
    }
}

// 5. Function to generate a random integer in the specified range [min, max]
// Returns a random integer between min and max.
int randomInt(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

// 6. Function to perform mutation on a chromosome
// Randomly toggles the presence of a knight based on a mutation rate.
void mutation(Chromosome *chromosome)
{
    float mutationRate = 0.5;

    for (int j = 0; j < boardSize; j++)
    {
        for (int k = 0; k < boardSize; k++)
        {
            float randomnumber = (float)rand() / RAND_MAX; // generate a random number between 0 and 1

            if (randomnumber < mutationRate)
            {
                chromosome->knights[j][k] = !chromosome->knights[j][k];
            }
        }
    }
}

// 7. Function to display the entire population on the screen (for debugging purposes)
// Prints each chromosome's knight positions and fitness value.
/*void Display_Population(Chromosome population[])
{
    for (int i = 0; i < PopulationSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            for (int k = 0; k < boardSize; k++)
            {
                printf("%d", population[i].knights[j][k]);
            }
        }
        printf("\t%d", population[i].fitness);
        printf("\n");
    }
    printf("\n");
}*/

// 8. Function to print the chessboard representing a solution
// Prints the chessboard with knights represented by 'O' and empty spaces by 'X'.
void printBoard(Chromosome *chromosome)
{
    printf("   1  2  3  4  5  6  7  8\n");

    for (int j = 0; j < boardSize; j++)
    {
        printf("%d |", j + 1);
        for (int k = 0; k < boardSize; k++)
        {
            if (chromosome->knights[j][k])
            {
                printf("O||");
            }
            else
            {
                printf("X||");
            }
        }
        printf("\n");
    }
}
#include <stdio.h>

int main()
{
    srand(time(NULL));

    Chromosome population[PopulationSize];
    Chromosome offspring[PopulationSize];

    RandomBoards(population);

    int generation;
    for (generation = 0; generation < maxGenerations; generation++)
    {
        // Calculate fitness for each chromosome in the population
        for (int i = 0; i < PopulationSize; i++)
        {
            fitnessfunction(&population[i]);
        }

        // Sort the population based on fitness
        NextGeneration(population);

        // Crossover and mutation to create the next generation
        for (int i = 0; i < PopulationSize; i += 2)
        {
            crossover(&population[i], &population[i + 1], &offspring[i], &offspring[i + 1]);
            mutation(&offspring[i]);
            mutation(&offspring[i + 1]);
        }

        // Replace the old population with the new one
        for (int i = 0; i < PopulationSize; i++)
        {
            population[i] = offspring[i];
        }
    }

    // Find the best solution in the final population
    Chromosome *Solution = &population[0];
    for (int i = 1; i < PopulationSize; i++)
    {
        if (population[i].fitness < Solution->fitness)
        {
            Solution = &population[i];
        }
    }

    // Display the best solution
    printf("Best solution found in generation %d:\n", generation);
    printBoard(Solution);

    return 0;
}