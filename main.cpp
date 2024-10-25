#include "branchAndBound.h"
#include <ctime>
#include <iomanip>

int main (int argc, char ** argv){
  clock_t start, end;
  start = clock();

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
  cout << "Lendo arquivo..." << endl;
  Data data(arquivo);

  int branchingS = branching_strategy(argv[2]);

  // cout <<"fo: " << data.getFO()->transpose() << endl;

  // MatrixXd A = MatrixXd(*data.getMatrixA());
  // cout << "matrizA: " << endl << A << endl;
  // cout << "rhs: " << data.getRHS()->transpose() << endl;
  // cout << "lb: " << data.getVectorL()->transpose() << endl;
  // cout << "ub: " << data.getVectorU()->transpose() << endl;

  cout << "Resolvendo..." << endl;
  Node best_solution = branchAndBound(data, branchingS);
  end = clock();

  cout << "obj = " << setprecision(5) << best_solution.relaxation.z << endl;
  double time_taken = double(end - start) / CLOCKS_PER_SEC;
  cout << "TEMPO USADO: " << time_taken << " segundos" << endl;
  return 0;
}


