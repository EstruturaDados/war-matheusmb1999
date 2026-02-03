// Jogo War - Nivel Mestre

/* Para preencher os territorios:
    America - Azul - 3 **  Africa - Vermelho - 3 ** Oceania - Verde - 3
    Europa - Amarelo - 3 ** Asia - Roxo - 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Estrutura para armazenar informacoes do territorio ---
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- As missões do jogo ---
const char* missoes[] = {
    "Conquistar todos os territorios com um exercito",
    "Eliminar um exercito adversario completamente",
    "Conquistar 2 territorios no total",
    "Conquistar 3 territorios no total",
    "Conquistar 4 territorios no total"
};
// -- Faz a contagem total de missoes ---
const int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

// --- Assinaturas das funções, serve para organizar o código ---
void limparBufferEntrada(void); // Limpa o buffer de entrada
void imprimirCabecalho(void); // Imprime o cabeçalho e instruções do jogo
void cadastroUnitario(struct Territorio *territorio); // Cadastra UM territorio
void cadastroTotal(struct Territorio *territorios, int *totalTerritorios); // Cadastra todos os territorios
void exibirMapa(const struct Territorio *territorios, int totalTerritorios); // Exibe o mapa do mundo
void batalha(struct Territorio *atacante, struct Territorio *defensor); // Parte de ataque em que se jogam os dados
void faseAtaque(struct Territorio *territorios, int totalTerritorios, char *missaoJogador); // O ataque por completo
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho); // Verifica se a missão foi cumprida
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes); // Atribui uma missão ao jogador
void menuAcoes(struct Territorio *territorios, int totalTerritorios, char *missaoJogador); // Menu de ações do jogador

// --- *** FUNÇÃO PRINCIPAL *** ---
int main() {
    struct Territorio *territorios; // Ponteiro para o array de territorios
    int totalTerritorios = 0; // Contador de territorios cadastrados
    char *missaoJogador; // Armazena a missão do jogador

    // --- malloc para o array de territorios ---
    territorios = (struct Territorio *)malloc(5 * sizeof(struct Territorio));

    // --- malloc para a missão do jogador ---
    missaoJogador = (char *)malloc(100 * sizeof(char));

    // --- Verificação de alocação bem-sucedida ---
    if (territorios == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1; // Retorna 1 para indicar um erro
    }

    if (missaoJogador == NULL) {
        printf("Erro na alocacao de memoria para a missao!\n");
        free(territorios);
        return 1;
    }
    // --- Inicializa o gerador de numeros aleatorios, unsigned para conversão explicita ---
    srand((unsigned)time(NULL));

    // --- Impressão do cabeçalho e instruções do jogo ---
    imprimirCabecalho();

    // --- O cadastro dos territorios ---
    cadastroTotal(territorios, &totalTerritorios); // struct Territorio *territorios, int *totalTerritorios

    // --- Exibe o mapa do mundo ---
    exibirMapa(territorios, totalTerritorios); // const struct Territorio *territorios, int totalTerritorios

    // --- Sorteia e atribui uma missão ao jogador ---
    atribuirMissao(missaoJogador, missoes, totalMissoes); // char* destino, const char* missoes[], int totalMissoes
    printf("\nSUA MISSAO É: %s\n", missaoJogador);
    printf("Pressione Enter para continuar...");
    getchar(); // Aguarda o usuário pressionar Enter

    // --- Menu de ações que o usuário pode realizar ---
    menuAcoes(territorios, totalTerritorios, missaoJogador); // struct Territorio *territorios, int totalTerritorios, char *missaoJogador

    // --- Libera a memoria alocada ---
    free(territorios);
    free(missaoJogador);

    return 0;
}


// --- Função para limpar o buffer de entrada após cada entrada de dados no teclado ---
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// --- Função para impressão do cabeçalho e instruções do jogo ---
void imprimirCabecalho(void) {
    printf("====================================\n");
    printf("      JOGO WAR - NIVEL MESTRE  \n");
    printf("====================================\n");
    printf("\nInstrucoes:\n");
    printf("1. Cadastre 5 territorios, cada um representando um continente.\n");
    printf("2. Cada territorio deve ter um nome, cor do exercito e numero de tropas.\n");
    printf("3. O jogo irá sortear uma missão para você cumprir.\n");
    printf("4. A cada rodada você poderá escolher Atacar, Verificar missão ou Sair.\n");
    printf("5. Em Atacar, escolha um territorio atacante e um defensor.\n");
    printf("6. O atacante deve ter pelo menos 2 tropas para iniciar o ataque.\n");
    printf("7. Você não pode atacar territorios do mesmo exercito.\n");
    printf("8. O jogo termina quando você cumprir sua missão ou decidir sair.\n");
    printf("\nBoa sorte !!!\n");
    printf("========================================================================\n");
}


// --- Função para cadastrar UM territorio ---
// Recebe um ponteiro para a estrutura Territorio
// -> Acessa os membros da estrutura através do ponteiro
// stdin é usado para ler strings com espaços
// strcspn é usado para remover o '\n' lido pelo fgets
void cadastroUnitario(struct Territorio *territorio) {
    printf("Nome do territorio: ");
    fgets(territorio->nome, 30, stdin);
    territorio->nome[strcspn(territorio->nome, "\n")] = 0;

    printf("Cor do exercito: ");
    fgets(territorio->cor, 10, stdin);
    territorio->cor[strcspn(territorio->cor, "\n")] = 0;

    printf("Numero de tropas: ");
    scanf("%d", &territorio->tropas);
    limparBufferEntrada();
}


// --- Função que chama a função de cadastro unitario e completa o cadastro total ---
// Recebe um ponteiro para o array de territorios e um ponteiro para o contador de territorios cadastrados
// Usa um loop do-while para garantir que exatamente 5 territorios sejam cadastrados
void cadastroTotal(struct Territorio *territorios, int *totalTerritorios) {
    do {
        printf("\nCadastro do territorio #%d\n", *totalTerritorios + 1);
        cadastroUnitario(&territorios[*totalTerritorios]);
        (*totalTerritorios)++; // Incrementa o contador de territorios cadastrados ao final de cada iteração
    } while (*totalTerritorios < 5);
}


// --- Função para exibir o mapa do mundo ---
// Recebe um ponteiro para o array de territorios e o total de territorios cadastrados
// Usa um loop for para iterar sobre cada territorio e imprimir suas informações
void exibirMapa(const struct Territorio *territorios, int totalTerritorios) {
    printf("\n=====================\n");
    printf("--- MAPA DO MUNDO ---\n");
    printf("=====================\n\n");
    for (int i = 0; i < totalTerritorios; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}


// --- Função de batalha: Executa a batalha entre os 2 territórios ---
// Recebe ponteiros para os territórios atacante e defensor
void batalha(struct Territorio *atacante, struct Territorio *defensor) {

    // Valida se a cor do territorio atacante é diferente do defensor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido! O territorio atacante e defensor pertencem ao mesmo exercito.\n");
        printf("Pressione Enter para continuar...");
        getchar(); // Pausa para o usuário ler a mensagem
        return; // Faz com que saia da função sem prosseguir com o restante
    }
    
    // Gera números aleatórios para simular a rolagem de dados
    int dadoAtacante = (rand() % 6) + 1; // Gera número aleatório entre 1 e 6
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Valor dos Dados:\n");
    printf("%s [%d] x [%d] %s\n", atacante->cor, dadoAtacante, dadoDefensor, defensor->cor);

    // Se o atacante vencer, as tropas do defensor são reduzidas em 1
    // Se o defensor vencer, as tropas do atacante são reduzidas em 1
    if (dadoAtacante > dadoDefensor) {
        defensor->tropas--;
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa!\n");
        if (defensor->tropas <= 0) {
            printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor); // Copia a cor do atacante para o território
            defensor->tropas = 1; // O território dominado começa com 1 tropa
            if (atacante->tropas > 1) {
                atacante->tropas--; // O atacante perde uma tropa ao conquistar
            }
        }
    } else {
        atacante->tropas--;
        printf("DERROTA DO ATAQUE! O atacante perdeu 1 tropa!\n");
    }

    printf("Pressione Enter para continuar...");
    getchar(); // Pausa para o usuário ler a mensagem
}


// --- Função da 'Fase de ATAQUE': Gerencia o processo de ataque por completo ---
// Recebe um ponteiro para o array de territorios, o total de territorios cadastrados e a missão do jogador
// Permite ao jogador escolher o território atacante e defensor, depois chama a função de batalha
void faseAtaque(struct Territorio *territorios, int totalTerritorios, char *missaoJogador) {

    int escolhaAtacante, escolhaDefensor;

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d): ", totalTerritorios);
        scanf("%d", &escolhaAtacante);
        limparBufferEntrada();

        // Validação da escolha do atacante
        if (escolhaAtacante < 1 || escolhaAtacante > totalTerritorios) {
            printf("Escolha invalida! Tente novamente.\n");
            printf("Pressione Enter para continuar...");
            getchar();
            exibirMapa(territorios, totalTerritorios); // Exibe o mapa atualizado
            return;
        }

        // Ponteiro para o território atacante selecionado
        struct Territorio *atacante = &territorios[escolhaAtacante - 1];

        // --- Verifica se o território atacante tem 2 tropas ou mais ---
            if (territorios[escolhaAtacante - 1].tropas < 2) {
                printf("O territorio %s nao tem tropas suficientes para atacar!\n", territorios[escolhaAtacante - 1].nome);
                printf("Escolha outro territorio atacante.\n");
                printf("Pressione Enter para continuar...");
                getchar();
                exibirMapa(territorios, totalTerritorios); // Exibe o mapa atualizado
                return;
            }

        printf("Escolha o territorio defensor (1 a %d): ", totalTerritorios);
        scanf("%d", &escolhaDefensor);
        limparBufferEntrada();

        // Validação da escolha do defensor
        if (escolhaDefensor < 1 || escolhaDefensor > totalTerritorios || escolhaDefensor == escolhaAtacante) {
            printf("Escolha invalida! Tente novamente.\n");
            printf("Pressione Enter para continuar...");
            getchar();
            exibirMapa(territorios, totalTerritorios); // Exibe o mapa atualizado
            return;
        }

        // Ponteiro para o território defensor selecionado
        struct Territorio *defensor = &territorios[escolhaDefensor - 1];

        // Chamada da função de batalha
        batalha(atacante, defensor);
        exibirMapa(territorios, totalTerritorios); // Exibe o mapa atualizado após a batalha

        // Verifica se a missão foi cumprida após o ataque
        if (verificarMissao(missaoJogador, territorios, totalTerritorios)) {
            printf("\n!!! Voce CUMPRIU A MISSAO e VENCEU o jogo !!!\n");
        }

}


// --- Função para atribuir uma missão ao jogador ---
// Recebe um ponteiro para o destino onde a missão será armazenada, o array de missões e o total de missões disponíveis
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes; // Sorteia um índice
    strcpy(destino, missoes[indiceSorteado]); // Copia a missão sorteada para o destino
}

// --- Função para contar cores únicas ---
// Recebe o mapa de territórios e o tamanho do mapa
// Retorna o número de cores únicas presentes no mapa
int contarCores(struct Territorio* mapa, int tamanho) {
    char coresTotal[10][10];
    int coresCont = 0;

    // i percorre todos os territórios
    // j percorre as cores já encontradas
    // se nao encontrada, adiciona a nova cor ao array e incrementa o contador
    // se encontrada, continua para o próximo território
    for (int i = 0; i < tamanho; i++) {
        int encontrada = 0;
        for (int j = 0; j < coresCont; j++) {
            if (strcmp(mapa[i].cor, coresTotal[j]) == 0) {
                encontrada = 1;
                break;
            }
        }
        if (!encontrada) {
            strcpy(coresTotal[coresCont], mapa[i].cor);
            coresCont++;
        }
    }
    return coresCont;
}

// --- Função para verificar se as missoes foram cumpridas ---
// Recebe a missão do jogador, o mapa de territórios e o tamanho do mapa
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho) {

    // --- Verifica cada tipo de missão ---
    // strcmp é usado para comparar strings, ele verifica qual é a missão do jogador

    // O loop for percorre todos os territórios para verificar se todos pertencem ao mesmo exército
    // strcmp compara as cores dos territórios
    // i percorre todos os territórios
    // j percorre as cores já encontradas
    // se nao encontrada, adiciona a nova cor ao array e incrementa o contador
    // se encontrada, continua para o próximo território
    if (strcmp(missao, "Conquistar todos os territorios com um exercito") == 0) {
        char* corReferencia = mapa[0].cor;
        for (int i = 1; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corReferencia) != 0) {
                return 0; // Missão não cumprida
            }
        }
        return 1; // Missão cumprida
    }
    
    // Conta quantas cores diferentes existem no mapa
    // Se há menos de 5 cores, um exército foi eliminado
    if (strcmp(missao, "Eliminar um exercito adversario completamente") == 0) {
        char coresPresentes[10][10];
        int coresCount = 0;

        for (int i = 0; i < tamanho; i++) {
            int encontrada = 0;
            for (int j = 0; j < coresCount; j++) {
                if (strcmp(mapa[i].cor, coresPresentes[j]) == 0) {
                    encontrada = 1;
                    break;
                }
            }
            if (!encontrada) {
                strcpy(coresPresentes[coresCount], mapa[i].cor);
                coresCount++;
            }
        }
        if (coresCount < 5) {  // Se há menos de 5 cores, um exército foi eliminado
            return 1; // Missão cumprida
        }
        return 0; // Missão não cumprida
    }

    if (strcmp(missao, "Conquistar 2 territorios no total") == 0) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[0].cor) == 0) {
                contador++;
            }
        }
        if (contador >= 2) {
            return 1; // Missão cumprida
        }
        return 0; // Missão não cumprida
    }

    if (strcmp(missao, "Conquistar 3 territorios no total") == 0) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[0].cor) == 0) {
                contador++;
            }
        }
        if (contador >= 3) {
            return 1; // Missão cumprida
        }
        return 0; // Missão não cumprida
    }
    
    if (strcmp(missao, "Conquistar 4 territorios no total") == 0) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[0].cor) == 0) {
                contador++;
            }
        }
        if (contador >= 4) {
            return 1; // Missão cumprida
        }
        return 0; // Missão não cumprida
    }
    return 0; // Missão não cumprida por padrão
}

// --- Menu de ações do jogador ---
// Recebe um ponteiro para o array de territorios, o total de territorios cadastrados e a missão do jogador
void menuAcoes(struct Territorio *territorios, int totalTerritorios, char *missaoJogador) {
    int opcao;
    do {
        printf("\nEscolha uma opcao:\n");
        printf("1. Atacar!\n");
        printf("2. Verificar missao\n");
        printf("0. Sair\n");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: // Chama a fase de ataque
                faseAtaque(territorios, totalTerritorios, missaoJogador);
                if (verificarMissao(missaoJogador, territorios, totalTerritorios)) {
                    opcao = 0; // Sai do menu se a missão for cumprida
                }
                break;

            case 2: // Informa a missão do jogador
                printf("\nSua missão é: %s. Continue jogando!\n", missaoJogador);
                break;
            
            case 0: // Sai do jogo
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}