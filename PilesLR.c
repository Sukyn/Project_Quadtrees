
#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                sucre syntaxique               */
/*                                               */
/*************************************************/

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define IS ==
#define then

typedef enum { FALSE, TRUE} bool;

/*************************************************/
/*                                               */
/*          definition type liste                */
/*                                               */
/*************************************************/

typedef struct Bloc
{
    int nombre;
    struct Bloc *suivant;
} Bloc;

typedef Bloc *Liste ;

/*************************************************/
/*                                               */
/*                predeclarations                */
/*                                               */
/*************************************************/

/* initialise une Liste � vide */
void initVide(Liste *L);

/* renvoie 1 si la Liste en parametre est vide, 0 sinon */
bool estVide(Liste l);

/* renvoie le premier element de la Liste en parametre */
int premier(Liste l);

/* renvoie une nouvelle Liste correspondant a celle en parametre, avec l'element x ajoute en haut de la pile */
Liste ajoute(int x, Liste l);

/* modifie la Liste en parametre: x est ajoute comme premier element */
void empile(int x, Liste* L);

/* renvoie une nouvelle Liste correspondant a celle en parametre sans son premier element */
Liste suite(Liste l);

/* modifie la Liste en parametre: le premier element est retire */
void depile(Liste* l);

/* affichage simple en recursif et en iteratif */
void affiche_rec(Liste l);
void affiche_iter(Liste l);

/* longueur en recursif et en iteratif */
int longueur_rec (Liste l);
int longueur_iter (Liste l);

/*  Elimination du dernier element en recursif et en iteratif  */
/*  VD est la sousprocedure utilitaire de la version recursive */
void VD (Liste *L);
void VireDernier_rec (Liste *L);
void VireDernier_iter (Liste *L);


/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/


void initVide( Liste *L)
{
    *L = NULL ;
}

bool estVide(Liste l)
{
    return l == NULL ;
}

int premier(Liste l)
{
    return l->nombre ;
}

Liste ajoute(int x, Liste l)
{
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->nombre = x ;
    tmp->suivant = l ;
    return tmp ;
}

void empile(int x, Liste *L)
{
      *L = ajoute(x,*L) ;
}

Liste suite(Liste l)
{
    return l->suivant ;
}


void depile(Liste *L)
{
    Liste tmp = *L ;
    *L = suite(*L) ;
    free(tmp) ;
}

/*************************************************/
/*                                               */
/*     Affiche, avec les briques de base         */
/*                                               */
/*************************************************/

void affiche_rec(Liste l)
{
    if(estVide(l))
        printf("\n");
    else
    {
        printf("%d ", premier(l));
        affiche_rec(suite(l));
    }
}


void affiche_iter(Liste l)
{
    Liste L2 = l;
    while(!estVide(L2))
    {
        printf("%d ", premier(L2));
        L2 = suite(L2);
    }
    printf("\n");
}

/*************************************************/
/*                                               */
/*     Longueur, sans les briques de base        */
/*                                               */
/*************************************************/

int longueur_rec (Liste l)
{
    if (l == NULL)
         return 0 ;
    else return (1 + longueur_rec(l->suivant)) ;
}


int longueur_iter (Liste l)
{
    Liste P = l;
    int cpt = 0 ;
    while (P ISNOT NULL)
    {   P = P->suivant ;
        cpt++ ;
    }
    return cpt ;
}

/*************************************************/
/*                                               */
/*       VireDernier,                            */
/*               sans les briques de base,       */
/*               ni le "->"                      */
/*                                               */
/*************************************************/

void VD (Liste *L)
          // *L non NULL ie liste non vide
{
     if ( ((**L).suivant) == NULL )
            depile(L) ;   // moralement : depile(& (*L)) ;
     else VD (& ( (**L).suivant )) ;
}

void VireDernier_rec (Liste *L)
{
     if ( (*L) ISNOT NULL )
          VD(L);        // moralement : VD(& (*L)) ;
}

void VireDernier_iter (Liste *L)
{
    if ( (*L) ISNOT NULL)
    {
        while ( ((**L).suivant) ISNOT NULL )
                 L = & ( (**L).suivant ) ;
        free(*L) ;
        *L = NULL ;
     }
}


/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste *L)
{
    if(NOT(estVide(*L)))
    {
        depile(L);
        VideListe(L);
    }

}

/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

void poup (Liste l)
{
        printf("Double Affichage \n") ;
        affiche_rec(l) ;
        affiche_iter(l) ;

        printf("Longueur en double %d %d \n\n",
                           longueur_rec(l),
                           longueur_iter(l)
               ) ;
}

/* Indique si une liste commence par deux éléments identiques
-[@param] l : prend en entrée une liste legal
-return un bool : TRUE si la liste commence par deux éléments identiques
                  FALSE sinon
*/
bool DebutDeuxIdentiques(Liste l){
  if (estVide(l) OR estVide(suite(l))){ // le cas ou la liste comprend moins de 2 éléments
    return FALSE;
  }
  return (premier(l) IS premier(suite(l)));
}

/* Indique si la liste ne contient que des 0
-Prend en entrée une liste l
-return un bool : TRUE si la liste ne contient que des 0
                  FALSE sinon*/
bool QueDesZeros(Liste l){
  if (estVide(l))
    return TRUE;
  else
    return (premier(l) IS 0 AND QueDesZeros(suite(l)));
}

/* Indique si la liste l1 est un sous ensemble de l2
-[@param] l1 : une liste l1
-[@param] l2 : une liste l2
-return un bool : TRUE si la liste l1 est un sous ensemble de la liste l2
                  FALSE sinon
*/
bool SousEnsemble(Liste l1, Liste l2){
  if (estVide(l2))
    return FALSE;
  if (estVide(l1))
    return TRUE;

  if (premier(l1) IS premier(l2)){
    l1=suite(l1);
    l2=suite(l2);
  } else {

    if(premier(l1) < premier(l2))
      return FALSE;
    else
      l2=suite(l2);
  }
  return SousEnsemble(l1,l2);
}

//void Permutations à faire

/* procdédure EliminePositionPaires élimine un élément sur deux : les pointeurs de la liste chainée ne pointent
plus vers le suivant, mais vers le suivant du suivant.
-prend en parametre [in/out] l'adresse de la liste L
-la procédure modifie la liste chainée.
*/
void EliminePositionPaires(Liste *L)
{
  if (NOT estVide(*L) AND NOT estVide(suite(*L))){
    (*L)->suivant = suite(*L)->suivant;
    EliminePositionPaires(&((*L)->suivant));
  }
}

/* procédure Begaye qui modifie la liste en entrée en dédoublant tous les éléments
strictement positifs de la liste et en éliminant tous les autres
-prend en parametre [in/out] l'adresse de la liste L

*/
void Begaye(Liste *L)
{
    if (NOT estVide(*L))
    {

      if (premier(*L) <= 0) {
        depile(L);
        Begaye(L);
      } else {
        empile(premier(*L), L);
        Begaye(&((suite(*L))->suivant));
      }

    }
}



/* fonction Max qui compare deux entiers et retourne ne plus grand
[in] first : un entier
[in] second : un entiers
return un int, le maximum des 2 paramètres.
*/
int Max(int first, int second){
  if (first > second) return first;
  else return second;
}

/* MaxZerosConsecutifsqui prend une liste en argument
et rend le plus grand  nombre de zeros consecutifs de la liste */

/* Itérative */
/*fonction MaxZerosConsecutifsIteratif, version itérative de MaxZerosConsecutifs
rend le plus grand nombre de zeros consécutifs de la liste
[in] L : une liste chainée L
return un entier.
*/
int MaxZerosConsecutifsIteratif(Liste L){
  int max = 0;
  int suite = 0;
  while (NOT estVide(L)){

    if (premier(L) IS 0) suite++; // on incrémente suite si la suite est aussi un zéro.
                                  // le nombre actuel de 0 consécutif dans cette partie de la liste.
    else suite = 0;

    max = Max(max, suite);
  }
  return max;
}

/* récursive avec une sous fonction avec trois arguments in
fonction MZCR rend le plus grand nombre de zeros consécutifs de la liste
MZCR va servie de sous fonction pour MaxZerosConsecutifsRec.
[in] L : une liste chainée L
[in] max : un entier
[in] suite : un entier
return l'entier max qui a été modifié dans la fonction.
*/
int MZCR(Liste L, int max, int suite){
  if (estVide(L)) return max;
  else {
    max = Max(max, suite);

    if (premier(L) IS 0) suite++;
    else suite = 0;

    return MZCR(L, max, suite);
  }
}


/* récursive avec une sous fonction avec trois arguments in
fonction MaxZerosConsecutifsIteratif, version itérative de MaxZerosConsecutifs,
appelle MZCR avec comme premiere valeur de max et suite : max = 0 et suite = 0
[in] L : une liste chainée L
return un entier, return ce que MZCR return à la fin.
*/
int MaxZerosConsecutifsRec(Liste L){
  return MZCR(L, 0, 0);
}


/* récursive avec une sous fonction avec un argument in et deux arguments out */

/* procédure MZCRB qui modifie la valeur de suite et max, max étant le nombre
maximum de zéros consécutifs dans la liste.
[in] L : une liste chainée L
[in/out] *max : un pointeur sur l'adresse de max
[in/out] *suite : un pointeur sur l'adresse de suite
*/
void MZCRB(Liste L, int *max, int *suite){
  if (NOT estVide(L)) {
    *max = Max(*max, *suite);

    if (premier(L) IS 0) (*suite)++;
    else *suite = 0;
    MZCRB(L, max, suite);
  }
}

/*fonction MaxZerosConsecutifsRecBis
[in] L : une liste chainée L
return un entier max, qui a été modifié dans MZCRB
*/
int MaxZerosConsecutifsRecBis(Liste L){
  int max = 0;
  int suite = 0;
  MZCRB(L, &max, &suite);
  return max;
}






/* La fonction EstPalindrome vue en TD.*/



/*procédure EstPalindromeAux qui vérifie si une liste d'entier est un palindrome,
modifie la valeur de *estPalin; Si la liste L est un palindrome,
      *estPalin prend la valeur TRUE,
      sinon, il prend la valeur FALSE.
[in/out] *P : un pointeur sur l'adresse de la liste P
[in] L : une liste d'entier L
[in/out] *estPalin : un pointeur sur l'adresse du bool estPalin

*/
void EstPalindromeAux(Liste *P, Liste L, bool *estPalin){
  if (NOT estVide(L)){
    EstPalindromeAux(P, suite(L), estPalin);
    if (premier(*P) ISNOT premier(L)) *estPalin = FALSE;
    *P = suite(*P);
  }
}


/*fonction EstPalindrome qui détermine si une liste est un palindrome ou non.
[in] L : une liste chainée d'entier L.
return estPalin.
*/
bool EstPalindrome(Liste L)
{
  bool estPalin = TRUE;
  Liste P = L;
  EstPalindromeAux(&P, L, &estPalin);
  return estPalin;
}

/* SommeAvantApres : prend une liste d’entiers en argument et rend vrai ssi il y a un
élément tel que la somme de tous les éléments qui le précèdent est égal à la somme de
tous les éléments qui le suivent
*/



/*Procédure SommeAvantApresAux
[in] L : une liste chainée d'entiers L
[in] cptAvant : un entiers
[in/out] *captApres : un pointeur sur l'adresse de l'entier cptApres
[in/out] *res: 
*/
void SommeAvantApresAux(Liste L, int cptAvant, int *cptApres, bool *res)
{
  if (NOT estVide(L))
  {
    SommeAvantApresAux(suite(L), cptAvant+premier(L), cptApres, res);
    if (cptAvant IS *cptApres){
      *res = TRUE;
    }
    *cptApres += premier(L);
  }
}


bool SommeAvantApres(Liste L)
{
  bool res;
  int cptAvant = 0, cptApres = 0;

  SommeAvantApresAux(L, cptAvant, &cptApres, &res);
  return res;
}
/* --------------- */




int main() {
    Liste l ;

        initVide (&l) ;

             empile(2, &l) ;

             empile(3, &l) ;
             empile(0, &l) ;
             empile(3, &l) ;
             empile(2, &l) ;

    poup(l);
    bool somme = EstPalindrome(l);
    printf("Somme ? = %d\n\n", somme);
    poup(l);

    return 0;
}
