[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ISYWJ2Nb)
# Tarea 3 - 2024-1

Recuerden leer con calma todo el enunciado, y atento a los anuncios.
Realicen las preguntas con tiempo en [Discussions](https://github.com/orgs/IIC2133-PUC/discussions).

## Compilar

```
make
```

## Recompilar

```
make clean && make
```

## Parte 1

```
./nyctalus_search input.txt output.txt
```

## Parte 2

```
./trash input.txt output.txt
```

## Parte 3

```
./find_groups input.txt output.txt
```

## Comparar respuesta

```
diff -b correct_output.txt student_output.txt
```

El código base viene estructurado de la siguiente manera: 

```
src/
-----------------------------------------------------------------------
│   ARCHIVOS DE LA TAREA ( Donde escribirás tu código )               │
-----------------------------------------------------------------------
├── nyctalus_search/
│   ├── main.c
│   ... Otros archivos de la parte 1
├── trash/
│   ├── main.c
│   ... Otros archivos de la parte 2
├── find_groups/
│   ├── main.c
│   ... Otros archivos de la parte 3

```
