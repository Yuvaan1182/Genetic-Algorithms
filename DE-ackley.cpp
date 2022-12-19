#include <bits/stdc++.h>
using namespace std;
#define A 20
#define pi 3.141592653589793238

class DiffEvol
{
    int dimension;
    double beta;
    int population_size;
    vector<double> fitness;
    vector<vector<double>> population;
    double lb, ub;
    double prob_recombination;

public:
    DiffEvol(double be, double dim, double lb_bound, double ub_bound, double prob_recomb)
    {
        lb = lb_bound;
        ub = ub_bound;
        dimension = dim;
        beta = be;
        prob_recombination = prob_recomb;
        population_size = 70;
    }

    // helper function to generate random number between 0 and 1
    double randomNumber()
    {

        double random = ((double)rand() / (RAND_MAX));
        return random;
    }

    void initializePopulation()
    {
        for (int i = 0; i < population_size; i++)
        {
            vector<double> solution;
            for (int j = 0; j < dimension; j++)
            {
                double val = lb + (ub - lb) * randomNumber();
                solution.push_back(val);
            }

            population.push_back(solution);
        }
    }

    double ackley(vector<double> sol)
    {
        double res = 0;
        for (int i = 0; i < dimension; i++)
        {
            res += -20 * exp(-0.2 * sqrt(0.5 * (sol[i] * sol[i]))) - exp(0.5 * (cos(2 * pi * sol[i]))) + exp(1) + 20;
        }

        return res;
    }

    double evaluateFitness(double val)
    {
        if (val == 0)
        {
            return INT_MAX;
        }

        return 1 / val;
    }

    int calculateFitness()
    {
        double max_fit = INT_MIN;
        int mx_fit_idx;

        for (int i = 0; i < population.size(); i++)
        {
            double fit_val = evaluateFitness(ackley(population[i]));

            if (fit_val > max_fit)
            {
                max_fit = fit_val;
                mx_fit_idx = i;
            }

            fitness.push_back(fit_val);
        }

        return mx_fit_idx;
    }

    void reproduce(int mx_fit_idx)
    {
        vector<vector<double>> new_pop;

        for (int i = 0; i < population_size; i++)
        {
            vector<double> mutation_child = mutation(i, mx_fit_idx);
            vector<double> crossover_child = crossover(mutation_child, i);

            double fitness_child = evaluateFitness(ackley(crossover_child));
            double fitness_parent = fitness[i];

            if (fitness_child > fitness_parent)
            {
                new_pop.push_back(crossover_child);
            }
            else
            {
                new_pop.push_back(population[i]);
            }
        }

        population = new_pop;
    }

    vector<double> mutation(int pa_idx, int mx_idx)
    {
        int r1_idx = randomNumber() * population_size;
        int r2_idx = randomNumber() * population_size;

        while (r1_idx == r2_idx or r1_idx == pa_idx or r1_idx == mx_idx or r2_idx == pa_idx or r2_idx == mx_idx)
        {
            r1_idx = randomNumber() * population_size;
            r2_idx = randomNumber() * population_size;
        }

        vector<double> trial;

        for (int i = 0; i < dimension; i++)
        {
            double val = population[mx_idx][i] + beta * (population[r1_idx][i] - population[r2_idx][i]);

            if (val < lb)
            {
                val = lb;
            }

            if (val > ub)
            {
                val = ub;
            }

            trial.push_back(val);
        }

        return trial;
    }

    vector<double> crossover(vector<double> trial, int pa_idx)
    {
        vector<double> sol = population[pa_idx];

        int delta = randomNumber() * dimension;

        for (int i = 0; i < dimension; i++)
        {
            if (randomNumber() < prob_recombination or i == delta)
            {
                sol[i] = trial[i];
            }
        }

        return sol;
    }

    vector<double> resultOfEvolution()
    {
        vector<double> res;

        double mx_fitness;

        for (int i = 0; i < population_size; i++)
        {
            double fit = evaluateFitness(ackley(population[i]));

            if (fit > mx_fitness)
            {
                mx_fitness = fit;
                res = population[i];
            }
        }

        return res;
    }
};

int main()
{
    srand(time(NULL));
    double be = 0.5;
    double dim = 2;
    double lb_bound = -5;
    double ub_bound = 5;
    double prob_recomb = 0.5;
    int generation = 70;

    DiffEvol *DE = new DiffEvol(be, dim, lb_bound, ub_bound, prob_recomb);
    DE->initializePopulation();

    while (generation--)
    {
        int best_fit_idx = DE->calculateFitness();
        DE->reproduce(best_fit_idx);
    }

    vector<double> res = DE->resultOfEvolution();

    double val = DE->ackley(res);

    for (auto x : res)
    {
        cout << x << " ";
    }

    cout << endl;

    cout << val << endl;

    return 0;
}