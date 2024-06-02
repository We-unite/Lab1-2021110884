#include "graph.h"
#include <signal.h>

bool sigInt = false;
static vector<string> breakIntoWords(string &text) {
    string word;
    istringstream iss;
    vector<string> words;
    bool isFirstWord = true;

    // 标点符号替换为空格
    for (char &c : text) {
        if (delimiters.find(c) != delimiters.end()) {
            c = ' ';
        } else {
            c = tolower(c);
        }
    }

    iss.str(text);
    string processedWord;
    while (getline(iss, word, ' ')) {
        for (char c : word) {
            if (isalpha(c)) {
                processedWord += c;
            }
        }
        if (!processedWord.empty()) {
            words.push_back(processedWord);
            processedWord.clear();
        }
    }

    return words;
}

TextGraph::TextGraph(const char *fileName) {
    ifstream ifs(fileName);
    string text((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>())); // 读取文件内容
    ifs.close();
    setGraph(text);
    saveGraph();
}

TextGraph::~TextGraph() {}

void TextGraph::setGraph(string &text) {
    words = breakIntoWords(text);

    for (size_t i = 0; i < words.size() - 1; ++i) {
        if (graph.find(words[i]) == graph.end()) {
            graph[words[i]] = map<string, int>();
        }
        if (graph[words[i]].find(words[i + 1]) == graph[words[i]].end()) {
            graph[words[i]][words[i + 1]] = 1;
        } else {
            graph[words[i]][words[i + 1]]++;
        }
    }
}

void TextGraph::saveGraph() {
    // print the graph in mermaid format into out.mmd
    ofstream file("out.mmd");
    file << "graph LR" << std::endl;
    for (const auto &pair : graph) {
        for (const auto &word : pair.second) {
            file << pair.first;
            if (mermaidKeywords.find(pair.first) != mermaidKeywords.end()) {
                file << pair.first;
            }
            file << "(\"" << pair.first << "\")-->|" << word.second << "|";
            file << word.first;
            if (mermaidKeywords.find(word.first) != mermaidKeywords.end()) {
                file << word.first;
            }
            file << "(\"" << word.first << "\")" << endl;
        }
    }
    file.close();

    system("mmdc --puppeteerConfigFile config.json -i out.mmd -o "
           "output.svg && rm out.mmd");
    printf("\033[32mGraph generated successfully!\033[0m\n");
}

void TextGraph::showGraph() {
    // to ensure the robustness of the program,
    // we use fork to open the browser
    if (fork() == 0) {
        // 以/dev/null为标准输出和标准错误输出
        freopen("/dev/null", "w", stdout);
        freopen("/dev/null", "w", stderr);
        system("firefox output.svg");
        exit(0);
    }
}

vector<string> TextGraph::queryBridgeWords(string word1, string word2) {
    vector<string> bridgeWords;
    if (graph.find(word1) == graph.end() || graph.find(word2) == graph.end()) {
        bridgeWords.push_back("No " + word1 + " or " + word2 +
                              " in the graph!");
        goto end;
    }

    for (const auto &pair : graph[word1]) {
        if (graph[pair.first].find(word2) != graph[pair.first].end()) {
            bridgeWords.push_back(pair.first);
        }
    }
    if (bridgeWords.empty()) {
        bridgeWords.push_back("No bridge words from " + word1 + " to " + word2 +
                              "!");
    }
end:
    return bridgeWords;
}

string TextGraph::generateNewText(string text) {
    vector<string> newWords = breakIntoWords(text);
    string newText, bridgeWord;
    for (size_t i = 0; i < newWords.size() - 1; ++i) {
        newText += newWords[i] + " ";
        if (graph.find(newWords[i]) != graph.end()) {
            bridgeWord = queryBridgeWords(newWords[i], newWords[i + 1])[0];
            if (bridgeWord.find("No") == string::npos) {
                newText += bridgeWord + " ";
            }
        }
    }
    newText += newWords.back();
    return newText;
}

vector<string> TextGraph::calcShortestPath(string word1, string word2) {
    vector<string> path;
    if (graph.find(word1) == graph.end() || graph.find(word2) == graph.end()) {
        return path;
    }
    // 并查集，基于words来记录父节点
    int n = words.size(), i;
    map<string, string> parent;
    map<string, int> dist;
    for (i = 0; i < n; ++i) {
        parent[words[i]] = words[i];
        dist[words[i]] = INT_MAX;
    }
    dist[word1] = 0;
    set<string> visited;
    string u = word1;
    while (u != word2) {
        visited.insert(u);
        for (const auto &pair : graph[u]) {
            if (dist[u] + pair.second < dist[pair.first]) {
                dist[pair.first] = dist[u] + pair.second;
                parent[pair.first] = u;
            }
        }
        int minDist = INT_MAX;
        for (i = 0; i < n; ++i) {
            if (visited.find(words[i]) == visited.end() &&
                dist[words[i]] < minDist) {
                minDist = dist[words[i]];
                u = words[i];
            }
        }
        if (minDist == INT_MAX) {
            return path;
        }
    }
    while (u != word1) {
        path.push_back(u);
        u = parent[u];
    }
    path.push_back(word1);
    reverse(path.begin(), path.end());
    return path;
}

void TextGraph::randomWalk() {
    // 捕获sigint
    signal(SIGINT, [](int) { sigInt = true; });
    srand(time(nullptr));
    int index = rand() % words.size();
    string word, next;
    vector<pair<string, string>> edges;

    next = words[index];
    while (true) {
        word = next;
        if (graph[word].size() == 0) {
            break;
        }
        index = rand() % graph[word].size();
        // get the next word from map<string,map<string,int>> graph
        auto it = graph[word].begin();
        for (int i = 0; i < index; ++i) {
            ++it;
        }
        next = it->first;
        // if word->next has been visited, break
        if (find(edges.begin(), edges.end(), make_pair(word, next)) !=
            edges.end()) {
            edges.push_back({word, next});
            break;
        }
        edges.push_back({word, next});
    }

    // setvbuf(stdout, nullptr, _IONBF, 0);
    printf("\033[33mThe random walk is: \033[0m\n");
    if (edges.empty()) {
        printf("%s\n", word.c_str());
        return;
    }
    for (const auto &edge : edges) {
        // 1.5s内如果出现sigint，直接返回（不要继续等待），否则继续
        if (sigInt) {
            cout << endl << "\033[31mInterrupted by SIGINT!\033[0m" << endl;
            sigInt = false;
            return;
        }
        cout << edge.first << " -> ";
        fflush(stdout);
        usleep(1000000);
    }
    cout << edges.back().second << endl;
    // 停止捕获sigint
    signal(SIGINT, SIG_DFL);
    return;
}
