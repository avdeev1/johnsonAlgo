#include "routine.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <vector> 
#include <algorithm>

#define BUF_SIZE 200

struct TEdge {
  int row;
  int col;
  int val;
};

bool CmpEdges( TEdge arg1, TEdge arg2 ) {
  if(arg1.row<arg2.row)
    return true;
  else
    if(arg1.row == arg2.row) {
          if(arg1.col < arg2.col)
            return true;
    }
  return false; 
}

int ParseGraph(char* graphPath, GraphMatrix **gr) {

  // Если функция fopen вернѐт 0, то завершим функцию ParseGraph с кодом ошибки -1.
  FILE * f;
  int sizeV;
  f = fopen(graphPath, "r");
  if( f == NULL ) {
    printf("The file (%s) was not opened\n", graphPath);
    return -1; 
  }

  // контейнер для временного хранения ребер
  GraphMatrix *graph = new GraphMatrix;
  (*gr) = graph;
  std::vector<TEdge> edges;

  // загрузим из файла все рѐбра в контейнер edges и отсортируем, используя для сравнения функцию CmpEdges.
  while(!feof(f)) {
    char buf[BUF_SIZE];
    char val_buf[4][BUF_SIZE/4];

    memset(buf, 0, sizeof(buf));
    fgets(buf, BUF_SIZE, f);

    if(buf[0] != 'c' && buf[0] != 0) {
       memset(val_buf, 0, sizeof(val_buf));
       int count = sscanf(buf,"%s %s %s %s", val_buf[0], val_buf[1], val_buf[2], val_buf[3]);
       
       if(count != 4) {
        printf("The file (%s) was corrupted on line (%s)\n", graphPath, buf);
        return -2; 
        }

      if(val_buf[0][0] == 'p') sizeV = atoi(val_buf[2]);
      if(val_buf[0][0] == 'a') {
        TEdge e = {atoi(val_buf[1]) - 1,
        atoi(val_buf[2]) - 1, atoi(val_buf[3])};
        edges.push_back(e);
      }
    } 
  }

  std::sort(edges.begin(), edges.end(), CmpEdges);

  // Удаляем дубликаты рѐбер
  // (оставляем ребро минимального веса)
  for (std::vector<TEdge>::iterator i = edges.begin(); i != edges.end()-1;) {
      std::vector<TEdge>::iterator j = i+1;

    if( ((*i).row == (*j).row) && ((*i).col == (*j).col)) {
      if((*j).val < (*i).val)
        i = edges.erase(i);
      else {
        i = edges.erase(j);
        i--; 
      }
    } else i++; 
  }
  
  // Заполняем структуру GraphMatrix.
  graph->sizeV = sizeV;
  graph->sizeE = edges.size(); graph->pointerB = new int[graph->sizeV + 1]; graph->column = new int[graph->sizeE]; graph->value = new int[graph->sizeE]; memset(graph->pointerB, 0, sizeof(int) * (graph->sizeV + 1));
    for(std::vector<TEdge>::iterator i = edges.begin(); i != edges.end();  i++) {
      int row = (*i).row;
      graph->pointerB[row]++;
  }

  int sum = 0;

  for(int i = 0; i < graph->sizeV; i++) {
    int tmp = graph->pointerB[i]; graph->pointerB[i] = sum;
    sum += tmp;
  }

  graph->pointerB[graph->sizeV] = sum;
  int *counter = new int[graph->sizeV];
  for (int i = 0; i < graph->sizeV; i++)
    counter[i] = 0;

  for(std::vector<TEdge>::iterator i = edges.begin(); i != edges.end();  i++) {
    int pos;
    int row = (*i).row;
    int col = (*i).col;
    int val = (*i).val;
    pos = graph->pointerB[row] + counter[row];
    graph->column[pos] = col;
    graph->value [pos] = val;
    counter[row]++;
  }

  printf("Graph from file (%s) has loaded.\n", graphPath);
  printf("Vertices: %i, Edges: %i.\n", graph->sizeV, graph->sizeE);
  fflush(stdout);

  delete []counter;
  fclose(f);

  return 0; 
}