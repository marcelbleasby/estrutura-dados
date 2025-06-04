#include<stdio.h>
#include<stdlib.h>

typedef struct Item {
    int codigo_produto;
    int tipo_produto;
    int descricao_produto;
    float preco_produto;
}Item;

typedef struct NoCodigoProduto {
    int codigo_produto;
    struct NoCodigoProduto *proximo;
} NoCodigoProduto;

typedef struct Caixa {
    int codigo_caixa;                      
    NoCodigoProduto *lista_codigos_produtos; 
    int quantidade_unidades_na_caixa;      
    int capacidade_maxima;
} Caixa;

typedef struct NoCaixa {
    Caixa caixa_data;
    struct NoCaixa *proximo;
} NoCaixa;

typedef struct PilhaCaixas {
    NoCaixa *topo;
    int tipo_produto_pilha;
    int capacidade_caixa_pilha;
} PilhaCaixas;