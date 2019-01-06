#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include "windows.h" 
#include <string>

#include "Boost.h"

using namespace std;

int main(int argc, char **argv) {
  GraphMatrix *gr;
  int *dist;
  bool printOutput = false; 
  LARGE_INTEGER freq;
  LARGE_INTEGER sQP, fQP; QueryPerformanceFrequency(&freq);
  
  if (argc < 2) {
    printf("\nUsage: program.exe <graph file> <Jo|FW> [-o]\n");
    return 1; 
    }

    ParseGraph(argv[1], &gr);

    if (argc == 3)
      if(string(argv[2]) == string("-o"))
        printOutput = true;

  dist = new int[gr->sizeV * gr->sizeV];
     QueryPerformanceCounter(&sQP);
     JohnsonBoost(gr, dist);
     QueryPerformanceCounter(&fQP);
     printf("Boost Johnson time: %f\n", (fQP.QuadPart-sQP.QuadPart) / (double)freq.QuadPart);
  
  if(printOutput) {
    FILE *distFile=fopen("01_dist.dat", "wb"); fwrite(dist, sizeof(int), gr->sizeV * gr->sizeV,
    distFile); fclose(distFile);
    printf("File (01_dist.dat) written.\n" ); 
  }

  delete[] gr->column; delete[] gr->pointerB; delete[] gr->value; delete gr;
  delete [] dist;

  return 0;
}