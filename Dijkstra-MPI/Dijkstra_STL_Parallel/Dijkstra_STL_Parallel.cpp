// #include <algorithm>
// #include <chrono>
// #include "execution"
// #include <fstream>
// #include <iostream>
// #include <vector>
//
// #define INF         1000000
// #define INPUT_FILE  "input_STL.txt"
// #define OUTPUT_FILE "output_STL.txt"
//
// /*
//  * Used to store informations about a graph node.
//  */
// class Node {
// public:
//     int index = -1;
//     int distance = INF;
//     bool visited = false;
// };
//
// /*
//  * @author Cocei_Janina
//  */
// int main()
// {
//     std::ifstream in(INPUT_FILE);
//     std::ofstream out(OUTPUT_FILE);
//
//     if (in.is_open() && out.is_open()) {
//         int no_of_nodes;
//         int source_node;
//
//         in >> no_of_nodes;
//         in >> source_node;
//
//         std::vector<std::vector<int>> adjacency_matrix(no_of_nodes);
//         std::vector<Node> nodes(no_of_nodes);
//         for (int i = 0; i < no_of_nodes; i++) {
//             adjacency_matrix[i].resize(no_of_nodes);
//             for (auto& iterator : adjacency_matrix[i]) {
//                 in >> iterator;
//             }
//             nodes[i].index = i;
//         }
//
//         auto start_time = std::chrono::high_resolution_clock::now();
//
//         nodes[source_node].distance = 0;
//
//         for (int i = 0; i < no_of_nodes - 1; i++) {
//             auto current_min_node = std::min_element(std::execution::par_unseq,
//                 std::begin(nodes), std::end(nodes), [](const auto& it1, const auto& it2) {
//                     return !it1.visited && it2.visited || (!it2.visited && !it1.visited
//                         && (it1.distance < it2.distance));
//                 });
//             current_min_node->visited = true;
//             const int node_index = (int)(current_min_node - std::begin(nodes));
//             std::for_each(std::execution::seq, std::begin(nodes), std::end(nodes),
//                 [&](Node& node) {
//                     if (!nodes[node.index].visited
//                         && adjacency_matrix[node_index][node.index] != 0
//                         && nodes[node_index].distance != INF
//                         && (nodes[node_index].distance + adjacency_matrix[node_index][node.index])
//                         < nodes[node.index].distance) {
//                         node.distance = nodes[node_index].distance + adjacency_matrix[node_index][node.index];
//                     }
//                 });
//         }
//
//         auto stop_time = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
//         std::cout << "Time: " << duration.count() << " microseconds." << std::endl;
//
//         out << "Vertex \t Distance from Source" << std::endl;
//         for (auto node_iterator = std::begin(nodes); node_iterator != std::end(nodes); node_iterator++) {
//             out << node_iterator - std::begin(nodes) << " \t\t" << node_iterator->distance << std::endl;
//         }
//
//         return 0;
//     }
//     else {
//         return -1;
//     }
//
// }