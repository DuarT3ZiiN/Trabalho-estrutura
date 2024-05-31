#include <stdio.h>
#include <stdlib.h>
#include "variaveis.h"

typedef struct Musicas {
    int valor;
    struct musicas* prox;

} Musicas;

Musicas* inicio;
Musicas* fim;

Musicas* inicializaLista(int valor){
    Musicas* elem = (Musicas*) malloc(sizeof(Musicas));

    if(elem == NULL){
        printf("Erro ao alocar item!!!\n");
    }

    elem->valor = valor;
    elem->prox = NULL;
    inicio = elem;
    fim = elem;
    return elem;
}void exibeLista(Musicas* itemInicial){

		Musicas* musicaAtual = inicio;

		while(1){
				printf("%d\n", musicaAtual->valor);
				musicaAtual = musicaAtual->prox;
				if(musicaAtual == inicio){
					break;
				}
		}
}


void adicionaElemento(int valor){
    Musicas* elem = (Musicas*) malloc(sizeof(Musicas));

    if(elem == NULL){
        printf("Erro ao inserir a musica !!!\n");
    }

    elem->valor = valor;
    elem->prox = inicio;
		fim->prox = elem;
    fim = elem;
}
int buscaPorElemento(int valor){
    Musicas* musicaAtual = inicio;
    int ix = 0;
    while(1){
        if(musicaAtual->valor == valor){
            printf("Elemento com valor %d encontrado!\n", valor);
            return ix;
        }
        musicaAtual = musicaAtual->prox;
        if(musicaAtual == inicio){
            break;
        }
        ix++;
    }
    return -1;
}



int removeElemento(int valor){
    Musicas* musicaAtual = inicio;
    Musicas* musicaAnterior = NULL;
    while(1){
        if(musicaAtual->valor == valor){
            printf("Elemento com valor %d encontrado! Removendo elemento...\n", valor);
            if(musicaAtual == inicio){ // removendo primeiro elemento
                inicio = musicaAtual->prox; // atualizar ponteiro para início da lista
								fim->prox = inicio;
            } else if(musicaAtual == fim){ // removendo último elemento
                musicaAnterior->prox = inicio; // atualizar ponteiro para fim da lista
                fim = musicaAtual;
            } else { // se for elemento do "meio"
                musicaAnterior->prox = itemAtual->prox;
            }
            free(musicaAtual);
            break;
        }
        musicaAtual = musicaAtual;
        musicaAnterior = itemAtual->prox;

				// se deu a volta na lista e ainda não encontrou...
        if(musicaAtual == inicio){
            break;
        }
    }
}

main(){
    FILE* arq = fopen("musicas.txt", "r");


    fclose(arq); 
    return 0;
}