
#include <stdio.h> // printf, ...
#include <stdlib.h> // malloc, free, ...
#include <math.h> // pow, ...

/* Sucre syntaxique */
typedef enum { FALSE, TRUE} bool;


/*
------------------------
Définition de la structure de nos images :
- Une image blanche est modélisée par un pointeur NULL
- Une image noire est noire si son champ toutnoir est vrai
- Une image qui est ni noire ni blanche possède 4 fils
------------------------
*/
typedef struct bloc_image
  { bool toutnoir ;
  struct bloc_image * fils[4] ;
  } bloc_image ;
typedef bloc_image *image ;

/*
Ce compteur nous permettra de vérifier qu'on libère bien la mémoire...
Il n'est pas nécessaire mais est pratique pour vérifier que notre code est propre.
*/
int compteur_memoire = 0;



/* ----------------------------------------------------------------------


                            CONSTRUCTEURS


---------------------------------------------------------------------- */


/* Fonction qui crée une nouvelle image blanche
@param : Aucun
@return : Une nouvelle image blanche
*/
image construit_blanc()
{
  image I = NULL ;
  compteur_memoire++;
  /* En réalité, on n'utilise pas vraiment la mémoire dans ce cas-ci,
  puisque le pointeur ne pointe sur rien.
  Il faudra enlever cette incrémentation à la fin, MAIS il faut d'abord
  faire en sorte qu'on n'appelle pas free() sur une image blanche dans les autres fonctions ! */
  return I ;
}


/* Fonction qui crée une nouvelle image noire
@param : Aucun
@return : Une nouvelle image noire
*/
image construit_noir()
{
  image I = (bloc_image*) malloc(sizeof(bloc_image)) ; // On alloue de la mémoire ...
  compteur_memoire++; // On la compte (cette fois-ci on utilise réellement de la mémoire !)
  I->toutnoir = TRUE ;
  for (int i = 0; i < 4; i++) {
    I->fils[i] = NULL ;
  }
  /* Il n'est pas obligatoire d'initialiser les fils à NULL parce que dans les faits si le paramètre
  toutnoir vaut TRUE, on ne cherchera pas à accéder aux fils, mais ça ne coûte rien et ça évite de faire des conneries */
  return I ;
}



/* Fonction qui compose une image à partir de 4 fils
@param : 4 images valides (élémentaires, i.e. noire ou blanche, ou composée)
@return : L'image ayant pour fils les paramètres
*/
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

/* ----------------------------------------------------------------------

                            DESTRUCTEURS

---------------------------------------------------------------------- */

/* Procédure qui permet de libérer la mémoire des images dont on n'a plus l'utilité
@param : L'image a libérer
@return : Aucun
*/
void rendmemoire(image* I){

  if ((*I != NULL) && !((*I)->toutnoir)) {
    for (int i = 0; i < 4; i++) {
      rendmemoire(&((*I)->fils[i]));
    }
  }
  compteur_memoire--;
  free(*I);
}






/* ----------------------------------------------------------------------


              FONCTIONS SUPPLEMENTAIRES / UTILITAIRES


---------------------------------------------------------------------- */

/* Fonction qui cherche la profondeur maximale d'une image
@param L'image dont on veut connaître la profondeur
@return La profondeur maximale de l'image
*/
/* Fonction auxiliaire */
int donne_profondeur_max_aux(image I, int profondeur){
  int max = 0;
  if ((I == NULL) || I->toutnoir) return profondeur;
  else {
      for (int i = 0; i < 4; i++) {
        if (donne_profondeur_max_aux(I->fils[i], profondeur+1) > max) max = resultat;
      }
  }
  return max;
}
/* Fonction principale */
int donne_profondeur_max(image I){ return donne_profondeur_max_aux(I, 1); }


/* Fonction identique à construit_compose mais qui construit dans le sens inverse
@param : Les 4 images à composer
@return : L'image composée en sens inverse
*/
image construit_compose_retourne(image i1, image i2, image i3, image i4) {
  image I = (bloc_image*) malloc(sizeof(bloc_image)) ;
  compteur_memoire++;
  I->toutnoir = FALSE ;
  I->fils[0] = i4 ;
  I->fils[1] = i3 ;
  I->fils[2] = i2 ;
  I->fils[3] = i1 ;
  return I ;
}



/* Fonction qui construit une image à partir d'un tableau de caractère
@param La chaîne de caractère
@return L'image décrite par cette chaîne
*/
/* Fonction auxiliaure */
image image_from_tabchar_aux(char image[], int indice, int* shift){
  if (image[indice+(*shift)] == 'N') return construit_noir();
  if (image[indice+(*shift)] == 'B') return construit_blanc();
  if (image[indice+(*shift)] == '.') {
    (*shift) += 4;
    return construit_compose_retourne( image_from_tabchar_aux(image, indice, shift),
                                 image_from_tabchar_aux(image, indice - 1, shift),
                                 image_from_tabchar_aux(image, indice - 2, shift),
                                 image_from_tabchar_aux(image, indice - 3, shift));
  }
  char s = image[indice+(*shift)];
  printf("La valeur entrée suivante n'est pas valide :");
  putchar(s);
  printf("\n");
  return construit_noir(); // Si un caractère n'est pas valide, on part du principe que ça ne sert pas d'essayer de réparer l'erreur.
}
/* Fonction principale */
image tabdechar_to_image(char phrase[]){
  char image1[256];
  int i = 0;
  // On enlève le retour chariot
  while(phrase[i] != '\n'){
    image1[i] = phrase[i];
    i++;
  }
  int shift=0;
  return image_from_tabchar_aux(image1, 0, &shift);
}

/* Fonction qui renvoie une image où tous les fils de l'image entrée sont à la même profondeur,
En fait, c'est l'inverse de la fonction arrondit() : Si on a une image élémentaire moins profond qu'une autre, on lui crée
artificiellement des fils qui lui sont identiques.
@param : L'image que l'on souhaite diviser
@return : L'image où tous les fils sont à la même profondeur
*/
/* Fonction auxiliaire */
image Division_aux (image I, int profondeur){
  if(est_blanche(I) || est_noire(I)){
    if(profondeur == 0) {
      if (est_blanche(I)) return construit_blanc();
                     else return construit_noir();
    }
    else return construit_compose(Division_aux(I,profondeur-1),
                                  Division_aux(I,profondeur-1),
                                  Division_aux(I,profondeur-1),
                                  Division_aux(I,profondeur-1));
  }
  profondeur--;
  return construit_compose(Division_aux(I->fils[0], profondeur),
                           Division_aux(I->fils[1], profondeur),
                           Division_aux(I->fils[2], profondeur),
                           Division_aux(I->fils[3], profondeur));
}
/* Fonction principale */
image Division (image I){
  // la profondeur a laquelle il faudra diviser tout les carrés qui ne le sont pas deja
  Division_aux(I, donne_profondeur_max(I));
}

/* Procédure qui nous permet de transformer une image en tableau de caractères
@param : L'image que l'on souhaite transcrire
@return : La chaîne de caractère associée
*/
/* Procédure auxiliaire */
void image_divise_to_char_aux(image I, int* i, char(* imageI)[]){
  if(I == NULL){
    (*imageI)[*i]='B';
    (*i)++;
  } else if (I->toutnoir) {
      (*imageI)[*i]='N';
      (*i)++;
  } else{
      for(int j=0; j<4; j++){
        image_divise_to_char_aux(I->fils[j], i, imageI);
      }
  }
}
/* Procédure principale */
void image_divise_to_char(image I, char(* imageI)[]){
  int i = 0;
  image_divise_to_char_aux(Division(I), &i, imageI);
}






/* ----------------------------------------------------------------------


                            AFFICHAGE


---------------------------------------------------------------------- */



/* Fonction qui affiche une image selon la notation [.,N,B]
@param : L'image que l'on souhaite afficher
@return : Aucun
*/
void affiche_normal(image I)
{
  if (I == NULL) printf("B") ;
  else if (I->toutnoir) printf("N") ;
  else {
    printf(".") ; //CF. plus bas pour comprendre la notation
    for (int i = 0; i < 4; i++) {
      affiche_normal(I->fils[i]) ;
    }
  }
}
/* Explications sur la notation du "point" :
Dans le sujet on a l'exemple de .N.BBNBB.N.NNB.NBNNBN,
en fait le point est print avant l'appel récursif, la suite suivante se lirait
plus naturellement par (N (BBNB) B (N(NNB(NBNN))BN))
On doit toujours se retrouver avec 4 fils, donc décomposer en 4 images
-> N est une image à lui seul
-> BBNB est bien une image car 4 fils élémentaires
-> NBNN est une image ayant des fils élémentaires, donc NNB(NBNN) est aussi une image, finalement (N(NNB(NBNN))BN) en est une
En combinant les 4, on obtient bien une image. Le point sert donc à montrer l'appel récursif */



/* Fonction qui affiche l'image en précisant la profondeur de chaque noeud
@param : L'image à afficher
@return : Aucun
*/

/* Fonction auxiliaire */
void affiche_prof_aux(image I, int profondeur) // On définit une fonction auxiliaire qui garde en mémoire la profondeur
{
  if (I == NULL) printf("B%d", profondeur) ;
  else if (I->toutnoir) printf("N%d", profondeur) ;
  else {
    printf(".%d", profondeur) ;
    for (int i = 0; i < 4; i++) {
      affiche_prof_aux(I->fils[i], profondeur+1) ;
    }
  }
}
/* Fonction principale */
void affiche_profondeur(image I) { affiche_prof_aux(I, 0) ; }


/* Procédure qui permet d'afficher une image en 2D
@param : L'image à afficher
@return : Aucun
*/
/* A FINIR */
/*
void affichage2kpixel(image image1){
  int profondeur = donne_profondeur_max(image1);
  int cases = pow(2,2*profondeur);
  char I[cases];
  image_divise_to_char(image1, &I);

  int debut = 0;
  int ligne = pow(2,profondeur)/2;
  char signe = '$';
  int indice = 0;

  for(int t = 0; t< ligne; t +=ligne ){ // ligne par ligne
    for(int i = 1; i<=ligne; i++){
      for(int j = 0; j< 2; j++){
        indice = pow(2, i+t)+j-2;
        printf("t=%d   ",t );
        printf("i=%d   ",i );
        printf("j=%d \n",j );
        printf("indice = %d\n", indice);

      }
      printf("\n");
    }
    for(int i = 0; i<ligne; i++){
      for(int j=2; j<4; j++){
        indice = pow(2, i+t)+j-1;
        printf("t=%d   ",t );
        printf("i=%d   ",i );
        printf("j=%d \n",j );
        printf("indice = %d\n", indice);
      }
    }
    printf("\n");
  }
}
*/

/*
if(est_blanche(image1)){
  printf(".");
}
else{
  printf("8");
}

*/

/* ----------------------------------------------------------------------


                    FONCTIONS DEMANDEES


---------------------------------------------------------------------- */

/* Fonction qui vérifie si une image est toute blanche
@param L'image que l'on veut tester
@return TRUE si l'image est toute blanche
*/
bool est_blanche(image I)
{
  if (I == NULL) return TRUE;
  if (I->toutnoir) return FALSE;
  return (est_blanche(I->fils[0])
       && est_blanche(I->fils[1])
       && est_blanche(I->fils[2])
       && est_blanche(I->fils[3]));
}

/* Fonction qui vérifie si une image est toute noire
@param L'image que l'on veut tester
@return TRUE si l'image est toute noire
*/
bool est_noire(image I)
{
  if (I == NULL) return FALSE;
  if (I->toutnoir) return TRUE;
  return (est_noire(I->fils[0])
       && est_noire(I->fils[1])
       && est_noire(I->fils[2])
       && est_noire(I->fils[3]));
}



/* Fonction qui copie une image dans un nouvel emplacement mémoire
@param L'image à copier
@return Une copie de l'image, que l'on peut modifier sans modifier l'originale
*/
image copie(image I)
{
  image I_copie = (bloc_image*) malloc(sizeof(bloc_image)) ;
  if (I == NULL) I_copie = construit_blanc() ;
  else if (I->toutnoir) I_copie = construit_noir() ;
  else I_copie = construit_compose(copie(I->fils[0]),
                                   copie(I->fils[1]),
                                   copie(I->fils[2]),
                                   copie(I->fils[3])) ;
  return I_copie ;
}
/* Pourquoi ne pas juste faire I_copie = I ?
Parce que ce sont des pointeurs, et donc ce ne serait pas une copie mais un pointeur vers le même
Il faut donc créer de nouvelles images */




/* Fonction qui calcule l'aire d'une image
Note : On considère que l'aire de l'image est la surface de ses carrés noirs, en partant du principe que l'image
de base est représentée par un carré de taille 1x1
@param : L'image dont on cherche l'aire
@return : Un flottant qui représente cette aire
*/
/* Fonction auxiliaire */
double aire_aux(image I, double cote){
  if (I == NULL) return 0 ;
  if (I->toutnoir) return cote*cote ;
  return (aire_aux(I->fils[0], cote/2) +
          aire_aux(I->fils[1], cote/2) +
          aire_aux(I->fils[2], cote/2) +
          aire_aux(I->fils[3], cote/2)) ;
}
/* Fonction principale */
double aire(image I)
{
  if (est_noire(I)) return 1 ;
  return aire_aux(I, 0.5);
}


/* Procédure qui simplifie une image :
C'est à dire que si tous les enfants d'une image sont de la même couleur, on considère qu'il n'est pas utile
d'avoir des fils et on les remplace par un élément simple (noir ou blanc)
@param : L'image que l'on souhaite simplifier
@return : Aucun
*/
void simplifie(image* I){
  if (*I != NULL && !((*I)->toutnoir)) {
    for (int i = 0; i < 4; i++) {
          simplifie(&((*I)->fils[i])) ;
    }
    if (est_blanche(*I)) { // En fait on regarde ici si les enfants sont unis
          rendmemoire(I);
          (*I) = construit_blanc() ;
    } else if (est_noire(*I)) {
          rendmemoire(I);
          (*I) = construit_noir();
    }
  }
}


/* Fonction qui vérifie si deux images représentent la même chose
@param : Les deux images que l'on souhaite comparer
@return : TRUE si les deux images sont identiques
*/
/* Fonction auxiliaire */
bool meme_dessin_aux(image I, image I2){

       if (I  == NULL)     return (I2 == NULL) ;
  else if (I  -> toutnoir) return (I2 -> toutnoir) ;
  else if (I2 == NULL)     return (I  == NULL);
  else if (I2 -> toutnoir) return (I  -> toutnoir);

  return (meme_dessin_aux(I->fils[0], I2->fils[0])
       && meme_dessin_aux(I->fils[1], I2->fils[1])
       && meme_dessin_aux(I->fils[2], I2->fils[2])
       && meme_dessin_aux(I->fils[3], I2->fils[3]) ) ;
}
/* Fonction principale */
bool meme_dessin(image I, image I2)
{
  /* Il est plus simple de simplifier au maximum nos images, on sait ainsi que si l'on tombe
  sur un élément blanc ou noir, c'est qu'il est à la profondeur minimale telle qu'il représente l'image,
  on peut donc comparer plus facilement les images (mais de manière moins optimisée...) */
  image I_copie = copie(I) ;
  simplifie(&I_copie) ;
  image I2_copie = copie(I2) ;
  simplifie(&I2_copie) ;
  return meme_dessin_aux(I_copie, I2_copie);
}




/* Procédure qui transforme une image en sa forme négative,
c'est à dire que les cases blanches deviennent noires et réciproquement.
@param : L'image que l'on souhaite inverser
@return : Aucun */
void negatif(image* I) {
  if (*I == NULL) {
    rendmemoire(I);
    *I = construit_noir();
  }
  else if ((*I)->toutnoir) {
    rendmemoire(I);
    *I = construit_blanc();
  } else {
    rendmemoire(I);
    *I =  construit_compose((*I)->fils[0],
                            (*I)->fils[1],
                            (*I)->fils[2],
                            (*I)->fils[3]))
  }
}


/* Procédure qui arrondit les fils à une profondeur demandée d'une image
@param : L'image que l'on souhaite arrondir, la profondeur a partir de laquelle on souhaite arrondir
@return : Aucun
*/

/* Fonction auxiliaire */
void arrondit_elementaire(image *I) {
  if (((*I) != NULL) && !((*I)->toutnoir)) {
    for (int i = 0; i < 4; i++) {
          arrondit_elementaire(&((*I)->fils[i])) ;
    }
    int compte_blanc = 0; // Cette variable permet de savoir si l'on doit arrondir en blanc ou en noir (la majorité l'emporte)
    for (int i = 0; i < 4; i++) {
      if (((*I)->fils[i]) == NULL) compte_blanc++;
    }
    if (compte_blanc >= 2) {
      rendmemoire(I);
      (*I) = NULL ;
    } else {
      rendmemoire(I);
      (*I)->toutnoir = TRUE ;
    }
  }
}
/* Fonction auxiliaire */
void arrondit_aux(image* I, int k, int n) {
  if (k <= n) arrondit_elementaire(I); // Si l'on est à la profondeur recherchée, on arrondit notre image en un élément (noir ou blanc)
  else if (((*I) != NULL) && !((*I)->toutnoir)) {
      for (int i = 0; i < 4; i++) {
        arrondit_aux(&((*I)->fils[i]), k, n+1);
      }
  }
}
/* Fonction principale */
void arrondit(image* I, int k) {
  arrondit_aux(I, k, 0);
}


/* Fonction qui renvoie une image représentant la différence entre deux images :
L'image rendue est noire là où l'une des deux images de départ est noire mais l'autre blanche
@param Les deux images que l'on compare
@return L'image de la différence
*/
void difference (image I1, image I2){
  // --- On travaille sur des images qui n'ont pas de fils identiques ---
  simplifie(&I1);
  simplifie(&I2);
  // ---

  // Si les deux images sont identiques, il n'y a aucune différence donc on renvoie une imge blanche
  if(meme_dessin(I1,I2)){
    return construit_blanc();
  }

  // Si les deux images sont unies mais opposées, on renvoie une image noire
  else if ((I1 == NULL && I2 -> toutnoir) || (I1->toutnoir && I2 == NULL))
    return = construit_noir();

  // Si la première image est unie mais pas la seconde (si la seconde était unie on serait rentrés dans un cas précédent)
  else if (I1 == NULL || I1 -> toutnoir){
          /* Si la première est blanche, la différence correspond simplement à l'autre image :
             En effet, si le fils est blanc alors la différence est blanche car ils sont identiques,
             si le fils est noir alors la différence est noire car les deux sont opposés */
          if (I1 == NULL) return I2;
          // Si la première est noire, la différence correspond à l'opposé de l'autre image par un raisonnement analogue
          else {
            image dif = copie(I2);
            negatif(&dif);
            return dif
          }
  }
  // Si la seconde image est unie mais pas la première
  else if(I2 == NULL || I2 -> toutnoir){
          // cf avant.
          if (I2 == NULL) return = I1;
          // cf avant.
          else{
            image dif = copie(I2);
            negatif(&dif);
            return dif
          }
      }

  // On gère désormais le cas où ni l'une ni l'autre n'est uni
  else {
        for (int i = 0; i<4 ; i++){
          difference(I1 -> fils[i], I2 -> fils[i]);
        }
  }
}




/* Fonction qui permet à l'utilisateur de rentrer une image depuis le terminal
@param : Aucun
@return : L'image construite à partir des indications de l'utilisateur
*/
// La fonction fait deux passes, ce qui est à l'origine du shift, on peut bien
// la simplifier en ne faisant qu'une passe
image lecture_au_clavier(){
  char image1[256];
  char flag = 's';
  int i = 0;
  while (flag != '\n'){
    flag = getchar();
    image1[i] = flag;
    i++;
  }
  int shift = 0;
  return image_from_tabchar_aux(image1, 0, &shift);
}


/* Fonction qui compte le nombre de sous image pleine à une profondeur donnée
@param : L'image que l'on examine, la hauteur qui nous intéresse
@return : Le nombre de sous images pleines
*/
/* A FINIR */
/*
void CompteSousImagePleine(image I, int hauteur, int* cpt, bool* is_full, int profondeur){
  if (hauteur == profondeur) {
      if (!(est_blanche(I) || est_noire(I))) CompteSousImagePleine(I, hauteur, cpt, is_full, profondeur-1) ;
      else {
         *(is_full) = TRUE ;
         if (hauteur == 0) (*cpt)++;
      }

  }
  else if (est_blanche(I) || est_noire(I)) *(is_full) = FALSE ;
  else {
    for(int i = 0; i < 4; i++) {
      CompteSousImagePleine(I->fils[i], hauteur, cpt, is_full, profondeur+1) ;
    }
  }

  if (*(is_full) && hauteur != 0 && profondeur == 0) {
    (*cpt)++;
    printf("incr. ici, h = %d, p = %d\n", hauteur, profondeur);
    *(is_full) = FALSE ;
  }

  affiche_normal(I);
  printf(" cpt = %d, p = %d\n ", *cpt, profondeur) ;
}
*/
















/* ----------------------------------------------------------------------


                                MAIN


---------------------------------------------------------------------- */





int main() {

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
                                                                        construit_noir(),
                                                                        construit_blanc(),
                                                                        construit_blanc()),
                                                      construit_blanc(),
                                                      construit_noir())) ;

  image Image3 = construit_compose(construit_noir(),
                                   construit_blanc(),
                                   construit_noir(),
                                   construit_noir());
  image Image4 = construit_noir();
  image I_copie = copie(Image1) ;
  image I2_copie = copie(Image2) ;

            //                      1   2    3    4        5    6    7    8        9   10  11   12       13  14  15   16   17      18  19  20      21  22  23  24      25  26  27          28   29  30  31     32  33  34  35      36  37  38  39      40  41  42  43        N     . N   B   N     . B   B     N   B  .  B   N     B  .  .   B   B   N     B   . N     B   B   N   .   B N   B   N   .     N   B   N   B
  char phrase[58] = {'.','.','.','B','B', 'N', 'B','.', 'N', 'N', 'B', 'N', '.','B','B','B', 'N', '.','N','N','N', 'B', 'N','.','N','B','N','.','B','B','N','B','.','B','N','B','.','.','B','B','N','B','.','N','B','B','N','.','B','N','B','N','.','N','B','N','B', '\n'};

  affiche_normal(tabdechar_to_image(phrase));
  printf(" est phrase\n" );
  for (int j = 0; j < 5; j++) {
    int i = 0;
    bool b = TRUE;
    CompteSousImagePleine(tabdechar_to_image(phrase), j, &i, &b, -1);
    printf("Valeur = %d, Compteur = %d\n", j, i);
  }
  // 0 = 43 ok
  // 1 = 9 ok
  // 2 = 3 (devrait etre 2)
  // 3 = 1 (devrait etre 0)
  // 4 ou + = 0 ok


  //affiche_normal(tabdechar_to_image(phrase));
  //printf(" est phrase\n" );
  //int i = 0;
  //bool b = TRUE;
  //CompteSousImagePleine(tabdechar_to_image(phrase), 1, &i, &b, 0);
  //printf("Compteur = %d", i);

  //affichage2kpixel(tabdechar_to_image(phrase));

  //image_divise_to_char(Image1);
  //image_divise_to_char(Image1);

  //printf("%d\n", donne_profondeur_max(Image1) );
  //printf("%d\n", donne_profondeur_max(Image2) );
//  affiche_normal(Division(Image1));
//  printf("\n");
  //affichage2kpixel(Image1);
}
