/**
*   1º Trabalho de Métodos Numéricos para Engenharia II
*   Professor: Murilo F. Tomé
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
*   Polinômio da forma P(x) = ax² + bx + c
*/
typedef struct {
	double a;
	double b;
	double c;
} polinomio;

/*
*   Calcula as raízes do polinômio de segundo grau e
*   retorna a parte real das mesmas
*/
void bhaskara(polinomio* p, double* raizes);

/*
*   Cria o polinômio de lagrange para os pontos  
*   Retorna 1 em caso de falha
*/
int lagrange(polinomio* p, double pontos[][2]);

/*
*   A função f(x)
*/
double f(double x);

/*
*   Aplica o método para a função fn, com os três pontos iniciais
*   Retorna 1 em caso de falha
*/
double solve(double (*fn)(double), double inicial[3]);

FILE *file; // para criar arquivo de saída

int main(int argc, char** argv) {
	double pontos[3] = {-2, -1, 0}; // pontos iniciais
	double raizes[4] = {0, 0, 0, 0}; // vetor para salvar as raízes
	int numRaizes= 0; // contador de raízes encontradas
	double r;
	int i, j;
	char raizEncontrada;
	char message[100]; // para salvar no arquivo

	file = fopen("out.txt", "w");

	for(i = 0; i < 12; i++){
		r = solve(f, pontos); // econtrar a raiz

		raizEncontrada = 0;

		//procurar nas raízes já encontradas
		for(j = 0; j < numRaizes; j++){
			if(abs(raizes[j] - r) < 1e-10){
				raizEncontrada = 1;
				break;
			}
		}

		if(!raizEncontrada){
			raizes[j] = r;
			numRaizes++;
		}

		for(j = 0; j < 3; j++){
			//avançar pontos iniciais
			pontos[j] += 1;
		}
	}

	printf("\n\nRAIZES:\n%.11f\n%.11f\n%.11f\n%.11f\n", 
		raizes[0], raizes[1], raizes[2], raizes[3]);

	sprintf(message, "########################\nRAIZES\n%.10f\n%.10f\n%.10f\n%.10f\n\n", 
		raizes[0], raizes[1], raizes[2], raizes[3]);
	fputs(message, file);

	fclose(file);

	return 0;	
}

double f(double x){
	return (7 - x)*(-pow(x,3) + 9 * pow(x, 2) - 18 * x + 6) -27*(pow(x, 2) - 4*x + 2);
}

double solve(double (*fn)(double), double inicial[3]){
	polinomio p2; // alocar o polinômio intrpolador
	int i, iter;
	double raizes[2], r1, r2, f1, f2, f3, x3, e;

	char message[100]; //para salvar no log

	double pontos[3][2];

	for(i = 0; i < 3; i++){
		pontos[i][0] = inicial[i];
		pontos[i][1] = fn(inicial[i]);
	}

	iter = 0;

	fputs("************************\n", file);

	sprintf(message, "Pontos iniciais: %f %f %f\n", inicial[0], inicial[1], inicial[2]);
	fputs(message, file);
	fputs("iter\terro\t\tx3\n", file);

	do {
		if(lagrange(&p2, pontos)){
			sprintf(message, "Falha ao criar o polinomio de lagrange! \n**Abortado**\n");
			fputs(message, file);
			return NAN;
		}

		bhaskara(&p2, raizes);

		r1 = raizes[0];
		r2 = raizes[1];
		f1 = fn(r1);
		f2 = fn(r2);

		if(abs(f1) < abs(f2)){
			x3 = r1;
		}else{
			x3 = r2;
		}

		f3 = fn(x3);

		for(i = 0; i < 2; i++){
			pontos[i][0] = pontos[i+1][0];
			pontos[i][1] = pontos[i+1][1];
		}

		pontos[i][0] = x3;
		pontos[i][1] = f3;

		e = fabs(pontos[2][1]); // cálculo do erro

		sprintf(message, "%d\t%-15g\t%g\n", iter, e, x3);

		fputs(message, file);

		iter++;

	}while(e > 1e-10);

	printf("\nCONVERGIU EM %d ITERACOES\nPONTO: %.11f\n", iter, x3);

	sprintf(message, "CONVERGIU\nPonto: %.11f\nErro: %g\nIteracoes: %u\n\n", x3, e, iter);
	fputs(message, file);


	return x3;
}

void bhaskara(polinomio* p, double* raizes) {
	double delta, r1, r2;
	delta = pow(p->b, 2) - 4 * p->a * p->c;

	if(delta < 0){
		r2 = r1 = -(p->b)/(2 * (p->a));
	}else{
		double raizDelta = sqrt(delta);

		r1 = (-(p->b) + raizDelta)/(2 * (p->a));
		r2 = (-(p->b) - raizDelta)/(2 * (p->a));
	}

	raizes[0] = r1;
	raizes[1] = r2;
}

int lagrange(polinomio* p, double pontos[][2]) {
	int i, j;

	double b, c;

	p->a = 0;
	p->b = 0;
	p->c = 0;

	for(i = 0; i < 3; i++){
		//calcular os polinomios de lagrange
		double denominador = 1;

		b = 0;
		c = 1;

		for(j = 0; j < 3; j++){
			if(i == j){
				continue;
			}

			denominador *= (pontos[i][0] - pontos[j][0]);
			b -= pontos[j][0];
			c *= pontos[j][0];
		}

		if(!denominador){
			//Divisão por zero
			return 1;
		}

		p->a += pontos[i][1] / denominador;
		p->b += b * pontos[i][1] / denominador;
		p->c += c * pontos[i][1] / denominador;
	}

	return 0;
}