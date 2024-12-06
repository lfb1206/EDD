#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int P[100];
unsigned int H[100];
unsigned int CP[100][200000];
unsigned int CH[100][200000];
unsigned int QC[100];
unsigned int first_print = 1;
unsigned long long tree[20][200000];
unsigned long long tree2[20][200000];
unsigned long long queryTree[20][200000];
unsigned long long queryTree2[20][200000];
unsigned long long prime = 999999937;
unsigned long long prime2 = 999999929;
unsigned long long p = 31;
unsigned long long p2 = 37;
unsigned long long p_pow[200000];
unsigned long long p_pow2[200000];

int power_of_two(int x) {
  int result = 1;
  for (int i = 0; i < x; i++)
    result = 2 * result;
  return result;
}

int floor_log2(int n) {
  int result = 0;
  int current = 1;
  
  while (current <= n) {
    current *= 2;  
    result += 1;
  }
  
  return result - 1;
}

void insert_hash(unsigned long long hash, int position, int tree_level) {
  unsigned long long index = hash;
  if (H[index] == -1) {
    P[index] = position;
    H[index] = tree_level;
    return;
  }
  CP[index][QC[index]] = position;
  CH[index][QC[index]] = tree_level;
  QC[index]++;
}

void hash_tree(const char *s, int tree_level) {
  tree[0][0] = (s[0] - 'A' + 1) % prime;
  insert_hash(tree[0][0], 0, 0);
  tree2[0][0] = (s[0] - 'A' + 1) % prime2;
  for (int i = 1; i <= tree_level; i++) {
    int quantity_of_nodes = power_of_two(i);
    tree[i][0] = (s[quantity_of_nodes - 1] - 'A' + 1) % prime;
    if (i < tree_level)
      insert_hash(tree[i][0], quantity_of_nodes - 1, i);
    tree2[i][0] = (s[quantity_of_nodes - 1] - 'A' + 1) % prime2;
    for (int j = 0; j < quantity_of_nodes - 1; j++) {
      if (i != tree_level) {
        insert_hash(((s[quantity_of_nodes + j] - 'A' + 1)) % prime, quantity_of_nodes + j, i);
      }
      tree[i][j + 1] = (tree[i][j] + (s[quantity_of_nodes + j] - 'A' + 1) * p_pow[j + 1]) % prime;
      tree2[i][j + 1] = (tree2[i][j] + (s[quantity_of_nodes + j] - 'A' + 1) * p_pow2[j + 1]) % prime2;
    }
  }
}

void hash_query(const char *s, int tree_level) {
  queryTree[0][0] = (s[0] - 'A' + 1) % prime;
  queryTree2[0][0] = (s[0] - 'A' + 1) % prime2;
  for (int i = 1; i <= tree_level; i++) {
    int quantity_of_nodes = power_of_two(i);
    queryTree[i][0] = (s[quantity_of_nodes - 1] - 'A' + 1) % prime;
    queryTree2[i][0] = (s[quantity_of_nodes - 1] - 'A' + 1) % prime2;
    for (int j = 0; j < quantity_of_nodes - 1; j++) {
      queryTree[i][j + 1] = (queryTree[i][j] + (s[quantity_of_nodes + j] - 'A' + 1) * p_pow[j + 1]) % prime;
      queryTree2[i][j + 1] = (queryTree2[i][j] + (s[quantity_of_nodes + j] - 'A' + 1) * p_pow2[j + 1]) % prime2;
    }
  }
}

int find_hash(int tree_position, int tree_level, int query_level, int quantity_query_level, int iterations) {
  if (iterations == 0) {
    return 1;
  }

  unsigned long long treeHash = (tree[tree_level][tree_position + quantity_query_level - 1] + prime) % prime;
  unsigned long long treeHash2 = (tree2[tree_level][tree_position + quantity_query_level - 1] + prime2) % prime2;
  unsigned long long queryHash = queryTree[query_level][quantity_query_level - 1] % prime; 
  unsigned long long queryHash2 = queryTree2[query_level][quantity_query_level - 1] % prime2; 
  if (tree_position > 0) {
    treeHash = (tree[tree_level][tree_position + quantity_query_level - 1] - tree[tree_level][tree_position - 1] + prime) % prime;
    treeHash2 = (tree2[tree_level][tree_position + quantity_query_level - 1] - tree2[tree_level][tree_position - 1] + prime2) % prime2;
    queryHash = queryTree[query_level][quantity_query_level - 1] * p_pow[tree_position] % prime; 
    queryHash2 = queryTree2[query_level][quantity_query_level - 1] * p_pow2[tree_position] % prime2; 
  }

  if (treeHash == queryHash && treeHash2 == queryHash2) {
    if (find_hash(tree_position * 2, tree_level + 1, query_level + 1, quantity_query_level * 2, iterations - 1)) {
      return 1;
    }
  }

  return 0;
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

  int N_NODOS;
  int x = fscanf(input_file, "%d\n", &N_NODOS);
  char arbol[N_NODOS + 1];
  x = fscanf(input_file, "%s\n", arbol);

  int N_CONSULTAS;
  x = fscanf(input_file, "%d\n", &N_CONSULTAS);

  p_pow[0] = 1;
  p_pow2[0] = 1;

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 200000; j++) {
      CP[i][j] = -1;
      CH[i][j] = -1;
      if (i == 0 && j > 0) {
        p_pow[j] = (p_pow[j - 1] * p) % prime;
        p_pow2[j] = (p_pow2[j - 1] * p2) % prime2;
      }
    }
    H[i] = -1;
    P[i] = -1;
    QC[i] = 0;
  }

  int tree_level = floor_log2(N_NODOS);
  hash_tree(arbol, tree_level);

  printf("\n");

  /* Eventos */
  for (int i = 0; i < N_CONSULTAS; i++) {
    int N_NODOS_CONSULTA;
    x = fscanf(input_file, "%d\n", &N_NODOS_CONSULTA);
    char consulta[N_NODOS_CONSULTA + 1];
    x = fscanf(input_file, "%s\n", consulta);
    first_print = 1;
    int query_level = floor_log2(N_NODOS_CONSULTA);
    hash_query(consulta, query_level);
    
    int max_pos = tree_level - query_level + 1;
    int level = H[queryTree[0][0]];
    int position = P[queryTree[0][0]];
    if (level != -1 && max_pos > level) {
      if (find_hash(2 * (position - power_of_two(level) + 1), level + 1, 1, 2, query_level)) {
        if (first_print) {
          fprintf(output_file, "%d", position);
          first_print = 0;
        } else {
          fprintf(output_file, " %d", position);
        }
      }
      int index = 1;
      position = CP[queryTree[0][0]][0];
      level = CH[queryTree[0][0]][0];
      for (int j = QC[queryTree[0][0]]; j > 0 && max_pos > level; j--, index++) {
        if (find_hash(2 * (position - power_of_two(level) + 1), level + 1, 1, 2, query_level)) {
          if (first_print) {
            fprintf(output_file, "%d", position);
            first_print = 0;
          } else {
            fprintf(output_file, " %d", position);
          }
        }
        position = CP[queryTree[0][0]][index];
        level = CH[queryTree[0][0]][index];
      }
    }

    if (first_print) {
      fprintf(output_file, "-1");
    }
    
    fprintf(output_file, "\n");
  }

  /* Cerra archivos */
  fclose(input_file);
  fclose(output_file);

  printf("\n%d\n", x);
  
  return 0;
}