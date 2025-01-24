#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<string> split(string input) {
    vector<string> output = {};
    string tempString = "";
    for (char elem : input) {
        if (elem == ' ') {
            output.push_back(tempString);
            tempString = "";
        }
        else {
            tempString += elem;
        }
    }
    if (tempString != "")
        output.push_back(tempString);

    return output;
}

int getIndex(vector<string> vectorOfSearch, string searchElem)
{
    auto it = find(vectorOfSearch.begin(), vectorOfSearch.end(), searchElem);

    if (it != vectorOfSearch.end())
    {
        int index = it - vectorOfSearch.begin();
        return index;
    }
    else {
        return -1;
    }
}


struct Edge {
    int startVertex;
    int endVertex;

    Edge(int start, int end) : startVertex(start), endVertex(end) {}
};

bool dfs(vector<vector<double>>& graph, int s, int t, vector<int>& parent) {
    int V = graph.size();
    vector<bool> visited( V, false);

    vector<int> stack;
    stack.push_back(s);
    visited[s] = true;
    parent[s] = -1;

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();

        for (int v = 0; v < V; v++) {
            if (visited[v] == false && graph[u][v] > 0) {
                stack.push_back(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return (visited[t] == true);
}

int fordFulkerson(vector<vector<double>>& graph, int s, int t) {
    int V = graph.size();

    vector<int> parent(V);
    double max_flow = 0;

    while (dfs(graph, s, t, parent)) {
        double path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, graph[u][v]);
        }

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            graph[u][v] -= path_flow;
            graph[v][u] += path_flow;
        }

        max_flow += path_flow;
    }
    return max_flow;
}

int main(int argc, char* argv[]) //
{
    setlocale(LC_ALL, "rus");
    fstream file;
    file.open(argv[1]); // "Input1.txt"
    ofstream output(argv[2]); //"Output_1.txt" 

    if (!file.is_open())
        return -1;

    string firstLine;
    getline(file, firstLine);

    vector<string> nodes = split(firstLine);
    vector<vector<double>> AdjMatrix = vector<vector<double>>(nodes.size(), vector<double>(nodes.size()));
    vector<Edge> edges = vector<Edge>();

    string currentLine;
    while (getline(file, currentLine)) {
        vector<string> line = split(currentLine);

        if (line.size() == 3) {
            double start = getIndex(nodes, line[0]);
            double end = getIndex(nodes, line[1]);
            AdjMatrix[start][end] = stof(line[2]);

            edges.push_back(Edge(start, end));
        }
    }

    float max_flow = fordFulkerson(AdjMatrix, 0, nodes.size() - 1);

    output << fixed << setprecision(2) << max_flow << endl;
    for (const auto& edge : edges) {
        output << nodes[edge.startVertex] << " " << nodes[edge.endVertex] << " " << fixed << setprecision(2) << AdjMatrix[edge.endVertex][edge.startVertex] << endl;
    }
}
