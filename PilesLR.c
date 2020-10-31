
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

/* Indique si une liste commence par deux éléments identiques*/
bool DebutDeuxIdentiques(Liste l){
  if (estVide(l) OR estVide(suite(l))){
    return FALSE;
  }
  return (premier(l) IS premier(suite(l)));
}

/* Indique si la liste ne contient que des 0 */
bool QueDesZeros(Liste l){
  if (estVide(l))
    return TRUE;
  else
    return (premier(l) ISNOT 0 AND QueDesZeros(suite(l)));
}

/* Indique si la liste l1 est un sous ensemble de l2 */
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

/* Elimine un élément sur deux */
void EliminePositionPaires(Liste *L)
{
  if (NOT estVide(*L) AND NOT estVide(suite(*L))){
    (*L)->suivant = suite(*L)->suivant;
    EliminePositionPaires(&((*L)->suivant));
  }
}

/* Begaye qui modifie la liste en entrée en dédoublant tous les éléments
strictement positifs de la liste et en éliminant tous les autres */
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

/* MaxZerosConsecutifsqui prend une liste en argument
et rend le plus grand nombre dezeros  cons ́ecutifs  de  la  liste */

/* La fonction EstPalindromevue en TD. */


/* SommeAvantApres*/
void SommeAvantApresAux(Liste L, int cptAvant, int *cptApres, bool *res)
{
  if (NOT estVide(L))
  {
    SommeAvantApresAux(suite(L), cptAvant+premier(L), cptApres, res);

    if (cptAvant IS *cptApres) *res = TRUE;
    *cptApres += premier(L);
  }
}


bool SommeAvantApres(Liste L)
{
  bool res = FALSE;
  int cptAvant = 0;
  int cptApres = 0;

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
             empile(4, &l) ;
             empile(-2, &l) ;
             empile(7, &l) ;

    poup(l);
    bool somme = SommeAvantApres(l);
    printf("Somme ? = %d\n\n", somme);
    poup(l);

    return 0;
}
