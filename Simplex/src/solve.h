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

struct SimplexRelaxation
{
    double z;
    VectorXd variables;
    bool feasible;
} typedef SimplexRelaxation;

SimplexRelaxation solve(Data& data);

#endif