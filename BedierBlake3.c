/*
Tout est dans le désorde dans ce projet...
plus pas bien aéré
*/

#include <stdio.h> // printf, random ...
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
*/
image construit_blanc(){
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
*/
image construit_noir(){
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
image construit_compose(image i1, image i2, image i3, image i4){
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
bool est_blanche(image I){
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
bool est_noire(image I){
  if (I == NULL) return FALSE;
  if (I->toutnoir) return TRUE;
  return (est_noire(I->fils[0])
       && est_noire(I->fils[1])
       && est_noire(I->fils[2])
       && est_noire(I->fils[3]));
}

/*Procédure qui remplace une image blanche par une image noire
@param : image* I : une image en in/out
@return : aucun
*/
void remplaceBlancParNoir(image* I){
  if(est_blanche(*I)){
    *I = construit_noir();
  }
}


/* Fonction qui copie une image dans un nouvel emplacement mémoire
@param L'image à copier
@return Une copie de l'image, que l'on peut modifier sans modifier l'originale
*/
image copie(image I){
  image I_copie;
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

/*Fonction qui vérifie si 2 images sont parfaitement identiques, sans les simplifier.
@param : les 2 images que l'on souhaite comparer
@return :
TRUE si les 2 images sont parfaitement identiques, FALSE sinon
*/
/*MemeDessin :  // attention MemeDessinParfait n'est pas MemeDessin
Quadratique
Bug etourderie... testez-là !*/
bool meme_dessin_parfait(image I, image I2){
  if (I == NULL){
    return (I2 == NULL) ;
  }
  else if (I->toutnoir){
      return (I2->toutnoir) ;
    }
  else if (I2==NULL || I2->toutnoir){
      return FALSE;
    }

  return (meme_dessin_parfait(I->fils[0], I2->fils[0])
       && meme_dessin_parfait(I->fils[1], I2->fils[1])
       && meme_dessin_parfait(I->fils[2], I2->fils[2])
       && meme_dessin_parfait(I->fils[3], I2->fils[3]) ) ;
}




/* Fonction qui cherche la profondeur maximale d'une image
@param L'image dont on veut connaître la profondeur
@return La profondeur maximale de l'image
*/
int donne_profondeur_max(image I){
  if ((I == NULL) || I->toutnoir) return 0;
  else {
    int max = 0;
        for (int i = 0; i < 4; i++) {
          int resultat = donne_profondeur_max(I->fils[i]);
          if (resultat > max) max = resultat; //On trouve le plus profond des 4 fils
        }
    return max+1 ; //On incrémente de 1 pour compter le nouveau noeud
  }
}

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
  while(phrase[i] != '\n'){
    image1[i] = phrase[i];
    i++;
  }
  int shift=0;
  return image_from_tabchar_aux(image1, 0, &shift);}

/* Fonction qui renvoie une image où tous les fils de l'image entrée sont à la même profondeur,
En fait, c'est l'inverse de la fonction arrondit() : Si on a une image élémentaire moins profond qu'une autre, on lui crée
artificiellement des fils qui lui sont identiques.
@param : L'image que l'on souhaite diviser
@return : L'image où tous les fils sont à la même profondeur
*/
/* Fonction auxiliaire */
image Division_aux (image I, int profondeur){
  if(I == NULL || I->toutnoir){
    if(profondeur == 0) {
      if (I == NULL) return construit_blanc();
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
  Division_aux(I, donne_profondeur_max(I));}



/* Fonction qui construit une image de profondeur de n, toute blanche
@param : n la donne_profondeur
@return : Une image blanche de profondeur n
*/
image construit_image_prof(int n){
  if (n == 0) return construit_blanc();
  else return construit_compose(construit_image_prof(n -1),
                                construit_image_prof(n -1),
                                construit_image_prof(n -1),
                                construit_image_prof(n -1));}



  /*Fonction qui trie un tableau d'entier
  @param :tab[] : le tableau a trier,
          taille: la taille de tab.
  @return : un tableau trié.
  */
  int* trieTableau(int tab[], int taille){
    for(int h = 1; h<taille; h++){
      int pos = h;
      while(pos>0 && tab[pos]<tab[pos-1]){
        int tmp = tab[pos];
        tab[pos]=tab[pos-1];
        tab[pos-1]=tmp;
        pos--;
      }
    }
    return tab;
  }

/*fonction qui supprime les doublons dans un tableau d'entier, et les remplace par d'autres entier au hasard.
@param : int tab[] : le tableu d'entier dont on veut supprimer les doublons
         int taille : la taille du tableau
         int max : la valeur maximum que peut prendre les entiers dans le tableau
@ return : le tableau trié et sans doublons, ayant le meme nombre d'éléments que le tableau entré en parametre
*/
int* enleveDoublon(int tab[], int taille, int max){
  tab = trieTableau(tab, taille);
  bool doublon = FALSE;
  for(int i = 0; i< taille -1; i++){
    if(tab[i] == tab[i+1]){
      doublon=TRUE;
      tab[i] = rand()%(max);
    }
  }
  if(doublon == TRUE){
    tab = enleveDoublon(tab,taille, max);
  }
  return tab;
}




/* ----------------------------------------------------------------------
                            AFFICHAGE
---------------------------------------------------------------------- */

/* Fonction qui affiche une image selon la notation [.,N,B]
@param : L'image que l'on souhaite afficher
@return : Aucun
*/
void affiche_normal(image I){
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
void affiche_prof_aux(image I, int profondeur){ // On définit une fonction auxiliaire qui garde en mémoire la profondeur{
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



/*fonction qui prend un tableau d'entier et le renvoie mélanger ( en echangant 2 à 2 les valeurs)
@param : int aleas[] : un tableau d'entier
         int max : un entier indiquant la taille du tableau
*/
void repartitNoir(int aleas[], int max){
  for(int i = 0; i< max; i++){
    int pos1 = rand()%max;
    int pos2 = rand()%max;
    int tmp;

    tmp = aleas[pos1];
    aleas[pos1]=aleas[pos2];
    aleas[pos2]=tmp;
  }
}

/*procédure auxiliaire de alea, renvoie une image blanche avec des pixels noirs
@param : int n : un entier indiquant la profonfeur de l'image
         int aleas[] : un tableau d'entier avec le nombre de cases egales au nombres de fils de l'images voulue, avec des 1 la ou on veut des pixels noirs
*/
image construit_alea(int n, int aleas[], int* i){
  if(n==0){
    (*i)++;
    if (aleas[*i] == 1) return construit_noir();
    else return construit_blanc();
  }
  else return construit_compose(construit_alea((n-1), aleas, i),
                                construit_alea((n-1), aleas, i),
                                construit_alea((n-1), aleas, i),
                                construit_alea((n-1), aleas, i));
}

/* Fonction qui retourne une image de profondeur  choisit, comprenant un nombre
donné de pixels noirs.
@param : int profondeur : la profondeur voulue
         int pixelsnoir : le nombre de pixels noir à placer
@return : une image avec pixelsnoir points noirs
*/
image alea(int profondeur,int pixelsnoir){

  int max = (int) pow(4,profondeur);
  if(pixelsnoir > max){
    return construit_noir();
  }
  int aleas[max];
  int pos = 0;
  for(int j = 0 ; j < pixelsnoir; j++){
    aleas[j]=1;
    pos++;
  }
  for(int i = pos; i< max; i++){
    aleas[i]=0;
  }

  repartitNoir(aleas, max);
  int pi = -1;
  return construit_alea(profondeur, aleas, &pi);
}


/*
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
*/

char a2k(int k, image I, int height, int length, int size){
  if (I == NULL) return '.';
  if (I->toutnoir) return '8';
  if (k == 0) return '/';
  else {
    size = size/2;
    if (height >= size) {
      if (length >= size)      return a2k(k-1, I->fils[3], height%(size), length%size, size);
      else                      return a2k(k-1, I->fils[2], height%size, length%size, size);
    } else if (length >= size) return a2k(k-1, I->fils[1], height%size, length%size, size);
      else                      return a2k(k-1, I->fils[0], height%size, length%size, size);
  }
}

void affichage2kpixel(int k, image I){
  int size = pow(2, k);
  for (int height = 0; height < size; height++) {
    for (int length = 0; length < size; length++){
      putchar(a2k(k, I, height, length, size));
    }
    putchar('\n');
  }
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

double aire(image I){
  if (est_blanche(I)) return 0;
  else if (est_noire(I)) return 1;
  else return (aire(I->fils[0])
            +  aire(I->fils[1])
            +  aire(I->fils[2])
            +  aire(I->fils[3]))/4;
}


/* Procédure qui simplifie une image :
C'est à dire que si tous les enfants d'une image sont de la même couleur, on considère qu'il n'est pas utile
d'avoir des fils et on les remplace par un élément simple (noir ou blanc)
@param : L'image que l'on souhaite simplifier
@return : Aucun
*/

/* Simplifie : quadratique, pourtant vous chauffiez*/
void simplifie(image* I){
  if (*I != NULL && !((*I)->toutnoir)) {
    if (est_blanche(*I)) { // En fait on regarde ici si les enfants sont unis
          rendmemoire(I);
          (*I) = construit_blanc() ;
    } else if (est_noire(*I)) {
          rendmemoire(I);
          (*I) = construit_noir();
    } else {
      for (int i = 0; i < 4; i++) {
            simplifie(&((*I)->fils[i])) ;
      }
    }
  }
}

/* Fonction qui vérifie si deux images représentent la même chose
@param : Les deux images que l'on souhaite comparer
@return : TRUE si les deux images sont identiques
*/
bool meme_dessin(image I, image I2){

  if (est_blanche(I))                 return (est_blanche(I2)) ;
  else if (est_noire(I))              return (est_noire(I)) ;
  else if (I2==NULL || I2->toutnoir)  return FALSE ;

  return (meme_dessin(I->fils[0], I2->fils[0])
       && meme_dessin(I->fils[1], I2->fils[1])
       && meme_dessin(I->fils[2], I2->fils[2])
       && meme_dessin(I->fils[3], I2->fils[3]) ) ;
}

/* Procédure qui transforme une image en sa forme négative,
c'est à dire que les cases blanches deviennent noires et réciproquement.
@param : L'image que l'on souhaite inverser
@return : Aucun
*/
void negatif(image* I) {
  if (*I == NULL) {
    *I = construit_noir();
  }
  else if ((*I)->toutnoir) {
    rendmemoire(I);
    *I = construit_blanc();
  } else {
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
    if (aire(*I) < 0.5) {
      rendmemoire(I);
      (*I) = NULL ;
    } else (*I)->toutnoir = TRUE ;
  }
}
/* Fonction principale */
void arrondit(image* I, int k) {
  if (k == 0) arrondit_elementaire(I); // Si l'on est à la profondeur recherchée, on arrondit notre image en un élément (noir ou blanc)
  else if (((*I) != NULL) && !((*I)->toutnoir)) {
      for (int i = 0; i < 4; i++) {
        arrondit(&((*I)->fils[i]), k-1);
      }
  }
}

/* Fonction qui renvoie une image représentant la différence entre deux images :
L'image rendue est noire là où l'une des deux images de départ est noire mais l'autre blanche
@param Les deux images que l'on compare
@return L'image de la différence
*/
image difference (image I1, image I2){
  if (I1 != NULL && !(I1->toutnoir)) {
      if (I2 == NULL || I2->toutnoir) {
        return construit_compose(difference(I1->fils[0], I2),
                                                             difference(I1->fils[1], I2),
                                                             difference(I1->fils[2], I2),
                                                             difference(I1->fils[3], I2));
      } else {
        return construit_compose(difference(I1->fils[0], I2->fils[0]),
                                                             difference(I1->fils[1], I2->fils[1]),
                                                             difference(I1->fils[2], I2->fils[2]),
                                                             difference(I1->fils[3], I2->fils[3]));
      }
  } else  {
    if (I2 != NULL && !(I2->toutnoir)) {
    return construit_compose(difference(I1, I2->fils[0]),
                                    difference(I1, I2->fils[1]),
                                  difference(I1, I2->fils[2]),
                                  difference(I1, I2->fils[3]));
    } else {
      if (I1 == NULL) {
        if (I2 == NULL) {
          return construit_blanc();
        }
        else {
          return construit_noir();
        }
      }
      else {
        if (I2 == NULL) { return construit_noir();
        }
        else {
          return construit_blanc();
        }
      }
    }
  }
}

/* Fonction qui permet à l'utilisateur de rentrer une image depuis le terminal
@param : Aucun
@return : L'image construite à partir des indications de l'utilisateur
*/
image lecture_au_clavier(){
  image p = NULL;
  char input = getchar();
  if (input == '.'){
    image f0 = lecture_au_clavier();
    image f1 = lecture_au_clavier();
    image f2 = lecture_au_clavier();
    image f3 = lecture_au_clavier();
    return construit_compose(f0, f1, f2, f3);
  } else if (input == 'N')  return construit_noir();
    else if (input == 'B')  return construit_blanc();
    else return lecture_au_clavier();
}

image lecture_au_fichier(FILE* fichier){

  if(fichier != NULL){
    char image1[256];
    char flag = 's';
    int i = 0;
    while(flag !='\n'){
      flag = fgetc(fichier);
      image1[i] = flag;
      i++;
    }
    int shift = 0;
    return image_from_tabchar_aux(image1, 0, &shift);
  }
  else{
    return construit_blanc();
  }

}


/* Fonction qui compte le nombre de sous image pleine à une profondeur donnée
@param : L'image que l'on examine, la hauteur qui nous intéresse
@return : Le nombre de sous images pleines
*/

bool estPleine(image I, int h){
    if (h == 0)
        return ((I == NULL) || (I->toutnoir));
    else if (I != NULL && !(I->toutnoir))
        return estPleine(I->fils[0], h-1)
        && estPleine(I->fils[1], h-1)
        && estPleine(I->fils[2], h-1)
        && estPleine(I->fils[3], h-1);
    else return (h == 0);
}

int CompteSousImagePleine(image I, int hauteur){
    if((I == NULL) || (I->toutnoir)){
        return estPleine(I, hauteur);
    } else {
        return estPleine(I, hauteur)
             + CompteSousImagePleine(I->fils[0], hauteur)
             + CompteSousImagePleine(I->fils[1], hauteur)
             + CompteSousImagePleine(I->fils[2], hauteur)
             + CompteSousImagePleine(I->fils[3], hauteur);
    }
}


/* Fonction qui prend en argument la profondeur k et renvoie une image de profondeur k
choisie aléatoirement tel qu'au centre la densité de noirs soit proche de 1 et au bord proche de 0
@param : la profondeur
@return : Une image de cette profondeur qui ressemble à une nebuleuse
*/
image nebuleuse_aux(int profondeur, int pos_x, int pos_y, int original){
  if (profondeur == 0) {

    float random = (float)(rand()%100)/100;
    double far_from_center = sqrt(pow((original/2 - pos_x), 2) + pow((original/2 - pos_y), 2)); // Distance Euclidienne
    double max_distance = (double)original/sqrt(2);
    double n = far_from_center/max_distance;
    if (random > n) return construit_noir();
    else return construit_blanc();
  }
  else {
    return construit_compose(nebuleuse_aux(profondeur-1, pos_x,                             pos_y,                             original),
                             nebuleuse_aux(profondeur-1, pos_x + (int)pow(2, profondeur-1), pos_y,                             original),
                             nebuleuse_aux(profondeur-1, pos_x,                             pos_y + (int)pow(2, profondeur-1), original),
                             nebuleuse_aux(profondeur-1, pos_x + (int)pow(2, profondeur-1), pos_y + (int)pow(2, profondeur-1), original));
  }
}

image nebuleuse(int profondeur){
  int length = (int)pow(2, profondeur);
  return nebuleuse_aux(profondeur, 0, 0, length);
}




/*Fonction qui compte les images noires
@param : image I : une image dont on veut compter le nombre d'images noires dont elle est constituée
         int* cpt : un entier qui nous servira de compteur
@return: un entier indiquant le nombre d'image noires constituant I
*/
int compteImageNoire(image I){

  if(est_noire(I)) return 1;
  else if (est_blanche(I)) return 0;
  else return (compteImageNoire(I -> fils[0])
             + compteImageNoire(I -> fils[1])
             + compteImageNoire(I -> fils[2])
             + compteImageNoire(I -> fils[3]));
}




/* ----------------------------------------------------------------------
                          FONCTIONS DE TEST
---------------------------------------------------------------------- */
/*Procédure de test pour est_blanche
@param : aucun
@return: aucun
*/
void testEstBlanche(){
  image Image1 = construit_blanc();
  image Image2 = construit_compose(construit_blanc(),
                                   construit_blanc(),
                                   construit_blanc(),
                                   construit_blanc());
  assert(est_blanche(Image1));
  assert(est_blanche(Image2));
}

/*Procédure de test pour est_noire
@param : aucun
@return: aucun
*/
void testEstNoire(){
  image Image1 = construit_noir();
  image Image2 = construit_compose(construit_noir(),
                                   construit_noir(),
                                   construit_noir(),
                                   construit_noir());
  assert(est_noire(Image1));
  assert(est_noire(Image2));
}

/*Procédure de test pour construit_blanc
@param : aucun
@return: aucun
*/
void testConstruitBlanc(){
  image I1 = construit_blanc();
  assert(est_blanche(I1));
}

/*Procédure de test pour construit_noir
@param : aucun
@return: aucun
*/
void testConstruitNoir(){
  image I2 = construit_noir();
  assert(est_noire(I2));
}

/*Procédure de test pour construit_compose
@param : aucun
@return: aucun
*/
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

/*Procédure de test pour copie
@param : aucun
@return: aucun
*/
void testCopie(){
  image nebu = nebuleuse(3);
  image nebu2 = copie(nebu);
  assert(meme_dessin(nebu,nebu2));
}

/*Procédure de test pour donne_profondeur_max
@param : aucun
@return: aucun
*/
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

/*Procédure de test pour tabdechar_to_image
@param : aucun
@return: aucun
*/
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

/*Procédure de test pour Division
@param : aucun
@return: aucun
*/
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

/*Procédure de test pour construit_image_prof
@param : aucun
@return: aucun
*/
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

/*Procédure de test pour aire
@param : aucun
@return: aucun
*/
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
  assert(aire(I1)==0.5625);
  assert(aire(I2)==0);
  assert(aire(I3)==1);
  assert(aire(I4)==0.5);
}

/*Procédure de test pour simplifie
@param : aucun
@return: aucun
*/
void testSimplifie(){
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


  image Image2 = construit_compose(construit_noir(),
                                       construit_blanc(),
                                       construit_noir(),
                                       construit_compose(construit_blanc(),
                                                         construit_blanc(),
                                                         construit_blanc(),
                                                         construit_noir())) ;

  simplifie(&Image1);

  assert(meme_dessin_parfait(Image2, Image1));

}

/*Procédure de test pour meme_dessin
@param : aucun
@return: aucun
*/
void testMemeDessin(){
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
  image Image2 = construit_compose(construit_noir(),
                                   construit_blanc(),
                                   construit_noir(),
                                   construit_compose(construit_blanc(),
                                                     construit_blanc(),
                                                     construit_blanc(),
                                                     construit_noir())) ;

  image Image3 = construit_noir();
  image Image4 = construit_compose(construit_noir(),
                                   construit_noir(),
                                   construit_noir(),
                                   construit_noir());
  assert(meme_dessin(Image1, Image2));
  assert(meme_dessin(Image3, Image4));
}

/*Procédure de test pour negatif
@param : aucun
@return: aucun
*/
void testNegatif(){
  image Image1 = construit_compose(construit_noir(),
                                   construit_blanc(),
                                   construit_noir(),
                                   construit_compose(construit_blanc(),
                                                     construit_blanc(),
                                                     construit_blanc(),
                                                     construit_noir())) ;

  image Image2 = construit_compose(construit_blanc(),
                                   construit_noir(),
                                   construit_blanc(),
                                   construit_compose(construit_noir(),
                                                     construit_noir(),
                                                     construit_noir(),
                                                     construit_blanc())) ;

  negatif(&Image1);
  assert(meme_dessin(Image1, Image2));} //Probleme résolu

  /*Procédure de test pour arrondit
  @param : aucun
  @return: aucun
  */
void testArrondit(){ //Probleme résolu
  image Image1 = construit_compose(construit_noir(),
                                   construit_blanc(),
                                   construit_noir(),
                                   construit_compose(construit_blanc(),
                                                     construit_blanc(),
                                                     construit_noir(),
                                                     construit_compose(construit_noir(),
                                                                       construit_noir(),
                                                                       construit_blanc(),
                                                                       construit_noir()))) ;

  image Image1arr = construit_compose(construit_noir(),
                                      construit_blanc(),
                                      construit_noir(),
                                      construit_blanc()) ;

  arrondit(&Image1,1);

  assert(meme_dessin(Image1,Image1arr));


}

/*Procédure de test pour difference
@param : aucun
@return: aucun
*/
void testDifference(){

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

  image Image2 = construit_compose(construit_noir(),
                                  construit_blanc(),
                                  construit_blanc(),
                                  construit_compose(construit_blanc(),
                                                    construit_blanc(),
                                                    construit_blanc(),
                                                    construit_compose(construit_noir(),
                                                                      construit_blanc(),
                                                                      construit_blanc(),
                                                                      construit_noir()))) ;


  image diff12 = construit_compose(construit_blanc(),
                                  construit_blanc(),
                                  construit_noir(),
                                  construit_compose(construit_blanc(),
                                                    construit_blanc(),
                                                    construit_blanc(),
                                                    construit_compose(construit_blanc(),
                                                                      construit_noir(),
                                                                      construit_noir(),
                                                                      construit_blanc()))) ;


  //affichage2kpixel(Image2);
  image Image3;
  printf("Avant l'appel de difference dans testDifference\n");
  Image3 = difference(Image1,Image2);


  assert(meme_dessin(Image3,diff12));} //Segmentation Fault NON RESOLU !

/*Procédure de test pour lecture_au_clavier
  @param : aucun
  @return: aucun
  */
void testLectureAuClavier(){
  printf("Rentrez .NNB.NNB.BNNN svp\n");
  image I = construit_compose(construit_noir(),
                              construit_noir(),
                              construit_blanc(),
                              construit_compose(construit_noir(),
                                                construit_noir(),
                                                construit_blanc(),
                                                construit_compose(construit_blanc(),
                                                                  construit_noir(),
                                                                  construit_noir(),
                                                                  construit_noir())));
  image Ilue = lecture_au_clavier();
  assert(meme_dessin(I,Ilue));

}

/*Procédure de test pour
@param : aucun
@return: aucun
*/
void testAlea(){
  image I = alea(5, 64);
  int In = 0;
  int* ptrIn = & In;
  int in = compteImageNoire(I);
  assert(64 ==  in);

  image N = alea(2, 1024);
  image noire = construit_noir();
  assert(meme_dessin(N,noire));
}

/*Procédure de test pour CompteSousImagePleine
@param : aucun
@return: aucun
*/
void testCompteSousImagePleine(){
   image I1 = construit_compose(construit_compose(construit_blanc(),
                                                  construit_blanc(),
                                                  construit_noir(),
                                                  construit_blanc()),
                                construit_compose(construit_blanc(),
                                                  construit_blanc(),
                                                  construit_noir(),
                                                  construit_blanc()),
                                construit_compose(construit_blanc(),
                                                  construit_blanc(),
                                                  construit_blanc(),
                                                  construit_noir()),
                                construit_compose(construit_noir(),
                                                  construit_noir(),
                                                  construit_noir(),
                                                  construit_blanc()));
  //char phrase2[59] = {'.','.','.','B','B', 'N', 'B','.', 'N', 'N', 'B', 'N', '.','B','B','B', 'N', '.','N','N','N', 'B', 'N','.','N','B','N','.','B','B','N','B','.','B','N','B','.','.','B','B','N','B','.','N','B','B','N','.','B','N','B','N','.','N','B','N','B', '\n'};
  image I2 = construit_compose(construit_compose(construit_compose(construit_blanc(),
                                                                   construit_blanc(),
                                                                   construit_noir(),
                                                                   construit_blanc()),
                                                 construit_compose(construit_noir(),
                                                                   construit_noir(),
                                                                   construit_blanc(),
                                                                   construit_noir()),
                                                 construit_compose(construit_blanc(),
                                                                   construit_blanc(),
                                                                   construit_blanc(),
                                                                   construit_noir()),
                                                 construit_compose(construit_noir(),
                                                                   construit_noir(),
                                                                   construit_noir(),
                                                                   construit_blanc())),
                              construit_noir(),
                              construit_compose(construit_noir(),
                                                construit_blanc(),
                                                construit_noir(),
                                                construit_compose(construit_blanc(),
                                                                  construit_blanc(),
                                                                  construit_noir(),
                                                                  construit_blanc())),
                              construit_compose(construit_blanc(),
                                                construit_noir(),
                                                construit_blanc(),
                                                construit_compose(construit_compose(construit_blanc(),
                                                                                    construit_blanc(),
                                                                                    construit_noir(),
                                                                                    construit_blanc()),
                                                                  construit_compose(construit_noir(),
                                                                                    construit_blanc(),
                                                                                    construit_blanc(),
                                                                                    construit_noir()),
                                                                  construit_compose(construit_blanc(),
                                                                                    construit_noir(),
                                                                                    construit_blanc(),
                                                                                    construit_noir()),
                                                                  construit_compose(construit_noir(),
                                                                                    construit_blanc(),
                                                                                    construit_noir(),
                                                                                    construit_blanc()))));
  //image I2 = tabdechar_to_image(phrase2)
  //assert(CompteSousImagePleine(I1,2)==1);
  assert(CompteSousImagePleine(I2,2)==2);
}


/* ----------------------------------------------------------------------
                                MAIN
---------------------------------------------------------------------- */

int main() {

  srand(time(NULL));

  testEstBlanche();
  testEstNoire();
  testConstruitBlanc();
  testConstruitNoir();
  testConstruitCompose();
  testCopie();
  testDonneProfondeurMax();
  testDivision();
  testConstruitImageProf();
  testAire();
  testSimplifie();
  testMemeDessin();
  testNegatif();
  testArrondit();
  testAlea();
  testLectureAuClavier(); //Elle fonctionne, c'est juste qu'il faut rentrer un truc si on la met pas en commentaire et c'est chiant

  //testTabdeChartoImage(); //erreur de segmentation
  //testDifference(); // segmentation fault
  testCompteSousImagePleine();
  image I = alea(5,80);
  affichage2kpixel(5, I);
    putchar('\n');
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
  //alea
  // nebuleuse
  // main()
  Permutations dans PilesLR.c
  erreur de segmentation dans : Difference
                                tabdechar_to_image
                                compte
*/


// Playlist pour travailler : https://www.youtube.com/watch?v=uuI_IgFFVT8
