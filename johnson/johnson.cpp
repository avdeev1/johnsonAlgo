#include <boost/property_map/property_map.hpp> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp> 

#include Boost.h

using namespace boost;

typedef property<boost::edge_weight_t, int> EdgeWeight;

typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeWeight > Graph;

void JohnsonBoost(GraphMatrix *gr, int *dist){
  int v = gr->sizeV;
  Graph g;
  for(int i = 0; i < v; i++) {
    int okr_s = gr->pointerB[i ]; int okr_f = gr->pointerB[i + 1];
    for(int okr_i = okr_s; okr_i < okr_f; okr_i++) add_edge(i, gr->column[okr_i],
    EdgeWeight(gr->value[okr_i]), g);
  }

  int **d = new int*[v];
  for(int i=0; i<v; i++)
  d[i] = new int[v];

  // use Boost
  johnson_all_pairs_shortest_paths(g, d);

  for(int i=0; i<v; i++)
     for(int j=0; j<v; j++)
       dist[i*v+j] = d[i][j];

  for(int i=0; i<v; i++)
     delete[] d[i];
  delete [] d;
}