[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/QqzYvsyP)
# Tarea 2 - 2024-1

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
./find_sequences input.txt output.txt
```

## Parte 2

```
./organizer input.txt output.txt
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
├── find_sequences/
│   ├── main.c
│   ... Otros archivos de la parte 1
├── organizer/
│   ├── main.c
│   ... Otros archivos de la parte 2
├── patterns/
│   ├── main.c
│   ... Otros archivos de la parte 3

```
