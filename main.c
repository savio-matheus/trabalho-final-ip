#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "logica.h"
#include "structs.h"

// *************************************************************************

void modCadastro();
void modConsulta();
void modAltera();
void modListagem();

int rotina_abertura(short int cab);

FILE *fp = NULL;

// *************************************************************************

int main()
{
	int op;

	cria_tela(0, 1);

	while(1)
	{
		printf("\t1 - Cadastrar nova turma\n");
		printf("\t2 - Consultar um aluno\n");
		printf("\t3 - Alterar dados de um aluno\n");
		printf("\t4 - Listar dados de uma turma\n");
		printf("\t5 - Terminar a execucao\n");
		printf("\nEscolha uma opcao: ");

		scanf(" %d", &op);

		switch (op)
		{
			case 1:
				modCadastro();
				cria_tela(1, 1);
				break;

			case 2:
				modConsulta();
				cria_tela(2, 1);
				break;

			case 3:
				modAltera();
				cria_tela(3, 1);
				break;

			case 4:
				modListagem();
				cria_tela(4, 1);
				break;

			case 5:
				cria_tela(0, 0);
				printf("Tchau\n");
				
				if(fp != NULL)
				{
					fclose(fp);
					fp = NULL;
				}
				return 0;

			default:
				cria_tela(5, 1);
		}
	}
}

// *************************************************************************

void modCadastro()
{
	char nome_arq[100];
	char diretorio[300];
	short int erro = 0;
	short int op;

	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	do
	{
		cria_tela(0, 2);
		if(erro)
		{
			cria_tela(6, 2);
			erro = 0;
		}

		mostra_dir();
		printf("Salvar em: ");
		scanf(" %300[^\n]", diretorio);

		if(chdir(diretorio))
		{
			erro = 1;
			continue;
		}

		printf("Digite o nome de um arquivo de turma: ");
		scanf(" %100[^\n]", nome_arq);

		if(strcmp(nome_arq, "fim") == 0)
		{
			return;
		}

		do{
			printf("1 - Criar novo arquivo\n");
			printf("2 - Adicionar a arquivo existente\n");
			printf("Escolha uma opcao: ");
			scanf(" %hi", &op);

			if(op == 1)
			{
				fp = cria_arq(nome_arq);
			}
			else if(op == 2)
			{
				fp = abre_arq(nome_arq);
			}
			else
			{
				cria_tela(5, 2);
			}
		}while( op != 1 && op != 2 );

		if(fp == NULL)
		{
			erro = 1;
		}
	}while(erro);

	cria_tela(0, 2);
	printf("Insira os dados\n");

	while(entra_dados(-1, fp) == 0);
}

// *************************************************************************

void modConsulta()
{
	long int posic = 0;
	unsigned int matricula;
	struct dados temp;

	if(rotina_abertura(3))
	{
		return;
	}

	cria_tela(0, 3);

	while(1)
	{
		if(posic == -1)
		{
			cria_tela(7, 3);
		}

		printf("Numero de matricula: ");
		scanf(" %u", &matricula);

		if(!matricula)
		{
			return;
		}

		cria_tela(0, 3);

		posic = busca(matricula, fp, &temp);

		if(posic != -1)
		{
			imprime_dados(&temp);
		}
	}
}

// *************************************************************************

void modAltera()
{
	int catch = 0;
	long int posic = 0;
	unsigned int matricula;
	struct dados temp;

	if(rotina_abertura(4))
	{
		return;
	}

	do
	{
		if(posic == -1)
		{
			cria_tela(7, 4);
		}
		else if(catch == 0)
		{
			cria_tela(0, 4);
		}
		else
		{
			cria_tela(8, 4);
		}

		printf("\nNumero de matricula: ");
		scanf(" %u", &matricula);

		if(!matricula)
		{
			return;
		}

		cria_tela(0, 4);

		posic = busca(matricula, fp, &temp);

		if(posic != -1)
		{
			imprime_dados(&temp);
		}
		else
		{
			continue;
		}

		printf("Insira os dados atualizados\n");
		catch = entra_dados(posic, fp);

	}while(1);
}

// *************************************************************************

void modListagem()
{
	short int op;
	long int blocos;
	struct dados *temp = NULL;

	if(rotina_abertura(5))
	{
		return;
	}

	cria_tela(0, 5);

	while(1)
	{
		if(temp == NULL)
		{
			blocos = blocos_struct(fp);
				
			temp = (struct dados*) malloc(
				sizeof (struct dados) * blocos + 1);
			rewind(fp);
			fread(temp, sizeof(struct dados), blocos, fp);
		}

		printf("\t1 - listar os alunos por ordem crescente de matricula\n");
		printf("\t2 - listar os alunos por ordem alfabetica de nome\n");
		printf("\t3 - listar os alunos por ordem crescente de nota final\n");
		printf("\t4 - terminar e voltar para processar nova turma\n");
		printf("\t5 - terminar execucao do modulo\n");
		printf("Escolha uma opcao: ");

		scanf(" %hi", &op);

		printf("\n");

		switch (op)
		{
			case 1:
				cria_tela(0, 5);
				sort_matricula(temp, blocos);

				for(int i = 0; i < blocos; i++)
				{
					imprime_dados(&temp[i]);
				}
				free(temp);
				temp = NULL;
				break;

			case 2:
				cria_tela(0, 5);
				sort_nome(temp, blocos);

				for(int i = 0; i < blocos; i++)
				{
					imprime_dados(&temp[i]);
				}
				free(temp);
				temp = NULL;
				break;

			case 3:
				cria_tela(0, 5);
				sort_nota(temp, blocos);

				for(int i = 0; i < blocos; i++)
				{
					imprime_dados(&temp[i]);
				}
				free(temp);
				temp = NULL;
				break;

			case 4:
				if(rotina_abertura(5))
				{
					if(temp != NULL)
					{
						free(temp);
						temp = NULL;
					}
					return;
				}
				else
				{
					cria_tela(0, 5);
					if(temp != NULL)
					{
						free(temp);
						temp = NULL;
					}
				}
				break;

			case 5:
				if(temp != NULL)
				{
					free(temp);
					temp = NULL;
				}
				return;

			default:
				cria_tela(5, 5);

		}
	}
}

// *************************************************************************

int rotina_abertura(short int cab)
{
	char nome_arq[100];
	char diretorio[300];
	char op = 'n';
	short int erro = 0;

	if(fp)
	{
		cria_tela(0, 0);
		printf("Continuar no arquivo atual [s/n]? ");
		scanf(" %c", &op);

		if(op == 'n' || op == 'N')
		{
			fclose(fp);
			fp = NULL;
		}
	}

	while(op == 'N' || op == 'n')
	{
		if(erro)
		{
			cria_tela(6, 0);
			erro = 0;
		}

		cria_tela(0, cab);

		mostra_dir();
		printf("Abrir de: ");
		scanf(" %300[^\n]", diretorio);
			
		if(chdir(diretorio))
		{
			erro = 1;
			continue;
		}

		printf("Digite o nome de um arquivo de turma: ");
		scanf(" %100[^\n]", nome_arq);

		if(strcmp(nome_arq, "fim") == 0)
		{
			return 1;
		}

		fp = abre_arq(nome_arq);

		if(fp == NULL)
		{
			erro = 1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}