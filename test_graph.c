#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>  // enables srand(time(NULL)) to make pseudorandom generator better
#include "graph.h"

Graph create_non_random_wheel_graph() {
	Graph wheel_graph = create_graph_with_no_edges(5);
	wheel_graph.weights_from_to[0][1] = 2;
	wheel_graph.weights_from_to[1][0] = 2;
	wheel_graph.weights_from_to[0][2] = 2;
	wheel_graph.weights_from_to[2][0] = 2;
	wheel_graph.weights_from_to[0][3] = 2;
	wheel_graph.weights_from_to[3][0] = 2;
	wheel_graph.weights_from_to[0][4] = 2;
	wheel_graph.weights_from_to[4][0] = 2;
	wheel_graph.weights_from_to[1][2] = 2;
	wheel_graph.weights_from_to[2][3] = 2;
	wheel_graph.weights_from_to[3][4] = 2;
	wheel_graph.weights_from_to[4][1] = 2;
	return wheel_graph;
}

Graph create_non_random_cycle_graph() {
	Graph cycle_graph = create_graph_with_no_edges(4);
	cycle_graph.weights_from_to[0][1] = 2;
	cycle_graph.weights_from_to[1][2] = 2;
	cycle_graph.weights_from_to[2][3] = 2;
	cycle_graph.weights_from_to[3][0] = 2;
	return cycle_graph;
}

void demo() {
	// These are not random graphs.  This is just to illustrate what some of the graph types
	// will look like when it works.
	Graph wheel_graph = create_non_random_wheel_graph();
	print_graph("Wheel (not random)", wheel_graph);
	//assert(is_strongly_connected(wheel_graph));
	free_graph(&wheel_graph);

	Graph cycle_graph = create_non_random_cycle_graph();
	print_graph("Cycle (not random)", cycle_graph);
	//assert(is_strongly_connected(cycle_graph));
	free_graph(&cycle_graph);
	/*
	After completing the get_in_degree(…) and get_out_degree(…), output should look like this:
	--
	Wheel (not random)
	∙ A (in=4/out=4): →B (2), →C (2), →D (2), →E (2)
	∙ B (in=2/out=2): →A (2), →C (2)
	∙ C (in=2/out=2): →A (2), →D (2)
	∙ D (in=2/out=2): →A (2), →E (2)
	∙ E (in=2/out=2): →A (2), →B (2)

	Cycle (not random)
	∙ A (in=1/out=1): →B (2)
	∙ B (in=1/out=1): →C (2)
	∙ C (in=1/out=1): →D (2)
	∙ D (in=1/out=1): →A (2)
	*/

	Graph random_complete_graph = create_random_complete_graph(4);
	Graph random_cycle_graph = create_random_cycle_graph(4) ;
	print_graph ("COMPLETE GRAPH\n\n", random_complete_graph) ; 
	print_graph("CYCLE GRAPH\n\n", random_cycle_graph);
	free_graph (&random_complete_graph); 
	free_graph (&random_cycle_graph);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	demo();

	return EXIT_SUCCESS;
}
#define VERSION_TEST_GRAPH_C 1
