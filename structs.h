#ifndef STRUCTS_H
#define STRUCTS_H

struct dados{
	unsigned int matricula;
	char nome[100];
	float notasProva[6];
	float notasExerc[6];
	float notaTrab;
	double notaFinal;
	unsigned int faltas;
	char estado[33];
};

#endif