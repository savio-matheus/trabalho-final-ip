#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

#if defined (__MINGW32__) || defined(_MSC_VER)
#define LIMPA_TELA system("cls")
#define MOSTRA_DIR system("dir")
#else
#define LIMPA_TELA system("clear")
#define MOSTRA_DIR system("ls")
#endif

long int blocos_struct( FILE *fp )
{
	// Fonte: http://allanlima.wordpress.com/category/dicas-e-truques-de-c-e-c/
	long posicaoAtual = ftell(fp);
	long tamanho;
	fseek(fp, 0, SEEK_END);
	tamanho = ftell( fp );
	fseek(fp, posicaoAtual, SEEK_SET);

	return (tamanho / sizeof (struct dados));
}

void imprime_dados( struct dados *p )
{
	printf("\n===============\n");
	printf("Matricula: %u\n", p->matricula);
	printf("Nome: %s\n", p->nome);

	printf("Notas de prova: ");
	for(int i = 0; i < 6; i++)
	{
		printf("%.2f", p->notasProva[i]);
		if(i + 1 < 6)
		{
			printf(", ");
		}
		else
		{
			printf("\n");
		}
	}

	printf("Notas de Listas: ");
	for(int i = 0; i < 6; i++)
	{
		printf("%.2f", p->notasExerc[i]);
		if(i + 1 < 6)
		{
			printf(", ");
		}
		else
		{
			printf("\n");
		}
	}

	printf("Nota de trabalho: %.2f\n", p->notaTrab);
	printf("Nota final: %.2lf\n", p->notaFinal);
	printf("Faltas: %u\n", p->faltas);
	printf("%s\n", p->estado);
	printf("===============\n\n");
}

void sort_nota( struct dados *p, long int n )
{
	long int i, j, min;
	struct dados temp_lf;

	for(i = 0; i < n; i++)
	{
		min = i;
		for(j = i + 1; j < n; j++)
		{
			if(p[j].notaFinal < p[min].notaFinal)
			{
				min = j;
			}
		}
		temp_lf = p[i];
		p[i] = p[min];
		p[min] = temp_lf;
	}
}

void sort_nome( struct dados *p, long int n )
{
	long int i, j, min;
	struct dados temp_c;

	for(i = 0; i < n; i++)
	{
		min = i;
		for(j = i + 1; j < n; j++)
		{
			if(strcmp(p[j].nome, p[min].nome) <= 0)
			{
				min = j;
			}
		}
		temp_c = p[i];
		p[i] = p[min];
		p[min] = temp_c;
	}
}

void sort_matricula( struct dados *p, long int n )
{
	long int i, j, min;
	struct dados temp_u;

	for(i = 0; i < n; i++)
	{
		min = i;
		for(j = i + 1; j < n; j++)
		{
			if(p[j].matricula < p[min].matricula)
			{
				min = j;
			}
		}
		temp_u = p[i];
		p[i] = p[min];
		p[min] = temp_u;
	}
}

int busca( unsigned int matric, FILE *fp, struct dados *temp )
{
	long int posicaoAtual, posicaoMatric;

	if(fp == NULL)
	{
		return 1;
	}
	
	posicaoAtual = ftell(fp);

	rewind(fp);

	while(fread(temp, sizeof (struct dados), 1, fp))
	{ 
		if(temp->matricula == matric)
		{
			posicaoMatric = ftell(fp);
			fseek(fp, posicaoAtual, SEEK_SET);

			return posicaoMatric - sizeof (struct dados);
		}
	}

	fseek(fp, posicaoAtual, SEEK_SET);

	return -1;
}

int entra_dados( long int posic, FILE *fp )
{
	struct dados temp;
	struct dados atual;

	const double
		PESO_NP = 0.8,
		PESO_NLT = 0.1;

	const char
		*APROVADO ="Aprovado",
		*REP_NOTA = "Reprovado por nota",
		*REP_FALTA = "Reprovado por falta",
		*REP_NOTA_FALTA = "Reprovado por nota e por falta";

	const short int
		MAX_FALTAS = 32;

	unsigned int tempMatric;
	double NP = 0;
	double NL = 0;
	double NF = 0;
	float NT;

	if(posic >= 0)
	{
		fseek(fp, posic, SEEK_SET);
		fread(&atual, sizeof(struct dados), 1, fp);

		do{
			printf("Matricula: ");
			scanf(" %u", &tempMatric);

			if(tempMatric == atual.matricula)
			{
				break;
			}

		}while(busca(tempMatric, fp, &temp) != -1);

		fseek(fp, posic, SEEK_SET);
	}
	else
	{
		do{
			printf("Matricula: ");
			scanf(" %u", &tempMatric);

		}while(busca(tempMatric, fp, &temp) != -1);

		fseek(fp, 0, SEEK_END);
	}

	if(tempMatric == 0)
	{
		return 1;
	}

	temp.matricula = tempMatric;

	printf("Nome: ");
	scanf(" %100[^\n]", temp.nome);
	temp.nome[0] = toupper(temp.nome[0]);

	printf("Notas de prova: ");
	for(int i = 0; i < 6; i++)
	{
		scanf(" %f", &temp.notasProva[i]);
		NP += (double) temp.notasProva[i];
	}

	printf("Notas de listas de exercicios: ");
	for(int i = 0; i < 6; i++)
	{
		scanf(" %f", &temp.notasExerc[i]);
		NL += (double) temp.notasExerc[i];
	}

	printf("Nota de trabalho: ");
	scanf(" %f", &temp.notaTrab);
	NT = temp.notaTrab;

	printf("Numero de ausencias: ");
	scanf(" %u", &temp.faltas);

	// Cálculo de nota e situação do aluno.
	NP /= 6;
	NL /= 6;
	NF = (double)
		(PESO_NP * NP + PESO_NLT * NL + PESO_NLT * NT);

	temp.notaFinal = (double) NF;

	if(temp.notaFinal >= 6.0)
	{
		if(temp.faltas < MAX_FALTAS)
		{
			strcpy(temp.estado, APROVADO);
		}
		else
		{
			strcpy(temp.estado, REP_FALTA);
		}
	}
	else
	{
		if(temp.faltas < MAX_FALTAS)
		{
			strcpy(temp.estado, REP_NOTA);
		}
		else
		{
			strcpy(temp.estado, REP_NOTA_FALTA);
		}
	}

	fwrite(&temp, sizeof (struct dados), 1, fp);

	return 0;
}

FILE *cria_arq( char *nome_arq )
{
	FILE *fp;
	fp = fopen(nome_arq, "w+b");

	if(fp == NULL)
	{
		return NULL;
	}

	return fp;
}

FILE *abre_arq( char *nome_arq )
{
	FILE *fp;

	fp = fopen(nome_arq, "r+b");

	if(fp == NULL)
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_END);

	return fp;
}

void cria_tela( short int msg, short int cab )
{
	LIMPA_TELA;

	char *mensagens[] = {
		"\0",
		"<- Cadastro encerrado\n",
		"<- Consulta encerrada\n",
		"<- Alteracao encerrada\n",
		"<- Listagem encerrada\n",
		"<- Opcao invalida!\n",
		"<- Algo deu errado, tente novamente\n",
		"<- Nenhuma matricula encontrada\n",
		"<- Atualizacao cancelada\n",
		"Tchau\n"
	};

	char *cabecalhos[] = {
		"\0",
		"======= Menu principal\n",
		"======= Modulo de Cadastro\n",
		"======= Modulo de Consulta\n",
		"======= Modulo de Alteracao\n",
		"======= Modulo de Listagem\n"
	};

	if(msg)
	{
		printf("%s", mensagens[msg]);
	}

	if(cab)
	{
		printf("%s", cabecalhos[cab]);
	}
}

void mostra_dir()
{
	MOSTRA_DIR;
}