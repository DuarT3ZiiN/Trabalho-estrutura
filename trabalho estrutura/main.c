#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

#define MAX 300

typedef struct Menu
{
    int busca;
    int opcao;
    int resultadoBusca;
    char novaMusica[40];
    char novoGenero[25];
    char buscaNome[40];
    char buscaAlbum[40];
} Menu;

typedef struct Artistas
{
    char nome[100];
    char genero[25];
    char album[40];
} Artistas;

typedef struct Lista
{
    Artistas *AB;
    int quantidade;
} Lista;

int sort(const void *a, const void *b)
{
    return strcmp(((Artistas *)a)->nome, ((Artistas *)b)->nome);
}

void iniciandoLista(Lista *lista)
{
    lista->AB = NULL;
    lista->quantidade = 0;
}

void inserindoArtistas(Lista *lista, const Artistas *artistas)
{
    lista->quantidade++;
    lista->AB = (Artistas *)realloc(lista->AB, lista->quantidade * sizeof(Artistas));
    if (lista->AB == NULL)
    {
        printf("Erro.\n");
        exit(1);
    }
    lista->AB[lista->quantidade - 1] = *artistas;
}

void imprimirLista(Lista *lista)
{
    for (int i = 0; i < lista->quantidade; i++)
    {
        printf("Nome: %s\n", lista->AB[i].nome);
        printf("Genero: %s\n", lista->AB[i].genero);
        printf("Album:\n%s\n", lista->AB[i].album);
        printf("-------------\n");
    }
}

void liberarLista(Lista *lista)
{
    free(lista->AB);
    lista->AB = NULL;
    lista->quantidade = 0;
}

int buscaBinaria(Lista *lista, char *nome)
{
    int esq = 0;
    int dir = lista->quantidade - 1;

    while (esq <= dir)
    {
        int referencia = esq + (dir - esq) / 2;
        int cmp = strcmp(lista->AB[referencia].nome, nome);

        if (cmp == 0)
        {
            return referencia;
        }
        else if (cmp < 0)
        {
            esq = referencia + 1;
        }
        else
        {
            dir = referencia - 1;
        }
    }

    return -1;
}

void ordenar(Lista *lista, const Artistas *artistas, const char *arquivo)
{
    int pos0 = 0;

    while (pos0 < lista->quantidade && strcmp(artistas->nome, lista->AB[pos0].nome) > 0)
    {
        pos0++;
    }

    lista->quantidade++;
    lista->AB = (Artistas *)realloc(lista->AB, lista->quantidade * sizeof(Artistas));

    if (lista->AB == NULL)
    {
        perror("Erro.\n");
        exit(1);
    }

    for (int i = lista->quantidade - 1; i > pos0; i--)
    {
        lista->AB[i] = lista->AB[i - 1];
    }

    lista->AB[pos0] = *artistas;

    FILE *albumArquivo = fopen(arquivo, "w");
    if (albumArquivo == NULL)
    {
        perror("Erro ao abrir arquivo.\n");
        exit(1);
    }

    for (int i = 0; i < lista->quantidade; i++)
    {
        fprintf(albumArquivo, "%s\n%s\n%s\n-------------\n", lista->AB[i].nome, lista->AB[i].genero, lista->AB[i].album);
    }

    fclose(albumArquivo);
}

void removerArtista(Lista *lista, int posicao)
{
    if (posicao < 0 || posicao >= lista->quantidade)
    {
        perror("Posicao invalida.\n");
        return;
    }

    FILE *arquivoTemporario = fopen("temporario.txt", "w");
    if (arquivoTemporario == NULL)
    {
        perror("Erro.\n");
        exit(1);
    }

    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro.\n");
        fclose(arquivoTemporario);
        exit(1);
    }

    int parametro = 0;
    char parametroTexto[MAX];
    Artistas artistas;

    while (fgets(parametroTexto, MAX, arquivo))
    {
        parametroTexto[strcspn(parametroTexto, "\n")] = '\0';

        if (strcmp(parametroTexto, "-------------") == 0)
        {
            if (parametro >= 4)
            {
                if (parametro / 4 != posicao)
                {
                    fprintf(arquivoTemporario, "%s\n%s\n%s\n-------------\n", artistas.nome, artistas.genero, artistas.album);
                }
            }
            parametro = 0;
        }
        else
        {
            switch (parametro)
            {
            case 0:
                strcpy(artistas.nome, parametroTexto);
                break;
            case 1:
                strcpy(artistas.genero, parametroTexto);
                break;

            case 2:
                strcpy(artistas.album, parametroTexto);
                break;
            }
            parametro++;
        }
    }

    fclose(arquivo);
    fclose(arquivoTemporario);

    remove("artistas.txt");
    rename("temp.txt", "artistas.txt");

    for (int i = posicao; i < lista->quantidade - 1; i++)
    {
        lista->AB[i] = lista->AB[i + 1];
    }

    lista->quantidade--;
    lista->AB = (Artistas *)realloc(lista->AB, lista->quantidade * sizeof(Artistas));
    if (lista->quantidade == 0)
    {
        free(lista->AB);
        lista->AB = NULL;
    }
}

void editarArtista(Lista *lista, int posicao, const Artistas *novoArtista)
{
    if (posicao < 0 || posicao >= lista->quantidade)
    {
        perror("Posicao invalida.\n");
        return;
    }

    FILE *arquivoTemporario = fopen("temp.txt", "w");
    if (arquivoTemporario == NULL)
    {
        perror("Erro.\n");
        exit(1);
    }

    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro.\n");
        fclose(arquivoTemporario);
        exit(1);
    }

    int parametro = 0;
    char parametroTexto[MAX];
    Artistas artistas;

    while (fgets(parametroTexto, MAX, arquivo))
    {
        parametroTexto[strcspn(parametroTexto, "\n")] = '\0';

        if (strcmp(parametroTexto, "-------------") == 0)
        {
            if (parametro >= 4)
            {
                if (parametro / 4 != posicao)
                {
                    fprintf(arquivoTemporario, "%s\n%s\n%s\n-------------\n", artistas.nome, artistas.genero, artistas.album);
                }
                else
                {
                    fprintf(arquivoTemporario, "%s\n%s\n%s\n-------------\n", novoArtista->nome, novoArtista->genero, novoArtista->album);
                }
            }
            parametro = 0;
        }
        else
        {
            switch (parametro)
            {
            case 0:
                strcpy(artistas.nome, parametroTexto);
                break;
            case 1:
                strcpy(artistas.genero, parametroTexto);
                break;
            case 2:
                strcpy(artistas.album, parametroTexto);
                break;
            }
            parametro++;
        }
    }

    fclose(arquivo);
    fclose(arquivoTemporario);

    remove("artistas.txt");
    rename("temp.txt", "artistas.txt");

    lista->AB[posicao] = *novoArtista;
}

int buscaSequencial(Lista *lista, char *album)
{
    for (int i = 0; i < lista->quantidade; i++)
    {
        if (strstr(lista->AB[i].album, album) != NULL)
        {
            return i;
        }
    }
    return -1;
}

int main()
{

    Lista lista;
    Menu Menu;
    iniciandoLista(&lista);

    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo\n");
        exit(1);
    }

    char parametroTexto[MAX];
    Artistas artistas;
    int parametro = 0;

    while (fgets(parametroTexto, MAX, arquivo))
    {
        parametroTexto[strcspn(parametroTexto, "\n")] = '\0';

        if (strcmp(parametroTexto, "-------------") == 0)
        {
            if (parametro >= 4)
            {
                inserindoArtistas(&lista, &artistas);
            }
            parametro = 0;
        }
        else
        {
            switch (parametro)
            {
            case 0:
                strcpy(artistas.nome, parametroTexto);
                break;
            case 1:
                strcpy(artistas.genero, parametroTexto);
                break;
            case 2:
                strcpy(artistas.album, parametroTexto);
                break;
            default:
                strcat(artistas.album, "\n");
                strcat(artistas.album, parametroTexto);
                break;
            }
            parametro++;
        }
    }
    fclose(arquivo);

    // parei aqui, ainda presica debugar

    do
    {
        printf("\nMenu de Interacao :\n");
        printf("(1) - Buscar Artistas ou Bandas por nome\n");
        printf("(2) - Buscar Artistas ou Bandas por álbum\n");
        printf("(3) - Adicionar Artistas ou Bandas\n");
        printf("(4) - Editar Artistas ou Bandas\n");
        printf("(5) - Remover Artistas ou Bandas\n");
        printf("(6) - Mostrar Lista de Artistas e Bandas\n");
        printf("(7) - Sair\n");
        printf("\nEscolha uma das opcoes(Informe o valor): ");
        scanf("%d", &Menu.opcao);

        switch (Menu.opcao)
        {
        case 1:
            printf("\n\nInforme o Artista ou Banda para pesquisa: ");
            getchar();
            fgets(Menu.buscaNome, sizeof(Menu.buscaNome), stdin);
            Menu.buscaNome[strcspn(Menu.buscaNome, "\n")] = '\0';

            Menu.busca = buscaBinaria(&lista, Menu.buscaNome);

            if (Menu.busca != -1)
            {
                printf("\nArtista ou Banda encontrado\n");
                printf("Nome: %s\n", lista.AB[Menu.busca].nome);
                printf("Gênero Musical: %s\n", lista.AB[Menu.busca].genero);
                printf("Álbuns:\n%s\n", lista.AB[Menu.busca].album);
            }
            else
            {
                printf("Artista ou Banda nao encontrado\n");
            }
            break;

        case 2:
            printf("\n\nInforme o nome do album que você deseja encontrar: ");
            getchar();
            fgets(Menu.buscaAlbum, sizeof(Menu.buscaAlbum), stdin);
            Menu.buscaAlbum[strcspn(Menu.buscaAlbum, "\n")] = '\0';

            Menu.resultadoBusca = buscaSequencial(&lista, Menu.buscaAlbum);

            if (Menu.resultadoBusca != -1)
            {
                printf("\nAlbum encontrado\n Especificacoes do Criador do Album:\n");
                printf("Nome: %s\n", lista.AB[Menu.resultadoBusca].nome);
                printf("Albuns:\n%s\n", lista.AB[Menu.resultadoBusca].album);
            }
            else
            {
                printf("\nAlbum nao encontrado. \n");
            }
            break;

        case 3:
            printf("\nInfomre o nome do Artista ou Banda: ");
            getchar();
            fgets(artistas.nome, sizeof(artistas.nome), stdin);
            artistas.nome[strcspn(artistas.nome, "\n")] = '\0';

            printf("Informe o Genero Musical: ");
            fgets(artistas.genero, sizeof(artistas.genero), stdin);
            artistas.genero[strcspn(artistas.genero, "\n")] = '\0';

            printf("Informe os Albuns (Separe com Virgula): ");
            fgets(artistas.album, sizeof(artistas.album), stdin);
            artistas.album[strcspn(artistas.album, "\n")] = '\0';

            ordenar(&lista, &artistas, "artistas.txt");
            printf("\nArtista adicionado.\n");
            break;

        case 4:
            printf("\n\nInforme o nome do Artista ou Banda: ");
            getchar();
            fgets(Menu.buscaNome, sizeof(Menu.buscaNome), stdin);
            Menu.buscaNome[strcspn(Menu.buscaNome, "\n")] = '\0';

            Menu.busca = buscaBinaria(&lista, Menu.buscaNome);

            if (Menu.busca != -1)
            {
                printf("\nInforme o novo Nome do Artisa ou Banda: ");
                fgets(Menu.novaMusica, sizeof(Menu.novaMusica), stdin);
                Menu.novaMusica[strcspn(Menu.novaMusica, "\n")] = '\0';

                printf("Informe o novo genero musical: ");
                fgets(Menu.novoGenero, sizeof(Menu.novoGenero), stdin);
                Menu.novoGenero[strcspn(Menu.novoGenero, "\n")] = '\0';

                strcpy(artistas.nome, Menu.novaMusica);
                strcpy(artistas.genero, Menu.novoGenero);

                editarArtista(&lista, Menu.busca, &artistas);
                printf("\nArtista editado.\n");
            }
            else
            {
                printf("\nArtista nao encontrado para ser editado.\n");
            }
            break;

        case 5:
            printf("\n\nInforme o nome do artista para ser  removido: ");
            getchar();
            fgets(Menu.buscaNome, sizeof(Menu.buscaNome), stdin);
            Menu.buscaNome[strcspn(Menu.buscaNome, "\n")] = '\0';

            Menu.busca = buscaBinaria(&lista, Menu.buscaNome);

            if (Menu.busca != -1)
            {
                removerArtista(&lista, Menu.busca);
                printf("\nArtista removido.\n");
            }
            else
            {
                printf("\nArtista nao encontrado.\n");
            }
            break;

        case 6:
            printf("\nLista de Artista e Bandas \n\n");
            qsort(lista.AB, lista.quantidade, sizeof(Artistas), sort);
            imprimirLista(&lista);
            break;

        case 7:
            printf("Obrigado!!\n");
            break;

        default:
            printf("\nOpcao invalida. \nEscolha uma opçao de 1 a 7.\n");
            break;
        }

    } while (Menu.opcao != 7);

    liberarLista(&lista);

    return 0;
}