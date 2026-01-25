// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
// int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

//     return 0;
// }

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.


// *********** MEU CÓDIGO INCIIA ABAIXO ***********
// Jogo War - Nivel Aventureiro

/* Colinha para preencher os territorios:
    America - Azul - 5
    Africa - Vermelho - 3
    Oceania - Verde - 4
    Europa - Amarelo - 4
    Asia - Roxo - 5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para armazenar informacoes do territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Assinaturas das funções, serve para organizar o código
void limparBufferEntrada(void); // Limpa o buffer de entrada
void cadastrarTerritorio(struct Territorio *territorio); // Cadastra um territorio
void exibirMapa(struct Territorio *territorios, int totalTerritorios); // Exibe o mapa do mundo
void atacar(struct Territorio *atacante, struct Territorio *defensor); // Realiza um ataque entre territorios

// Função para cadastrar territorio
void cadastrarTerritorio(struct Territorio *territorio) {
    printf("Nome do territorio: ");
    fgets(territorio->nome, 30, stdin);
    territorio->nome[strcspn(territorio->nome, "\n")] = 0; // Remove o '\n'

    printf("Cor do exercito: ");
    fgets(territorio->cor, 10, stdin);
    territorio->cor[strcspn(territorio->cor, "\n")] = 0;

    printf("Numero de tropas: ");
    scanf("%d", &territorio->tropas);
    limparBufferEntrada();
}

// Função para exibir o mapa do mundo
void exibirMapa(struct Territorio *territorios, int totalTerritorios) {
    printf("\n=====================\n");
    printf("--- MAPA DO MUNDO ---\n");
    printf("=====================\n\n");
    for (int i = 0; i < totalTerritorios; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// Função de ataque
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    // Valida se a cor do territorio atacante é diferente do defensor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido! O territorio atacante e defensor pertencem ao mesmo exercito.\n");
        printf("Pressione Enter para continuar...");
        getchar(); // Pausa para o usuário ler a mensagem
        return;
    }
    
    int dadoAtacante = (rand() % 6) + 1; // Gera número aleatório entre 1 e 6
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou o dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou o dado e tirou: %d\n", defensor->nome, dadoDefensor);

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
        if (atacante->tropas < 0) {
            atacante->tropas = 0; // Evita valores negativos
        }
    }

    printf("Pressione Enter para continuar...");
    getchar(); // Pausa para o usuário ler a mensagem
}

// Função para limpar o buffer de entrada
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// *** FUNÇÃO PRINCIPAL ***
int main() {
    struct Territorio *territorios;
    int totalTerritorios = 0; // Contador de territorios cadastrados

    // malloc para o array de territorios
    territorios = (struct Territorio *)malloc(5 * sizeof(struct Territorio));
    
    // Verificação de alocação bem-sucedida
    if (territorios == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1; // Retorna 1 para indicar um erro
    }

    srand((unsigned)time(NULL)); // Inicializa o gerador de numeros aleatorios uma vez, unsigned para conversão explicita

    // Impressão do cabeçalho e instruções do jogo
    printf("=====================================\n");
    printf("\nJOGO WAR - VERSÃO DIGITAL EM C\n");
    printf("\n=====================================\n");
    printf("Regras do jogo e instrucoes:\n");
    printf("1. Cadastre 5 territorios com nome, cor e numero de tropas.\n");
    printf("2. Durante a fase de ataque, escolha um territorio atacante e um defensor.\n");
    printf("   O territorio atacante não pode ter apenas 1 tropa disponivel.\n");
    printf("   O territorio defensor deve ser diferente do atacante.\n");
    printf("   Os territorios só podem ser atacados se pertencerem a exercitos de cores diferentes.\n");
    printf("3. O resultado do ataque sera decidido por uma batalha de dados.\n");
    printf("   Quem tirar o menor numero perde uma tropa.\n");
    printf("   Caso o territorio defensor perca o seu ultimo territorio, ele sera dominado pelo atacante.\n");
    printf("   Quem conquistar um territorio comeca com 1 tropa naquele territorio.\n");
    printf("   Lembrando: Ao conquistar, o atacante move 1 tropa para o territorio conquistado.\n");

    // Loop para cadastrar territorios até o limite de 5 continentes
    do {
        printf("\nCadastro do territorio #%d\n", totalTerritorios + 1);
        cadastrarTerritorio(&territorios[totalTerritorios]);
        totalTerritorios++; // Incrementa o contador de territorios cadastrados ao final de cada iteração
    } while (totalTerritorios < 5);

    // Chamamos a função para exibir o mapa do mundo
    exibirMapa(territorios, totalTerritorios);

    // Fase de ataque
    int opcaoAtacar;
    do {
        printf("\n--- Fase de Ataque ---\n");
        printf("Digite o numero do territorio atacante (1 a 5, 0 para sair): ");
        scanf("%d", &opcaoAtacar);
        limparBufferEntrada();

        if (opcaoAtacar >= 1 && opcaoAtacar <= 5) {
            // Validação para garantir que o território atacante tenha 2 tropas ou mais
            if (territorios[opcaoAtacar - 1].tropas < 2) {
                printf("O territorio %s nao tem tropas suficientes para atacar!\n", territorios[opcaoAtacar - 1].nome);
                printf("Escolha outro territorio atacante.\n");
                printf("Pressione Enter para continuar...");
                getchar();
                continue; // Volta ao início do loop
            }
            int opcaoDefender;
            printf("Digite o numero do territorio defensor (1 a 5): ");
            scanf("%d", &opcaoDefender);
            limparBufferEntrada();

            if (opcaoDefender >= 1 && opcaoDefender <= 5 && opcaoDefender != opcaoAtacar) {
                // Chama a função de ataque
                atacar(&territorios[opcaoAtacar - 1], &territorios[opcaoDefender - 1]);
                // Chama a função para exibir o mapa atualizado após o ataque
                exibirMapa(territorios, totalTerritorios);
            } else {
                printf("Opcao de defensor invalida!\n");
                printf("Pressione Enter para continuar...");
                getchar();
            }
        } else if (opcaoAtacar != 0) {
            printf("Opcao de ataque invalida!\n");
            printf("Pressione Enter para continuar...");
            getchar();
        }
    } while (opcaoAtacar != 0);

    // Libera a memoria alocada
    free(territorios);

    printf("Saindo do jogo...\n");

    return 0;
}