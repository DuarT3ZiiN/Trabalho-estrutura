#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

typedef struct Musica {
    char Artista[100];
    char Nome[100];
    struct Musica* prox;
    struct Musica* ant;
} Musica;

typedef struct Playlist {
    Musica* atual;
} Playlist;

Musica* criarMusica(const char*Artista, const char* Nome){
    Musica* novaMusica = (Musica*)malloc(sizeof(Musica));
    if(novaMusica == NULL){
        perror("Falha ao alocar memoria para uma nova musica"); //perror = printa o erro de uma maneira específica
        exit(EXIT_FAILURE);
    }
    strcpy(novaMusica->Artista, Artista);
    strcpy(novaMusica->Nome, Nome);
    novaMusica->prox = novaMusica;
    novaMusica->ant = novaMusica;
    return novaMusica;
}

void addMusica(Playlist* playlist, const char* Artista, const char* Nome){
    Musica* novaMusica = criarMusica(Artista, Nome);
    if(playlist->atual == NULL){
        playlist->atual = novaMusica;
    }else{
        Musica* ultima = playlist->atual->ant;
        ultima->prox = novaMusica;
        novaMusica->ant = ultima;
        novaMusica->prox = playlist->atual;
        playlist->atual->ant = novaMusica;
    }
}

void carregarMusicasTxt(Playlist* playlist, const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "r");
    if(arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char Texto[200];
    while (fgets(Texto, sizeof(Texto), arquivo)){
        char* token = strtok(Texto, ";"); //função que divide strings com tokens
        if(token){
            char Artista[100];
            strcpy(Artista, token);
            token = strtok(NULL, "\n");
            if(token){
                char Nome[100];
                strcpy(Nome, token);
                addMusica(playlist, Artista, Nome);
            }
        }
    }

    fclose(arquivo);
}

void printarPlaylist(Playlist* playlist){
    if(playlist->atual == NULL){
        printf("A playlist esta vazia.\n");
        return;
    }

    Musica* inicio = playlist->atual;
    Musica* temp = inicio;
    do{
        printf("Artista: %s, Nome: %s\n", temp->Artista, temp->Nome);
        temp = temp->prox;
    }while (temp != inicio);
}

void salvarMusica(Playlist* playlist, const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "w"); //criando aqruivo
    if(arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    if(playlist->atual){
        Musica* inicio = playlist->atual;
        Musica* temp = inicio;
        do {
            fprintf(arquivo, "%s;%s\n", temp->Artista, temp->Nome);
            temp = temp->prox;
        } while (temp != inicio);
    }

    fclose(arquivo);
}

void addMusicaPlaylist(Playlist* playlist){
    char Artista[100];
    char Nome[100];

    printf("Digite o nome do Compositor para ser adicionado: ");
    fgets(Artista, sizeof(Artista), stdin);
    Artista[strcspn(Artista, "\n")] = 0; 

    printf("Digite o nome da musica para ser adicionada: ");
    fgets(Nome, sizeof(Nome), stdin);
    Nome[strcspn(Nome, "\n")] = 0;  

    addMusica(playlist, Artista, Nome);
    salvarMusica(playlist, "musicas.txt");
}

void removerMusica(Playlist* playlist){
    if(playlist->atual == NULL){
        printf("A playlist esta vazia.\n");
        return;
    }

    char Nome[100];
    printf("Digite o nome da musica para remover: ");
    fgets(Nome, sizeof(Nome), stdin);
    Nome[strcspn(Nome, "\n")] = 0;

    Musica* inicio = playlist->atual;
    Musica* temp = inicio;
    do{
        if(strcmp(temp->Nome, Nome) == 0){
            if(temp->prox == temp){
                free(temp);
                playlist->atual = NULL;
            }else{
                temp->ant->prox = temp->prox;
                temp->prox->ant = temp->ant;
                if(temp == inicio){
                    playlist->atual = temp->prox;
                }
                free(temp);
            }
            salvarMusica(playlist, "musicas.txt");
            printf("Musica removida.\n");
            return;
        }
        temp = temp->prox;
    } while (temp != inicio);

    printf("Musica nao encontrada.\n");
}

void procurarMusica(Playlist* playlist){
    if(playlist->atual == NULL){
        printf("A playlist esta vazia.\n");
        return;
    }

    char Nome[100];
    printf("Digite o nome da musica para a busca: ");
    fgets(Nome, sizeof(Nome), stdin);
    Nome[strcspn(Nome, "\n")] = 0;

    Musica* inicio = playlist->atual;
    Musica* temp = inicio;
    do{
        if(strcmp(temp->Nome, Nome) == 0){
            printf("Artista: %s, Nome: %s\n", temp->Artista, temp->Nome);
            return;
        }
        temp = temp->prox;
    }while(temp != inicio);

    printf("Musica nao encontrada.\n");
}

void Avancar(Playlist* playlist){
    if(playlist->atual){
        playlist->atual = playlist->atual->prox;
        printf("Musica atual: Artista: %s, Nome: %s\n", playlist->atual->Artista, playlist->atual->Nome);
    }else{
        printf("A playlist esta vazia.\n");
    }
}

void Voltar(Playlist* playlist){
    if(playlist->atual){
        playlist->atual = playlist->atual->ant;
        printf("Musica atual: Artista: %s, Nome: %s\n", playlist->atual->Artista, playlist->atual->Nome);
    }else{
        printf("A playlist esta vazia.\n");
    }
}

void sortNome(Playlist* playlist){
    if(!playlist->atual || playlist->atual->prox == playlist->atual){
        return;
    }

    Musica* inicio = playlist->atual;
    Musica* i = inicio;
    do{
        Musica* j = i->prox;
        while(j != inicio){
            if(strcmp(i->Nome, j->Nome) > 0){
                char temp_artista[100];
                char temp_nome[100];
                strcpy(temp_artista, i->Artista);
                strcpy(temp_nome, i->Nome);
                strcpy(i->Artista, j->Artista);
                strcpy(i->Nome, j->Nome);
                strcpy(j->Artista, temp_artista);
                strcpy(j->Nome, temp_nome);
            }
            j = j->prox;
        }
        i = i->prox;
    }while(i != inicio);
}

void printSort(Playlist* playlist){
    sortNome(playlist);
    printarPlaylist(playlist);
}


int main(){
    Playlist playlist = {NULL};

    carregarMusicasTxt(&playlist, "musicas.txt");

    printf("================================\n");
    printf("Menu de Interacao:\n");
    printf("1. Exibir playlist na ordem de cadastro\n");
    printf("2. Exibir playlist ordenada pelo nome da musica\n");
    printf("3. Adicionar nova musica\n");
    printf("4. Remover uma musica\n");
    printf("5. Buscar uma musica\n");
    printf("6. Avançar para a proxima musica\n");
    printf("7. Retornar a musica anterior\n");
    printf("8. Sair\n");
    printf("================================\n");

    int escolha;

        do{
        printf("\nDigite sua escolha: ");
        scanf("%d", &escolha);
        getchar();


        switch (escolha) {
            case 1:
                printarPlaylist(&playlist);
                break;
            case 2:
                printSort(&playlist);
                break;
            case 3:
                addMusicaPlaylist(&playlist);
                break;
            case 4:
                removerMusica(&playlist);
                break;
            case 5:
                procurarMusica(&playlist);
                break;
            case 6:
                Avancar(&playlist);
                break;
            case 7:
                Voltar(&playlist);
                break;
            case 8:
                printf("Obrigado pelo acesso!\n");
                break;
            default:
                printf("Escolha invalida. Por favor, tente novamente.\n");
                break;
        }

    } while(escolha !=8);
    return 0;
}