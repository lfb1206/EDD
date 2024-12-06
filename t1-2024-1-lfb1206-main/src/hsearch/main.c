#include "avl_tree_2D.h"

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

  /* Construcción de las estructuras */

  int N_HEADS;
  int result;
  result = fscanf(input_file, "%d\n", &N_HEADS);
  if (result != 1) {
    printf("Error leyendo la cantidad de cabezas\n");
    return 1;
  }
  Head *heads = calloc(N_HEADS, sizeof(Head));

  /* TODO: Inicializar árboles de búsqueda */
  Node* id_root = NULL;
  Node* year_root = NULL;
  NodeToTree* year_district_root = NULL;
  Node* x_range_root = NULL;
  NodeToTree* circle_root = NULL;
  
  /* Guardar cabezas */
  for (int i = 0; i < N_HEADS; i++) {
    Head *h = &heads[i];
    result = fscanf(input_file, "SAVE %d %s %d %d %d %d\n", &h->id, h->name, &h->year, &h->region, &h->x, &h->y);
    if (result != 6) {
      printf("Error al leer los datos de la cabeza %d\n", i);
      return 1;
    }
    id_root = tree_insert(id_root, node_create(h->id, i));
    year_root = tree_insert(year_root, node_create(h->year, i));
    year_district_root = node_to_tree_insert(year_district_root, node_to_tree_create(h->year, h->region, i));
    x_range_root = tree_insert(x_range_root, node_create(h->x, i));
    circle_root = node_to_tree_insert(circle_root, node_to_tree_create(h->x, h->y, i));
  }

  int N_BUSQUEDAS;
  result = fscanf(input_file, "%d", &N_BUSQUEDAS);
  if (result != 1) {
    printf("Error leyendo la cantidad de eventos\n");
    return 1;
  }

  int first_number;
  int second_number;
  int third_number;
  /* Búsquedas */
  char command[32];
  for (int i = 0; i < N_BUSQUEDAS; i++) {
    int quantity = 0;
    result = fscanf(input_file, "%s", command);
    if (result != 1) {
      printf("Error al leer la búsqueda %d", i);
      return 1;
    }
    if (string_equals(command, "WITH-ID")) {
      fscanf(input_file, "%d", &first_number);
      Node* node = search_node(id_root, first_number);
      if (node == NULL) {
        quantity = 0;
        fprintf(output_file, "WITH-ID %d: %d\n", first_number, quantity);
      } else {
        quantity = 1;
        fprintf(output_file, "WITH-ID %d: %d\n", first_number, quantity);
        Head *h = &heads[node->head_index];
        fprintf(output_file, "%d %s\n", h->id, h->name);
      }
    } else if (string_equals(command, "WITH-YEAR")) {
      fscanf(input_file, "%d", &first_number);
      Node* node = search_node(year_root, first_number);
      if (node == NULL) {
        quantity = 0;
        fprintf(output_file, "WITH-YEAR %d: %d\n", first_number, quantity);
      } else {
        fprintf(output_file, "WITH-YEAR %d: %d\n", first_number, node->quantity_same_priority);
        while (node != NULL) {
          Head *h = &heads[node->head_index];
          fprintf(output_file, "%d %s\n", h->id, h->name);
          node = node->same_priority_next;
        }
      }
    } else if (string_equals(command, "WITH-YEAR-DISTRICT")) {
      fscanf(input_file, "%d %d", &first_number, &second_number);
      Node* node = search_node_to_tree(year_district_root, first_number, second_number);
      if (node == NULL) {
        quantity = 0;
        fprintf(output_file, "WITH-YEAR-DISTRICT %d %d: %d\n",first_number, second_number, quantity);
      } else {
        fprintf(output_file, "WITH-YEAR-DISTRICT %d %d: %d\n",first_number, second_number, node->quantity_same_priority);
        while (node != NULL) {
          Head *h = &heads[node->head_index];
          fprintf(output_file, "%d %s\n", h->id, h->name);
          node = node->same_priority_next;
        }
      }
    } else if (string_equals(command, "IN-X-RANGE")) {
      fscanf(input_file, "%d %d", &first_number, &second_number);
      int index = 0;
      Node *nodes[N_HEADS];
      int *nodes_quantity = &quantity;
      int *nodes_index = &index;
      search_nodes_within_range(x_range_root, first_number, second_number, nodes, nodes_quantity, nodes_index);
      if (quantity == 0)
        fprintf(output_file, "IN-X-RANGE %d %d: %d\n",first_number, second_number, *nodes_quantity);
      else {
        fprintf(output_file, "IN-X-RANGE %d %d: %d\n",first_number, second_number, *nodes_quantity);
        for (int i = 0; i < index; i++) {
          Node* node = nodes[i];
          while (node != NULL) {
            Head *h = &heads[node->head_index];
            fprintf(output_file, "%d %s\n", h->id, h->name);
            node = node->same_priority_next;
          }
        }
      }
    } else if (string_equals(command, "IN-CIRCLE")) {
      fscanf(input_file, "%d %d %d", &first_number, &second_number, &third_number);
      int index = 0;
      Node *nodes[N_HEADS];
      int *nodes_quantity = &quantity;
      int *nodes_index = &index;
      search_node_to_tree_within_range(circle_root, first_number, second_number, third_number, nodes, nodes_quantity, nodes_index);
      if (quantity == 0)
        fprintf(output_file, "IN-CIRCLE %d %d %d: %d\n",first_number, second_number, third_number, quantity);
      else {
        fprintf(output_file, "IN-CIRCLE %d %d %d: %d\n",first_number, second_number, third_number, quantity);
        for (int i = 0; i < index; i++) {
          Node* node = nodes[i];
          while (node != NULL) {
            Head *h = &heads[node->head_index];
            fprintf(output_file, "%d %s\n", h->id, h->name);
            node = node->same_priority_next;
          }
        }
      }
    } else
      printf("BÚSQUEDA NO ENCONTRADA: %s\n", command);
  }

  /* Cerramos los archivos */
  fclose(input_file);
  fclose(output_file);

  /* TODO: Liberación de memoria */
  free(heads);
  tree_free(id_root);
  tree_free(year_root);
  node_to_tree_free(year_district_root);
  tree_free(x_range_root);
  node_to_tree_free(circle_root);

  return 0;
}
