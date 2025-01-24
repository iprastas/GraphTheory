#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>

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

struct Node {
    int x, y;
    Node(int _x, int _y) : x(_x), y(_y) {}
};

struct Edge {
    int to;
    double weight;
    Edge(int _to, double _weight) : to(_to), weight(_weight) {}
};

double getDistanceBetween(Node first, Node second) {
    return sqrt((second.x - first.x) * (second.x - first.x) + (second.y - first.y) * (second.y - first.y));
}

pair<double, vector<Node>> dijkstra(vector<vector<Edge>>& graph, vector<Node>& points, int start, int end) {
    int n = graph.size();
    vector<double> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    dist[start] = 0;

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push(make_pair(0, start)); //добавляем начальное расстояние и текущую вершину

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (u == end) { //нашли кратчайший путь
            vector<Node> shortestPath;
            int p = end;
            while (p != -1) {
                shortestPath.push_back(points[p]);
                p = parent[p];
            }
            reverse(shortestPath.begin(), shortestPath.end());
            return make_pair(dist[end], shortestPath);
        }
        for (Edge& e : graph[u]) { // для каждой смежной вершины считаем расстояние и вес
            int v = e.to;
            double weight = e.weight;
            if (dist[u] + weight < dist[v]) { // если меньше, то обновляем данные и добавляем в очередь
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    return make_pair(INT_MAX, vector<Node>()); // Путь не найден
}

int main(int argc, char* argv[]) //
{
    setlocale(LC_ALL, "rus");
    fstream file;
    file.open(argv[1]); //"Input4.txt"
    ofstream output(argv[2]); //"Ouput_4.txt"

    if (!file.is_open())
        return -1;

    string firstLine;
    getline(file, firstLine);

    int N = stoi(split(firstLine)[0]);
    int R = stoi(split(firstLine)[1]); 

    string currentLine;

    vector<Node> nodes = vector<Node>();
    vector<vector<Edge>> adjList = vector<vector<Edge>>(N);

    while (getline(file, currentLine)) {
        nodes.push_back(Node(stoi(split(currentLine)[0]), stoi(split(currentLine)[1])));
    }

    file.close();

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            double dist = getDistanceBetween(nodes[i], nodes[j]);
            if (dist <= R) {
                adjList[i].push_back(Edge(j, dist));
                adjList[j].push_back(Edge(i, dist));
            }
        }
    }

    int start = 0; // откуда
    int end = nodes.size() - 1; // куда
    pair<double, vector<Node>> result = dijkstra(adjList, nodes, start, end); // вызываем алгоритм Дейкстры с начальной вершиной 0

    if (result.first != INT_MAX) {
        output << result.first << endl;
        for (const Node& p : result.second) {
            output << p.x << " " << p.y << endl;
        }
    }
    else {
        output << -1;
    }

    return 0;
}
