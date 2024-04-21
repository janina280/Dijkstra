// #include <fstream>
// #include <iostream>
// #include <vector>
// #include <string>
// #include <chrono>
// #include <omp.h>
//
// #define INFINITE 1000000
// #define OUTPUT_FILENAME "output_openMPI.txt"
//
// typedef struct {
// 	int    no_of_nodes;
// 	int    source_node;
// 	std::vector<std::vector<int>> adjacency_matrix;
// } graph;
//
//
// /*
//  * Read the graph from a file.
//  */
// void read_graph(const std::string& filename, graph& my_graph) {
// 	std::ifstream in(filename);
// 	in >> my_graph.no_of_nodes;
// 	in >> my_graph.source_node;
// 	my_graph.adjacency_matrix.resize(my_graph.no_of_nodes);
//
// 	for (auto& row : my_graph.adjacency_matrix) {
// 		row.resize(my_graph.no_of_nodes);
// 		for (auto& iterator : row) {
// 			in >> iterator;
// 		}
// 	}
//
// 	in.close();
// }
//
// /*
//  * Returns the index for current minimum node.
//  */
// int get_index_for_min_distance(const int distances[], const bool visited[], int no_of_nodes) {
// 	int min = INFINITE;
// 	int min_index = -1;
//
// 	for (int v = 0; v < no_of_nodes; v++) {
// 		if (!visited[v] && distances[v] <= min) {
// 			min = distances[v];
// 			min_index = v;
// 		}
// 	}
// 	return min_index;
// }
//
// // Function to print the distance array 
// void write_solution(const int distances[], const graph& my_graph)
// {
// 	std::ofstream out(OUTPUT_FILENAME);
// 	out << "Vertex \tDistance from " << my_graph.source_node << "(source node)" << '\n';
// 	for (int i = 0; i < my_graph.no_of_nodes; i++) {
// 		out << i << " \t\t" << distances[i] << '\n';
// 	}
//
// 	out.close();
// }
//
// /*
//  * Implementation for Dijkstra's algorithm.
//  */
// void dijkstra(graph& myGraph, int src)
// {
// 	//char graph[V][V] = G->node;
// 	int* distances = static_cast<int*>(malloc(myGraph.no_of_nodes * sizeof(int)));
// 	bool* visited = static_cast<bool*>(malloc(myGraph.no_of_nodes * sizeof(bool)));
// 	int numVisited = 0;
//
// 	for (int i = 0; i < myGraph.no_of_nodes; i++) {
// 		distances[i] = INFINITE;
// 		visited[i] = false;
// 	}
//
// 	distances[src] = 0;
//
// 	int v = 0;
// 	int u = 0;
// #pragma omp parallel private(v,u)
// 	{
// 		while (numVisited < myGraph.no_of_nodes) {
// 			u = get_index_for_min_distance(distances, visited, myGraph.no_of_nodes);
// 			visited[u] = true;
// 			numVisited++;
// #pragma omp parallel for
// 			for (v = 0; v < myGraph.no_of_nodes; v++) {
// 				// Update dist[v] only if total weight of path from src to v through u is smaller than current value of dist[v] 
// 				if (!visited[v] && myGraph.adjacency_matrix[u][v] && distances[u] != INFINITE && distances[u] + myGraph.adjacency_matrix[u][v] < distances[v]) {
// 					distances[v] = distances[u] + myGraph.adjacency_matrix[u][v];
// 				}
// 			}
// 		}
// 	}
//
// 	write_solution(distances, myGraph);
//
// 	free(distances);
// 	free(visited);
// }
//
// /*
//  * author: Cocei Janina
//  */
// int main()
// {
// 	graph my_graph;
//
// 	read_graph("input_openMPI.txt", my_graph);
//
// 	const auto start_time = std::chrono::high_resolution_clock::now();
// 	dijkstra(my_graph, my_graph.source_node);
// 	const auto stop_time = std::chrono::high_resolution_clock::now();
// 	const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
// 	std::cout << "Time: " << duration.count() << " microseconds." << std::endl;
// }