
#include <stdio.h> // printf, ...
#include <stdlib.h> // malloc, free, ...
#include <math.h> // pow, ...
#include <time.h>
#include <assert.h>

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
*/ // test OK
image construit_blanc()
{
  compteur_memoire++;
  /* En réalité, on n'utilise pas vraiment la mémoire dans ce cas-ci,
  puisque le pointeur ne pointe sur rien.
  Il faudra enlever cette incrémentation à la fin, MAIS il faut d'abord
  faire en sorte qu'on n'appelle pas free() sur une image blanche dans les autres fonctions ! */
  return NULL ;
}

/* Fonction qui crée une nouvelle image noire
@param : Aucun
@return : Une nouvelle image noire
*/  // test OK
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
image construit_compose(image i1, image i2, image i3, image i4)  // Test OK
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


                            BRIQUES DE BASE


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

/*
void testConstruitBlanc(){
  bool estBlanc = est_blanche(construit_blanc)
  assert(estBlanc);
}
void testConstruitNoir(){
  assert(est_noire(construit_noir));
}*/

/* Fonction qui copie une image dans un nouvel emplacement mémoire
@param L'image à copier
@return Une copie de l'image, que l'on peut modifier sans modifier l'originale
*/  // Test ok
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
        int resultat = donne_profondeur_max_aux(I->fils[i], profondeur+1);
        if (resultat > max) max = resultat;
      }
  }
  return max;
}
/* Fonction principale */
int donne_profondeur_max(image I){
  return donne_profondeur_max_aux(I, 0);
}      // Test ok

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
}   // test OK

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
}  // Test OK

/* Procédure qui nous permet de transformer une image en tableau de caractères
@param : L'image que l'on souhaite transcrire
@return : La chaîne de caractère associée
*/
/* Procédure auxiliaire */

void image_divise_to_char_aux(image I, int pos_x, int pos_y, char(* imageI)[], int length, int original){
if(I == NULL){
    (*imageI)[pos_y*original+pos_x]='.';

  } else if (I->toutnoir) {
      (*imageI)[pos_y*original+pos_x]='8';

  } else{
    length = length/2;
      image_divise_to_char_aux(I->fils[0], pos_x, pos_y,                   imageI, length, original);
      image_divise_to_char_aux(I->fils[1], pos_x + length, pos_y,          imageI, length, original);
      image_divise_to_char_aux(I->fils[2], pos_x, pos_y + length,          imageI, length, original);
      image_divise_to_char_aux(I->fils[3], pos_x + length, pos_y + length, imageI, length, original);
  }
}
/* Procédure principale */
void image_divise_to_char(image I, char(* imageI)[], int length){
  image_divise_to_char_aux(Division(I), 0, 0, imageI, length, length);
}

/* Fonction qui construit une image de profondeur de n, toute blanche
@param : n la donne_profondeur
@return : Une image blanche de profondeur n
*/
image construit_image_prof(int n){
  if (n == 0) return construit_blanc();
  else return construit_compose(construit_image_prof(n -1),
                                construit_image_prof(n -1),
                                construit_image_prof(n -1),
                                construit_image_prof(n -1));
}  // Test OK

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
void affichage2kpixel(image image1){
  int profondeur = donne_profondeur_max(image1);
  int length = pow(2, profondeur);
  char I[length*length];
  image_divise_to_char(image1, &I, length);


  for (int i = 0; i < length*length; i++){
    if (i%length == 0) putchar('\n');
    putchar(I[i]);

  }

  printf("\n");
}

/* ----------------------------------------------------------------------


                    FONCTIONS DEMANDEES


---------------------------------------------------------------------- */

/* Fonction qui calcule l'aire d'une image
Note : On considère que l'aire de l'image est la surface de ses carrés noirs, en partant du principe que l'image
de base est représentée par un carré de taille 1x1
@param : L'image dont on cherche l'aire
@return : Un flottant qui représente cette aire
*/
/* Fonction auxiliaire */
double aire_aux(image I, double cote){
  if (est_blanche(I)){
    return 0;
  }
  else{
    if (est_noire(I)){
      return cote*cote;
    }
    else{
      return (aire_aux(I->fils[0], cote/2) +
              aire_aux(I->fils[1], cote/2) +
              aire_aux(I->fils[2], cote/2) +
              aire_aux(I->fils[3], cote/2)) ;
    }
  }
}

/* Fonction principale */
double aire(image I){
  aire_aux(I,1);
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

  if (I == NULL){
    return (I2 == NULL) ;
  }
  else if (I->toutnoir){
      return (I2->toutnoir) ;
    }
  else if (I2==NULL || I2->toutnoir){
      return FALSE;
    }

  return (meme_dessin_aux(I->fils[0], I2->fils[0])
       && meme_dessin_aux(I->fils[1], I2->fils[1])
       && meme_dessin_aux(I->fils[2], I2->fils[2])
       && meme_dessin_aux(I->fils[3], I2->fils[3]) ) ;
}
/* Fonction principale */
bool meme_dessin(image I, image I2) {
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
    for (int i = 0; i < 4; i++) {
      negatif(&((*I)->fils[i]));
    }
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
image difference (image I1a, image I2a){
  // --- On travaille sur des images qui n'ont pas de fils identiques ---
  image I1 = copie(I1a) ;
  simplifie(&I1) ;
  image I2 = copie(I2a) ;
  simplifie(&I2) ;
  // ---

  // Si les deux images sont identiques, il n'y a aucune différence donc on renvoie une imge blanche
  if(meme_dessin(I1,I2)) return construit_blanc();

  // Si les deux images sont unies mais opposées, on renvoie une image noire
  else if ((I1 == NULL && I2 -> toutnoir) || (I1->toutnoir && I2 == NULL)) return construit_noir();

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
            return dif;
          }
  }
  else //aucune des 2 n'est unie.
    return construit_compose(difference(I1 -> fils[0], I2 -> fils[0]),
                             difference(I1 -> fils[1], I2 -> fils[1]),
                             difference(I1 -> fils[2], I2 -> fils[2]),
                             difference(I1 -> fils[3], I2 -> fils[3]));
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
int CompteSousImagePleine(image I, int n) {
  if (I == NULL || I->toutnoir) {
    if (n == 0) return 1;
    else return 0;
  }
  else {
    int v1 = donne_profondeur_max(I->fils[0]);
    if (v1 > n)
        return (CompteSousImagePleine(I->fils[0], n) +
                CompteSousImagePleine(I->fils[1], n) +
                CompteSousImagePleine(I->fils[2], n) +
                CompteSousImagePleine(I->fils[3], n));

    int v2 = donne_profondeur_max(I->fils[1]);
    if (v2 > n)
        return (CompteSousImagePleine(I->fils[0], n) +
                CompteSousImagePleine(I->fils[1], n) +
                CompteSousImagePleine(I->fils[2], n) +
                CompteSousImagePleine(I->fils[3], n));

    int v3 = donne_profondeur_max(I->fils[2]);
    if (v3 > n)
        return (CompteSousImagePleine(I->fils[0], n) +
                CompteSousImagePleine(I->fils[1], n) +
                CompteSousImagePleine(I->fils[2], n) +
                CompteSousImagePleine(I->fils[3], n));

    int v4 = donne_profondeur_max(I->fils[3]);
    if (v4 > n)
        return (CompteSousImagePleine(I->fils[0], n) +
                CompteSousImagePleine(I->fils[1], n) +
                CompteSousImagePleine(I->fils[2], n) +
                CompteSousImagePleine(I->fils[3], n));

    if (v1 == n && v2 == n && v3 == n && v4 == n)
      return 1;


   return 0;
  }
}

/* Fonction qui prend en argument la profondeur k, et un entier n et quir
endra une image dont la partie noire sera constistuée de n pixels noirs à profondeur k,
positionnées aléatoirement.  Chaque image pouvant sortir de préférence avec équiprobabilité.
@param : profondeur et dot_count le nombre de points à placer
@return : une image avec dot_count points noirs
*/

/*image alea(int profondeur, int dot_count){
    if (profondeur == 0) {


      if (dot_count > 0) {
        return construit_noir();
      }
      else return construit_blanc();
    }
    else {

      // S'il y a plus de points que de capacité...
      if (dot_count > 2*pow(2, profondeur)) return construit_noir();

      // Sinon

      /* TROUVER UN MOYEN DE REPARTIR LES POINTS ALEATOIREMENT ENTRE LES 4 FILS
      int random = rand()%4
      int n1 = dot_count*random/4;
      int n2 = dot_count/4;
      int n3 = dot_count/4;
      int n4 = dot_count - n3 - n2 - n1;

      printf("DOT = %d, 1 = %d, 2 = %d, 3 = %d; 4 = %d\n", dot_count, n1, n2, n3, n4);
      return construit_compose(alea(profondeur-1, n1),
                               alea(profondeur-1, n2),
                               alea(profondeur-1, n3),
                               alea(profondeur-1, n4));
    }
  }
*/

/* Fonction qui prend en argument la profondeur k et renvoie une image de profondeur k
choisie aléatoirement tel qu'au centre la densité de noirs soit proche de 1 et au bord proche de 0
@param : la profondeur
@return : Une image de cette profondeur qui ressemble à une nebuleuse
*/
image nebuleuse_aux(int profondeur, int pos_x, int pos_y, int length, int original){
  if (profondeur == 0) {

    float random = (float)(rand()%100)/100;
    double far_from_center = sqrt(pow((original/2 - pos_x), 2) + pow((original/2 - pos_y), 2)); // Distance Euclidienne
    double max_distance = (double)original/sqrt(2);
    double n = far_from_center/max_distance; // Définir ici un moyen d'avoir 0 si proche du centre, 1 sinon, qui dépend donc de la distance au centre
    //printf("Distance : %f\n", far_from_center);
    if (random > n) return construit_noir();
    else return construit_blanc();
  }
  else {
    length = length/2;
    return construit_compose(nebuleuse_aux(profondeur-1, pos_x, pos_y, length, original),
                             nebuleuse_aux(profondeur-1, pos_x + length, pos_y, length, original),
                             nebuleuse_aux(profondeur-1, pos_x, pos_y + length, length, original),
                             nebuleuse_aux(profondeur-1, pos_x + length, pos_y + length, length, original));
  }
}

image nebuleuse(int profondeur){
  int length = (int)pow(2, profondeur);
  return nebuleuse_aux(profondeur, 0, 0, length, length);
}

/* ----------------------------------------------------------------------


                          FONCTIONS DE TEST


---------------------------------------------------------------------- */

void testConstruitBlanc(){
  image I1 = construit_blanc();
  assert(est_blanche(I1));
}

void testConstruitNoir(){
  image I2 = construit_noir();
  assert(est_noire(I2));
}

void testConstruitCompose(){
  image I1 = construit_compose(construit_blanc(),
                               construit_blanc(),
                               construit_blanc(),
                               construit_blanc());
  assert(est_blanche(I1));

  image I2 = construit_compose(construit_noir(),
                               construit_noir(),
                               construit_noir(),
                               construit_noir());
  assert(est_noire(I2));

  image I3 = construit_compose(construit_blanc(),
                               construit_noir(),
                               construit_blanc(),
                               construit_blanc());

  assert(est_blanche(I1->fils[0]) && est_noire(I3->fils[1]));


}

void testCopie(){
  image nebu;
  nebu = nebuleuse(3);
  image nebu2;
  nebu2=copie(nebu);
  assert(meme_dessin(nebu,nebu2));
}

void testDonneProfondeurMax(){
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

  assert(donne_profondeur_max(Image1)==3);
}

void testTabdeChartoImage(){

  char imagetest[10] = {'.','B','B','N','.','B','B','B','N'};
  image imagetest1 = tabdechar_to_image(imagetest);
  image imagetest2 = construit_compose(construit_blanc(),
                                       construit_blanc(),
                                       construit_noir(),
                                       construit_compose(construit_blanc(),
                                                         construit_blanc(),
                                                         construit_blanc(),
                                                         construit_noir()));
  assert(meme_dessin(imagetest1,imagetest2));


}

void testDivision(){
  image I1 = construit_compose(construit_noir(),
                               construit_blanc(),
                               construit_noir(),
                               construit_compose(construit_blanc(),
                                                 construit_blanc(),
                                                 construit_blanc(),
                                                 construit_noir()));
  image I2 = construit_compose(construit_compose(construit_noir(),
                                                 construit_noir(),
                                                 construit_noir(),
                                                 construit_noir()),
                               construit_compose(construit_blanc(),
                                                 construit_blanc(),
                                                 construit_blanc(),
                                                 construit_blanc()),
                               construit_compose(construit_noir(),
                                                 construit_noir(),
                                                 construit_noir(),
                                                 construit_noir()),
                               construit_compose(construit_blanc(),
                                                 construit_blanc(),
                                                 construit_blanc(),
                                                 construit_noir()));
  image I3 = Division(I1);

  assert(meme_dessin(I3,I2));
}

void testConstruitImageProf(){
  image I1 = construit_image_prof(2);
  image I2 = construit_compose(construit_compose(construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc()),
                              construit_compose(construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc()),
                              construit_compose(construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc()),
                              construit_compose(construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc(),
                                                construit_blanc()));
  assert(meme_dessin(I1,I2));
}

void testAire(){
  image I1 = construit_compose(construit_noir(),
                               construit_blanc(),
                               construit_noir(),
                               construit_compose(construit_blanc(),
                                                 construit_blanc(),
                                                 construit_blanc(),
                                                 construit_noir()));
  image Image3 = construit_compose(construit_noir(),
                                  construit_blanc(),
                                  construit_noir(),
                                  construit_noir());
  image I2 = construit_blanc();
  image I3 = construit_noir();
  image I4 = construit_compose(construit_noir(),
                               construit_blanc(),
                               construit_noir(),
                               construit_blanc());
  assert(aire(Image3)==0.75);
  assert(aire(I2)==0);
  assert(aire(I3)==1);
  assert(aire(I4)==0.5);
}

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

  //affichage2kpixel(tabdechar_to_image(phrase));
  //image I = alea(3, 5);

  //affichage2kpixel(I);


  //Fonctions de tests.
  testConstruitBlanc();
  testConstruitNoir();
  testConstruitCompose();
  testCopie();
  testDonneProfondeurMax();
  testTabdeChartoImage();
  testDivision();
  testConstruitImageProf();
  testAire();

  return 0;
}

/* LISTE DES FONCTIONS A FAIRE
  // construit_blanc()
  // construit_noir()
  // construit_compose()
  // affiche_normal()
  // affiche_profondeur()
  // est_noire()
  // est_blanche
  // copie
  // aire
  // meme_dessin
  // difference
  // rendmemoire
  A OPTIMISER lecture_au_clavier
  // CompteSousImagePleine
  // arrondit
  // negatif
  // simplifie
  // affichage2kpixel
  alea
  nebuleuse
  main()
  Permutations dans PilesLR.c
*/
