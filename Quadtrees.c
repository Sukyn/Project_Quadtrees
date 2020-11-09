
#include <stdio.h>
#include <stdlib.h>

typedef enum { FALSE, TRUE} bool;

typedef struct bloc_image
  { bool toutnoir ;
  struct bloc_image * fils[4] ;
  } bloc_image ;
typedef bloc_image *image ;

//Ce compteur nous permettra de vérifier qu'on libère bien la mémoire...
int compteur_memoire = 0;


image construit_blanc()
{
  image I = NULL ;
  compteur_memoire++;
  return I ;
}

image construit_noir()
{
  image I = (bloc_image*) malloc(sizeof(bloc_image)) ;
  compteur_memoire++;
  I->toutnoir = TRUE ;
  for (int i = 0; i < 4; i++) {
    I->fils[i] = NULL ;
  }
  return I ;
}

image construit_compose(image i1, image i2, image i3, image i4)
{
  image I = (bloc_image*) malloc(sizeof(bloc_image)) ;
  compteur_memoire++;
  I->toutnoir = FALSE ;
  I->fils[0] = i1 ;
  I->fils[1] = i2 ;
  I->fils[2] = i3 ;
  I->fils[3] = i4 ;
  return I ;
}

void affiche_normal(image I)
{
  if (I == NULL) printf("B") ;
  else {
    if (I->toutnoir) printf("N") ;
    else {
      printf(".") ; //CF. plus bas pour comprendre la notation
      for (int i = 0; i < 4; i++) {
      affiche_normal(I->fils[i]) ;
      }

    }
  }
}
/* Explications sur la notation du point pour Doriane :
Dans le sujet on a l'exemple de .N.BBNBB.N.NNB.NBNNBN,
en fait le point est print avant l'appel récursif, la suite suivante se lirait
plus naturellement par (N (BBNB) B (N(NNB(NBNN))BN))
On doit toujours se retrouver avec 4 fils, donc décomposer en 4 images
-> N est une image à lui seul
-> BBNB est bien une image car 4 fils élémentaires
-> NBNN est une image élémentaire, donc NNB(NBNN) en est une aussi, finalement (N(NNB(NBNN))BN) en est une
En combinant les 4, on obtient bien une image. Le point sert donc à montrer l'appel récursif */

void affiche_prof_aux(image I, int profondeur) // On définit une fonction auxiliaire qui garde en mémoire la profondeur
{
  if (I == NULL) printf("B%d", profondeur) ;
  else {
    if (I->toutnoir) printf("N%d", profondeur) ;
    else {
      printf(".%d", profondeur) ;
      for (int i = 0; i < 4; i++) {
      affiche_prof_aux(I->fils[i], profondeur+1) ; //We go deeper inside
      }

    }
  }
}

void affiche_profondeur(image I)
{
  affiche_prof_aux(I, 0) ;
}

bool est_blanche(image I)
{
  if (I == NULL) return TRUE;
  if (I->toutnoir) return FALSE;
  return (est_blanche(I->fils[0])
       && est_blanche(I->fils[1])
       && est_blanche(I->fils[2])
       && est_blanche(I->fils[3]));

}

bool est_noire(image I)
{
  if (I->toutnoir) return TRUE;
  if (I == NULL) return FALSE;
  return (est_noire(I->fils[0])
       && est_noire(I->fils[1])
       && est_noire(I->fils[2])
       && est_noire(I->fils[3]));
}

image copie(image I)
{
  image I_copie = (bloc_image*) malloc(sizeof(bloc_image)) ; // cd : peut etre m'expliquer cette igne
  if (I == NULL) I_copie = construit_blanc() ; //On construit une nouvelle image blanche
  else {
    if (I->toutnoir) I_copie = construit_noir() ; //On construit une nouvelle image noire
    else
    {
      I_copie->toutnoir = FALSE ;
      for (int i = 0; i < 4; i++) {
        I_copie->fils[i] = copie(I->fils[i]) ; //Il faut appeler sur copie pour créer de nouvelles images
      }
      compteur_memoire++;
    }
  }
  return I_copie ;
}
/* Pourquoi ne pas juste faire I_copie = I ?
Parce que ce sont des pointeurs, et donc ce ne serait pas une copie mais un pointeur vers le même
Il faut donc créer de nouvelles images */

double aire_aux(image I, double cote){
  if (I == NULL) return 0 ;
  if (I->toutnoir) return cote*cote ;
  return (aire_aux(I->fils[0], cote/2) +
          aire_aux(I->fils[1], cote/2) +
          aire_aux(I->fils[2], cote/2) +
          aire_aux(I->fils[3], cote/2)) ;
}


double aire(image I)
{
  if (est_noire(I)) return 1 ; // cd : a quoi sert cette ligne alors que dans aire_aux on a if (I->toutnoir) return cote*cote ;
  return aire_aux(I, 0.5); // cd : Pourquoi ne pas directement appeler aire_aux(I , 1) sans faire la premiere ligne?
}



void rendmemoire(image* I){

  if (*I != NULL && !((*I)->toutnoir)) { // cd : pourquoi la on met * avant I contrairement a precedemment?
    for (int i = 0; i < 4; i++) {
      rendmemoire(&((*I)->fils[i]));
    }
  }
  compteur_memoire--;
  free(*I);

}


//-----------
bool is_divided(image I)
{
  // Est-ce 4 images blanches ?
  if (I->fils[0] == NULL &&
      I->fils[1] == NULL &&
      I->fils[2] == NULL &&
      I->fils[3] == NULL) return TRUE ;
  // Est-ce 4 images noires ?
  if  (((I->fils[0]) != NULL && (I->fils[0])->toutnoir)
    && ((I->fils[1]) != NULL && (I->fils[1])->toutnoir)
    && ((I->fils[2]) != NULL && (I->fils[2])->toutnoir)
    && ((I->fils[3]) != NULL && (I->fils[3])->toutnoir)) return TRUE ;
  // Sinon...
  return FALSE ; // cd : et dans le cas ou fils[0] par exemple est divisé aussi en 4 image noires? La ca ne va que en profondeur 2 c'est ca?
}

void simplifie(image* I){
  if (*I != NULL && !((*I)->toutnoir)) {
    for (int i = 0; i < 4; i++) {
          simplifie(&((*I)->fils[i])) ;
    }
    if (is_divided(*I)) {
        if ((*I)->fils[0] == NULL) {
          for (int i = 0; i < 4; i++) {
                rendmemoire(&((*I)->fils[i])) ;
          }
          (*I) = NULL ;

        }
        else
        {
          for (int i = 0; i < 4; i++) {
                rendmemoire(&((*I)->fils[i])) ;
          }
          (*I)->toutnoir = TRUE ;
        }
    }
  }
}
//------------


bool meme_dessin_aux(image I, image I2)
{

  if (I == NULL) return (I2 == NULL) ;
  if (I->toutnoir) return (I2->toutnoir) ;
  return (meme_dessin_aux(I->fils[0], I2->fils[0])
       && meme_dessin_aux(I->fils[1], I2->fils[1])
       && meme_dessin_aux(I->fils[2], I2->fils[2])
       && meme_dessin_aux(I->fils[3], I2->fils[3]) ) ;
}

bool meme_dessin(image I, image I2)
{
  image I_copie = copie(Image1) ;
  simplifie(&I_copie) ;
  image I2_copie = copie(Image2) ;
  simplifie(&I2_copie) ;
  return meme_dessin_aux(I_copie, I2_copie);
}

void negatif(image* I) {
  if (*I == NULL) {
    rendmemoire(I);
    *I = construit_noir();
  }
  else if ((*I)->toutnoir) {
          rendmemoire(I);
          *I = construit_blanc();
      }
       else {
         for (int i = 0; i < 4; i++) {
         negatif(&((*I)->fils[i]));
         }
       }
}

void arrondit_elementaire(image *I) {
  if (((*I) != NULL) && !((*I)->toutnoir)) {
    for (int i = 0; i < 4; i++) {
          arrondit_elementaire(&((*I)->fils[i])) ;
    }
    int compte_blanc = 0;
    for (int i = 0; i < 4; i++) {
      if (((*I)->fils[i]) == NULL) compte_blanc++;
    }
    if (compte_blanc >= 2) {
      for (int i = 0; i < 4; i++) {
            rendmemoire(&((*I)->fils[i])) ;
      }
      (*I) = NULL ;
    } else {
      for (int i = 0; i < 4; i++) {
            rendmemoire(&((*I)->fils[i])) ;
      }
      (*I)->toutnoir = TRUE ;
    }
  }
}



void arrondit_aux(image* I, int k, int n) { // cd : peut etre m'expliquer un peu plus celle ci
  if (k <= n) {
    arrondit_elementaire(I);
  }
  else {
    if (((*I) != NULL) && !((*I)->toutnoir)) {
      for (int i = 0; i < 4; i++) {
      arrondit_aux(&((*I)->fils[i]), k, n+1);
      }
    }
  }
}

void arrondit(image* I, int k) {
  arrondit_aux(I, k, 0);
}








int main() {

  //printf("\n - Memory - %d\n", compteur_memoire);
  image Image1 = construit_compose(construit_noir(),
                                   construit_blanc(),
                                   construit_noir(),
                                   construit_compose(construit_blanc(),
                                                     construit_blanc(),
                                                     construit_blanc(),
                                                     construit_compose(construit_noir(),
                                                                       construit_noir(),
                                                                       construit_noir(),
                                                                       construit_noir()))) ;
  //  printf("\n - Memory - %d\n", compteur_memoire);
  image Image2 = construit_compose(construit_compose(construit_noir(),
                                                     construit_noir(),
                                                     construit_noir(),
                                                     construit_compose(construit_noir(),
                                                                       construit_noir(),
                                                                       construit_noir(),
                                                                       construit_compose(construit_noir(),
                                                                                         construit_noir(),
                                                                                         construit_noir(),
                                                                                         construit_noir()))),
                                    construit_blanc(),
                                    construit_noir(),
                                    construit_compose(construit_blanc(),
                                                      construit_compose(construit_blanc(),
                                                                        construit_blanc(),
                                                                        construit_blanc(),
                                                                        construit_blanc()),
                                                      construit_blanc(),
                                                      construit_noir())) ;
  /*
  printf("\n - Memory - %d\n", compteur_memoire);
  printf("Image 1 AVANT : \n");
  affiche_profondeur(Image1) ;
  printf("\nImage 2 AVANT : \n");
  affiche_profondeur(Image2) ;
  printf("\n");
  */
  image I_copie = copie(Image1) ;
  /*
  printf("\n - Memory - %d\n", compteur_memoire);
  simplifie(&I_copie) ;
  printf("\n - Memory - %d\n", compteur_memoire);
  */
  image I2_copie = copie(Image2) ;
  /*
  printf("\n - Memory - %d\n", compteur_memoire);
  simplifie(&I2_copie) ;
  printf("\n - Memory - %d\n", compteur_memoire);

  printf("Image 1 APRES : \n");
  affiche_profondeur(I_copie) ;
  printf("\nImage 2 APRES : \n");
  affiche_profondeur(I2_copie) ;
  printf("\n") ;

  bool val = meme_dessin(Image1, Image2);

  printf("Meme dessin ? : %d", val) ;
  printf("\n - Memory - %d\n", compteur_memoire);
  */



  /*
  printf("\n\n NEGATIF \n");
  affiche_normal(I_copie);
  printf("\n");
  negatif(&I_copie);
  affiche_normal(I_copie);
  printf("\n - Memory - %d\n", compteur_memoire);
  */


  /*
  printf("\n\n RENDMEMOIRE \n");
  affiche_normal(I2_copie);

  printf("\n %d \n", compteur_memoire);

  printf("\n - Memory - %d\n", compteur_memoire);
  rendmemoire(&I2_copie);
  rendmemoire(&I_copie);
  rendmemoire(&Image1);
  rendmemoire(&Image2);
  printf("\n - Memory - %d\n", compteur_memoire);
  */

  /*
  affiche_normal(I2_copie);
  printf("\n");
  arrondit(&I2_copie,2);
  affiche_normal(I2_copie);
  */

}
