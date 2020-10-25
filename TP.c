#include <stdio.h>

//Calculez e en utilisant l’expression e=∑1/n!

long factorielle(int n)
{
  if (n == 0){
    return 1;
  } else {
    return(n * factorielle(n-1));
  }
}

/* precision est la precision demandée */
double e(float precision){
  double v_n = 1.; //v_n avec v_1 = 1/1!
  double v_nplus = 1.5; //v_n+1 avec v_2 = 1/1! + 1/2!
  n = 2;
  while ((v_nplus - v_n) > precision){
    v_n = v_nplus;
    n++;
    v_nplus += 1/factorielle(n);
  }
  return v_n;
}

//Implementez la fonction puissance(x,n),x réel,n entier positif ou nul.
double puissance(double x, unsigned int n){
  double res=1;
  while(n>=1) {
    res*=x;
    n--;
  }
  return(res);
}




void main{
  double precision = 0.000001
  printf("1.1^10 = %f", puissance(1.1, 10));
  printf("1.01^100 = %f", puissance(1.01, 100));
  printf("1.001^1000 = %f", puissance(1.001, 1000));
  printf("e (méthode 1/n!) = %f", e(precision));
  printf("e (méthode (1+1/n)^n)) = %f", puissance(1 + precision, int(1/precision)));

}
