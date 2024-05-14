#include "graph.h"

const set<char> delimiters = {',', '.', '!', '(', ')', '-',  '+', '=', '[', ']',
                              '{', '}', ':', ';', '"', '\'', '<', '>', '?'};
// mermaid的关键字集合
const set<string> mermaidKeywords = {"graph", "subgraph",  "end",  "style",
                                     "click", "direction", "fill", "stroke"};

int main() {
    TextGraph *tg = nullptr, *tmp;
    string str1, str2;
    int command, length;
    vector<string> v;
    while (1) {
        system("reset");
        printf("\t\t\tWelcome to the TextGraph System\n"
               "1. Create a graph with a text file\n"
               "2. Show the graph\n"
               "3. Query the bridge words\n"
               "4. Generate a new text\n"
               "5. Calculate the shortest v\n"
               "6. Random walk\n"
               "7. Exit\n"
               "\nPlease input the command: ");
        cin >> command;
        if (tg == nullptr && command != 1 && command != 7) {
            // output with red color
            printf("\033[31mCreate a graph first!\033[0m\n");
            sleep(1);
            continue;
        }
        if (command == 7) {
            goto exit;
        }
        switch (command) {
        case 1:
            printf("Input the file name: ");
            cin >> str1;
            tmp = tg;
            tg = new TextGraph(str1.c_str());
            if (tmp != nullptr) {
                delete tmp;
            }
            break;
        case 2:
            tg->showGraph();
            break;
        case 3:
            cout << "Input two words: ";
            cin >> str1 >> str2;
            v = tg->queryBridgeWords(str1, str2);
            if (v[0].find("No") != string::npos) {
                cout << v[0] << endl;
            } else {
                cout << "The bridge words from " << str1 << " to " << str2
                     << " are: ";
                for (int i = 0; i < v.size(); i++) {
                    cout << v[i];
                    if (i != v.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }
            break;
        case 4:
            cout << "Input a text: ";
            getchar();
            getline(cin, str1);
            cout << "The new text is: " << tg->generateNewText(str1) << endl;
            break;
        case 5:
            cout << "Input two words: ";
            cin >> str1 >> str2;
            v = tg->calcShortestPath(str1, str2);
            if (v.size() == 0) {
                cout << "\033[31mNo path from " << str1 << " to " << str2
                     << "\033[0m" << endl;
            } else {
                cout << "The shortest path from " << str1 << " to " << str2
                     << " is: " << endl;
                for (int i = 0; i < v.size(); i++) {
                    cout << v[i];
                    if (i != v.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << endl;
            }
            break;
        case 6:
            cout << "The random walk is: " << endl << tg->randomWalk() << endl;
            break;
        case 7:
            delete tg;
            goto exit;
            break;
        default:
            printf("Invalid command!\n");
            break;
        }
        sleep(2);
    }
exit:
    system("reset");
    printf("\t\tGood Bye!\n");
    sleep(1);
    return 0;
}