#ifndef _GRAPH_H
#define _GRAPH_H

#include <algorithm>
#include <cctype>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

extern const set<char> delimiters;
extern const set<string> mermaidKeywords;

class TextGraph {
  private:
    map<string, map<string, int>> graph;
    vector<string> words;
    void setGraph(string &text);
    void saveGraph();

  public:
    TextGraph(const char *fileName);
    ~TextGraph();
    void showGraph();
    vector<string> queryBridgeWords(string word1, string word2);
    string generateNewText(string text);
    vector<string> calcShortestPath(string word1, string word2);
    string randomWalk();
};

#endif
