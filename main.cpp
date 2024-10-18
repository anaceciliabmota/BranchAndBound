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

Problem select_node(int branchingS, list<Problem> &tree, list<Problem>::iterator &i)
{
	if (branchingS == 1)
	{
		// BFS
		i = tree.begin();
		return tree.front();
	}
	// DFS
	i = --tree.end();
	return tree.back();
}

void feasibility(Problem& p){
  p.feasible = true;
  for(int i = 0; i < p.s.variaveis_basicas.rows(); i++){
    double x = p.s.variaveis_basicas(i, 1);
    int x_inteiro = round(x);
    if(abs(x - x_inteiro) > EPSILON){
      p.feasible = false;
    }
  }
}


Problem BnB(Data& data, int branchingS){


  list<Problem>::iterator it;
  Problem root(data);
  Solution s = solve(root.data);
  root.setSolution(s);
  cout << s.z << endl;
  cout << root.s.z << endl;
  list<Problem> tree;

  tree.push_back(root);

  double upper_bound = -numeric_limits<double>::infinity();
  Problem best(data); //is going to be change by the end of the algorithm

  int cont = 0;
  while(!tree.empty()){
    cont++;
    Problem p = select_node(branchingS, tree, it);
    p.setSolution(solve(p.data));
    cout << p.s.z << endl;
    
    feasibility(p);
    if(p.feasible){
      cout << "VIAVEL: " << p.s.z << endl;
    }
    if (p.s.z < upper_bound)
  	{
      tree.erase(it);
   		continue;

    }else if(p.feasible){

      if(p.s.z > upper_bound){
        upper_bound = p.s.z;
        best = p;
      }
    }else{
      
      VectorXd ub = *(p.data.getVectorU());
      cout << ub.transpose() << endl;
      VectorXd lb = *(p.data.getVectorL());
      cout << lb.transpose() << endl;
      int index;
      double x;
      double max_value = numeric_limits<double>::infinity();

      int tamanho = p.s.variaveis_basicas.rows();

      for(int i = 0; i < p.s.variaveis_basicas.rows(); i++){
        
        double aux = p.s.variaveis_basicas(i, 1);
      
        if(abs(aux - 0.5) < max_value && p.s.variaveis_basicas(i, 0) < tamanho){
          x = aux;
          index = static_cast<int>(p.s.variaveis_basicas(i, 0));
          max_value = abs(x-0.5);
        }
      }

  
      // for(int i = 0; i < p.s.variaveis_nao_basicas.rows(); i++){
        
      //   double aux = p.s.variaveis_nao_basicas(i, 1);
        
      //   if(abs(aux - 0.5) < max_value){
      //     x = aux;
      //     index = static_cast<int>(p.s.variaveis_nao_basicas(i, 0));
      //     max_value = abs(x - 0.5);
      //   }
      // }
      cout << endl;

      cout << x << " " << index << " " << endl;

      Problem novo1(p.data);
      Problem novo2(p.data);

      ub(index) = 1;
      lb(index) = 1;

    //   cout << ub.transpose() << endl;
    //   cout << lb.transpose() << endl << endl;

      novo1.data.setVectorL(lb);
      novo1.data.setVectorU(ub);

      ub(index) = 0;
      lb(index) = 0;

      novo2.data.setVectorL(lb);
      novo2.data.setVectorU(ub);

      tree.push_back(novo1);
      tree.push_back(novo2);
    }
    //if(cont > 5){
      // break;
    //}
    cout << p.s.z << endl;
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

    cout << branchingS << endl;

    cout <<"fo: " << data.getFO()->transpose() << endl;

    MatrixXd A = MatrixXd(*data.getMatrixA());
    cout << "matrizA: " << endl << A << endl;
    cout << "rhs: " << data.getRHS()->transpose() << endl;
    cout << "lb: " << data.getVectorL()->transpose() << endl;
    cout << "ub: " << data.getVectorU()->transpose() << endl;

    Solution s = solve(data);

    cout << s.z << endl;

    double x = 23.456;
    cout << x - static_cast<int>(x) << endl;

    Problem best_solution = BnB(data, branchingS);

    cout << best_solution.s.z << endl;

    return 0;
}


