#include "Simplex/src/solve.h"
#include "Simplex/src/Data.h"
#include <list>
#include <iostream>
#include <string>

#define EPSILON 1e-5

int branching_strategy(char s[]){
  //função que atribui um valor numérico para representar a estratégia de branching escolhida
	int branchingS;
	if (strcmp(s, "BFS") == 0)
		branchingS = 1;
	else if (strcmp(s, "DFS") == 0)
		branchingS = 2;
	else{
		cout << "Estrategia de ramificação não válida" << endl;
    exit(0);
	}
	return branchingS;
}

Node select_node(int branchingS, list<Node> &tree, list<Node>::iterator &i){
	
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
  for(int i = 0; i < node.relaxation.variaveis.size(); i++){
    double x = node.relaxation.variaveis(i);
    int x_inteiro = round(x);
    if(abs(x - x_inteiro) > EPSILON)
      //verifica se a diferença é significativa
      node.feasible = false;
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

  list<Node> tree;
  tree.push_back(root);
  
  Node best; //armazenará a melhor solução viável
  best.relaxation.z = numeric_limits<double>::infinity();

  while(!tree.empty()){
    
    Node node = select_node(branchingS, tree, it); // seleciona o nó
    Data modified_data = modifyData(data, node); // adiciona as restrições 'extras'
    node.setSolution(solve(modified_data)); // resolve
    feasibility(node); // verifica se solução é viável (inteira)
   
    if(node.feasible && node.relaxation.feasible){
      // corte pelo critério da integralidade
      if(node.relaxation.z + EPSILON < best.relaxation.z)
        best = node;

    }else if(!node.relaxation.feasible || node.relaxation.z > best.relaxation.z){
      // corte por inviabilidade e por limite
      tree.erase(it);
      continue;
    }else{

      int index;
      double diff = numeric_limits<double>::infinity();

      // encontra a variável mais próxima de 0.5
      for(int i = 0; i < node.relaxation.variaveis.rows(); i++){
        double val = node.relaxation.variaveis(i);
        if(abs(val - 0.5) + EPSILON < diff){
          index = i;
          diff = abs(val - 0.5);
        }
      }

      // adiciona filho em que xj == 1
      Node child1;
      child1.ub = node.ub; 
      child1.lb = node.lb; 
      child1.ub(index) = 1;
      child1.lb(index) = 1;
      tree.push_back(child1);

      // adiciona filho em que xj == 0
      Node child2;
      child2.ub = node.ub;
      child2.lb = node.lb;
      child2.ub(index) = 0;
      child2.lb(index) = 0;
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


