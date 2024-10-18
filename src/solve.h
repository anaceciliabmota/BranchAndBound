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

struct Solution
{
    double z;
    MatrixXd variaveis_basicas;
    MatrixXd variaveis_nao_basicas;
} typedef Solution;

struct Problem{
    Solution s;
    Data data;
    bool feasible;
    Problem(Data d) : data(d) {}

    void setSolution(Solution sol) {
        s = sol;
    }

} typedef Problem;

Solution solve(Data& data);

#endif