#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

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

int getIndex(vector<string> vectorOfSearch, string searchElem) //ищем индекс из вектора всех узлов
{
    auto it = find(vectorOfSearch.begin(), vectorOfSearch.end(), searchElem);

    if (it != vectorOfSearch.end())
    {
        int ind = it - vectorOfSearch.begin();
        return ind;
    }
    else {
        return -1;
    }
}

int main(int argc, char* argv[]) //изменяется входной файл. отсутсвует первая строка
{
    setlocale(LC_ALL, "rus");
    fstream InputFile;
    ofstream mxOutput("Mx.txt"); //argv[2]
    ofstream lsOutput("Ls.txt"); //argv[3]
    InputFile.open("Input1.txt"); //argv[1]

    if (!InputFile.is_open())
        return -1;
    vector<string> nodes = vector<string>(); //вектор всех узлов (для считывания 1 строки input файла)
    vector<int> pairs = vector<int>(); // вектор иформации, как именно связаны узлы
    vector<string> tempPair = vector<string>(); // вспомогательный вектор

    string curLine;
    while (getline(InputFile, curLine)) {
        tempPair = split(curLine);
        if (find(nodes.begin(), nodes.end(), tempPair[0]) == nodes.end())
            nodes.push_back(tempPair[0]);
        if (find(nodes.begin(), nodes.end(), tempPair[1]) == nodes.end())
            nodes.push_back(tempPair[1]);
    }
    sort(nodes.begin(), nodes.end());
    InputFile.clear();
    InputFile.seekg(0, ios::beg);

    while (getline(InputFile, curLine)) {
        tempPair = split(curLine);
        pairs.push_back(getIndex(nodes, tempPair[0]));
        pairs.push_back(getIndex(nodes, tempPair[1]));
    }

    string* mxOutputRow = new string[nodes.size()];
    for (int i = 0; i < nodes.size(); i++) {
        int* colNodes = new int[nodes.size()]; //массив для записи существования дуг м.у i узлом и j-ым
        for (int j = 0; j < nodes.size(); j++)
            colNodes[j] = 0;
        for (int j = 0; j < pairs.size(); j++) {
            if (pairs[j] == i) {
                if (j % 2 == 0) //смотрим какая часть пары подошла под условие
                    colNodes[pairs[j + 1]] += 1;
                else
                    colNodes[pairs[j - 1]] += 1;
            }
        }
        mxOutputRow[i] = "";
        for (int j = 0; j < nodes.size(); j++) {
            mxOutputRow[i] += to_string(colNodes[j]) + " ";
        }
    }
    for (int i = 0; i < nodes.size(); i++) { //запись в файл
        mxOutput << mxOutputRow[i] << endl;
    }
    mxOutput.close();


    string* lsOutputRows = new string[nodes.size()];
    for (int i = 0; i < nodes.size(); i++)
        lsOutputRows[i] = nodes[i] + ": ";

    for (int i = 0; i < pairs.size(); i++) {
        if (i % 2 == 0) // проверяем на какой мы сейчас части пары и распределяем по строкам  
            lsOutputRows[pairs[i]] += nodes[pairs[i + 1]] + " ";
        else
            lsOutputRows[pairs[i]] += nodes[pairs[i - 1]] + " ";
    }

    for (int i = 0; i < nodes.size(); i++) {
        lsOutput << lsOutputRows[i] << endl;
    }
    lsOutput.close();

    return 0;
}