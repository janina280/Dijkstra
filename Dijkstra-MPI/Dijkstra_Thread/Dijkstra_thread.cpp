#include <iostream>
#include <vector>
#include <limits>
#include <thread>

using namespace std;

// Structura pentru reprezentarea unui nod în graf
struct Node {
    int index;
    int distance;
    bool visited;
    vector<pair<int, int>> neighbors; // vecinii nodului, fiecare cu ponderea muchiei
};

// Implementarea algoritmului lui Dijkstra
vector<int> dijkstra(vector<Node>& graph, int source) {
    int n = graph.size();

    // Initializare distante si marcari
    vector<int> distance(n, numeric_limits<int>::max());
    vector<bool> visited(n, false);

    // Setare distanta pentru sursa
    distance[source] = 0;

    // Numarul de thread-uri pe care dorim să împărțim iterația
    size_t num_threads = thread::hardware_concurrency();

    // Functia care va fi executată de fiecare fir de execuție
    auto thread_function = [&](size_t tid) {
        for (size_t i = tid; i < n - 1; i += num_threads) {
            int u = -1;
            // Alegerea nodului nevizitat cu cea mai mica distanta
            for (size_t j = 0; j < n; ++j) {
                if (!visited[j] && (u == -1 || distance[j] < distance[u])) {
                    u = j;
                }
            }
            // Actualizarea distantei pentru vecinii nodului selectat
            for (auto& neighbor : graph[u].neighbors) {
                int v = neighbor.first;
                int weight = neighbor.second;
                distance[v] = min(distance[v], distance[u] + weight);
            }
            visited[u] = true;
        }
        };

    // Vectorul pentru firele de execuție
    vector<thread> threads;

    // Crearea și pornirea firelor de execuție
    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_function, i);
    }

    // Așteptarea ca toate firele de execuție să se termine
    for (auto& t : threads) {
        t.join();
    }

    return distance;
}

int main() {
    vector<Node> graph = {
        {0, 0, false, {{1, 4}, {2, 1}}},
        {1, numeric_limits<int>::max(), false, {{2, 2}, {3, 5}}},
        {2, numeric_limits<int>::max(), false, {{3, 2}}},
        {3, numeric_limits<int>::max(), false, {}}
    };

    int source = 0;
    vector<int> shortest_distances = dijkstra(graph, source);

    cout << "Cele mai scurte distante de la nodul " << source << " la celelalte noduri:\n";
    for (size_t i = 0; i < shortest_distances.size(); ++i) {
        cout << "Nod " << i << ": " << shortest_distances[i] << "\n";
    }

    return 0;
}
