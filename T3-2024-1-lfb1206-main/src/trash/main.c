#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int minimum_dumps(int *dumps, int dumps_quantity, int amount) {
  int solutions[amount + 1];
  for (int i = 0; i <= amount; i++)
    solutions[i] = -1;

  
  solutions[0] = 0;
  
  for (int i = 1; i <= amount; i++)
    for (int j = 0; j < dumps_quantity; j++)
      if (dumps[j] <= i && solutions[i - dumps[j]] != -1 && (solutions[i] > solutions[i - dumps[j]] + 1 || solutions[i] == -1))
        solutions[i] = solutions[i - dumps[j]] + 1;

  return solutions[amount];
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

  int NUM_BASURALES;
  fscanf(input_file, "%d", &NUM_BASURALES);

  int *basurales = (int *)malloc(NUM_BASURALES * sizeof(int));
  //Leer los basurales
  for (int q = 0; q < NUM_BASURALES; q++) {
    fscanf(input_file, "%d", &basurales[q]);
  }

  int NUM_A_SUMAR;
  fscanf(input_file, "%d", &NUM_A_SUMAR);
  
  int result = minimum_dumps(basurales, NUM_BASURALES, NUM_A_SUMAR);

  fprintf(output_file, "%d", result);

  free(basurales);

  /* Cerrar archivos */
  fclose(input_file);
  fclose(output_file);

  return 0;
}
