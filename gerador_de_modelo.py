import random
from mip import *

def generate_model(n, m, output_file):
    # Cria o modelo MIP
    model = Model(sense=MAXIMIZE, solver_name=CBC)

    # Cria variáveis binárias
    vars = [model.add_var(var_type=BINARY) for i in range(n)]

    # Gera coeficientes aleatórios para a função objetivo
    obj_coeffs = [random.randint(1, 10) for _ in range(n)]
    
    # Define a função objetivo
    model.objective = xsum(obj_coeffs[i] * vars[i] for i in range(n))

    # Gera restrições aleatórias
    constraints = []
    for i in range(m):
        # Coeficientes aleatórios para as variáveis na restrição
        coeffs = [random.randint(1, 10) for _ in range(n)]
        rhs = random.randint(n, 2 * n)  # Lado direito aleatório para a restrição
        constraints.append(coeffs + [rhs])
        model += xsum(coeffs[i] * vars[i] for i in range(n)) <= rhs

    
    model.optimize()

    # Exibe o valor ótimo (se o modelo foi resolvido com sucesso)
    if model.num_solutions:
        print(f"Valor ótimo: {model.objective_value}")
    else:
        print("Nenhuma solução encontrada.")


    # Salva o modelo no arquivo de texto
    with open(output_file, 'w') as f:
        # Escreve o número de variáveis e restrições
        f.write(f"{n} {m}" + "\n")

        # Escreve os coeficientes da função objetivo
        f.write(" ".join(map(str, obj_coeffs)) + "\n")

        # Escreve as restrições
        for c in constraints:
            f.write(" ".join(map(str, c)) + "\n")

# Exemplo de uso
if __name__ == "__main__":
    n = int(input("Digite o número de variáveis: "))
    m = int(input("Digite o número de restrições: "))
    output_file = "modelo_gerado.txt"
    generate_model(n, m, output_file)
    print(f"Modelo gerado e salvo em {output_file}")
