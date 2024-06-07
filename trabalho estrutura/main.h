#ifndef MUSICAS_H
#define MUSICAS_H

typedef struct Musica Musica;
typedef struct Play Play;

Musica* craindo(const char* artista, const char* nome);
void adicioando(Play* playlist, const char* artista, const char* nome);
void carregando(Play* playlist, const char* nome_arquivo);
void printando(Play* playlist);
void salvando(Play* playlist, const char* nome_arquivo);
void adicionado2(Play* playlist);
void removendo(Play* playlist);
void procurando(Play* playlist);
void avanca(Play* playlist);
void volta(Play* playlist);

#endif