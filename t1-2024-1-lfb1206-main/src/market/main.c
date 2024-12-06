#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

/* Retorna true si ambos strings son iguales */
static bool string_equals(char *string1, char *string2) {
  return !strcmp(string1, string2);
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

  int N_EVENTOS;

  int result;
  result = fscanf(input_file, "%d", &N_EVENTOS);
  if (result != 1) {
    printf("Error leyendo la cantidad de eventos\n");
    return 1;
  }

  /* TODO: Construcción de las estructuras */

  Heap* heap_compra = heap_create(N_EVENTOS);
  Heap* heap_venta = heap_create(N_EVENTOS);

  /* Eventos */
  char command[32];
  int user;
  int amount;
  for (int i = 0; i < N_EVENTOS; i++) {
    result = fscanf(input_file, "%s", command);
    if (result != 1) {
      printf("Ocurrió un problema en el evento %d\n", i);
      return 1;
    } else if (string_equals(command, "BUY")) {
      fscanf(input_file, "%d %d", &amount, &user);
      fprintf(output_file, "User %d compra a monto %d\n", user, amount);
      if (heap_venta->capacity > 0) {
        Node* node_venta = heap_min(heap_venta);
        if (amount >= node_venta->amount) {
          fprintf(output_file, "Orden Ejecutada: %d -> %d: %d\n", node_venta->user, user, amount);
          heap_extract_min(heap_venta);
        } else {
          heap_insert(heap_compra, user, -amount, i);
        }
      } else {
        heap_insert(heap_compra, user, -amount, i);
      }
    } else if (string_equals(command, "SELL")) {
      fscanf(input_file, "%d %d", &amount, &user);
      fprintf(output_file, "User %d vende a monto %d\n", user, amount);
      if (heap_compra->capacity > 0) {
        Node* node_compra = heap_min(heap_compra);
        if (node_compra->amount <= -amount) {
          fprintf(output_file, "Orden Ejecutada: %d -> %d: %d\n", user, node_compra->user, amount);
          heap_extract_min(heap_compra);
        } else {
          heap_insert(heap_venta, user, amount, i);
        }
      } else {
        heap_insert(heap_venta, user, amount, i);
      }
    } else if (string_equals(command, "STATUS")) {
      if (heap_compra->capacity > 0) {
        Node* node_compra = heap_min(heap_compra);
        fprintf(output_file, "Estado del mercado\n");
        fprintf(output_file, "	%d por %d\n", -node_compra->amount, node_compra->user);
        if (heap_venta->capacity > 0) {
          Node* node_venta = heap_min(heap_venta);
          fprintf(output_file, "	SPREAD %d\n", node_venta->amount + node_compra->amount);
          fprintf(output_file, "	%d por %d\n", node_venta->amount, node_venta->user);
        }
      } else if (heap_venta->capacity > 0) {
        Node* node_venta = heap_min(heap_venta);
        fprintf(output_file, "Estado del mercado\n");
        fprintf(output_file, "	%d por %d\n", node_venta->amount, node_venta->user);
      } else {
        fprintf(output_file, "Mercado inactivo: No hay registros de transacciones.\n");
      }
    } else {
      printf("EVENTO NO ENCONTRADO: %s\n", command);
    }
  }

  /* Cerra archivos */
  fclose(input_file);
  fclose(output_file);

  /* TODO: Liberar memoria */

  heap_free(heap_compra);
  heap_free(heap_venta);

  return 0;
}
