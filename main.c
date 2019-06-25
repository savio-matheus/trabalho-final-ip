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

int rotina_abertura(char *CABECALHO);
void rotina_ordenacao(short int op);

FILE *fp = NULL;

// *************************************************************************

int main()
{
	int op, retorno;

	limpa_tela();

	while(1)
	{
		printf("======= Menu principal\n");
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
				limpa_tela();
				printf("<- Cadastro encerrado\n");
				break;

			case 2:
				modConsulta();
				limpa_tela();
				printf("<- Consulta encerrada\n");
				break;

			case 3:
				modAltera();
				limpa_tela();
				printf("<- Alteracao encerrada\n");
				break;

			case 4:
				modListagem();
				limpa_tela();
				printf("<- Listagem encerrada\n");
				break;

			case 5:
				fecha_arq(fp);
				limpa_tela();
				printf("Tchau\n");
				return 0;

			default:
				limpa_tela();
				printf("<- Opcao invalida!\n");
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

	fecha_arq(fp);

	do
	{
		limpa_tela();
		if(erro)
		{
			printf("<- Algo deu errado, tente novamente\n");
			erro = 0;
		}

		printf("======= Modulo de Cadastro\n");

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
			scanf(" %d", &op);

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
				limpa_tela();
				printf("<- Opcao invalida\n");
				printf("======= Modulo de Cadastro\n");
			}
		}while( op != 1 && op != 2 );

		if(fp == NULL)
		{
			erro = 1;
		}
	}while(erro);

	limpa_tela();
	printf("======= Modulo de Cadastro\n");
	printf("Insira os dados\n");

	while(entra_dados(0, 0, fp) == 0);
}

// *************************************************************************

void modConsulta()
{
	long int posic = 0;
	unsigned int matricula;
	struct dados temp;

	if(rotina_abertura("======= Modulo de Consulta")) return;

	limpa_tela();
	printf("======= Modulo de Consulta\n");

	while(1)
	{
		if(posic == -1)
		{
			limpa_tela();
			printf("<- Nenhuma matricula encontrada\n");
			printf("======= Modulo de Consulta\n");
		}

		printf("Numero de matricula: ");
		scanf(" %u", &matricula);

		if(!matricula)
		{
			return;
		}

		limpa_tela();
		printf("======= Modulo de Consulta\n");

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

	if(rotina_abertura("======= Modulo de Alteracao")) return;

	do
	{
		if(posic == -1)
		{
			limpa_tela();
			printf("<- Nenhuma matricula encontrada\n");
			printf("======= Modulo de Alteracao\n");
		}
		else if(catch == 0)
		{
			limpa_tela();
			printf("======= Modulo de Alteracao\n");
		}
		else
		{
			limpa_tela();
			printf("<- Atualizacao cancelada\n");
			printf("======= Modulo de Alteracao\n");
		}

		printf("\nNumero de matricula: ");
		scanf(" %u", &matricula);

		if(!matricula)
		{
			return;
		}

		limpa_tela();
		printf("======= Modulo de Alteracao\n");

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
		catch = entra_dados(posic, 1, fp);

	}while(1);
}

// *************************************************************************

void modListagem()
{
	short int op;

	if(rotina_abertura("======= Modulo de Listagem")) return;

	limpa_tela();

	while(1)
	{
		printf("======= Modulo de Listagem\n");
		printf("\t1 - listar os alunos por ordem crescente de matricula\n");
		printf("\t2 - listar os alunos por ordem alfabetica de nome\n");
		printf("\t3 - listar os alunos por ordem crescente de nota final\n");
		printf("\t4 - terminar e voltar para processar nova turma\n");
		printf("\t5 - terminar execucao do modulo\n");
		printf("Escolha uma opcao: ");

		scanf(" %d", &op);

		printf("\n");

		switch (op)
		{
			case 1:
				limpa_tela();
				printf("======= Modulo de Listagem\n");
				rotina_ordenacao(op);
				break;

			case 2:
				limpa_tela();
				printf("======= Modulo de Listagem\n");
				rotina_ordenacao(op);
				break;

			case 3:
				limpa_tela();
				printf("======= Modulo de Listagem\n");
				rotina_ordenacao(op);
				break;

			case 4:
				if(rotina_abertura("======= Modulo de Listagem"))
				{
					return;
				}

				limpa_tela();
				break;

			case 5:
				return;

			default:
				limpa_tela();
				printf("<- Opcao Invalida\n");

		}
	}
}

// *************************************************************************

int rotina_abertura(char *CABECALHO)
{
	char nome_arq[100];
	char diretorio[300];
	char op = 'n';
	short int erro = 0;

	if(fp)
	{
		limpa_tela();
		printf("Continuar no arquivo atual [s/n]? ");
		scanf(" %c", &op);

		if(op == 'n' || op == 'N')
		{
			fecha_arq(fp);
		}
	}

	while(op == 'N' || op == 'n')
	{
		limpa_tela();

		if(erro)
		{
			printf("<- Algo deu errado, tente novamente\n");
			erro = 0;
		}

		printf("%s\n", CABECALHO);

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

void rotina_ordenacao(short int op)
{
	long int blocos;
	char catch;
	struct dados *temp = NULL;

	blocos = blocos_struct(fp);
				
	temp = (struct dados*) malloc(
		sizeof (struct dados) * blocos);
	rewind(fp);
	fread(temp, sizeof(struct dados), blocos, fp);

	if(op == 1)
	{
		sort_matricula(temp, fp, blocos);
	}
	else if(op == 2)
	{
		sort_nome(temp, fp, blocos);
	}
	else
	{
		sort_nota(temp, fp, blocos);
	}

	for(int i = 0; i < blocos; i++)
	{
		imprime_dados(&temp[i]);
	}
		
	free(temp);
}