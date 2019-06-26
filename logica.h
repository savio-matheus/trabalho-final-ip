#ifndef LOGICA_H
#define LOGICA_H
#include "logica.c"

/*
*	Funções para entrada e saída de informações
*/

// Retorna a quantidade de structs "dados" em um arquivo.
long int blocos_struct( FILE *fp );

void imprime_dados( struct dados *p );

// Ordena as structs salvas em um vetor de acordo
// com certo critério.
void sort_nota( struct dados *p, FILE *fp, long int n );
void sort_nome( struct dados *p, FILE *fp, long int n );
void sort_matricula( struct dados *p, FILE *fp, long int n );

// Retorna -1 se não encontrar a matrícula indicada ou
// retorna a posição no arquivo se a encontrar e salva seus
// dados na struct passada.
int busca( unsigned int matric, FILE *fp, struct dados *temp );

// Retorna 1 se o cadastro for cancelado com uma matrícula = 0
// e 0 se for concluída.
int entra_dados( long int posic, short int alterando, FILE *fp );

void fecha_arq( FILE *fp );
FILE *cria_arq( char *nome_arq );
FILE *abre_arq( char *nome_arq );

// Limpa a tela e exibe duas mensagens (se requisitado).
void cria_tela( short int msg, short int cab );

void mostra_dir();

#endif