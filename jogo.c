#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NO {
    char jogador[50];
    char tipo_de_vento[20];
    int tempo_de_jogo;
    struct NO* proximo;
} NO;

typedef struct Lista {
    NO* inicio_lista;
} Lista;

typedef struct NoFila {
    char jogadorEntrando[50];
    char jogadorSaindo[50];
    struct NoFila* proximo;
} NoFila;

typedef struct Fila {
    NoFila* inicio_fila;
    NoFila* final_fila;
} Fila;

typedef struct NoPilha {
    char jogador[50];
    char tipoEstatistica[20];
    struct NoPilha* proximo;
} NoPilha;

typedef struct Pilha {
    NoPilha* topo;
} Pilha;

void adicionarEvento(Lista* lista, char jogador[], char tipoEvento[], int tempo_de_jogo) {
    NO* novoNo = (NO*)malloc(sizeof(NO));
    strcpy(novoNo->jogador, jogador);
    strcpy(novoNo->tipo_de_vento, tipoEvento);
    novoNo->tempo_de_jogo = tempo_de_jogo;
    novoNo->proximo = lista->inicio_lista;
    lista->inicio_lista = novoNo;
}

void removerJogador(Lista* lista, char jogador[]) {
    NO* atual = lista->inicio_lista;
    NO* anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->jogador, jogador) == 0) {
            if (anterior == NULL) {
                lista->inicio_lista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Erro: O jogador %s não existe na lista.\n", jogador);
}

void enfileirar(Fila* fila, Lista* lista, char jogadorEntrando[], char jogadorSaindo[]) {
    NO* jogadorAtual = lista->inicio_lista;
    int jogadorEncontrado = 0;

    while (jogadorAtual != NULL) {
        if (strcmp(jogadorAtual->jogador, jogadorSaindo) == 0) {
            jogadorEncontrado = 1;
            break;
        }
        jogadorAtual = jogadorAtual->proximo;
    }

    if (!jogadorEncontrado) {
        printf("Erro: O jogador %s não existe na lista.\n", jogadorSaindo);
        return;
    }

    NoFila* novoNo = (NoFila*)malloc(sizeof(NoFila));
    strcpy(novoNo->jogadorEntrando, jogadorEntrando);
    strcpy(novoNo->jogadorSaindo, jogadorSaindo);
    novoNo->proximo = NULL;
    removerJogador(lista, jogadorSaindo);

    if (fila->final_fila == NULL) {
        fila->inicio_fila = fila->final_fila = novoNo;
    } else {
        fila->final_fila->proximo = novoNo;
        fila->final_fila = novoNo;
    }
}

void imprimirSubstituicoes(Fila* fila) {
    printf("\nJogadores que entraram como substitutos:\n");

    NoFila* atual = fila->inicio_fila;
    while (atual != NULL) {
        printf("%s ", atual->jogadorEntrando);
        atual = atual->proximo;
    }
}

void desenfileirar(Fila* fila) {
    if (fila->inicio_fila == NULL) {
        printf("Fila de substituições vazia.\n");
        return;
    }

    NoFila* temp = fila->inicio_fila;
    fila->inicio_fila = temp->proximo;

    if (fila->inicio_fila == NULL) {
        fila->final_fila = NULL;
    }

    free(temp);
}

void empilhar(Pilha* pilha, char jogador[], char tipoEstatistica[]) {
    NoPilha* novoNo = (NoPilha*)malloc(sizeof(NoPilha));
    strcpy(novoNo->jogador, jogador);
    strcpy(novoNo->tipoEstatistica, tipoEstatistica);
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
}

void desempilhar(Pilha* pilha) {
    if (pilha->topo == NULL) {
        printf("Pilha de estatísticas vazia.\n");
        return;
    }

    NoPilha* temp = pilha->topo;
    pilha->topo = temp->proximo;

    free(temp);
}

int main() {
    Lista listaEventos;
    listaEventos.inicio_lista = NULL;

    Fila filaSubstituicoes;
    filaSubstituicoes.inicio_fila = filaSubstituicoes.final_fila = NULL;

    Pilha pilhaEstatisticas;
    pilhaEstatisticas.topo = NULL;

    int escolha;
    do {
        printf("\nMENU:\n\n");
        printf("\t1 - Adicionar Evento\n");
        printf("\t2 - Enfileirar Substituicao\n");
        printf("\t3 - Desenfileirar Substituicao\n");
        printf("\t4 - Empilhar Estatistica\n");
        printf("\t5 - Desempilhar Estatistica\n");
        printf("\t6 - Exibir Eventos da Partida\n");
        printf("\t7 - Imprimir imprimir Substituicoes\n");
        printf("\t0 - Sair\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: {
                char jogador[50];
                char tipoEvento[20];
                int tempo;
                printf("Digite o nome do jogador: ");
                scanf("%s", jogador);
                printf("Digite o tipo do evento: ");
                scanf("%s", tipoEvento);
                printf("Digite o tempo de jogo: ");
                scanf("%d", &tempo);
                adicionarEvento(&listaEventos, jogador, tipoEvento, tempo);
                break;
            }
            case 2: {
                char jogadorEntrando[50];
                char jogadorSaindo[50];
                printf("Digite o nome do jogador que entra: ");
                scanf("%s", jogadorEntrando);
                printf("Digite o nome do jogador que sai: ");
                scanf("%s", jogadorSaindo);
                enfileirar(&filaSubstituicoes, &listaEventos, jogadorEntrando, jogadorSaindo);
                break;
            }
            case 3:
                desenfileirar(&filaSubstituicoes);
                break;
            case 4: {
                char jogador[50];
                char tipoEstatistica[20];
                printf("Digite o nome do jogador: ");
                scanf("%s", jogador);
                printf("Digite o tipo da estatistica: ");
                scanf("%s", tipoEstatistica);
                empilhar(&pilhaEstatisticas, jogador, tipoEstatistica);
                break;
            }
            case 5:
                desempilhar(&pilhaEstatisticas);
                break;
            case 6: {
                printf("\nEventos da Partida:\n");
                NO* eventoAtual = listaEventos.inicio_lista;
                while (eventoAtual != NULL) {
                    printf("Jogador: %s, Tipo: %s, Tempo: %d\n", eventoAtual->jogador, eventoAtual->tipo_de_vento, eventoAtual->tempo_de_jogo);
                    eventoAtual = eventoAtual->proximo;
                }
                break;
            }
            case 7:
                imprimirSubstituicoes(&filaSubstituicoes);
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (escolha != 0);

    return 0;
}

