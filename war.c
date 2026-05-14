// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO - NIVEL AVENTUREIRO
//         SISTEMA DE MISSÕES ESTRATÉGICAS
// ============================================================================
// NOVIDADES NESTA VERSÃO:
// - Vetor de missões pré-definidas sorteadas automaticamente por cor
// - Missão armazenada dinamicamente com malloc para cada cor
// - atribuirMissao(): sorteia e copia a missão para o jogador
// - verificarMissao(): checa se a missão foi cumprida (conquista OU eliminação)
// - exibirMissao(): exibe a missão do jogador (passagem por valor)
// - Verificação automática ao final de cada ataque
// ============================================================================

//BiBliotecas
#include <stdio.h> //Permite usar printf e scanf
#include <stdlib.h> //Permite usar funções de alocação de memória (malloc, calloc, free) e rand()
#include <string.h> //Permite usar função de string como char []
#include <time.h> //Permite usar a função time() para gerar números aleatórios diferentes a cada execução

//--- Constantes ---
#define MAX_CORES 10  // Maximo de cores ou jogadores distintos no mapa
#define TAM_MISSAO 100 // Tamanho maximo da string missao
#define TOTAL_MISSOES 6 // QNTD. de missoes disponiveis no sorteio

//Definição de Estrutura (Struct)
struct Territorio {
    char nome [30]; // Nome do territorio
    char cor [10];  // Cor da tropa
    int tropas;     // Qntd. de tropas
};

//--- Estrutura do Jogador (baseada em cor) ---
// Cada cor unica no mapa é tratada como jogador
// A missao é alocada dinamicamente com malloc.
struct Jogador {
    char cor[10]; // Identificador do jogador
    char* missao; // Ponteiro para a missao alocada dinamicamente.
};

// Prototipos das Funções
void limparBuffer(); //Limpar os buffers do teclado
void cadastrarTerritorios(struct Territorio* mapa, int quantidade); // cadastrar os territorios
void exibirTerritorios(struct Territorio* mapa, int quantidade); // Exibe todos Territorios cadastrados
void realizarAtaque(struct Territorio* mapa, int quantidade, struct Jogador* jogadores, int totalJogadores); // Responsavel por inicializar um ataque
void atacar(struct Territorio* atacante, struct Territorio* defensor); // executa a logica de batalha

// --- Funções do Sistema de Missões ---
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);       // Sorteia e copia a missão
void exibirMissao(char* missao);                                              // Exibe (passagem por valor)
int  verificarMissao(char* missao, struct Territorio* mapa, int tamanho,     // Verifica se missão foi cumprida
                     char* corJogador);

// --- Funções Auxiliares ---
int  detectarCores(struct Territorio* mapa, int quantidade,                  // Detecta cores únicas no mapa
                   struct Jogador* jogadores, char* missoes[], int totalMissoes);
int  encontrarJogador(struct Jogador* jogadores, int totalJogadores,         // Acha jogador pela cor
                      char* cor);
void verificarVitoria(struct Territorio* mapa, int quantidade,               // Checa vitória após cada ataque
                      struct Jogador* jogadores, int totalJogadores);
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores,      // Libera toda memória dinâmica
                    int totalJogadores);

//Função Principal (main)
int main() {
    int quantidade, opcao;
    
    srand(time(NULL)); // Inicializa gerador de numeros aleatorios

    // --- Vetor de missões pré-definidas ---
    // Cada string é uma missão possível sorteada para um jogador.
    char* missoes[TOTAL_MISSOES] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Dominar pelo menos metade dos territorios do mapa",
        "Conquistar 2 territorios inimigos consecutivos",
        "Eliminar todas as tropas da cor azul",
        "Ser o unico jogador com tropas em mais de 4 territorios"
    };

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

    // Cadastra os territórios
    cadastrarTerritorios(mapa, quantidade);

    // --- Detecta cores e atribui missões ---
    // O array de jogadores guarda até MAX_CORES jogadores distintos
    struct Jogador jogadores[MAX_CORES];
    int totalJogadores = detectarCores(mapa, quantidade, jogadores, missoes, TOTAL_MISSOES);

    // Exibe as missões atribuídas a cada jogador
    printf("\n========= MISSOES DO TURNO =========\n");
    for (int i = 0; i < totalJogadores; i++) {
        printf("Jogador [%s]: ", jogadores[i].cor);
        exibirMissao(jogadores[i].missao); // passagem por valor (char*)
    }
    printf("====================================\n");

    //Menu Principal
    do {
        printf("\n========================================\n");
        printf("                  MENU                  \n");
        printf("========================================\n");
        printf("1 - Exibir todos os territorios\n");
        printf("2 - Realizar Ataque\n");
        printf("3 - Verificar minha Missao\n");
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
            realizarAtaque(mapa, quantidade, jogadores, totalJogadores);
            break;

        case 3: {
            // Exibe a missao de uma cor especifica consultada pelo usuario
            char corConsulta[10];
            printf("Digite sua cor para ver a missao: ");
            scanf("%9s", corConsulta);
            limparBuffer();
            int idx = encontrarJogador(jogadores, totalJogadores, corConsulta);
            if(idx >= 0) {
                printf("Missao de [%s]: ", corConsulta);
                exibirMissao(jogadores[idx].missao);
            }else {
                printf("Cor não Encontrada!\n");
            }
            break;
        }

        case 0:
            printf("\n Encerrando o Jogo...\n");
            break;
        
        default: 
            printf("Opção Invalida!\n");
            break;
        }
    } while(opcao != 0);

    liberarMemoria(mapa, jogadores, totalJogadores); // Libera memoria antes de finalizar o programa

    return 0;
}

    //----- Implementação das Funções ------
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
            printf("%d - %-20s | Cor: %-10s | Tropas: %d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);

        }
    }

//=========================================================================================================
//         SISTEMA DE MISSOES
//========================================================================================================

/**
 * Atribuir Missoes
 * * Sorteia uma missão aleatória do vetor e copia para 'destino' usando strcpy.
 * @param destino   Ponteiro para a string onde a missão será armazenada (referência)
 * @param missoes   Vetor de strings com as missões disponíveis
 * @param totalMissoes  Quantidade de missões no vetor
 */

    void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
        int sorteio = rand() % totalMissoes; // Sorteia indice aleatorio
        strcpy(destino, missoes[sorteio]); // Copia a missao sorteada para o destino
    }

/**
 * exibirMissao
 * Apenas exibe a missão — passagem por valor (o ponteiro é copiado, não modifica original).
 * @param missao  Ponteiro (valor) para a string da missão
 */

    void exibirMissao(char* missao) {
        printf("%s\n", missao);
    }

/**
 * verificarMissao
 * Verifica se o jogador (identificado por corJogador) cumpriu sua missão.
 * Lógica implementada:
 *   - "Conquistar 3 territorios": conta territórios da cor do jogador >= 3
 *   - "Dominar pelo menos metade": conta territórios da cor >= metade do mapa
 *   - "Conquistar 2 territorios consecutivos": verifica 2 seguidos no array
 *   - "Eliminar todas as tropas da cor X": verifica se cor X sumiu do mapa
 * @param missao      String da missão (passagem por referência para leitura)
 * @param mapa        Array de territórios
 * @param tamanho     Total de territórios
 * @param corJogador  Cor do jogador sendo verificado
 * @return 1 se missão cumprida, 0 caso contrário
 */

    int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, char* corJogador) {
        
        // --- Missão: Conquistar 3 territórios seguidos ---
    if (strstr(missao, "3 territorios seguidos") != NULL) {
        int consecutivos = 0, maxConsecutivos = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                consecutivos++;
                if (consecutivos > maxConsecutivos) maxConsecutivos = consecutivos;
            } else {
                consecutivos = 0;
            }
        }
        return (maxConsecutivos >= 3) ? 1 : 0;
    }

    // --- Missão: Dominar pelo menos metade dos territórios ---
    if (strstr(missao, "metade dos territorios") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) count++;
        }
        return (count >= tamanho / 2) ? 1 : 0;
    }

    // --- Missão: Conquistar 2 territórios consecutivos ---
    if (strstr(missao, "2 territorios inimigos consecutivos") != NULL) {
        int consecutivos = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                consecutivos++;
                if (consecutivos >= 2) return 1;
            } else {
                consecutivos = 0;
            }
        }
        return 0;
    }

    // --- Missão: Ser o único com tropas em mais de 4 territórios ---
    if (strstr(missao, "mais de 4 territorios") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) count++;
        }
        return (count > 4) ? 1 : 0;
    }

    // --- Missão: Eliminar todas as tropas de uma cor específica ---
    // Detecta qual cor deve ser eliminada a partir do texto da missão
    char* cores[] = {"vermelha", "azul", "verde", "amarela", "branca"};
    char* mapaCorNome[] = {"vermelho", "azul", "verde", "amarelo", "branco"};
    int numCores = 5;

    for (int c = 0; c < numCores; c++) {
        if (strstr(missao, cores[c]) != NULL) {
            // Verifica se ainda existe algum território dessa cor no mapa
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, mapaCorNome[c]) == 0) {
                    return 0; // Ainda existe — missão não cumprida
                }
            }
            return 1; // Cor sumiu do mapa — missão cumprida!
        }
    }

    return 0; // Missão não reconhecida ou não cumprida
}

// ============================================================================
//                     FUNÇÕES AUXILIARES DE JOGADORES
// ============================================================================

/**
 * detectarCores
 * Varre o mapa, identifica cores únicas e atribui uma missão a cada uma.
 * A missão é alocada dinamicamente com malloc.
 * @return Número de jogadores (cores únicas) encontrados
 */
int detectarCores(struct Territorio* mapa, int quantidade,
                  struct Jogador* jogadores, char* missoes[], int totalMissoes) {
    int totalJogadores = 0;

    for (int i = 0; i < quantidade; i++) {
        int jaExiste = 0;

        // Verifica se a cor deste território já foi registrada
        for (int j = 0; j < totalJogadores; j++) {
            if (strcmp(jogadores[j].cor, mapa[i].cor) == 0) {
                jaExiste = 1;
                break;
            }
        }

        // Nova cor encontrada — registra o jogador e atribui missão
        if (!jaExiste && totalJogadores < MAX_CORES) {
            strcpy(jogadores[totalJogadores].cor, mapa[i].cor);

            // Aloca dinamicamente espaço para a missão deste jogador
            jogadores[totalJogadores].missao = (char*) malloc(TAM_MISSAO * sizeof(char));
            if (jogadores[totalJogadores].missao == NULL) {
                printf("Erro: falha ao alocar memoria para missao!\n");
                exit(1);
            }

            // Sorteia e atribui a missão
            atribuirMissao(jogadores[totalJogadores].missao, missoes, totalMissoes);

            totalJogadores++;
        }
    }

    return totalJogadores;
}

/**
 * encontrarJogador
 * Busca o índice de um jogador pela cor.
 * @return Índice no array ou -1 se não encontrado
 */
int encontrarJogador(struct Jogador* jogadores, int totalJogadores, char* cor) {
    for (int i = 0; i < totalJogadores; i++) {
        if (strcmp(jogadores[i].cor, cor) == 0) return i;
    }
    return -1;
}

/**
 * verificarVitoria
 * Chamada após cada ataque. Verifica silenciosamente se algum jogador
 * cumpriu sua missão e declara o vencedor se sim.
 */
void verificarVitoria(struct Territorio* mapa, int quantidade,
                      struct Jogador* jogadores, int totalJogadores) {
    for (int i = 0; i < totalJogadores; i++) {
        if (verificarMissao(jogadores[i].missao, mapa, quantidade, jogadores[i].cor)) {
            printf("\n*******************************************\n");
            printf("  VITORIA! Jogador [%s] cumpriu a missao!\n", jogadores[i].cor);
            printf("  Missao: %s\n", jogadores[i].missao);
            printf("*******************************************\n");
        }
    }
}

// ============================================================================
//                         SISTEMA DE ATAQUE
// ============================================================================

/**
 * realizarAtaque
 * Gerencia seleção de atacante e defensor, valida entradas e chama atacar().
 * Ao final, chama verificarVitoria() para checar se alguém venceu.
 */
void realizarAtaque(struct Territorio* mapa, int quantidade,
                    struct Jogador* jogadores, int totalJogadores) {
    int indiceAtacante, indiceDefensor;

    exibirTerritorios(mapa, quantidade);

    printf("\nDigite o numero do territorio Atacante (0 para cancelar): ");
    scanf("%d", &indiceAtacante);
    limparBuffer();
    if (indiceAtacante == 0) return;

    printf("Digite o numero do territorio Defensor (0 para cancelar): ");
    scanf("%d", &indiceDefensor);
    limparBuffer();
    if (indiceDefensor == 0) return;

    // Valida índices e evita ataque a si mesmo
    if (indiceAtacante < 1 || indiceAtacante > quantidade ||
        indiceDefensor < 1 || indiceDefensor > quantidade ||
        indiceAtacante == indiceDefensor) {
        printf("Escolha invalida!\n");
        return;
    }

    struct Territorio* atacante = &mapa[indiceAtacante - 1];
    struct Territorio* defensor = &mapa[indiceDefensor - 1];

    // Não permite atacar território da mesma cor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Nao e permitido atacar territorio da mesma cor!\n");
        return;
    }

    // Executa o combate
    atacar(atacante, defensor);

    // Verifica missões após o ataque
    verificarVitoria(mapa, quantidade, jogadores, totalJogadores);
}

/**
 * atacar
 * Simula o combate com dados (1-6).
 * Atacante vence: defensor perde 1 tropa; se zerou, território é conquistado.
 * Defensor vence: atacante perde 1 tropa; se zerou, território muda de dono.
 * Empate: nenhuma tropa é perdida.
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    printf("\n--- Ataque Iniciado ---\n");
    printf("%s (%s) -> atacando -> %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    if (atacante->tropas < 1 || defensor->tropas < 1) {
        printf("Um dos territorios nao tem tropas suficientes!\n");
        return;
    }

    // Rolagem de dados
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    printf("Dado Atacante: %d | Dado Defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        defensor->tropas--;
        printf("%s venceu a batalha!\n", atacante->nome);
    } else if (dadoDefensor > dadoAtacante) {
        atacante->tropas--;
        printf("%s perdeu a batalha!\n", atacante->nome);
    } else {
        printf("Empate — nenhuma tropa foi perdida!\n");
    }

    // --- Verificação de Conquista: defensor zerou ---
    if (defensor->tropas <= 0) {
        printf("\n*** %s foi conquistado! ***\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor); // Muda cor do território

        // Move 1 tropa do atacante para o território conquistado
        if (atacante->tropas >= 2) {
            defensor->tropas = 1;
            atacante->tropas -= 1;
            printf("O exercito %s domina %s com 1 tropa!\n", atacante->cor, defensor->nome);
        } else {
            defensor->tropas = 1;
            printf("%s conquistou, mas nao tem tropas extras para guarnecer.\n", atacante->nome);
        }
    }
    // --- Verificação de Conquista: atacante zerou ---
    else if (atacante->tropas <= 0) {
        printf("\n*** %s foi destruido! ***\n", atacante->nome);
        strcpy(atacante->cor, defensor->cor); // Território muda de dono

        if (defensor->tropas >= 2) {
            atacante->tropas = 1;
            defensor->tropas -= 1;
            printf("O exercito %s agora domina %s com 1 tropa!\n", defensor->cor, atacante->nome);
        } else {
            atacante->tropas = 0;
        }
    }
}

// ============================================================================
//                         LIBERAÇÃO DE MEMÓRIA
// ============================================================================

/**
 * liberarMemoria
 * Libera o mapa de territórios e as missões alocadas com malloc para cada jogador.
 */
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int totalJogadores) {
    free(mapa); // Libera o array de territórios

    // Libera a missão de cada jogador individualmente
    for (int i = 0; i < totalJogadores; i++) {
        free(jogadores[i].missao);
    }

    printf("\nMemoria liberada com sucesso!\n");
}