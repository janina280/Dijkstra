// #include <iostream>
// #include <chrono>
// #include <mpi.h>
// #include <fstream>
//
// #define INFINITE 1000000
//
// MPI_Datatype create_block_column(int, int);
// void share_matrix(int*, int[], int, MPI_Datatype, int, MPI_Comm);
// void dijkstra(int[], int[], int[], int, int, int, int, MPI_Comm);
// void init(const int[], int[], int[], int[], int, int, int);
// int get_minimum_distance(const int[], const int[], int);
// int get_node_id(int, int, int);
// void write_result(const std::string&, int[], int, int, int, MPI_Comm);
//
// std::chrono::steady_clock::time_point start_time;
//
// /*
// * @author Cocei_Janina
// */
// int main(int argc, char* argv[])
// {
//     int* adjacency_matrix = nullptr;
//     int* current_process_matrix = nullptr;
//     int* current_process_distances = nullptr;
//     int* current_process_pred = nullptr;
//     int no_of_nodes = 0;
//     int current_process_part = 0;
//     int source_node = 0;
//     int no_of_processes = 0;
//     int current_process_id = 0;
//     const std::string input_file = "input_MPI.txt";
//     const std::string output_file = "output_MPI.txt";
//
//     MPI_Comm communication;
//     MPI_Datatype MPI_BLOCK_TYPE;
//
//     MPI_Init(&argc, &argv);
//     communication = MPI_COMM_WORLD;
//     MPI_Comm_size(communication, &no_of_processes);
//     MPI_Comm_rank(communication, &current_process_id);
//
//     if (current_process_id == 0) {
//         std::fstream in(input_file);
//         in >> no_of_nodes;
//         in >> source_node;
//         if (no_of_nodes % no_of_processes) {
//             std::cout << "Number of nodes and number of processes must be divisible\n";
//             exit(0);
//         }
//         adjacency_matrix = static_cast<int*>(malloc((no_of_nodes) * (no_of_nodes) * sizeof(int)));
//         for (int i = 0; i < no_of_nodes; i++) {
//             for (int j = 0; j < no_of_nodes; j++) {
//                 in >> adjacency_matrix[(i * no_of_nodes) + j];
//             }
//         }
//
//         in.close();
//     }
//
//     MPI_Barrier(communication);
//     MPI_Bcast(&no_of_nodes, 1, MPI_INT, 0, communication);
//
//     current_process_part = no_of_nodes / no_of_processes;
//     current_process_matrix = static_cast<int*>(malloc(no_of_nodes * current_process_part * sizeof(int)));
//     current_process_distances = static_cast<int*>(malloc(no_of_nodes * current_process_part * sizeof(int)));
//     current_process_pred = static_cast<int*>(malloc(no_of_nodes * current_process_part * sizeof(int)));
//
//     MPI_BLOCK_TYPE = create_block_column(no_of_nodes, current_process_part);
//
//     if (current_process_id == 0) {
//         start_time = std::chrono::high_resolution_clock::now();
//     }
//
//     share_matrix(adjacency_matrix, current_process_matrix, no_of_nodes * current_process_part,
//         MPI_BLOCK_TYPE, current_process_id, communication);
//     dijkstra(current_process_matrix, current_process_distances, current_process_pred, no_of_nodes, current_process_part,
//         current_process_id, source_node, communication);
//
//     write_result(output_file, current_process_distances, no_of_nodes, current_process_part, current_process_id, communication);
//
//     free(current_process_matrix);
//     free(current_process_distances);
//     free(current_process_pred);
//
//     MPI_Type_free(&MPI_BLOCK_TYPE);
//
//     MPI_Finalize();
//     return 0;
// }
//
// /*
// Create a block column that represents a new data type.
// */
// MPI_Datatype create_block_column(int no_of_nodes, int current_process_part) {
//     MPI_Aint lower_bound, extent;
//     MPI_Datatype MPI_BLOCK_TYPE;
//     MPI_Datatype first_vector;
//     MPI_Datatype block_column_vector;
//
//     MPI_Type_contiguous(current_process_part, MPI_INT, &MPI_BLOCK_TYPE);
//     MPI_Type_get_extent(MPI_BLOCK_TYPE, &lower_bound, &extent);
//
//     MPI_Type_vector(no_of_nodes, current_process_part, no_of_nodes, MPI_INT, &first_vector);
//     MPI_Type_create_resized(first_vector, lower_bound, extent, &block_column_vector);
//     MPI_Type_commit(&block_column_vector);
//
//     MPI_Type_free(&MPI_BLOCK_TYPE);
//     MPI_Type_free(&first_vector);
//
//     return block_column_vector;
// }
//
// /*
// Share the matrix beetween processes in equals parts.
// */
// void share_matrix(int* adjacency_matrix, int current_process_matrix[], int size,
//     MPI_Datatype MPI_BLOCK_TYPE, int current_process_id, MPI_Comm communication) {
//
//     MPI_Scatter(adjacency_matrix, 1, MPI_BLOCK_TYPE, current_process_matrix, size, MPI_INT, 0, communication);
//
//     free(adjacency_matrix);
// }
//
// /*
// The Dijkstra's algorithm.
// */
// void dijkstra(int adjacency_matrix[], int current_process_distances[], int current_process_pred[], int no_of_nodes,
//     int current_process_part, int current_process_id, int source_node, MPI_Comm communication) {
//
//     int* visited = nullptr;
//     int node = 0;
//     int new_dist = 0;
//     int node_id = 0;
//
//     visited = static_cast<int*>(malloc(current_process_part * sizeof(int)));
//     init(adjacency_matrix, current_process_distances, current_process_pred, visited, current_process_part,
//         current_process_id, source_node);
//
//     for (int i = 1; i < no_of_nodes; i++) {
//         node_id = get_minimum_distance(current_process_distances, visited, current_process_part);
//
//         int current_process_min[2], global_min[2];
//         int global_min_distance;
//
//         if (node_id < INFINITE) {
//             current_process_min[0] = current_process_distances[node_id];
//             current_process_min[1] = get_node_id(node_id, current_process_part, current_process_id);
//         }
//         else {
//             current_process_min[0] = INFINITE;
//             current_process_min[1] = INFINITE;
//         }
//
//         MPI_Allreduce(current_process_min, global_min, 1, MPI_2INT, MPI_MINLOC, communication); //locate min value from ALL the proceses
//         node = global_min[1];
//         global_min_distance = global_min[0];
//
//         if (node / current_process_part == current_process_id) {
//             node_id = node % current_process_part;
//             visited[node_id] = 1;
//         }
//
//         //Update the current distance
//         for (int i = 0; i < current_process_part; i++)
//             if (!visited[i]) {
//                 new_dist = global_min_distance + adjacency_matrix[node * current_process_part + i];
//                 if (new_dist < current_process_distances[i]) {
//                     current_process_distances[i] = new_dist;
//                     current_process_pred[i] = node;
//                 }
//             }
//     }
// }
//
// /*
// Initialize distances and visited vector.
// */
// void init(const int adjacency_matrix[], int current_process_distances[], int current_process_pred[], int visited[],
//     int current_process_part, int current_process_id, int source_node) {
//
//     for (int i = 0; i < current_process_part; i++) {
//         current_process_distances[i] = adjacency_matrix[0 * current_process_part + i];
//         current_process_pred[i] = 0;
//         visited[i] = (current_process_id * current_process_part + i) == source_node;
//     }
// }
//
// /*
// Returns position of node which offers the minimum local distance possible.
// */
// int get_minimum_distance(const int current_process_distances[], const int visited[], int current_process_part) {
//     int min_dist_index;
//     int min_dist_value = INFINITE;
//
//     min_dist_index = INFINITE;
//     for (int i = 0; i < current_process_part; i++)
//         if (!visited[i])
//             if (current_process_distances[i] < min_dist_value) {
//                 min_dist_index = i;
//                 min_dist_value = current_process_distances[i];
//             }
//
//     return min_dist_index;
// }
//
// /*
// Convert a local node id to a global node id.
// */
// int get_node_id(int local_id, int current_process_part, int current_process_id) {
//     return local_id + current_process_id * current_process_part;
// }
//
// /*
// Write result in the output file.
// */
// void write_result(const std::string& output_file, int currrent_process_distances[], int no_of_nodes, int current_process_part,
//     int current_process_id, MPI_Comm communication) {
//     int* distances = nullptr;
//
//     if (current_process_id == 0) {
//         auto stop_time = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
//         std::cout << "Time: " << duration.count() << " microseconds." << std::endl;
//         distances = static_cast<int*>(malloc(no_of_nodes * sizeof(int)));
//     }
//
//     MPI_Gather(currrent_process_distances, current_process_part, MPI_INT, distances, current_process_part, MPI_INT, 0, communication);
//
//     if (current_process_id == 0) {
//         std::ofstream out(output_file);
//         out << "Vertex \tDistance from " << 0 << "(source node)" << '\n';
//         for (int i = 0; i < no_of_nodes; i++) {
//             out << i << " \t\t" << distances[i] << '\n';
//         }
//         free(distances);
//     }
// }