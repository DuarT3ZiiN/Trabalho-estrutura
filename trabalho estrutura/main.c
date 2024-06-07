#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

typedef struct Musica {
    char artista[100];
    char nome[100];
    struct Musica* prox;
    struct Musica* ant;
} Musica;

typedef struct Playlist {
    Musica* atual;
} Playlist;

Musica* CriarMusica(const char* artista, const char* nome){
    Musica* nova_musica = (Musica*)malloc(sizeof(Musica));
    if(nova_musica == NULL){
        perror("Falha ao alocar memoria para uma nova musica"); //perror = printa o erro de uma maneira específica
        exit(EXIT_FAILURE);
    }
    strcpy(nova_musica->artista, artista);
    strcpy(nova_musica->nome, nome);
    nova_musica->prox = nova_musica;
    nova_musica->ant = nova_musica;
    return nova_musica;
}

void AddMusica(Playlist* playlist, const char* artista, const char* nome){
    Musica* nova_musica = CriarMusica(artista, nome);
    if(playlist->atual == NULL){
        playlist->atual = nova_musica;
    }else{
        Musica* ultima = playlist->atual->ant;
        ultima->prox = nova_musica;
        nova_musica->ant = ultima;
        nova_musica->prox = playlist->atual;
        playlist->atual->ant = nova_musica;
    }
}

void CarregarMusicasTxt(Playlist* playlist, const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "r");
    if(arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo)){
        char* token = strtok(linha, ";"); //função que divide strings com tokens
        if(token){
            char artista[100];
            strcpy(artista, token);
            token = strtok(NULL, "\n");
            if(token){
                char nome[100];
                strcpy(nome, token);
                AddMusica(playlist, artista, nome);
            }
        }
    }

    fclose(arquivo);
}

void PrintarPlaylist(Playlist* playlist){
    if(playlist->atual == NULL){
        printf("A playlist esta vazia.\n");
        return;
    }

    Musica* inicio = playlist->atual;
    Musica* temp = inicio;
    do{
        printf("Artista: %s, Nome: %s\n", temp->artista, temp->nome);
        temp = temp->prox;
    }while (temp != inicio);
}

void SalvarMusica(Playlist* playlist, const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "w"); //criando aqruivo
    if(arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    if(playlist->atual){
        Musica* inicio = playlist->atual;
        Musica* temp = inicio;
        do {
            fprintf(arquivo, "%s;%s\n", temp->artista, temp->nome);
            temp = temp->prox;
        } while (temp != inicio);
    }

    fclose(arquivo);
}

void AddMusicaPlaylist(Playlist* playlist){
    char artista[100];
    char nome[100];

    printf("Digite o nome do Compositor para ser adicionado: ");
    fgets(artista, sizeof(artista), stdin);
    artista[strcspn(artista, "\n")] = 0; 

    printf("Digite o nome da musica para ser adicionada: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;  

    AddMusica(playlist, artista, nome);
    SalvarMusica(playlist, "musicas.txt");
}

void RemoverMusica(Playlist* playlist){
    if(playlist->atual == NULL){
        printf("A playlist esta vazia.\n");
        return;
    }

    char nome[100];
    printf("Digite o nome da musica para remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    Musica* inicio = playlist->atual;
    Musica* temp = inicio;
    do{
        if(strcmp(temp->nome, nome) == 0){
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
            SalvarMusica(playlist, "musicas.txt");
            printf("Musica removida.\n");
            return;
        }
        temp = temp->prox;
    } while (temp != inicio);

    printf("Musica nao encontrada.\n");
}

void ProcurarMusica(Playlist* playlist){
    if(playlist->atual == NULL){
        printf("A playlist esta vazia.\n");
        return;
    }

    char nome[100];
    printf("Digite o nome da musica para a busca: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    Musica* inicio = playlist->atual;
    Musica* temp = inicio;
    do{
        if(strcmp(temp->nome, nome) == 0){
            printf("Artista: %s, Nome: %s\n", temp->artista, temp->nome);
            return;
        }
        temp = temp->prox;
    }while(temp != inicio);

    printf("Musica nao encontrada.\n");
}

void Avancar(Playlist* playlist){
    if(playlist->atual){
        playlist->atual = playlist->atual->prox;
        printf("Musica atual: Artista: %s, Nome: %s\n", playlist->atual->artista, playlist->atual->nome);
    }else{
        printf("A playlist esta vazia.\n");
    }
}

void Voltar(Playlist* playlist){
    if(playlist->atual){
        playlist->atual = playlist->atual->ant;
        printf("Musica atual: Artista: %s, Nome: %s\n", playlist->atual->artista, playlist->atual->nome);
    }else{
        printf("A playlist esta vazia.\n");
    }
}

void SortNome(Playlist* playlist){
    if(!playlist->atual || playlist->atual->prox == playlist->atual){
        return;
    }

    Musica* inicio = playlist->atual;
    Musica* i = inicio;
    do{
        Musica* j = i->prox;
        while(j != inicio){
            if(strcmp(i->nome, j->nome) > 0){
                char temp_artista[100];
                char temp_nome[100];
                strcpy(temp_artista, i->artista);
                strcpy(temp_nome, i->nome);
                strcpy(i->artista, j->artista);
                strcpy(i->nome, j->nome);
                strcpy(j->artista, temp_artista);
                strcpy(j->nome, temp_nome);
            }
            j = j->prox;
        }
        i = i->prox;
    }while(i != inicio);
}

void PrintandoSort(Playlist* playlist){
    SortNome(playlist);
    PrintarPlaylist(playlist);
}


int main(){
    Playlist playlist = {NULL};

    CarregarMusicasTxt(&playlist, "musicas.txt");

    printf("================================\n");
    printf("Menu para Interacao:\n");
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
                PrintarPlaylist(&playlist);
                break;
            case 2:
                PrintandoSort(&playlist);
                break;
            case 3:
                AddMusicaPlaylist(&playlist);
                break;
            case 4:
                RemoverMusica(&playlist);
                break;
            case 5:
                ProcurarMusica(&playlist);
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