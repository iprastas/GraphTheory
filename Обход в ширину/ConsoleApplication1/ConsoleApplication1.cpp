#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <queue>
using namespace std;

vector<string> split(string input) {
    vector<string> output = {};
    string tempString = "";
    for (char el : input) {
        if (el == ' ') {
            output.push_back(tempString);
            tempString = "";
        }
        else {
            tempString += el;
        }
    }
    if (tempString != "")
        output.push_back(tempString);

    return output;
}

int getIndex(vector<pair<int, int>> vectorOfSearch, pair<int, int> searchElem) {
    auto it = find(vectorOfSearch.begin(), vectorOfSearch.end(), searchElem);

    if (it != vectorOfSearch.end()) {
        int index = it - vectorOfSearch.begin();
        return index;
    }
    else {
        return -1;
    }
}

namespace GraphUtils {
    class Graph {
    
    public:
        int numVertices; // Number of vertices in the graph

        std::vector<std::vector<int>> adjList; // Adjacency list representation of the graph

        vector<pair<int, int>> nodes; //узлы

        Graph(int numVertices) {
            this->numVertices = numVertices;
            adjList.resize(numVertices);
        }

        void addEdge(int src, int dest) {
            adjList[src].push_back(dest);
        }

        vector<int> FindCycleWithBFS(int startVertex, vector<bool>& visited, vector<int>& parent, queue<int>& queue) {
            queue.push(startVertex);
            visited[startVertex] = true;

            while (!queue.empty()) {
                int currentVertex = queue.front();
                queue.pop();

                for (int neighbor : adjList[currentVertex]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        queue.push(neighbor);
                        parent[neighbor] = currentVertex;
                    }
                    else if (parent[currentVertex] != neighbor) {
                        // Cycle found
                        std::vector<int> cycle;
                        cycle.push_back(neighbor);
                        cycle.push_back(currentVertex);

                        return cycle;
                    }
                }
            }

            return std::vector<int>(); // No cycle found
        }
    };
}

int main() //int argc, char* argv[]
{ 
    setlocale(LC_ALL, "rus");
    fstream File;
    ofstream Output("Output_new.txt"); //  argv[2]
    File.open("Input.txt"); //argv[1])

    if (!File.is_open())
        return -1;

    string firstLine;
    getline(File, firstLine);

    int numNodes = stoi(firstLine);
    GraphUtils::Graph graph(numNodes);

    string curLine;
    while (getline(File, curLine)) {
        vector<string> splitStr = split(curLine);

        pair<int, int> node1(stoi(splitStr[0]), stoi(splitStr[1]));
        int firstNodeIndex = 0;
        if (getIndex(graph.nodes, node1) == -1) {
            graph.nodes.push_back(node1);
            firstNodeIndex = graph.nodes.size() - 1;
        }
        else {
            firstNodeIndex = getIndex(graph.nodes, node1);
        }

        pair<int, int> node2(stoi(splitStr[2]), stoi(splitStr[3]));
        int secondNodeIndex = 0;
        if (getIndex(graph.nodes, node2) == -1) {
            graph.nodes.push_back(node2);
            secondNodeIndex = graph.nodes.size() - 1;
        }
        else {
            secondNodeIndex = getIndex(graph.nodes, node2);
        }

        graph.addEdge(secondNodeIndex, firstNodeIndex);
        graph.addEdge(firstNodeIndex, secondNodeIndex);
    }

    std::vector<bool> state(numNodes, false); // Track visited vertices
    std::vector<int> pred(numNodes, -1); // Track parent of each vertex

    std::queue<int> queue;

    std::vector<int> cycle = graph.FindCycleWithBFS(0, state, pred, queue);
    if (cycle.empty()) {
        Output << "No cycle found in the graph.\n";
    }
    else {
        int neighbor = cycle[0];
        int vertex = pred[cycle[1]];

        vector<int>PredNeigh;
        while (pred[neighbor] != -1) {
            neighbor = pred[neighbor];
            PredNeigh.push_back(neighbor);
        }

        while (vertex != neighbor || pred[vertex] != -1) {
            cycle.push_back(vertex);
            vertex = pred[vertex];
            auto ind = find(PredNeigh.begin(), PredNeigh.end(), vertex);
            if ( ind != PredNeigh.end()) {
                for (int i = ind - PredNeigh.begin(); i >= 0; --i) {
                    cycle.push_back(PredNeigh[i]);
                }
                break;
            }
        }
        if (pred[vertex] == -1) {
            auto ind = find(PredNeigh.begin(), PredNeigh.end(), vertex);
            if (ind != PredNeigh.end()) {
                for (int i = ind - PredNeigh.begin(); i >= 0; --i) {
                    cycle.push_back(PredNeigh[i]);
                }
            }
        }
        
        Output << cycle.size() << endl;
        for (int i = cycle.size() - 1; i >= 0; i--) {
            Output << graph.nodes[cycle[i]].first << " " << graph.nodes[cycle[i]].second << endl;
        }
    }

    return 0;
}
