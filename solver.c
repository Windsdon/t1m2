/**
*   1º Trabalho de Métodos Numéricos para Engenharia II
*   Professor Murilo F. Tomé
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
*/
void lagrange(polinomio* p, double pontos[][2]);

int main(int argc, char** argv) {
	polinomio hue;
	double pontos[][2] = {1, 1, -1, 1, 0, 0};

	lagrange(&hue, pontos);

	printf("polinomio: %g %g %g\n", hue.a, hue.b, hue.c);
}

void bhaskara(polinomio* p, double* raizes) {
	double delta, r1, r2;
	delta = pow(p->b, 2) - 4 * p->a * p->c;

	if(delta < 0){
		r1 = -(p->b)/(2 * (p->a));
		r2 = r1;
	}else{
		double raizDelta = sqrt(delta);

		r1 = (-(p->b) + raizDelta)/(2 * (p->a));
		r2 = (-(p->b) - raizDelta)/(2 * (p->a));
	}

	raizes[0] = r1;
	raizes[1] = r2;
}

void lagrange(polinomio* p, double pontos[][2]) {
	int i, j;
	polinomio* ls = malloc(3 * sizeof(polinomio));

	p->a = 0;
	p->b = 0;
	p->c = 0;

	for(i = 0; i < 3; i++){
		//calcular os polinomios de lagrange
		double denominador = 1;

		ls[i].a = 1;
		ls[i].b = 0;
		ls[i].c = 1;

		for(j = 0; j < 3; j++){
			if(i == j){
				continue;
			}

			denominador *= (pontos[i][0] - pontos[j][0]);
			ls[i].b -= pontos[j][0];
			ls[i].c *= pontos[j][0];
		}

		ls[i].a /= denominador;
		ls[i].b /= denominador;
		ls[i].c /= denominador;

		p->a += ls[i].a * pontos[i][1];
		p->b += ls[i].b * pontos[i][1];
		p->b += ls[i].c * pontos[i][1];
	}


}
