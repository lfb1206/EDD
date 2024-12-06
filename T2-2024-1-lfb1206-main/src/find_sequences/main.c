#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Referencia a link https://github.com/mc-cari/McNanoL-ICPC-Team-Notebook/blob/master/Strings/SuffixAutomatonFULL.cpp


// L: maxlen of state
// Lk: link
// C: indicador si es clone
// F: first ocurrence of the state
// Ilk: inverse links
int sz, last, n; 
int L[100000];
int Lk[100000];
int C[100000];
int F[100000];
int N[100000][4];
int Ilk[100000][100];
int QO[100000];

int char_to_index(char c) {
  switch (c)
  {
  case 'A':
    return 0;
  case 'G':
    return 1;
  case 'T':
    return 2;
  case 'C':
    return 3;
  }
  return -1;
}

void extend_suffix_automaton(int c) {
  int cur = sz++;
  int p = last; 
  C[cur] = 0;
  L[cur] = L[last] + 1;
  F[cur] = L[cur] - 1;

  while(p != -1 && N[p][c] == -1) {
    N[p][c] = cur;
    p = Lk[p];
  }

  if(p == -1) { 
    Lk[cur] = 0;
    last = cur;
    return;
  }
  
  int q = N[p][c];

  if(L[p] + 1 == L[q]) {
    Lk[cur] = q;
    last = cur;
    return;
  }

  int w = sz++; 
  C[w] = 1;
  L[w] = L[p] + 1;
  Lk[w] = Lk[q];

  for (int i = 0; i < 4; i++)
    N[w][i] = N[q][i];

  F[w] = F[q];

  while (p != -1 && N[p][c] == q) {
    N[p][c] = w;
    p = Lk[p];
  }

  Lk[q] = w;
  Lk[cur] = w;
  last = cur;
}

void suffix_automaton_init(char *s) {
  for(int i = 0; i < 100000; i++) {
    QO[i] = 0;
    for(int j = 0; j < 4; j++)
      N[i][j] = -1;
  }
  
  last = L[0] = 0;
  Lk[0] = -1;
  sz = 1;
  for (int i = 0; s[i] != '\0'; i++)
    extend_suffix_automaton(char_to_index(s[i]));

  for(int i = 0; i < sz; i++) {
    if (i){
      Ilk[Lk[i]][QO[Lk[i]]] = i;
      QO[Lk[i]]++;
    }
  }
}

int find(char *s) {
  int p = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    int x = char_to_index(s[i]);
    if(N[p][x] == -1) return -1;
    p = N[p][x];
  }
  return p;
}

void output_all_occurrences(FILE *output_file, int u, int length) {
  if (QO[u] == 0) {
    fprintf(output_file, "%d\n", F[u] - length + 1);
  } else {
    for (int i = 0; i < QO[u] + 1; i++){
      fprintf(output_file, "%d\n", F[Ilk[u][i]] - length + 1);
    }
  }
}

void all_occur(FILE *output_file, char *s, int length) {
  int p = find(s);
  if(p == -1) return;
  output_all_occurrences(output_file, p, length);
}

static bool check_arguments(int argc, char **argv) {
  if (argc != 3) {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    exit(1);
  }
  return true;
}

int main(int argc, char **argv) {
  check_arguments(argc, argv);

  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  int TAMANO_DOCUMENTO;
  fscanf(input_file, "%d", &TAMANO_DOCUMENTO);

  // Leer el documento
  char documento[TAMANO_DOCUMENTO + 1];  // +1 para el carácter nulo
  fscanf(input_file, "%s", documento);

  int largo_cadena, cantidad_consultas;
  fscanf(input_file, "%d", &largo_cadena);
  fscanf(input_file, "%d", &cantidad_consultas);

  suffix_automaton_init(documento);

  // Leer las cadenas de consulta y buscarlas en el documento
  for (int q = 0; q < cantidad_consultas; q++) {
    char cadena[largo_cadena + 1];  // +1 para el carácter nulo
    fscanf(input_file, "%s", cadena);
    // Implementa aca tu tarea

    fprintf(output_file, "Consulta %d\n", q);

    all_occur(output_file, cadena, largo_cadena);

  }

  /* Cerrar archivos */
  fclose(input_file);
  fclose(output_file);

  return 0;
}
