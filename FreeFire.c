#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10

// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade;
} Item;

// Define os critérios possíveis (nome, tipo ou prioridade).
typedef enum {
    NOME = 1,
    TIPO,
    PRIORIDADE
} CriterioOrdenacao;

// Vetor mochila e variáveis globais
Item mochila[MAX_ITENS];
int numItens = 0;
int comparacoes = 0;
bool ordenadaPorNome = false;

// Função para limpar a tela
void limparTela() {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// Exibe menu principal
void exibirMenu() {
    printf("\n=== CODIGO DA ILHA – NIVEL MESTRE ===\n");
    printf("1 - Adicionar item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("4 - Ordenar itens\n");
    printf("5 - Buscar item por nome (busca binaria)\n");
    printf("0 - Sair\n");
    if (ordenadaPorNome)
        printf(">> Status: Mochila ordenada por NOME \n");
    else
        printf(">> Status: Mochila NAO ordenada por nome \n");
}

// Insere novo item na mochila
void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    getchar(); // limpa buffer
    printf("Nome do item: ");
    fgets(novo.nome, 50, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo do item: ");
    fgets(novo.tipo, 30, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    printf("Prioridade (1 a 5): ");
    scanf("%d", &novo.prioridade);

    mochila[numItens] = novo;
    numItens++;
    ordenadaPorNome = false; // sempre que insere, perde a ordenação
    printf("Item adicionado com sucesso!\n");
}

// Remove item pelo nome
void removerItem() {
    if (numItens == 0) {
        printf("A mochila está vazia!\n");
        return;
    }

    char nome[50];
    getchar();
    printf("Digite o nome do item a remover: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        for (int j = encontrado; j < numItens - 1; j++) {
            mochila[j] = mochila[j + 1];
        }
        numItens--;
        printf("Item removido com sucesso!\n");
    } else {
        printf("Item não encontrado.\n");
    }
}

// Lista todos os itens
void listarItens() {
    if (numItens == 0) {
        printf("A mochila está vazia!\n");
        return;
    }

    printf("\n=== ITENS NA MOCHILA ===\n");
    printf("%-20s %-15s %-10s %-10s\n", "Nome", "Tipo", "Qtd", "Prioridade");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < numItens; i++) {
        printf("%-20s %-15s %-10d %-10d\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
}

// Função auxiliar de comparação
bool comparar(Item a, Item b, CriterioOrdenacao criterio) {
    comparacoes++;
    if (criterio == NOME)
        return strcmp(a.nome, b.nome) < 0;
    else if (criterio == TIPO)
        return strcmp(a.tipo, b.tipo) < 0;
    else
        return a.prioridade > b.prioridade; // prioridade maior vem primeiro
}

// Ordenação por Insertion Sort
void insertionSort(CriterioOrdenacao criterio) {
    comparacoes = 0;
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        while (j >= 0 && comparar(chave, mochila[j], criterio)) {
            mochila[j + 1] = mochila[j];
            j--;
        }
        mochila[j + 1] = chave;
    }
}

// Menu para escolher critério de ordenação
void menuDeOrdenacao() {
    int opc;
    printf("\n=== MENU DE ORDENACAO ===\n");
    printf("1 - Ordenar por Nome\n");
    printf("2 - Ordenar por Tipo\n");
    printf("3 - Ordenar por Prioridade\n");
    printf("Escolha: ");
    scanf("%d", &opc);

    if (opc < 1 || opc > 3) {
        printf("Opção inválida.\n");
        return;
    }

    insertionSort(opc);
    printf("Itens ordenados com sucesso!\n");
    printf("Comparações realizadas: %d\n", comparacoes);

    if (opc == NOME)
        ordenadaPorNome = true;
    else
        ordenadaPorNome = false;
}

// Busca binária por nome (só funciona se estiver ordenada por nome)
void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("A mochila precisa estar ordenada por nome primeiro!\n");
        return;
    }

    char nomeBusca[50];
    getchar();
    printf("Digite o nome do item que deseja buscar: ");
    fgets(nomeBusca, 50, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int inicio = 0, fim = numItens - 1;
    bool encontrado = false;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(nomeBusca, mochila[meio].nome);

        if (cmp == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            encontrado = true;
            break;
        } else if (cmp < 0)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }

    if (!encontrado)
        printf("Item não encontrado na mochila.\n");
}

// Função principal
int main() {
    int opcao;
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparTela();

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida, tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
