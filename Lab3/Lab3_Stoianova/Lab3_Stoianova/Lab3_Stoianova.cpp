#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <stack>
using namespace std;

void dfs(char currLetter, unordered_map<char, vector<char>>& adjList, unordered_set<char>& state, stack<char>& Stack) {
    state.insert(currLetter);

    for (const auto& neighbor : adjList[currLetter]) {
        if (state.find(neighbor) == state.end()) {
            dfs(neighbor, adjList, state, Stack);
        }
    }
    Stack.push(currLetter);
}

void defineAdj(vector<string>& glossary, unordered_set<char>& uniqLetters, unordered_map<char, vector<char>>& adjList) {
    // Заполнение уникальных букв
    for (const string& word : glossary) {
        for (char c : word) {
            uniqLetters.insert(c);
        }
    }

    // Построение списков смежности
    for (int i = 0; i < glossary.size() - 1; i++) {
        const string& currentWord = glossary[i];
        const string& nextWord = glossary[i + 1];
        int minLength = min(currentWord.length(), nextWord.length());

        for (int j = 0; j < minLength; j++) {
            if (currentWord[j] != nextWord[j]) {
                adjList[currentWord[j]].push_back(nextWord[j]);
                break;
            }
        }
    }
}

int main() //int argc, char* argv[]
{
    setlocale(LC_ALL, "rus");
    fstream file;
    ofstream Output("Output3.txt"); // argv[2]
    file.open("Input2.txt"); // argv[1]

    if (!file.is_open())
        return -1;

    string firstLine;
    getline(file, firstLine);

    unordered_set<char> Nodes; // уникальные буквы в глоссарии
    unordered_map<char, vector<char>> adjList; // списки смежности для каждой буквы
    unordered_set<char> state;
    
    string currentLine;
    vector<string> glossary;
    while (getline(file, currentLine)) {
        glossary.push_back(currentLine);
    }

    defineAdj(glossary, Nodes, adjList);

    // Определение порядка с помощью алгоритма топологической сортировки
    stack<char> Stack;
    for (const auto& letter : Nodes) {
        if (state.find(letter) == state.end()) {
            dfs(letter, adjList, state, Stack);
        }
    }

    // Вывод порядка
    while (!Stack.empty()) {
        Output << Stack.top();
        Stack.pop();
    }


    file.close();
    Output.close();
}
