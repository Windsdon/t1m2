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

/*
*   A função f(x)
*/
double f(double x);

int main(int argc, char** argv) {
	polinomio p2;
	int i;
	double raizes[2], r1, r2, f1, f2, x3;
	double pontos[][2] = {
		0.1, f(0.1),
		0.2, f(0.2),
		0.3, f(0.3)
	};

	printf("pontos iniciais: (%g,%g), (%g,%g), (%g,%g)\n", 
			pontos[0][0], pontos[0][1],
			pontos[1][0], pontos[1][1],
			pontos[2][0], pontos[2][1]);

	i = 20;

	do {
		lagrange(&p2, pontos);
		bhaskara(&p2, raizes);

		r1 = raizes[0];
		r2 = raizes[1];
		f1 = f(r1);
		f2 = f(r2);

		printf("polinomio: %g %g %g\n", p2.a, p2.b, p2.c);
		printf("raizes: %g %g\n", r1, r2);
		printf("valor de f: %g %g\n", f1, f2);

		if(abs(f1) < abs(f2)){
			x3 = r1;
		}else{
			x3 = r2;
		}

		pontos[0][0] = pontos[1][0];
		pontos[0][1] = pontos[1][1];

		pontos[1][0] = pontos[2][0];
		pontos[1][1] = pontos[2][1];

		pontos[2][0] = x3;
		pontos[2][1] = f(x3);

		printf("novos pontos: (%g,%g), (%g,%g), (%g,%g)\n", 
			pontos[0][0], pontos[0][1],
			pontos[1][0], pontos[1][1],
			pontos[2][0], pontos[2][1]
		);

		i--;
	}while(pontos[2][1] > 1e-10);
}

double f(double x){
	return (x - 1) * (x - 2) * (x - 3) * (x - 4);
	return (7 - x)*(-pow(x,3) + 9 * pow(x, 2) - 18 * x + 6) -27*(pow(x, 2) - 4*x + 2);
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
		p->c += ls[i].c * pontos[i][1];
	}


}
