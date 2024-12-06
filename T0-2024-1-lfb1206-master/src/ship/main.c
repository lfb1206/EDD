#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct planet {
	int number;
	int orders_received;
} Planet;

typedef struct order {
	int id;
	int planetid;
	struct order* next;
} Order;

typedef struct ship {
	int id;
	Order* next_order;
} Ship;

Planet* planet_init(int number)
{
	Planet* new_planet = (Planet*)malloc(sizeof(Planet));
	new_planet->number = number;
	new_planet->orders_received = 0;
	return new_planet;
}

void receive_order(Planet* planet)
{
	planet->orders_received++;
}

void planet_destroy(Planet* planet_to_destroy)
{
	free(planet_to_destroy);
}

Ship* ship_init(int id)
{
	Ship* new_ship = malloc(sizeof(Ship));
	new_ship->id = id;
	new_ship->next_order = NULL;
	return new_ship;
}

void ship_add_order(Ship* ship, Order* new_order)
{
	if (ship->next_order)
	{
		Order* last_order = ship->next_order;
		while (last_order->next)
		{
			last_order = last_order->next;
		}
		last_order->next = new_order;
	}
	else
	{
		ship->next_order = new_order;
	}
}

void ship_add_order_after_max_order(Ship* ship, Order* new_order)
{
	if (ship->next_order)
	{
		Order* last_order = ship->next_order;
		Order* max_id_order = ship->next_order;
		while (last_order->next)
		{
			last_order = last_order->next;
      if (last_order->id > max_id_order->id)
      {
        max_id_order = last_order;
      }
		}
    new_order->next = max_id_order->next;
		max_id_order->next = new_order;
	}
	else
	{
		ship->next_order = new_order;
	}
}

int count_ship_pending_orders(Ship* ship_to_count)
{
	int count = 0;
  Order* current_order = ship_to_count->next_order;
	while (current_order)
  {
    current_order = current_order->next;
		count++;
  }
	return count;
}

int ship_remove_order(Ship* ship_to_remove_order, int order_id)
{
	Order* current_order = ship_to_remove_order->next_order;
	Order* previous_order = NULL;
	while (current_order)
	{
		if (current_order->id == order_id)
		{
			if (previous_order)
			{
				previous_order->next = current_order->next;
			}
			else
			{
				ship_to_remove_order->next_order = current_order->next;
			}
			free(current_order);
			return 1;
		}
		previous_order = current_order;
		current_order = current_order->next;
	}
	return 0;
}

void order_destroy(Order* order_to_destroy)
{
	if (order_to_destroy->next)
	{
		order_destroy(order_to_destroy->next);
	}
	free(order_to_destroy);
}

void ship_destroy(Ship* ship_to_destroy)
{
	if (ship_to_destroy->next_order)
	{
		order_destroy(ship_to_destroy->next_order);
	}
	free(ship_to_destroy);
}

Order* order_init(int id, int planetid)
{
	Order* new_order = (Order*)malloc(sizeof(Order));
	new_order->id = id;
	new_order->planetid = planetid;
	new_order->next = NULL;
	return new_order;
}

static bool string_equals(char *string1, char *string2) {
  return strcmp(string1, string2) == 0;
}

void planets_init(Planet* planets[], int size)
{
  for (int i = 0; i < size; i++)
    planets[i] = planet_init(i);
}

void ships_init(Ship* ships[], int size)
{
  for (int i = 0; i < size; i++)
    ships[i] = ship_init(i);
}

Planet** sort_planets_by_orders_received(Planet** planets, int size)
{
  Planet** sorted_planets = calloc(size, sizeof(Planet*));
  for (int i = 0; i < size; i++)
  {
    sorted_planets[i] = planets[i];
  }
  for (int i = 0; i < size; i++)
    for (int j = i + 1; j < size; j++)
    {
      if (sorted_planets[i]->orders_received < sorted_planets[j]->orders_received) {
        Planet* temp = sorted_planets[i];
        sorted_planets[i] = sorted_planets[j];
        sorted_planets[j] = temp;
      } else if (sorted_planets[i]->orders_received == sorted_planets[j]->orders_received && sorted_planets[i]->number > sorted_planets[j]->number) {
        Planet* temp = sorted_planets[i];
        sorted_planets[i] = sorted_planets[j];
        sorted_planets[j] = temp;
      } 
  }
  return sorted_planets;
}

void invert_ship_orders(Ship* ship)
{
  Order* current_order = ship->next_order;
  Order* previous_order = NULL;
  Order* next_order = NULL;
  while (current_order)
  {
    next_order = current_order->next;
    current_order->next = previous_order;
    previous_order = current_order;
    current_order = next_order; 
  }
  ship->next_order = previous_order;
}

int count_ship_orders_to_a_certain_planet(Ship* ship, int planet_id)
{
  int count = 0;
  Order* current_order = ship->next_order;
  while (current_order)
  {
    if (current_order->planetid == planet_id)
      count++;
    current_order = current_order->next;
  }
  return count;
}

int transfer_ship_orders_to_a_certain_planet(Ship* receiving_ship, Ship* transfering_ship, int planet_id)
{
  Order* current_order = transfering_ship->next_order;
	Order* next_order = current_order->next;
	Order* previous_order = NULL;
  int count = 0;
  while (current_order)
  {
    if (current_order->planetid == planet_id)
    {
      next_order = current_order->next;
      if (previous_order)
        previous_order->next = next_order;
      else
        transfering_ship->next_order = next_order;
      current_order->next = NULL;
      ship_add_order(receiving_ship, current_order);
      current_order = next_order;
      count++;
    }
    else
    {
      previous_order = current_order;
      current_order = current_order->next;
    }
  }
	return count;
}

int coordinate_ships_orders(Ship* first_ship, Ship* second_ship, int planet_id)
{
  int first_ship_count = count_ship_orders_to_a_certain_planet(first_ship, planet_id);
  int second_ship_count = count_ship_orders_to_a_certain_planet(second_ship, planet_id);
  if (first_ship_count == 0 || second_ship_count == 0)
    return 0;
  else if (first_ship_count < second_ship_count)
    return transfer_ship_orders_to_a_certain_planet(second_ship, first_ship, planet_id);
  else
    return transfer_ship_orders_to_a_certain_planet(first_ship, second_ship, planet_id);
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

  int N_PLANETAS;
  int N_NAVES;
  int N_EVENTOS;

  int result;
  result = fscanf(input_file, "%d", &N_PLANETAS);
  if (result != 1) {
    printf("Error leyendo la cantidad de planetas");
    return 1;
  }
  result = fscanf(input_file, "%d", &N_NAVES);
  if (result != 1) {
    printf("Error leyendo la cantidad de naves");
    return 1;
  }
  result = fscanf(input_file, "%d", &N_EVENTOS);
  if (result != 1) {
    printf("Error leyendo la cantidad de eventos");
    return 1;
  }

  char command[32];

  Planet** planets = calloc(N_PLANETAS, sizeof(Planet*));
  planets_init(planets, N_PLANETAS);

  Ship** ships = calloc(N_NAVES, sizeof(Ship*));
  ships_init(ships, N_NAVES);

  for (int i = 0; i <= N_EVENTOS; i++)
  {
    fgets(command, 32, input_file);
    char *input = strtok(command, " ");
    if (string_equals(input, "REGISTRAR-PEDIDO")) {
      int order_id = atoi(strtok(NULL, " "));
      int ship_id = atoi(strtok(NULL, " "));
      int planet_id = atoi(strtok(NULL, " "));
      Order* order_to_register = order_init(order_id, planet_id);
      ship_add_order(ships[ship_id], order_to_register);
      fprintf(output_file, "REGISTRADO PEDIDO %i\n", order_id);
    } else if (string_equals(input, "REPORTE-NAVE")) {
      int ship_id = atoi(strtok(NULL, " "));
      int pending_orders = count_ship_pending_orders(ships[ship_id]);
      if (pending_orders == 0)
        fprintf(output_file, "LA NAVE %i NO TIENE PEDIDOS PENDIENTES\n", ships[ship_id]->id);
      else
        fprintf(output_file, "PEDIDOS %i PENDIENTES: %i\n", ships[ship_id]->id, pending_orders);
    } else if (string_equals(input, "REPORTE-PEDIDOS\n")) {
      int count = 0;
      fprintf(output_file, "REPORTE-PEDIDOS\n");
      for (int i = 0; i < N_NAVES; i++)
      {
        Order* order_to_report = ships[i]->next_order;
        if (order_to_report)
        {
          fprintf(output_file, "    NAVE %i\n", ships[i]->id);
          while (order_to_report->next)
          {
            fprintf(output_file, "        PEDIDO %i CON PLANETA %i\n", order_to_report->id, order_to_report->planetid);
            count++;
            order_to_report = order_to_report->next;
          }
          fprintf(output_file, "        PEDIDO %i CON PLANETA %i\n", order_to_report->id, order_to_report->planetid);
          count++;
        }
      }
      fprintf(output_file, "TOTAL DE PEDIDOS: %i\n", count);
    } else if (string_equals(input, "PEDIDO-CONTAMINADO")) {
      int ship_id = atoi(strtok(NULL, " "));
      int order_id = atoi(strtok(NULL, " "));
      if (ship_remove_order(ships[ship_id], order_id))
        fprintf(output_file, "PEDIDO %i HA SIDO ELIMINADO\n", order_id);
      else
        fprintf(output_file, "PEDIDO %i NO ENCONTRADO EN NAVE %i\n", order_id, ships[ship_id]->id);
    } else if (string_equals(input, "ENTREGAR-PEDIDOS\n")) {
      int orders_delivered = 0;
      for (int i = 0; i < N_NAVES; i++)
      {
        Ship* delivering_ship = ships[i];
        Order* current_order = delivering_ship->next_order;
        if (current_order)
        {
          Planet* planet = planets[current_order->planetid];
          Order* current_order = delivering_ship->next_order;
          Order* next_order = NULL;
          while (current_order)
          {
            next_order = current_order->next;
            if (current_order->planetid == planet->number)
            {
              fprintf(output_file, "PEDIDO %i ENTREGADO EN PLANETA %i\n", current_order->id, planet->number);
              receive_order(planet);
              ship_remove_order(delivering_ship, current_order->id);
            }
            current_order = next_order;
          }
          orders_delivered++;
        }
      }
      if (orders_delivered == 0)
        fprintf(output_file, "NO HAY PEDIDOS POR ENTREGAR\n");
    } else if (string_equals(input, "REPORTE-PLANETAS\n")) {
      Planet** sorted_planets = sort_planets_by_orders_received(planets, N_PLANETAS);
      int count = 0;
      fprintf(output_file, "PLANETAS-ORDENADOS\n");
      for (int i = 0; i < N_PLANETAS; i++)
      {
        fprintf(output_file, "    PLANETA %i: %i pedidos\n", sorted_planets[i]->number, sorted_planets[i]->orders_received);
        count += sorted_planets[i]->orders_received;
      }
      fprintf(output_file, "TOTAL-PEDIDOS-ENTREGADOS: %i\n", count);
      free(sorted_planets);
    } else if (string_equals(input, "TOMAR-DESPUES-MAX")) {
      int order_id = atoi(strtok(NULL, " "));
      int ship_id = atoi(strtok(NULL, " "));
      int planet_id = atoi(strtok(NULL, " "));
      Order* order_to_register = order_init(order_id, planet_id);
      ship_add_order_after_max_order(ships[ship_id], order_to_register);
      fprintf(output_file, "REGISTRADO PEDIDO %i\n", order_id);
    } 
    else if (string_equals(input, "INVERSO")) {
      int ship_id = atoi(strtok(NULL, " "));
      invert_ship_orders(ships[ship_id]);
    } else if (string_equals(input, "COORDINAR-PEDIDOS")) {
      int ship1_id = atoi(strtok(NULL, " "));
      int ship2_id = atoi(strtok(NULL, " "));
      int planet_id = atoi(strtok(NULL, " "));
      int transfered_orders = coordinate_ships_orders(ships[ship1_id], ships[ship2_id], planet_id);
      int first_ship_pending_orders = count_ship_pending_orders(ships[ship1_id]);
      int second_ship_pending_orders = count_ship_pending_orders(ships[ship2_id]);
      fprintf(output_file, "PAQUETES TRANSFERIDOS: %i\n", transfered_orders);
      fprintf(output_file, "    NAVE %i: %i PEDIDOS\n", ship1_id, first_ship_pending_orders);
      fprintf(output_file, "    NAVE %i: %i PEDIDOS\n", ship2_id, second_ship_pending_orders);
    }
  }

  fclose(input_file);
  fclose(output_file);

  for (int i = 0; i < N_PLANETAS; i++)
    planet_destroy(planets[i]);

  for (int i = 0; i < N_NAVES; i++)
    ship_destroy(ships[i]);
  
  free(planets);
  free(ships);

  return 0;
}
