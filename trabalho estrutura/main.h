#ifndef MUSICAS_H
#define MUSICAS_H

typedef struct Musica Musica;
typedef struct Play Play;

Musica* criar(const char* Artista, const char* Nome);
void add(Play* playlist, const char* Artista, const char* Nome);
void carregaar(Play* playlist, const char* nome_arquivo);
void printar(Play* playlist);
void salvar(Play* playlist, const char* nome_arquivo);
void add2(Play* playlist);
void removendo(Play* playlist);
void procurar(Play* playlist);
void avanca(Play* playlist);
void volta(Play* playlist);

#endif