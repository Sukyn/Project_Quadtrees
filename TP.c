//Calculez e en utilisant l’expression e=∑1/n!
float e(float precision){
  float v_n = 1; //v_n avec v_1 = 1/1
  float v_nplus = 1.5; //v_n+1 avec v_2 = 1/1 + 1/2
  n = 2;
  while (v_nplus - v_n) > precision{
    v_n = v_nplus;
    n++;
    v_nplus += 1/n;
  }
}

void main{
  e(0.0001);
}
