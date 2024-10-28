 
#include "Data.h"

#define infinity std::numeric_limits<double>::infinity()

using namespace std;
using namespace Eigen;
Data::Data(MatrixXd& mps_A, VectorXd& mps_b, VectorXd& mps_c, VectorXd& mps_ub, VectorXd& mps_lb)
   {
      A = mps_A.sparseView();
      rhs = mps_b;
      fo = mps_c;
      u = mps_ub;
      l = mps_lb;
  }

Data::Data(std::string fileName) {

    int m, n, col;
    double number;
    //std::string line;
    ifstream readFile(fileName);

    if (readFile.is_open()) {
      getDimensions(readFile, &m, &n);

      rhs = VectorXd(m);

      //le vetor c
      fo = LeVetor(readFile, n, m);

      fo = -1 * fo;

      MatrixXd A_dense = LeMatrix(readFile, m, n, rhs);

      A = A_dense.sparseView();

      l = VectorXd(n+m); 

      l.head(n+m).setZero(); // fixa lowebounds em 0

      u = VectorXd(n+m);

      for (int i = 0; i < n; i++){
         u(i) = 1; //fixa upperbounds em 1
      }
      for (int i = n; i < m+n; i++){
         u(i) = infinity; //os upperbound das variaveis de folga são de valor infinito
      }

      readFile.close();
    } else {
        cerr << "Não foi possível abrir o arquivo " << fileName << endl;
    }

}

void Data::getDimensions(ifstream &readFile, int* m, int* n){
      string line;
      getline(readFile, line);
      std::istringstream thisLine(line);
      thisLine >> *n >> *m;
}

MatrixXd Data::LeMatrix(ifstream &readFile, int m, int n, VectorXd& rhs){
   // Leitura das linhas da matriz A
      string line;
      double number;
      int col;
      //n + m colunas porque as m ultimas colunas sao as variaveis de folga
      MatrixXd matrizA(m, n+m);
      for (int row = 0; row < m; ++row) {
         getline(readFile, line);
         std::istringstream thisLine(line);
         col = 0;
         while (thisLine >> number) {
            if (col == n){
               rhs(row) = number;
               continue;
            }else{
                matrizA(row, col) = number;
            col++;
            }
         }

         matrizA(row, n + row) = 1; 

         for (int i = n; i < n + m; ++i) {
            if (i != n + row) {
                  matrizA(row, i) = 0; 
            }
         }
      }
      return matrizA;
}

VectorXd Data::LeVetor(ifstream &readFile, int n, int m){
   VectorXd vetor(n+m);
   string number;
   int col = 0;
   string line;
   getline(readFile, line);
   std::istringstream thisLine(line);
   while(thisLine >> number){
      vetor(col) = stod(number);
      col++;
   }
   for (int i = n; i < n + m; ++i) {
      vetor(i) = 0; 
   }
   return vetor;
}


double Data::safe_stod(const std::string& str) {
    try {
        size_t pos;
        double val = std::stod(str, &pos);
        // Verifica se a string inteira foi convertida
        if (pos != str.length()) {
            throw std::invalid_argument("A string contém caracteres inválidos.");
        }
        return val;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: argumento inválido. A string fornecida não é um número válido: " << str << std::endl;
        throw; // Lança a exceção novamente para ser tratada no nível superior, se necessário
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: número fora do intervalo permitido: " << str << std::endl;
        throw; // Lança a exceção novamente para ser tratada no nível superior, se necessário
    }
}






