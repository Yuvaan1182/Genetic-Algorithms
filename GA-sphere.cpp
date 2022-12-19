#include <bits/stdc++.h>
using namespace std;

class Genetic
{
    int population_size;
    double lb_bound;
    int ub_bound;
    int chromosome_size;
    double best_fit;
    vector<double> best_child;
    vector<vector<double>> population;

public:
    // Genetic Constructor
    Genetic(int pop_size, int num_of_chromosomes)
    {
        population_size = pop_size;
        chromosome_size = num_of_chromosomes;
        best_fit = INT_MIN;
        lb_bound = -10;
        ub_bound = 10;
    }

    // helper function to generate random number between 0 and 1
    double randomNumber()
    {
        return ((double)rand() / (RAND_MAX));
    }

    // Sphere function => fitness value calculation for each chromosome
    double sphere(vector<double> chromosome)
    {
        double val = 0;
        for (int i = 0; i < chromosome_size; i++)
        {
            val += chromosome[i] * chromosome[i];
        }

        return val;
    }

    // Creating first Generation
    void initializePopulation()
    {
        for (int i = 0; i < population_size; i++)
        {
            vector<double> chromosome;
            for (int j = 0; j < chromosome_size; j++)
            {
                double gene = lb_bound + randomNumber() * (ub_bound - lb_bound);
                chromosome.push_back(gene);
            }

            population.push_back(chromosome);
        }
    }

    // Performing One Point Crossover
    void crossover(vector<double> &first_pa, vector<double> &second_pa)
    {
        int point_of_crossover = chromosome_size * randomNumber();

        while (point_of_crossover == chromosome_size)
        {
            point_of_crossover = chromosome_size * randomNumber();
        }

        for (int i = point_of_crossover; i < chromosome_size; i++)
        {
            swap(first_pa[i], second_pa[i]);
        }
    }

    void createMatingPool(vector<vector<double>> &roullete_pop, vector<double> fitness, double tot_prob)
    {
        for (int i = 0; i < population_size; i++)
        {
            double prob_offspring = fitness[i] / tot_prob;

            int num_of_offspring_roullete_pop = (prob_offspring *= 100);

            for (int j = 0; j < num_of_offspring_roullete_pop; j++)
            {
                roullete_pop.push_back(population[i]);
            }
        }
    }

    vector<vector<double>> generateNewPopulation(vector<vector<double>> &roullete_pop, int roullete_pop_size)
    {
        vector<vector<double>> new_pop;

        for (int i = 0; i < population_size / 2; i++)
        {
            int first_child = randomNumber() * roullete_pop_size;
            int second_child = randomNumber() * roullete_pop_size;

            crossover(roullete_pop[first_child], roullete_pop[second_child]);

            new_pop.push_back(roullete_pop[first_child]);
            new_pop.push_back(roullete_pop[second_child]);
        }

        return new_pop;
    }

    void mutateCrossOverPopulation(vector<vector<double>> &crossover_pop, int crossover_pop_size)
    {
        for (int i = 0; i < crossover_pop_size; i++)
        {
            vector<double> child = crossover_pop[i];

            int mutation_idx = chromosome_size * randomNumber();
            child[mutation_idx] = lb_bound + (ub_bound - lb_bound) * randomNumber();

            double mutated_child_fitness = 1 / sphere(child);
            double parent_fitness = 1 / sphere(population[i]);

            if (mutated_child_fitness > parent_fitness)
            {
                crossover_pop[i] = child;
            }

            if (mutated_child_fitness > best_fit)
            {
                best_fit = mutated_child_fitness;
                best_child = child;
            }
        }
    }

    void roulleteWheel()
    {
        vector<double> fitness(population_size);
        vector<vector<double>> roullet_pop;
        double tot_val = 0;

        for (int i = 0; i < population_size; i++)
        {
            double val = sphere(population[i]);
            tot_val = val;
            fitness[i] = (1 / val);
        }

        // generating roullet wheel population
        createMatingPool(roullet_pop, fitness, tot_val);

        vector<vector<double>> crossover_pop = generateNewPopulation(roullet_pop, roullet_pop.size());

        // mutating some of crossover childs
        mutateCrossOverPopulation(crossover_pop, crossover_pop.size());

        population = crossover_pop;
    }

    void showResult()
    {
        cout << best_fit << endl;
        for (auto x : best_child)
        {
            cout << x << " ";
        }
        cout << endl;
    }
};

int main()
{
    // generating new seed for random number generation
    srand(time(NULL));

    int population_size = 50;
    int chromosome_size = 2;
    int num_of_generation = 2;

    Genetic *G = new Genetic(population_size, chromosome_size);
    G->initializePopulation();

    while (num_of_generation--)
    {
        G->roulleteWheel();
    }

    G->showResult();

    return 0;
}