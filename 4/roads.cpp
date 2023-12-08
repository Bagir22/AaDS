#include "iostream"
#include "map"
#include <fstream>
#include <sstream>

/*
Общими требованиями к лабораторной работе являются:
   1) вводить  граф из файла в понятной для пользователя форме
    (не в виде матрицы смежности, без дублирования информации и т.
    п.);
   2) обеспечить   возможность   многократных   запросов   без
    повторного запуска программы.

    5. Имеется сеть автомобильных  дорог.  Известны  расстояния
    всех   участков   дорог.   Некоторые  участки  аварийноопасны.
    Методом поиска в глубину найти лучший путь из пункта А в пункт
    В. Лучшим считается путь,  имеющий в первую очередь наименьшую
    суммарную протяженность аварийноопасных,  а во  вторую очередь
    наименьшую длину (7).

    Январев Данила ПС-22
    CLion 2023.2.2
 */

using namespace std;

struct Edge {
    int start;
    int end;
    int weight;
    int dangerous;
};

struct Graph {
    map<int, vector<Edge>> adjacencyList;
};

void addEdge(Graph& graph, int start, int end, int weight, int dangerous) {
    Edge forwardEdge = {start, end, weight, dangerous};
    Edge backwardEdge = {end, start, weight, dangerous};

    graph.adjacencyList[start].push_back(forwardEdge);
    graph.adjacencyList[end].push_back(backwardEdge);
}

void printGraph(const Graph& graph) {
    cout << "Graph:" << endl;
    for (const auto& entry : graph.adjacencyList) {
        int vertex = entry.first;
        const vector<Edge>& edges = entry.second;

        cout << "Node " << vertex << " is connected to nodes: ";
        for (const Edge& edge : edges) {
            cout << edge.end << " ";
        }
        cout << endl;
    }
}


void makeGraph(istream& inFile, Graph& g, int& readedLines) {
    if (!inFile) {
        cout << "File can't be open" << endl;
        return;
    }

    int start, end, weight, dangerous;

    string line;
    while (getline(inFile, line)) {
        readedLines++;
        if (line == "#") {
            break;
        } else {
            istringstream iss(line);
            if (!(iss >> start >> end >> weight >> dangerous)) {
                cerr << "Error reading line: " << line << endl;
            } else {
                addEdge(g, start, end, weight, dangerous);
            }
        }
    }
}

void findAllPathsDFS(Graph& graph, int current, int destination, vector<Edge>& path, vector<vector<Edge>>& allPaths) {
    for (const Edge& edge : graph.adjacencyList[current]) {
        int neighbor = edge.end;

        if (find_if(path.begin(), path.end(), [neighbor](const Edge& e) { return e.end == neighbor; }) == path.end()) {
            path.push_back(edge);

            if (neighbor == destination) {
                allPaths.push_back(path);
            } else {
                findAllPathsDFS(graph, neighbor, destination, path, allPaths);
            }

            path.pop_back();
        }
    }
}

vector<vector<Edge>> findAllPaths(Graph& graph, int start, int destination) {
    vector<Edge> path;
    vector<vector<Edge>> allPaths;
    findAllPathsDFS(graph, start, destination, path, allPaths);
    return allPaths;
}

vector<vector<Edge>> findSafestShortestPaths(Graph& graph, int start, int destination) {
    vector<vector<Edge>> allPaths = findAllPaths(graph, start, destination);

    if (allPaths.empty()) {
        return {};
    }

    int minDangerSum = INT_MAX;
    for (const vector<Edge>& path : allPaths) {
        int dangerSum = 0;
        for (const Edge& edge : path) {
           if (edge.dangerous == 1) {
               dangerSum += edge.weight;
           }
        }
        minDangerSum = min(minDangerSum, dangerSum);
    }

    vector<vector<Edge>> safestPaths;
    for (const vector<Edge>& path : allPaths) {
        int dangerSum = 0;
        for (const Edge& edge : path) {
            if (edge.dangerous == 1) {
                dangerSum += edge.weight;
            }
        }
        if (dangerSum == minDangerSum) {
            safestPaths.push_back(path);
        }
    }

    int min_length = INT_MAX;
    int minWeightSum = INT_MAX;
    for (const vector<Edge>& path : safestPaths) {
        int weightSum = 0;
        for (const Edge& edge : path) {
            weightSum += edge.weight;
        }
        minWeightSum = min(minWeightSum, weightSum);
    }

    vector<vector<Edge>> safestShortestPaths;

    for (const vector<Edge>& path : safestPaths) {
        int weightSum = 0;
        for (const Edge& edge : path) {
            weightSum += edge.weight;
        }
        if (weightSum == minWeightSum) {
            safestShortestPaths.push_back(path);
        }
    }

    return safestShortestPaths;
}

int main() {
    Graph graph;

    char choice;

    cout << "Input file name: " << endl;
    string inputFile;
    cin >> inputFile;

    ofstream outFile("out.txt", ios_base::out);
    ifstream inFile(inputFile);
    outFile.clear();
    int readedLines = 0;

    makeGraph(inFile, graph, readedLines);
    inFile.close();

    while(true) {
        ifstream inFile(inputFile, ios_base::in);
        ofstream outFile("out.txt", ios_base::app);
        int currLine = 0;
        string line;
        if (currLine < readedLines) {
            while (getline(inFile, line)) {
                currLine++;
                if (currLine == readedLines) {
                    break;
                }
            }
        }

        int start, end;
        if (!inFile.eof()) {
            while (getline(inFile, line)) {
                istringstream iss(line);
                if (!(iss >> start >> end)) {
                    cerr << "Error reading line: " << line << endl;
                    return 0;
                } else {
                    vector<vector<Edge>> safestPaths = findSafestShortestPaths(graph, start, end);
                    if (safestPaths.size() != 0) {
                        outFile << "From " << start << " To " << end << endl;
                        outFile << "Safest Path:" << endl;
                        const vector<Edge>& safestPath = safestPaths[0];

                        for (const Edge& edge : safestPath) {
                            outFile << edge.start;
                            outFile << " ";
                        }

                        outFile << safestPath.back().end << endl;

                        int totalWeight = 0;
                        int totalDangerous = 0;
                        for (const Edge& edge : safestPath) {
                            totalWeight += edge.weight;
                            if (edge.dangerous == 1) {
                                totalDangerous += edge.weight;
                            }
                        }
                        outFile << "Total weight: " << totalWeight << endl;
                        outFile << "Total dangerous weight: " << totalDangerous << endl;
                    } else {
                        outFile << "Safest path not found" << endl;
                    }

                    readedLines++;

                    inFile.close();
                    outFile.close();

                    cout << "Continue (Y/N)?";
                    cin >> choice;

                    if (choice != 'Y' && choice != 'y') {
                        return 0;
                    }
                }
            }
        }
    }
}