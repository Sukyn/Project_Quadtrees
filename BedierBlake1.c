#include <stdio.h>

#define ISNOT !=
#define NOT !
#define AND &&
#define OR ||
#define then

typedef enum { false, true } bool;

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
  int n = 2;
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


//Implémentez les deux méthodes pour calculer la fonction d’Ackermann.
//regarder pour mettre r en inout
int Ackerman(int m, int n){
	if (m == 0){
		return n+1;
	}
	else{
    int r = 1;
		for(int i; i <= n+1; i++){
			r=r+Ackerman(m-1,r);
		}
    return r;
	}
}


int Acker(int m,int n){
	if (m==0){
		return n+1;
	}
	else{
		if (n == 0){
			return Acker(m-1,1);
		}
		else{
			return Acker(m-1,Acker(m,n-1));
		}
	}
}




//La suite de réels (x n ) n∈N est définie par récurrence: x 0 = 1 puis ∀n ≥ 1, x n = x n−1 + 1/x n−1 .
//On a donc x 0 = 1, x 1 = 2, x 2 = 2.5, x 3 = 2.9, etc.
//Ecrire le pseudo-code de la fonction X qui prend n en argument et rend x n .
//Donner une version itérative et une version récursive (sans utiliser de sous-fonctionnalité).
//Utilisez les deux méthodes pour calculer X 100

//itérative
double X_it(int n){
  int compteur = 0;
  int res = 1;
  while (n>compteur){
    compteur++;
    res = res + 1/res;
  }
  return res;
}

//récursive
double X_rec(int n){
  if (n == 0){
    return 1;
  } else {
    int temp_res = X_rec(n-1);
    return temp_res + 1/temp_res;
  }
}



int main(){
  double precision = 0.000001;
  printf("1.1^10 = %f", puissance(1.1, 10));
  printf("1.01^100 = %f", puissance(1.01, 100));
  printf("1.001^1000 = %f", puissance(1.001, 1000));
  printf("e (méthode 1/n!) = %f", e(precision));
  printf("e (méthode (1+1/n)^n)) = %f", puissance(1 + precision, (int)(1/precision)));
  printf("X100 itératif = %f", X_it(100));
  printf("X100 récursif = %f", X_rec(100));

}
