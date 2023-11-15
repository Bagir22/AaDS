#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

/*
   Общими требованиями к лабораторной работе являются:
   1) вводить  граф из файла в понятной для пользователя форме
(не в виде матрицы смежности, без дублирования информации и т.
п.);
   2) обеспечить   возможность   многократных   запросов   без
повторного запуска программы.

2. На плоскости заданы координаты N элементов,  являющихся
выводами  печатной  платы.  Некоторые  элементы  связаны между
собой.  Требуется выбрать  начальный  элемент  для  построения
пути, не содержащего циклов, максимальной длины. Путь строится
по следующему правилу:  из всех возможных элементов, связанных
с данным, в путь включается ближайший элемент (7).

Январев Данила ПС-22
VS Code
*/

struct Vertex {
    int id;
    std::vector<std::pair<int, int> > neighbors; // Пара (вершина, вес)
};

struct Graph {
    map<int, Vertex> vertices;
};

void addEdge(Graph& g, int u, int v, int weight) {
    g.vertices[u].neighbors.push_back(std::make_pair(v, weight));
    g.vertices[v].neighbors.push_back(std::make_pair(u, weight));
}

int findStartVertex(Graph& g) {
    int maxDegree = -1;
    int startVertex = -1;

    for (const auto& vertex : g.vertices) {
        int degree = vertex.second.neighbors.size();
        if (degree > maxDegree) {
            maxDegree = degree;
            startVertex = vertex.first;
        }
    }

    return startVertex;
}

void dfs(Graph& g, int currentVertex, std::vector<int>& path, std::vector<bool>& visited, int currentLength, std::vector<int>& maxPath, int& maxLength) {
    visited[currentVertex] = true;
    path.push_back(currentVertex);

    for (const auto& neighbor : g.vertices[currentVertex].neighbors) {
        int neighborVertex = neighbor.first;
        int edgeWeight = neighbor.second;

        if (!visited[neighborVertex]) {
            int newLength = currentLength + edgeWeight;
            dfs(g, neighborVertex, path, visited, newLength, maxPath, maxLength);
        }
    }

    if (currentLength > maxLength) {
        maxLength = currentLength;
        maxPath = path;
    }

    visited[currentVertex] = false;
    path.pop_back();
}


vector<int> findMaxPath(Graph& g, int startVertex) {
    std::vector<int> maxPath;
    std::vector<int> currentPath;
    std::vector<bool> visited(g.vertices.size(), false);
    int maxLength = 0;

    for (const auto& vertex : g.vertices) {
        dfs(g, vertex.first, currentPath, visited, 0, maxPath, maxLength);
    }

    return maxPath;
}

int main() {
    Graph graph;

    ifstream file("graph.txt");
    int u, v, weight;
    while (file >> u >> v >> weight) {
        addEdge(graph, u, v, weight);
    }

    int startVertex = findStartVertex(graph);
    std::vector<int> maxPath = findMaxPath(graph, startVertex);

    std::cout << "Максимальный путь без циклов: ";
    for (int vertex : maxPath) {
        std::cout << vertex << " ";
    }

    return 0;
}

