# Branch and Bound
Implementação do Branch and Bound desenvolvida em **C++** para resolver problemas de Programação Linear Inteira, especificamente para variáveis binárias

## Sobre
Esse projeto foi desenvolvido para a disciplina de Pesquisa Operacional na Universidade Federal da Paraíba e tem como objetivo resolver diferentes instâncias de Programação Linear Inteira através do algoritmo Branch and Bound.
- **Input**: O programa recebe dois argumentos, o primeiro é o caminho para o arquivo `.txt` com a instância, e o segundo é a estratégia de ramificação da árvore do algoritmo, aceitando "BFS" para busca em largura e "DFS" para busca em profundidade.
    ```bash
    ./solve caminho/para/instancia.txt sigla_para_estratégia
    ```
- **Output**: O programa mostra, durante a execução, o atual valor da função objetivo, a cada 10 iterações. No fim, o programa retorna o valor da solução ótima, assim como o valor de cada variável.

## Instancias
A pasta [`instancias`](instancias) contem as instâncias passadas pelo docente para validação do resolvedor e possuem o seguinte formato:

```bash
3 2 # 3 variáveis e 2 restrições
5 10 8 # coeficientes das variáveis na função objetivo
3 5 2 6 # restrição 1
4 4 4 7 #restrição 2
```

## Relaxação

Para resolver as relaxações, foi utilizada uma iplementação autoral do Simplex Revisado, a qual pode ser vista com mais detalhes no repositório [SIMPLEX](https://github.com/anaceciliabmota/SIMPLEX) desse mesmo perfil.
