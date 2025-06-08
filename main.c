#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    PARAFINA = 1,
    LEASH,
    QUILHA,
    DECK
} TipoProduto;

typedef struct {
    int codigo_produto;
    TipoProduto tipo_produto;
    char descricao_produto[100];
    float preco_produto;
} Item;

typedef struct NoProduto {
    Item produto;
    struct NoProduto* proximo;
    struct NoProduto* anterior;
} NoProduto;

typedef struct {
    NoProduto* inicio;
    NoProduto* fim;
} ListaProdutos;

typedef struct NoCodigoProduto {
    int codigo_produto;
    struct NoCodigoProduto* proximo;
} NoCodigoProduto;

typedef struct {
    int codigo_caixa;
    NoCodigoProduto* lista_codigos_produtos;
    int quantidade_unidades_na_caixa;
    int capacidade_maxima;
} Caixa;

typedef struct NoCaixa {
    Caixa caixa_data;
    struct NoCaixa* proximo;
} NoCaixa;

typedef struct {
    NoCaixa* topo;
    TipoProduto tipo_produto_pilha;
    int capacidade_caixa_pilha;
} PilhaCaixas;

const char* getNomeTipoProduto(TipoProduto tipo) {
    switch (tipo) {
        case PARAFINA: return "Parafina";
        case LEASH:    return "Leash";
        case QUILHA:   return "Quilha";
        case DECK:     return "Deck";
        default:       return "Desconhecido";
    }
}

void inicializarLista(ListaProdutos* lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
}

void adicionarProdutoOrdenado(ListaProdutos* lista, Item produto) {
    NoProduto* novo_no = (NoProduto*)malloc(sizeof(NoProduto));
    if (!novo_no) {
        perror("Falha ao alocar memória para novo produto");
        exit(EXIT_FAILURE);
    }
    novo_no->produto = produto;
    novo_no->proximo = NULL;
    novo_no->anterior = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo_no;
        lista->fim = novo_no;
        return;
    }

    if (produto.preco_produto < lista->inicio->produto.preco_produto) {
        novo_no->proximo = lista->inicio;
        lista->inicio->anterior = novo_no;
        lista->inicio = novo_no;
        return;
    }

    NoProduto* atual = lista->inicio;
    while (atual->proximo != NULL && atual->proximo->produto.preco_produto < produto.preco_produto) {
        atual = atual->proximo;
    }

    novo_no->proximo = atual->proximo;
    
    if (atual->proximo != NULL) {
        atual->proximo->anterior = novo_no;
    } else {
        lista->fim = novo_no;
    }
    
    atual->proximo = novo_no;
    novo_no->anterior = atual;
}

void inicializarPilha(PilhaCaixas* pilha, TipoProduto tipo, int capacidade) {
    pilha->topo = NULL;
    pilha->tipo_produto_pilha = tipo;
    pilha->capacidade_caixa_pilha = capacidade;
}

void empilhar(PilhaCaixas* pilha, Caixa caixa) {
    NoCaixa* novo_no = (NoCaixa*)malloc(sizeof(NoCaixa));
    if (!novo_no) {
        perror("Falha ao alocar memória para nova caixa na pilha");
        exit(EXIT_FAILURE);
    }
    novo_no->caixa_data = caixa;
    novo_no->proximo = pilha->topo;
    pilha->topo = novo_no;
}

void adicionarCodigoNaCaixa(Caixa* caixa, int codigo_produto) {
    NoCodigoProduto* novo_codigo = (NoCodigoProduto*)malloc(sizeof(NoCodigoProduto));
    if(!novo_codigo){
        perror("Falha ao alocar memoria para novo codigo na caixa");
        exit(EXIT_FAILURE);
    }
    novo_codigo->codigo_produto = codigo_produto;
    novo_codigo->proximo = caixa->lista_codigos_produtos;
    caixa->lista_codigos_produtos = novo_codigo;
    caixa->quantidade_unidades_na_caixa++;
}

void receberProduto(Item produto, ListaProdutos* lista_disponiveis, PilhaCaixas* pilhas[], int* contador_caixas) {
    adicionarProdutoOrdenado(lista_disponiveis, produto);

    PilhaCaixas* pilha_alvo = pilhas[produto.tipo_produto - 1];

    if (pilha_alvo->topo != NULL && pilha_alvo->topo->caixa_data.quantidade_unidades_na_caixa < pilha_alvo->capacidade_caixa_pilha) {
        adicionarCodigoNaCaixa(&(pilha_alvo->topo->caixa_data), produto.codigo_produto);
    } else {
        Caixa nova_caixa;
        nova_caixa.codigo_caixa = (*contador_caixas)++;
        nova_caixa.lista_codigos_produtos = NULL;
        nova_caixa.quantidade_unidades_na_caixa = 0;
        nova_caixa.capacidade_maxima = pilha_alvo->capacidade_caixa_pilha;
        
        adicionarCodigoNaCaixa(&nova_caixa, produto.codigo_produto);
        empilhar(pilha_alvo, nova_caixa);
    }
}

void imprimirPilha(PilhaCaixas pilha) {
    printf("=========================================================\n");
    printf("PILHA DE ESTOQUE - %s (Capacidade por caixa: %d)\n", getNomeTipoProduto(pilha.tipo_produto_pilha), pilha.capacidade_caixa_pilha);
    printf("=========================================================\n");

    if (pilha.topo == NULL) {
        printf("Pilha vazia.\n\n");
        return;
    }

    NoCaixa* atual = pilha.topo;
    int i = 1;
    while (atual != NULL) {
        printf("Caixa #%d (Cód: %d) | Quantidade: %d/%d | Códigos dos Produtos: ", 
               i++, atual->caixa_data.codigo_caixa, atual->caixa_data.quantidade_unidades_na_caixa, atual->caixa_data.capacidade_maxima);
        
        NoCodigoProduto* cod_atual = atual->caixa_data.lista_codigos_produtos;
        while (cod_atual != NULL) {
            printf("%d ", cod_atual->codigo_produto);
            cod_atual = cod_atual->proximo;
        }
        printf("\n");
        atual = atual->proximo;
    }
    printf("\n");
}

int main() {
    ListaProdutos lista_venda;
    inicializarLista(&lista_venda);

    PilhaCaixas pilha_parafina, pilha_leash, pilha_quilha, pilha_deck;
    inicializarPilha(&pilha_parafina, PARAFINA, 50);
    inicializarPilha(&pilha_leash, LEASH, 25);
    inicializarPilha(&pilha_quilha, QUILHA, 10);
    inicializarPilha(&pilha_deck, DECK, 5);
    
    PilhaCaixas* pilhas[] = {&pilha_parafina, &pilha_leash, &pilha_quilha, &pilha_deck};

    int contador_codigo_produto = 1;
    int contador_codigo_caixa = 1;

    printf("--- Tati Surf Co. - Simulando recebimento de 100 produtos ---\n\n");
    
    for (int i = 0; i < 100; i++) {
        Item novo_item;
        novo_item.codigo_produto = contador_codigo_produto++;
        novo_item.tipo_produto = (rand() % 4) + 1;
        sprintf(novo_item.descricao_produto, "Produto de marca %c", 'A' + (rand() % 5));
        novo_item.preco_produto = 50.0f + (rand() % 500);
        
        receberProduto(novo_item, &lista_venda, pilhas, &contador_codigo_caixa);
    }
    
    printf("--- Recebimento concluído. Verificando estoques. ---\n\n");

    imprimirPilha(pilha_parafina);
    imprimirPilha(pilha_leash);
    imprimirPilha(pilha_quilha);
    imprimirPilha(pilha_deck);

    return 0;
}
