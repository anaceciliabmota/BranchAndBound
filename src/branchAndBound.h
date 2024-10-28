#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include "../Simplex/src/solve.h"
#include "../Simplex/src/Data.h"
#include <list>
#include <iostream>
#include <string>
#include <iomanip>

#define EPSILON 1e-5


struct Node{
    SimplexRelaxation relaxation;
    VectorXd ub;
    VectorXd lb;
    bool feasible;

    void setSolution(SimplexRelaxation sol) {
        relaxation = sol;
    }

} typedef Node;

Node branchAndBound(Data& data, int branchingS);
int branching_strategy(char s[]);

void feasibility(Node& node);
Data modifyData(Data& data, Node& node);
Node select_node(int branchingS, list<Node> &tree, list<Node>::iterator &i);


#endif