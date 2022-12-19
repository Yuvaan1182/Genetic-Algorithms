#include <bits/stdc++.h>
using namespace std;

#define pi 3.141592653589793238

class particle
{
    int dim;
    double best_fit;
    vector<double> pos;
    vector<double> vel;
    vector<double> best_pos;

public:
    particle(int dimension, double lb, double ub)
    {
        this->dim = dimension;
        this->best_fit = INT_MIN;
        for (int i = 0; i < dimension; i++)
        {
            pos.push_back(lb + (ub - lb) * random());
            vel.push_back(lb + (ub - lb) * random());
        }
    }

    // helper function to generate random number between 0 and 1
    double random()
    {
        return ((double)rand() / (RAND_MAX));
    }

    vector<double> getPos()
    {
        return this->pos;
    }

    vector<double> getVel()
    {
        return this->vel;
    }

    double getBestFit()
    {
        return this->best_fit;
    }

    void updateBestFit(double fit_val)
    {
        this->best_fit = fit_val;
    }

    void updateBestPos()
    {
        this->best_pos = this->pos;
    }

    void updatePos()
    {
        for (int i = 0; i < this->dim; i++)
        {
            this->pos[i] += this->vel[i];
        }
    }

    void updateVel(vector<double> acc, double inertia, vector<double> glob_best)
    {
        for (int i = 0; i < this->dim; i++)
        {
            this->vel[i] = inertia * this->vel[i] + acc[i] * random() * (this->best_pos[i] - this->pos[i]) + acc[i] * random() * (glob_best[i] - this->pos[i]);
        }
    }
};

class PSOutility
{
    int dim;
    int pop_size;
    double lb, ub;
    double inertia;
    double glob_fit;
    vector<double> acc;
    vector<double> glob_best_pos;
    vector<particle *> population;

public:
    PSOutility(int dimension, int pop_size, double lb, double ub, vector<double> acceleration, double inertia)
    {
        this->dim = dimension;
        this->pop_size = pop_size;
        this->lb = lb;
        this->ub = ub;
        this->acc = acceleration;
        this->inertia = inertia;
    }

    void initializePopulation()
    {
        for (int i = 0; i < pop_size; i++)
        {
            particle *p = new particle(this->dim, this->lb, this->ub);
            double fitness_val = fitnessFunction(p);
            if (fitness_val > p->getBestFit())
            {
                p->updateBestFit(fitness_val);
                p->updateBestPos();

                if (fitness_val > this->glob_fit)
                {
                    this->glob_fit = fitness_val;
                    this->glob_best_pos = p->getPos();
                }
            }

            population.push_back(p);
        }
    }

    double fitnessFunction(particle *p)
    {
        double res;
        vector<double> particle_pos = p->getPos();

        for (int i = 0; i < this->dim; i++)
        {
            res += -20 * exp(-.2 * sqrt(.5 * (particle_pos[i] * particle_pos[i]))) - exp(.5 * cos(2 * pi * particle_pos[i])) + exp(1) + 20;
        }

        return res;
    }

    void performEvolution()
    {
        for (int i = 0; i < pop_size; i++)
        {
            // update velocity of particle
            population[i]->updateVel(this->acc, this->inertia, this->glob_best_pos);

            // update pos of particle
            population[i]->updatePos();

            // evaluate fitness
            double fit_val = fitnessFunction(population[i]);

            if (fit_val < population[i]->getBestFit())
            {
                population[i]->updateBestFit(fit_val);
                population[i]->updateBestPos();
            }

            if (fit_val < this->glob_fit)
            {
                this->glob_fit = fit_val;
                this->glob_best_pos = population[i]->getPos();
            }
        }
    }

    void showResults()
    {
        cout << this->glob_fit << endl;

        for (auto x : glob_best_pos)
        {
            cout << x << " ";
        }

        cout << endl;
    }
};

int main()
{
    int dim = 2;
    int pop_size = 100;
    vector<double> acc = {1.3, 2.3};
    double inertia = 0.5;
    double lb = -100;
    double ub = 100;
    int generations = 200;

    PSOutility *pso = new PSOutility(dim, pop_size, lb, ub, acc, inertia);

    // initialize population
    pso->initializePopulation();

    // loop through generations
    while (generations--)
    {
        // evolve the generation
        pso->performEvolution();
    }

    // show results of the curr evolution
    pso->showResults();

    return 0;
}
