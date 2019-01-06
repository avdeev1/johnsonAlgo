#ifndef DATA_STRUCT
#define DATA_STRUCT
struct GraphMatrix {
  int *pointerB;  // указатели на начало списка связанных
                  // ребер
  int *column;    // индексы связанных вершин
  int *value;     // веса ребер
  int sizeV;      // количество вершин
  int sizeE;      // количество ребер 
}; #endif