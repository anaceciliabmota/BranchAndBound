#include "src/solve.h"
#include "src/Data.h"
#include <list>
#include <iostream>
#include <string>
#define EPSILON 1e-5

int branching_strategy(char s[]){
	int branchingS;
	if (strcmp(s, "BFS") == 0)
	{
		branchingS = 1;
	}
	else if (strcmp(s, "DFS") == 0)
	{
		branchingS = 2;
	}
	else
	{
		cout << "Non valid Branching Strategy" << endl;
    exit(0);
		return 0;
	}
	return branchingS;
}

Node select_node(int branchingS, list<Node> &tree, list<Node>::iterator &i)
{
	if (branchingS == 1)
	{
		//BFS
		i = tree.begin();
		return tree.front();
	}
	//DFS
	i = --tree.end();
	return tree.back();
}


void feasibility(Node& node){
  node.feasible = true;
  //cout << node.relaxation.variaveis.transpose() << endl;
  for(int i = 0; i < node.relaxation.variaveis.size(); i++){
    double x = node.relaxation.variaveis(i);
    int x_inteiro = round(x);
    if(abs(x - x_inteiro) > EPSILON){
      node.feasible = false;
    }
  }
}

Data modifyData(Data& data, Node& node){
  
  Data new_data = data;
  new_data.setVectorU(node.ub);
  new_data.setVectorL(node.lb);
  return new_data;
}


Node branchAndBound(Data& data, int branchingS){

  list<Node>::iterator it;
  Node root;
  root.ub = *data.getVectorU();
  root.lb = *data.getVectorL();

  root.setSolution(solve(data));
  list<Node> tree;

  tree.push_back(root);
  
  Node best;
  best.relaxation.z = numeric_limits<double>::infinity();
  
  int cont = 0;

  while(!tree.empty()){
    cont++;
    Node node = select_node(branchingS, tree, it);
    
    Data modified_data = modifyData(data, node);

    node.setSolution(solve(modified_data));
    
    feasibility(node);
   
    if(node.feasible && node.relaxation.feasible){

      if(node.relaxation.z + EPSILON < best.relaxation.z)
        best = node;

    }else if(!node.relaxation.feasible || node.relaxation.z > best.relaxation.z){
      tree.erase(it);
      continue;

    }else{

      int index;
      double diff = numeric_limits<double>::infinity();

      for(int i = 0; i < node.relaxation.variaveis.rows(); i++){
        double val = node.relaxation.variaveis(i);
        if(abs(val - 0.5) < diff){
          index = i;
          diff = abs(val - 0.5);
        }
      }

      Node child1;
      Node child2;
      
      child1.ub = node.ub;
      child1.lb = node.lb;
      child1.ub(index) = 1;
      child1.lb(index) = 1;

      child2.ub = node.ub;
      child2.lb = node.lb;
      child2.ub(index) = 0;
      child2.lb(index) = 0;

      tree.push_back(child1);
      tree.push_back(child2);

    }
    tree.erase(it);
  }
  return best;
}



int main (int argc, char ** argv){

   
    if (argc < 2)
    {
      cout << "ERRO: necessário a passagem do caminho pra a instância como primeiro argumento e a estratégia de busca como segunda" << endl;
      cout << "Digite BFS para busca em largura" << endl;
      cout << "Digite DFS para busca em profundidade" << endl;
      return 1;
    }
    else if (argc < 3){
      cout << "ERRO: necessário a passagem da estratégia de busca como segundo argumento" << endl;
      cout << "Digite BFS para busca em largura" << endl;
      cout << "Digite DFS para busca em profundidade" << endl;
      return 1;
    }
    string arquivo = argv[1];
    Data data(arquivo);

    int branchingS = branching_strategy(argv[2]);

    cout <<"fo: " << data.getFO()->transpose() << endl;

    MatrixXd A = MatrixXd(*data.getMatrixA());
    cout << "matrizA: " << endl << A << endl;
    cout << "rhs: " << data.getRHS()->transpose() << endl;
    cout << "lb: " << data.getVectorL()->transpose() << endl;
    cout << "ub: " << data.getVectorU()->transpose() << endl;

    Node best_solution = branchAndBound(data, branchingS);

    cout << best_solution.relaxation.z << endl;

    return 0;
}


