#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct floorChaVo {
private:
	vector<char> plan;
	int number;

public:
	floorChaVo(int N) {
		plan = vector<char>();
		number = N;
	}

	void addLetter(char c) {
		plan.push_back(c);
	}

	int compareFloors(floorChaVo other, int weight = 1) {
		int q = 0;
		if (plan.size() == 0) return other.plan.size();
		if (other.plan.size() == 0) return plan.size();
		for (int i = 0; i < plan.size(); i++)
			if (plan[i] != other.plan[i])
				q++;
		return q * weight;
	}
};

struct Edge {
	int begin, end, weight;
};

vector<pair<int,int>> primMST(vector<vector<Edge>>& graph, int startVertex, int& sumWeight) {
	int numVertices = graph.size();

	vector<int> vectWeight(numVertices, INT_MAX); // веса
	vector<int> parent(numVertices, -1);
	vector<bool> inMST(numVertices, false); // флаг древесная ли вершина

	// Создаем очередь с приоритетом для хранения вершин и их ключей
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

	// Вставляем стартовую вершину в очередь и устанавливаем ее ключ равным 0
	pq.push(make_pair(0, startVertex));
	vectWeight[startVertex] = 0;

	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();

		inMST[u] = true; //вершина стала древесной

		for (auto edge : graph[u]) { //проходим по всем смежным вершинам
			int v = edge.end;
			int weight = edge.weight;

			if (!inMST[v] && vectWeight[v] > weight) { // обновление весов, если не древесная вершина
				vectWeight[v] = weight;
				pq.push(make_pair(vectWeight[v], v));
				parent[v] = u;
			}
		}
	}

	vector<pair<int, int>> result;
	for (int i = 1; i < numVertices; i++) {
		sumWeight += vectWeight[i];
		result.push_back(make_pair(i, parent[i]));
	}

	return result;
}

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

int main(int argc, char* argv[]) //
{
    setlocale(LC_ALL, "rus");
    fstream file;
    ofstream Output(argv[2]); //  "Output3_.txt"
    file.open(argv[1]); //  "Input3.txt"

    if (!file.is_open())
        return -1;

    string firstLine;
    getline(file, firstLine);
	int n = stoi(split(firstLine)[0]);
	int k = stoi(split(firstLine)[2]);
	int weight = stoi(split(firstLine)[3]);

	vector<floorChaVo> floors;
	vector<vector<Edge>> adjList(k+1); // список смежностей
	vector<int> parent;

	string currentLine;
	int numLine = 0;

	floors.push_back(floorChaVo(0));
	parent.push_back(0);

	while (getline(file, currentLine)) {
		if (numLine % n == 0) {
			floors.push_back(floorChaVo((int)numLine / n + 1));
			parent.push_back(parent.size());
		}
		for (int i = 0; i < currentLine.size(); i++)
			floors[floors.size() - 1].addLetter(currentLine[i]);
		numLine++;
	}

	file.close();

	for (int i = 0; i < floors.size(); i++) {
		for (int j = 0; j < floors.size(); j++) {
			if (i != j) {
				int floor_memory = floors[i].compareFloors(floors[j], weight);
				adjList[i].push_back({ i, j, floor_memory });
			}
		}
	}

	int sum = 0;
	int startVertex = 0;
	vector<pair<int, int>> ans = primMST(adjList, startVertex, sum);

	Output << sum << "\n";
	for (auto el : ans) {
		Output << el.first << " " << el.second << "\n";
	}

	Output.close();
}