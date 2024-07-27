#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <time.h> // enables srand(time(NULL)) to make pseudorandom generator better
#include "graph.h"

// HELPER FUNCTIONS
int _get_random_weight();
void _shuffle_idxs(size_t *idxs, size_t num_idxs);
size_t *_get_shuffled_idxs(size_t num_idxs);

int get_in_degree(Graph graph, int idx_of_vertex_to)
{
	int in_degree = 0;
	for (int i = 0; i < graph.num_vertices; i++)
	{
		if (graph.weights_from_to[i][idx_of_vertex_to] != 0)
		{
			in_degree++;
		}
	}

	return in_degree;
}

int get_out_degree(Graph graph, int idx_of_vertex_from)
{
	int out_degree = 0;
	if (idx_of_vertex_from >= 0 && idx_of_vertex_from < graph.num_vertices)
	{
		for (int i = 0; i < graph.num_vertices; i++)
		{
			if (graph.weights_from_to[idx_of_vertex_from][i] != 0)
			{
				out_degree++;
			}
		}
	}

	return out_degree;
}

Graph create_random_complete_graph(size_t num_vertices)
{
	Graph graph = create_graph_with_no_edges(num_vertices);

	for (size_t i = 0; i < num_vertices; i++)
	{
		for (size_t j = 0; j < num_vertices; j++)
		{
			if (i != j)
			{
				graph.weights_from_to[i][j] = _get_random_weight();
			}
		}
	}

	return graph;
}

Graph create_random_cycle_graph(size_t num_vertices)
{
	assert(num_vertices >= 2);
	Graph graph = create_graph_with_no_edges(num_vertices);

	// There should be exactly one Euler circuit, and no other edge.
	// Order of edges should be random.
	// Weights should be random.
	// In-degree and out-degree of every node should be 2.
	// Do not add any self-loops (i.e., edge from node to itself.)

	size_t *idxs = _get_shuffled_idxs(num_vertices);

	for (size_t i = 0; i < num_vertices - 1; i++)
	{
		graph.weights_from_to[idxs[i]][idxs[i + 1]] = _get_random_weight();
	}

	graph.weights_from_to[idxs[num_vertices - 1]][idxs[0]] = _get_random_weight();

	free(idxs);

	return graph;
}

Graph create_random_wheel_graph(size_t num_vertices)
{
	assert(num_vertices >= 3);
	Graph graph = create_graph_with_no_edges(num_vertices);
	// Resulting wheel graph should have one hub, with edges between that hub and every other vertex
	// in both directions. Outer vertices should be have edges in only one direction

	// Order should be random
	// Weights should be random
	// Choose one hub node. There should be a pair of edges between the hub node and every other node—in both directions
	// In-degree and out-degree of the hub node should be num_vertices - 1
	// In-degree and out-degree of every other node should be 2
	// Do not add any self-loops (i.e., edge from node to itself)

	// Choose one hub node
	size_t *idxs = _get_shuffled_idxs(num_vertices);
	size_t hub = idxs[0];

	// Connect hub to all other nodes with two edges each.
	for (size_t i = 0; i < num_vertices; i++)
	{
		if (i != hub)
		{
			graph.weights_from_to[i][hub] = _get_random_weight();
			graph.weights_from_to[hub][i] = _get_random_weight();
		}
	}

	// Connect all other nodes to the next node in the list, except for the last node
	for (size_t i = 1; i < num_vertices - 1; i++)
	{
		graph.weights_from_to[idxs[i]][idxs[i + 1]] = _get_random_weight();
	}

	// Connect the last node to the second node in the list.
	graph.weights_from_to[idxs[num_vertices - 1]][idxs[1]] = _get_random_weight();

	free(idxs);

	return graph;
}

Graph create_random_graph(size_t num_vertices, size_t num_edges)
{
	// Create a graph with num_vertices and num_edges edges between randomly selected pairs of vertices
	// Create num_edges edges between randomly selected pairs of vertices
	// Weights should be random
	// The resulting graph may or may not be (strongly or weakly) connected

	Graph graph = create_graph_with_no_edges(num_vertices);

	for (size_t _ = 0; _ < num_edges; _++)
	{
		size_t vertex_from = rand() % num_vertices;
		size_t vertex_to = rand() % num_vertices;
		graph.weights_from_to[vertex_from][vertex_to] = _get_random_weight();
	}

	return graph;
}

void _dfs(Graph graph, int *visited, int vertex)
{
	visited[vertex] = 1;
	for (int i = 0; i < graph.num_vertices; i++)
	{
		if (graph.weights_from_to[vertex][i] != 0 && visited[i] == 0) // if there is an edge from vertex to i and i has not been visited
		{
			_dfs(graph, visited, i);
		}
	}
}

bool is_strongly_connected(Graph graph)
{
	// instructor solution: 21 sloc
	// my solution: 18 sloc
	int *visited = calloc(graph.num_vertices, sizeof(*visited));

	Graph transposedGraph = create_graph_with_no_edges(graph.num_vertices);
	for (int i = 0; i < graph.num_vertices; i++)
	{
		for (int j = 0; j < graph.num_vertices; j++)
		{
			transposedGraph.weights_from_to[i][j] = graph.weights_from_to[j][i];
		}
	}

	// DFS
	// Pick a vertex, mark it as visited, and then visit all of its neighbors
	// If we visit all vertices for graph and transposed graph, then the graph is strongly connected
	// If we don't visit all vertices, then the graph is not strongly connected

	_dfs(graph, visited, 0);

	for (int i = 0; i < graph.num_vertices; i++)
	{
		return visited[i] == 0 ? false : true;
	}

	visited = calloc(graph.num_vertices, sizeof(*visited));

	_dfs(transposedGraph, visited, 0);

	for (int i = 0; i < graph.num_vertices; i++)
	{
		return visited[i] == 0 ? false : true;
	}

	free(visited);

	return true;
	
}

Graph create_graph_with_no_edges(size_t num_vertices)
{
	// Create a graph containing vertices but no edges.  Note:  weight of 0 indicates no edge.
	Graph graph = {
		.num_vertices = num_vertices,
		.weights_from_to = malloc((sizeof *graph.weights_from_to) * num_vertices + (sizeof **graph.weights_from_to) * num_vertices * num_vertices)};
	int *a_weight_0_to_0 = (int *)(&graph.weights_from_to[num_vertices]);
	for (size_t idx_of_vertex_from = 0; idx_of_vertex_from < num_vertices; idx_of_vertex_from++)
	{
		graph.weights_from_to[idx_of_vertex_from] = a_weight_0_to_0 + idx_of_vertex_from * num_vertices;
		for (int vertex_idx_to = 0; vertex_idx_to < num_vertices; vertex_idx_to++)
		{
			graph.weights_from_to[idx_of_vertex_from][vertex_idx_to] = 0;
		}
	}
	/* This "matrix" is stored as an array of arrays.  First, there is an array of addresses to
	 * each array (row).  Then, there are 'num_vertices' arrays (one for each row of the matrix).
	 * To reduce calls to malloc(..) and simplify freeing, we combine all of that into one block.
	 *
	 * You can simply refer to it the same as you would a 2D array.
	 * Ex: graph.weights_from_to[2][3] = 5;
	 *     // creates an edge from vertex 2 to vertex 3 with weight 5.
	 */
	return graph;
}

void print_graph(char const *label, Graph graph)
{
	// Print the graph as text. including the in-degree and out-degree.
	// ∙ Okay to modify this to your liking, if you wish.
	char *labels[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
					  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
	size_t num_labels = sizeof labels / sizeof labels[0];
	assert(graph.num_vertices < num_labels); // add more labels above, if you want to do larger graphs

	printf("%s\n", label);
	for (size_t idx_vertex = 0; idx_vertex < graph.num_vertices; idx_vertex++)
	{
		printf("∙ %s ", labels[idx_vertex]);
		int in_degree = get_in_degree(graph, idx_vertex);
		int out_degree = get_out_degree(graph, idx_vertex);
		printf("(in=%d/out=%d): ", in_degree, out_degree);
		bool found_edge = false;
		for (size_t vertex_idx_to = 0; vertex_idx_to < graph.num_vertices; vertex_idx_to++)
		{
			if (idx_vertex != vertex_idx_to)
			{
				int weight = graph.weights_from_to[idx_vertex][vertex_idx_to];
				if (weight >= 1)
				{
					if (found_edge)
					{
						printf(", ");
					}
					else
					{
						found_edge = true;
					}
					printf("→%s (%d)", labels[vertex_idx_to], weight);
				}
			}
		}
		if (!found_edge)
		{
			printf("(no edges)");
		}
		printf("\n");
	}
	printf("\n");
}

void free_graph(Graph *a_graph)
{
	// Free the matrix (array of arrays) the 'Graph' object refers to.
	free(a_graph->weights_from_to);
	a_graph->weights_from_to = NULL;
	a_graph->num_vertices = 0;
}

int _get_random_weight()
{
	// Return a random int from 1 to MAX_WEIGHT, inclusive.
	int MAX_WEIGHT = 5;
	int random_weight = rand() % MAX_WEIGHT + 1;
	assert(random_weight >= 0 && random_weight <= MAX_WEIGHT);
	return random_weight;
}

size_t *_get_shuffled_idxs(size_t num_idxs)
{
	// Return an array of indexes (numbers) from 0 to (num_idxs - 1), in random order.
	// ∙ Caller is responsible for freeing memory.
	// ∙ You can call _shuffle_idxs(…) to shuffle the array again in-place without recreating it.
	size_t *idxs = malloc(num_idxs * sizeof *idxs);
	for (size_t i = 0; i < num_idxs; i++)
	{
		idxs[i] = i;
	}
	_shuffle_idxs(idxs, num_idxs);
	for (int i = 0; i < num_idxs; i++)
	{
		printf("%zd%s", idxs[i], i + 1 < num_idxs ? ", " : "\n");
	}
	return idxs;
}

void _shuffle_idxs(size_t *idxs, size_t num_idxs)
{
	// Re-shuffle an array of indexes created with _get_shuffled_idxs(…).
	for (size_t src_pos = num_idxs - 1; src_pos >= 1; src_pos--)
	{
		size_t dst_pos = rand() % src_pos;
		size_t copy_of_dst = idxs[dst_pos];
		idxs[dst_pos] = idxs[src_pos];
		idxs[src_pos] = copy_of_dst;
	}
}

#define VERSION_GRAPH_C 1
/* vim: set tabstop=4 shiftwidth=4 fileencoding=utf-8 noexpandtab: */
