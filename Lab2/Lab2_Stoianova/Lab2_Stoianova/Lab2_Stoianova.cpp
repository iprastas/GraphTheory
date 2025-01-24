#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

enum State { UNVISITED, PROCESSED, VISITED };

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

int getIndex(vector<vector<int>> vectorOfSearch, vector<int> searchElem) {
    auto it = find(vectorOfSearch.begin(), vectorOfSearch.end(), searchElem);

    if (it != vectorOfSearch.end()){
        int index = it - vectorOfSearch.begin();
        return index;
    }
    else {
        return -1;
    }
}

bool dfs(int node, int parent, vector<vector<int>>& ListOfNode, vector<vector<int>>& adjList, vector<State>& state, vector<int>& pred, ofstream& outputFile) {
    state[node] = PROCESSED; // Помечаем вершину как посещаемую

    for (int neighbor : adjList[node]) {
        if (state[neighbor] == UNVISITED) {
            pred[neighbor] = node; // Записываем предшественника вершины
            if (dfs(neighbor,node, ListOfNode, adjList, state, pred, outputFile)) {
                return true; // Найден цикл
            }
        }
        else if (state[neighbor] == PROCESSED && pred[node] != neighbor) {
            // Найден цикл
            int curr = node;
            int i = 1;
            vector<string> cycle;
            while (curr != neighbor) {
                cycle.push_back(to_string(ListOfNode[curr][0]) + " " + to_string(ListOfNode[curr][1]));
                curr = pred[curr]; i++;
            }
            cycle.push_back(to_string(ListOfNode[neighbor][0]) + " " + to_string(ListOfNode[neighbor][1]));

            outputFile << i << endl;
            for (i = cycle.size()-1; i >= 0; i--) {
                outputFile << cycle[i] << endl;
            }
           
            return true;
        }
    }

    state[node] = VISITED; // Помечаем вершину как посещенную
    return false;
}

int main() //int argc, char* argv[]
{
	setlocale(LC_ALL, "rus");
	fstream File;
	ofstream Output("Output1.txt"); //argv[2]
	File.open("Input2.txt"); //argv[1]

    if (!File.is_open())
        return -1;

    string firstLine;
    getline(File, firstLine);

    int numNodes = stoi(firstLine);

    vector<vector<int>> nodes; //узлы
    vector<vector<int>> adjList; //соседние вершины
    vector<int> pred(numNodes, -1);
    vector<State> state(numNodes, UNVISITED);//состояние

    //vector<vector<int>> Time_In;
    //vector<vector<int>> Time_Out;

    string curLine;
    while (getline(File, curLine)) {
        vector<string> splitStr = split(curLine);

        vector<int> node1 = { stoi(splitStr[0]), stoi(splitStr[1]) };
        int firstNodeIndex = 0;
        if (getIndex(nodes, node1) == -1) {
            nodes.push_back(node1);
            adjList.push_back(vector<int>());
            firstNodeIndex = nodes.size() - 1;
        }
        else {
            firstNodeIndex = getIndex(nodes, node1);
        }

        vector<int> node2 = { stoi(splitStr[2]), stoi(splitStr[3]) };
        int secondNodeIndex = 0;
        if (getIndex(nodes, node2) == -1) {
            nodes.push_back(node2);
            adjList.push_back(vector<int>());
            secondNodeIndex = nodes.size() - 1;
        }
        else {
            secondNodeIndex = getIndex(nodes, node2);
        }

        adjList[secondNodeIndex].push_back(firstNodeIndex);
        adjList[firstNodeIndex].push_back(secondNodeIndex);
    }

    bool hasCycle = false;

    for (int i = 0; i < numNodes; i++) {
        if (state[i] == UNVISITED && dfs(i, i, nodes, adjList, state, pred, Output)) {
            hasCycle = true;
            break;
        }
    }

    if (!hasCycle) {
        Output << "Cycle not found" << endl;
    }

    return 0;
}
