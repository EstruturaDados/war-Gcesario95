// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO - NIVEL NOVATO
// ============================================================================ 
//OBJETIVO: NO NIVEL NOVATO

//BiBliotecas
#include <stdio.h> //Permiti usar printf e scanf
#include <string.h> //Permiti usar função de string como char []

//Definição de Estrutura (Struct)
struct Territorio {
    char nome [30]; // Nome do territorio
    char cor [10];  // Cor da tropa
    int tropas;     // Qntd. de tropas
};
// Função para limpar o buffer após cada scanf
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//Função Principal (main)
int main() {
    struct Territorio territorios[5]; // Aqui criamos um vetor (array) capaz de guardar 5 "fichas" do tipo Territorio.
    int i; // Variavel auxiliar para controlar o laço

// ENTRADA DE DADOS — LAÇO FOR
// O laço percorre de i=0 até i=4 (5 vezes),
// preenchendo cada ficha do vetor.
// territorios[i] acessa a ficha na posição i.
   for (i = 0; i < 5; i++) {
      printf("\n--- Cadastro do Territorio %d ---\n", i + 1);

      // Lê o nome do território
      printf("Nome do Territorio: \n");
      scanf("%s", territorios[i].nome); // O %s lê uma palavra (para para no espaço, territorios[i].nome acessa o campo "nome" da ficha i
      limparBuffer();

      // Lê o a cor do exercito
      printf("Cor do Exercito: \n");
      scanf("%s", territorios[i].cor); // O %s lê uma palavra (para para no espaço, territorios[i].nome acessa o campo "cor" da ficha i
      limparBuffer();

      // Lê a quantidade de tropas
      printf("Quantidade de Tropas: \n");
      scanf("%d", &territorios[i].tropas); // %d lê um número inteiro
      limparBuffer();
}
// Exibição dos Dados
// Após o cadastro, percorremos o vetor novamente e exibimos os dados de cada território.
    printf("\n========================================\n");
    printf("    Territorios Cadastrados    \n");
    printf("========================================\n");

    for (i = 0; i < 5; i++) {

        printf("\nTerritorio %d: \n", i + 1);
        printf(" Nome: %s \n", territorios[i].nome);
        printf(" Cor: %s \n", territorios[i].cor);
        printf(" Quantidade de Tropas: %d \n", territorios[i].tropas);
        printf("----------------------------------------\n");
    }
    return 0;
}

