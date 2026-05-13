// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO - NIVEL AVENTUREIRO
// ============================================================================ 
//OBJETIVO: FUNCIONALIDADE: ATAQUE ENTRE TERRITORIOS COM ALOCAÇÃO DINAMICA E PONTEIROS

//BiBliotecas
#include <stdio.h> //Permite usar printf e scanf
#include <stdlib.h> //Permite usar funções de alocação de memória (malloc, calloc, free) e rand()
#include <string.h> //Permite usar função de string como char []
#include <time.h> //Permite usar a função time() para gerar números aleatórios diferentes a cada execução

//Definição de Estrutura (Struct)
struct Territorio {
    char nome [30]; // Nome do territorio
    char cor [10];  // Cor da tropa
    int tropas;     // Qntd. de tropas
};

// Prototipos das Funções
void limparBuffer(); //Limpar os buffers do teclado
void cadastrarTerritorios(struct Territorio* mapa, int quantidade); // cadastrar os territorios
void exibirTerritorios(struct Territorio* mapa, int quantidade); // Exibe todos Territorios cadastrados
void realizarAtaque(struct Territorio* mapa, int quantidade); // Responsavel por inicializar um ataque
void atacar(struct Territorio* atacante, struct Territorio* defensor); // executa a logica de batalha
void liberarMemoria(struct Territorio* mapa); // Libera memoria aloada Dinamicamente

//Função Principal (main)
int main() {
    int quantidade, opcao;
    
    srand(time(NULL)); // Inicializa gerador de numeros aleatorios

    printf("===War Estruturado - Nivel Aventureiro ===\n\n");
    printf("Quantidade de Territorios deseja cadastrar? ");
    scanf("%d", &quantidade);
    limparBuffer();

    //Alocação Dinamica de Memoria
    struct Territorio* mapa = (struct Territorio*) calloc(quantidade, sizeof(struct Territorio));
    
    // Verifica se a memoria foi alocada corretamente
    if (mapa == NULL) {
        printf("Erro na alocação da Memoria! \n");
        return 1;
    }

    //Chama a funç~çao para cadastra Territorios
    cadastrarTerritorios(mapa, quantidade);

    //Menu Principal
    do {
        printf("\n========================================\n");
        printf("                  MENU                  \n");
        printf("========================================\n");
        printf("1 - Exibir todos os territorios\n");
        printf("2 - Realizar Ataque\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        //Verifica a opção escolhida
        switch (opcao) {
        case 1:
            exibirTerritorios(mapa, quantidade);
            break;

        case 2:
            realizarAtaque(mapa, quantidade);
            break;

        case 0:
            printf("\n Encerrando o Jogo...\n");
            break;
        
        default: 
            printf("Opção Invalida!\n");
            break;
        }
    } while(opcao != 0);

    liberarMemoria(mapa); // Libera memoria antes de finalizar o programa

    return 0;
}

    //----- Funções ------
    void limparBuffer() { // Funcao limparBuffer Remove caracteres restantes do teclado
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Continua lendo até encontrar ENTER ou EOF
    }
    
    //FUNÇÃO: cadastrarTerritorios Responsável por cadastrar os territórios
    void cadastrarTerritorios(struct Territorio* mapa, int quantidade) {
        printf("\n--- Cadastro de Territorios ---\n");

        //Percorre todos Territorios
        for(int i = 0; i < quantidade; i++) { 
            printf("\n--- Territorio %d ---\n", i + 1);
            printf("Nome: ");
            scanf("%29s", mapa[i].nome);
            limparBuffer();

            printf("Cor da Tropa: ");
            scanf("%9s", mapa[i].cor);
            limparBuffer();

        // Garante que o território tenha pelo menos 1 tropa
        do {
            printf("Quantidade de Tropas: ");
            scanf("%d", &mapa[i].tropas);
            limparBuffer();
            if (mapa[i].tropas < 1)
            printf("Quantidade deve ser pelo menos 1!\n");
        } while (mapa[i].tropas < 1);
        }
    }

    // FUNÇÃO: exibirTerritorios, Mostra todos os territórios cadastrados
    void exibirTerritorios(struct Territorio* mapa, int quantidade) { 
        printf("\n--- Territorios Cadastrados ---\n");
        for(int i = 0; i < quantidade; i++) {
            printf("%d - %s | Cor: %s | Tropas: %d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);

        }
    }

    // FUNÇÃO: realizarAtaque, Escolhe atacante e defensor
    void realizarAtaque(struct Territorio* mapa, int quantidade) {
        int indiceAtacante, indiceDefensor;

        //Mostra Territorios Disponiveis
        exibirTerritorios(mapa, quantidade);
        
        // Escolhe atacante
        printf("\n Digite o Numero do Territorio Atacante: ");
        scanf("%d", &indiceAtacante);
        limparBuffer();
        if(indiceAtacante == 0) return;

        // Escolhe Defensor
        printf("\n Digite o Numero do Territorio Defensor: ");
        scanf("%d", &indiceDefensor);
        limparBuffer();
        if(indiceDefensor == 0) return;
        
        // Valida escolhas
        if(indiceAtacante < 1 || indiceAtacante > quantidade ||
           indiceDefensor < 1 || indiceDefensor > quantidade ||
           indiceAtacante == indiceDefensor) {
            printf("Escolha invalida!\n");
            return;
        }

        // Ponteiros para os territórios escolhidos
        struct Territorio* atacante = &mapa[indiceAtacante-1];
        struct Territorio* defensor = &mapa[indiceDefensor-1];
        
        // Não permite atacar territórios da mesma cor
        if(strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Não é permitido atacar Territorio da mesma Cor!\n");
            return;
        }
        
        // Inicia batalha
        atacar(atacante, defensor);
    }

    //---- Função de Ataque ----
    // Executa a lógica do combate

    void atacar(struct Territorio* atacante, struct Territorio* defensor) {
        printf("\n--- Ataque Iniciado ---\n");
        printf("%s (%s) -> atacando -> %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);

        // Verifica se ambos possuem tropas
        if (atacante->tropas < 1 || defensor->tropas < 1) {
            printf("um dos Territorios nao tem tropas suficientes!\n");
            return;
        }

        // Gera valores aleatórios de 1 até 6
        int dadoAtacante = rand() % 6 + 1;
        int dadoDefensor = rand() % 6 + 1;

        printf("Dado Atacante: %d | Dado Defensor: %d\n", dadoAtacante, dadoDefensor);

        //Mensagens de quem ganhou a batalha
        if (dadoAtacante > dadoDefensor) {
            defensor->tropas--;
            printf("%s Venceu a Batalha!\n", atacante->nome);
        }else if (dadoDefensor > dadoAtacante) {
            atacante->tropas--;
            printf("%s Perdeu a batalha!\n", atacante->nome);
        }
        else {
            printf("Empate nenhuma tropa foi perdida!\n");
        }

        //--- Verificação de Conquista ---
        if (defensor->tropas <= 0) {
            printf("\n*** %s Foi Conquistado!!! ***\n", defensor->nome);
            strcpy(defensor->cor, atacante->cor); // Muda a cor do território conquistado

            // Move 1 tropa para o território conquistado
            if (atacante->tropas >= 2) {
                defensor->tropas = 1;
                atacante->tropas -= 1;
                printf("O Exercito %s agora Domina %s com 1 Tropa!\n", atacante->cor, defensor->nome);
        } else {
            defensor->tropas = 1;
            printf("%s conquistou, mas nao tem tropas para guarnecer.\n", atacante->nome);
        } 
        }
        else if (atacante->tropas <= 0) {
            printf("\n*** %s Foi Conquistado!!! ***\n", atacante->nome);
            strcpy(atacante->cor, defensor->cor); // Território muda de dono

            // Defensor move tropas
            if (defensor->tropas >= 2) {
                atacante->tropas = 1;
                defensor->tropas -= 1;
                printf("O Exercito %s agora Domina %s com 1 Tropa!\n", defensor->cor, atacante->nome);
            } else {
                atacante->tropas = 0;
            }
        }
    }
    
    // FUNÇÃO: liberarMemoria
    // Libera memória dinâmica utilizada
    void liberarMemoria(struct Territorio* mapa) {
        free(mapa);
        printf("Memoria Liberada com Sucesso!!!\n");
    }