#include <fstream>
#include <string> 
#include <iostream>

#include <stack>

using namespace std;

/*
  Общими требованиями к лабораторной работе являются:
   1) вводить   исходное   дерево  из  файла  в  понятной  для
пользователя форме, а не с клавиатуры;
   2) по требованию прользователя показывать дерево на экране;
   3) обрабатывать  дерево  в  оперативной памяти,  а не путем
многократного обращения к файлу;
   4) обеспечить   возможность   многократных   запросов   без
повторного запуска программы.

18. Задано бинарное  дерево.  Построить  прошитое   дерево,
соответствующее обходу сверху вниз. Составить программу удале-
ния поддерева с  корнем  в  заданной  вершине  без  повторного
построения дерева. Конечное дерево должно оставаться прошитым. 
Выдать информацию о нитях исходного и конечного деревьев (12).

Январев Данила ПС-22
VS Code
*/


struct Tnode {
    int val;
    Tnode* left;
    Tnode* right;
    bool ltag;
    bool rtag;
    int lvl;
};

void buildTree(const string& fileName, Tnode*& tree) {
    ifstream inFile(fileName);
    string line;

    if (!inFile) {
        cout << "Файл не может быть открыт.\n";
        return;
    }

    stack<Tnode*> nodeStack;
    int currentLevel = 0;

    while (getline(inFile, line)) {
        if (line == "#") {
            break;
        }

        int level = 0;
        while (line[level] == '.') {
            level++;
        }

        int value = stoi(line.substr(level));

        cout << "Val " << value << " Level " << level << endl;
        
        Tnode* newNode = new Tnode;
        newNode->val = value;
        newNode->ltag = false;
        newNode->rtag = false;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->lvl = level;

        while (currentLevel > level) {
            nodeStack.pop();
            currentLevel--;
        }

        if (!nodeStack.empty()) {
            Tnode* parent = nodeStack.top();
            if (!parent->left) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        } else {
            tree = newNode; 
        }

        nodeStack.push(newNode);
        currentLevel = level;
    }

    inFile.close();
}

void preOrderPrint(const Tnode* node) {
    if (node) {
        cout << "|" << string(node->lvl * 2, '-'); 
        cout << node->val << endl;
        preOrderPrint(node->left);
        preOrderPrint(node->right);
    }
}



int main() {
    char choice;

    cout << "Введите имя файла: " << endl;
    string inputFile;
	cin >> inputFile;
    
    Tnode* tree = nullptr;

    
    while(true) {
        buildTree(inputFile, tree);
        preOrderPrint(tree);

        cout << "Продолжить (Y/N)?";
        cin >> choice;

        if (choice != 'Y' && choice != 'y') {
            break;
        }
    }

    return 0;
}