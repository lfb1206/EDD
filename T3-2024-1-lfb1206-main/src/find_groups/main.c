#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dfs(int *visited, int **link, int *qlinks, int quantityOfNodes, int node) {
  int stack[quantityOfNodes];
  int top = 0;
  stack[top] = node;
  while (top >= 0) {
    int current = stack[top];
    top--;
    for (int i = 0; i < qlinks[current]; i++) {
      if (link[current][i] != -1 && visited[link[current][i]] == 0) {
        visited[link[current][i]] = 1;
        top++;
        stack[top] = link[current][i];
      }
    }
  }
}

int count_groups(int *visited, int **link, int *qlinks, int quantityOfNodes) {
  int count = 0;
  for (int i = 0; i < quantityOfNodes; i++)
    if (visited[i] == 0) {
      count++;
      visited[i] = 1;
      dfs(visited, link, qlinks, quantityOfNodes, i);
    }
  return count;
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

  int NODOS_GRAFO;
  fscanf(input_file, "%d", &NODOS_GRAFO);

  int* visited = (int*)malloc(NODOS_GRAFO * sizeof(int));
  int** link = (int**)malloc(NODOS_GRAFO * sizeof(int*));
  int* qlinks = (int*)malloc(NODOS_GRAFO * sizeof(int));

  for (int i = 0; i < NODOS_GRAFO; i++) {
    visited[i] = 0;
    qlinks[i] = 0;
    link[i] = NULL;
  }

  // Leer el documento
  int NUM_ARISTAS;
  fscanf(input_file, "%d", &NUM_ARISTAS);

  // Leer las aristas de consulta y buscarlas en el documento
  for (int q = 0; q < NUM_ARISTAS; q++) {
    int nodo1, nodo2;
    fscanf(input_file, "%d %d", &nodo1, &nodo2);
    qlinks[nodo1]++;
    qlinks[nodo2]++;
    link[nodo1] = (int*)realloc(link[nodo1], qlinks[nodo1] * sizeof(int));
    link[nodo2] = (int*)realloc(link[nodo2], qlinks[nodo2] * sizeof(int));
    link[nodo1][qlinks[nodo1] - 1] = nodo2;
    link[nodo2][qlinks[nodo2] - 1] = nodo1;
  }

  int result = count_groups(visited, link, qlinks, NODOS_GRAFO);

  fprintf(output_file, "%d\n", result);

  free(visited);
  for (int i = 0; i < NODOS_GRAFO; i++) {
    if (link[i] != NULL) {
      free(link[i]);
    }
  }
  free(link);
  free(qlinks);

  /* Cerrar archivos */
  fclose(input_file);
  fclose(output_file);

  return 0;
}
