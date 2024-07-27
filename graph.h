#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>  // enables srand(time(NULL)) to make pseudorandom generator better
#include "graph.h"

typedef struct {
	int** weights_from_to;
	size_t num_vertices;
} Graph;

// These are provided for you.
Graph create_graph_with_no_edges(size_t num_vertices);
void print_graph(char const* label, Graph graph);
void free_graph(Graph* a_graph);

// You fill in these functions.
int get_out_degree(Graph graph, int idx_of_vertex_from);
int get_in_degree(Graph graph, int idx_of_vertex_to);
Graph create_random_complete_graph(size_t num_vertices);
Graph create_random_cycle_graph(size_t num_vertices);
Graph create_random_wheel_graph(size_t num_vertices);
Graph create_random_graph(size_t num_vertices, size_t num_edges);
Graph create_random_cycle_graph(size_t num_vertices);
bool is_strongly_connected(Graph graph);

#define VERSION_GRAPH_H 1
#endif /* end of include guard: __GRAPH_H__ */
