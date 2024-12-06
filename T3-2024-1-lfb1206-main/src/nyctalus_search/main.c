#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(int *arr, int n) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;

    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }

    arr[j + 1] = key;
  }
}

int minimum_ships_to_cover_planets(int *planets, int planetsQuantity, int range) {

  insertion_sort(planets, planetsQuantity);
  
  int ships = 1;
  int startingPlanet = 0;

  for (int i = 0; i < planetsQuantity; i++) {
    if (planets[i] - planets[startingPlanet] > 2 * range) {
      startingPlanet = i;
      ships++;
    }
  }

  return ships;
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

    int CANT_PLANETAS;
    fscanf(input_file, "%d", &CANT_PLANETAS);

    int *POSICIONES_PLANETAS = (int *)malloc(CANT_PLANETAS * sizeof(int));
    //Leer las posiciones p_i de los planetas y alocarlos en un array 
    for (int q = 0; q < CANT_PLANETAS; q++) {
        fscanf(input_file, "%d", &POSICIONES_PLANETAS[q]);
    }

    // Leer el Rango
    int RANGO;
    fscanf(input_file, "%d", &RANGO);

    int ships = minimum_ships_to_cover_planets(POSICIONES_PLANETAS, CANT_PLANETAS, RANGO);

    fprintf(output_file, "%d\n", ships);

    /* Cerrar archivos */
    fclose(input_file);
    fclose(output_file);

    return 0;
}
