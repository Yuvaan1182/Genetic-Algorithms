#include <bits/stdc++.h>
using namespace std;

#define A 10
#define pi 3.141592653589793238

class ABC
{
    int dimension;
    long double lb_bound, ub_bound;
    vector<vector<long double>> population;
    vector<long double> fitness;
    vector<int> trials;
    int limit;
    int population_size;

public:
    ABC(int dim, long double lb, long double ub, int pop_size)
    {
        dimension = dim;
        lb_bound = lb;
        ub_bound = ub;
        population_size = pop_size;
        limit = population_size * dimension;
        trials.resize(population_size, 0);
    }

    long double randomNumber()
    {
        long double random = (long double)rand() / RAND_MAX;
        return random;
    }

    void initializePopulation()
    {
        for (int i = 0; i < population_size; i++)
        {
            vector<long double> ind;
            for (int j = 0; j < dimension; j++)
            {
                ind.push_back(randomNumber() * (ub_bound - lb_bound) + lb_bound);
            }

            population.push_back(ind);
        }
    }

    long double rastrigin(vector<long double> sol)
    {
        long double res = A * dimension;
        for (int i = 0; i < dimension; i++)
        {
            res += sol[i] * sol[i] + A * cos(2 * pi * sol[i]);
        }

        return res;
    }

    long double evaluateFitness(long double val)
    {
        if (val == 0)
        {
            return INT_MAX;
        }

        return 1 / val;
    }

    void calculateFitness()
    {
        for (int i = 0; i < population.size(); i++)
        {
            long double fit_val = evaluateFitness(rastrigin(population[i]));
            fitness.push_back(fit_val);
        }
    }

    void employee()
    {
        for (int i = 0; i < population_size; i++)
        {
            auto new_solution = this->genNewSoln(i);
            long double new_fitness = this->evaluateFitness(this->rastrigin(new_solution));
            if (new_fitness > fitness[i])
            {
                population[i] = new_solution;
                fitness[i] = new_fitness;
                trials[i] = 0;
            }
            else
            {
                trials[i] += 1;
            }
        }
    }

    void onlooker()
    {
        vector<long double> prob(population_size);
        long double total = 0;
        for (int i = 0; i < population_size; i++)
        {
            total += fitness[i];
        }
        for (int i = 0; i < population_size; i++)
        {
            prob[i] = fitness[i] / total;
        }

        int i = 0, j = 0;
        while (i < population_size)
        {
            long double rnd = randomNumber();
            if (rnd <= prob[i])
            {
                auto new_solution = this->genNewSoln(j);
                auto new_fitness = this->evaluateFitness(this->rastrigin(new_solution));
                if (new_fitness > fitness[i])
                {
                    population[i] = new_solution;
                    fitness[i] = new_fitness;
                    trials[i] = 0;
                }
                else
                {
                    trials[i]++;
                }
                i++;
            }
            j = (j + 1) % population_size;
        }
    }

    vector<long double> genNewSoln(int i)
    {
        auto current = population[i];
        auto partner = population[getRand(i)];
        auto res = current;
        int j = randomNumber() * dimension;
        res[j] = current[j] + ((randomNumber() * 2) - 1) * (current[j] - partner[j]);
        if (res[j] < lb_bound)
            res[j] = lb_bound;
        else if (res[j] > ub_bound)
            res[j] = ub_bound;
        return res;
    }

    int getRand(int exclude)
    {
        int rnd = randomNumber() * dimension;
        if (rnd == exclude)
            return getRand(exclude);
        else
            return rnd;
    }

    pair<long double, vector<long double>> getSolution()
    {
        int index = 0;
        for (int i = 0; i < population_size; i++)
        {
            if (fitness[i] > fitness[index])
            {
                index = i;
            }
        }
        return {fitness[index], population[index]};
    }

    void scout()
    {
        int k = 0;
        for (int i = 0; i < population_size; i++)
        {
            if (trials[k] < trials[i])
                k = i;
        }
        if (trials[k] <= limit)
            return;

        vector<long double> ind;
        for (int j = 0; j < dimension; j++)
        {
            ind.push_back(randomNumber() * (ub_bound - lb_bound) + lb_bound);
        }
        population[k] = ind;
        trials[k] = 0;
    }
};

int main()
{
    srand(time(NULL));
    int pop_size = 50;
    int generation = 10;
    int dim = 2;
    long double lb = -5.12;
    long double ub = 5.12;

    ABC *abc = new ABC(dim, lb, ub, pop_size);

    abc->initializePopulation();

    abc->calculateFitness();

    auto best = abc->getSolution();

    while (generation--)
    {
        // employed
        abc->employee();
        // onlooker
        abc->onlooker();
        // best
        auto temp_best = abc->getSolution();
        if (temp_best.first > best.first)
        {
            best = temp_best;
        }
        // scout
        abc->scout();
    }

    for (int i : best.second)
        cout << i << " ";
    cout << endl;
    cout << (abc->rastrigin(best.second)) << endl;
    return 0;
}