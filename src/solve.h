#ifndef SOLVE_H
#define SOLVE_H

#include "Data.h"
#include "mpsReader.h"
#include "Basis.h"
#include <iostream>
#include <algorithm>
#include <limits>

#include <vector>
#include <time.h> 
#include <cmath>
#include <algorithm>
#include "../Eigen/Dense"
#include "../Eigen/Sparse"
#include <iomanip>

// Declaração da função que você quer chamar



struct SimplexRelaxation
{
    double z;
    VectorXd variaveis;
    bool feasible;
} typedef SimplexRelaxation;

struct Node{
    SimplexRelaxation relaxation;
    VectorXd ub;
    VectorXd lb;
    bool feasible;

    void setSolution(SimplexRelaxation sol) {
        relaxation = sol;
    }

} typedef Node;

SimplexRelaxation solve(Data& data);

#endif