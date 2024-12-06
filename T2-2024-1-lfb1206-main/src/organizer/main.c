#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKAGE_SIZE 3

int N_PAQUETES;
int N_FILAS, N_COLUMNAS;

bool is_valid_location(char **distribution, int **package, int row, int col) {
  for (int i = 0; i < PACKAGE_SIZE; i++)
    for (int j = 0; j < PACKAGE_SIZE; j++)
      if (package[i][j] == 1 && (row + i >= N_FILAS || col + j >= N_COLUMNAS || distribution[row + i][col + j] != '1'))
        return false;
  return true;
}

void place_package(char **distribution, int **package, int row, int col, char package_id) {
  for (int i = 0; i < PACKAGE_SIZE; i++)
    for (int j = 0; j < PACKAGE_SIZE; j++)
      if (package[i][j] == 1)
        distribution[row + i][col + j] = package_id;
}

bool fit_package(char **distribution, int ***packages, char* packages_id, int package_to_check) {
  if (package_to_check >= N_PAQUETES) {
    return true;
  }

  int **package = packages[package_to_check];
  char package_id = packages_id[package_to_check];

  for (int i = 0; i < N_FILAS; i++) {
    for (int j = 0; j < N_COLUMNAS; j++) {
    if (is_valid_location(distribution, package, i, j)) {
        place_package(distribution, package, i, j, package_id);
        printf("%d\n", package_to_check);
        for (int i = 0; i < N_FILAS; i++) {
          for (int j = 0; j < N_COLUMNAS; j++) {
            printf("%c ", distribution[i][j]);
          }
          printf("\n");
        }
        printf("\n");

        if (fit_package(distribution, packages, packages_id, package_to_check + 1))
          return true;

        place_package(distribution, package, i, j, '1');
      }
    }
  }
  return false;
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

  fscanf(input_file, "%d", &N_FILAS);
  fscanf(input_file, "%d", &N_COLUMNAS);
  N_FILAS += 2;
  N_COLUMNAS += 2;

  int compartment[N_FILAS][N_COLUMNAS];
  char **distribution = (char **)malloc(N_FILAS * sizeof(char *));

  // Para leer la matriz de compartimientos
  for (int i = 0; i < N_FILAS; i++) {
    distribution[i] = (char *)malloc(N_COLUMNAS * sizeof(char));
    for (int j = 0; j < N_COLUMNAS; j++) {
      if (i < 2 || j < 2) {
        compartment[i][j] = 0;
      } else {
        fscanf(input_file, "%d", &compartment[i][j]);
      }
      distribution[i][j] = compartment[i][j] == 0 ? '0' : '1';
    }
  }

  fscanf(input_file, "%d", &N_PAQUETES);
  
  int ***packages = (int ***)malloc(N_PAQUETES * sizeof(int **));
  char* packages_id = (char*)malloc(N_PAQUETES * sizeof(char));

  for (int i = 0; i < N_PAQUETES; i++) {
    char package_buffer_id[2];
    fscanf(input_file, "%s\n", package_buffer_id);
    char package_id = package_buffer_id[0];

    int **package = (int **)malloc(PACKAGE_SIZE * sizeof(int *));

    for (int j = 0; j < PACKAGE_SIZE; j++) {
      package[j] = (int *)malloc(PACKAGE_SIZE * sizeof(int));
      for (int k = 0; k < PACKAGE_SIZE; k++) {
        fscanf(input_file, "%d", &package[j][k]);
      }
    }

    printf("Paquete %c\n", package_id);
    for (int j = 0; j < PACKAGE_SIZE; j++) {
      for (int k = 0; k < PACKAGE_SIZE; k++) {
        printf("%d ", package[j][k]);
      }
      printf("\n");
    }

    packages[i] = package;
    packages_id[i] = package_id;
  }

  fit_package(distribution, packages, packages_id, 0);

  for (int i = 2; i < N_FILAS; i++) {
    for (int j = 2; j < N_COLUMNAS; j++) {
      if (j == 2)
        fprintf(output_file, "%c", distribution[i][j]);
      else
        fprintf(output_file, " %c", distribution[i][j]);
    }
    fprintf(output_file, "\n");
  }

  /* Cerra archivos */
  fclose(input_file);
  fclose(output_file);

  for (int i = 0; i < N_FILAS; i++) {
    free(distribution[i]);
  }

  for (int i = 0; i < N_PAQUETES; i++) {
    for (int j = 0; j < PACKAGE_SIZE; j++) {
      free(packages[i][j]);
    }
    free(packages[i]);
  }

  free(distribution);
  free(packages);
  free(packages_id);

  return 0;
}
