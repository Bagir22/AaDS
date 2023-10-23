#include <fstream>
#include <string> 
#include <iostream>

#include <stack>
#include <deque>

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
    Tnode* next;  
    bool rtag;
    int lvl;
};

void buildTree(const string& fileName, Tnode*& tree) {
    ifstream inFile(fileName);
    string line;

    if (!inFile) {
        cout << "File can't be open" << endl;
        return;
    }

    stack<Tnode*> nodeStack;

    while (getline(inFile, line)) {
        if (line == "#") {
            break;
        }

        int level = 0;
        while (line[level] == '.') {
            level++;
        }

        int value = stoi(line.substr(level));

        Tnode* newNode = new Tnode;
        newNode->val = value;
        newNode->rtag = false;
        newNode->left = nullptr;
        newNode->right = nullptr;

        if (nodeStack.empty()) {
            tree = newNode;
        } else {
            Tnode* parent = nodeStack.top();
            while (parent->lvl >= level && !nodeStack.empty()) {
                nodeStack.pop();
                if (!nodeStack.empty()) {
                    parent = nodeStack.top();
                }
            }
            if (parent->lvl < level) {
                if (!parent->left) {
                    parent->left = newNode;
                } else {
                    parent->right = newNode;
                }
            }

            //cout << " Push " << value << " to parent " << parent->val << endl;
        }

        newNode->lvl = level;
        nodeStack.push(newNode);
    }

    inFile.close();
}

void thravesalTree(Tnode* node, deque<Tnode*>& values) {
    stack<Tnode*> nodeStack;
    Tnode* current = node;

    while (current || !nodeStack.empty()) {
        while (current) {
            values.push_back(current);
            nodeStack.push(current);
            current = current->left;
        }

        current = nodeStack.top();
        nodeStack.pop();
        current = current->right;
    }
}

void threadTree(deque<Tnode*>& values) {
    while (!values.empty())
    {
        if (values.front()->left == nullptr && values.front()->right == nullptr) {
            //cout <<  "Front " << values.front()->val << endl;
            Tnode* parent = values.front();
            values.pop_front();
            if (!values.empty()) {
                parent->next = values.front();
                parent->rtag = true;
            }            
        } else {
            values.pop_front();
        }        
    }    
}

void deleteTree(Tnode*& root) {
    if (!root) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
    root = nullptr;
}

void deleteByValue(Tnode*& node, int value, bool& isDeleted)
{
    if (node == nullptr || isDeleted) {
        return;
    }

    if (node->val == value) {
        isDeleted = true;
        deleteTree(node);
        return;
    }

    deleteByValue(node->right, value, isDeleted);
    deleteByValue(node->left, value, isDeleted);
}

void deleteSubtree(Tnode *root, int value)
{
    bool isDeleted = false;

    deleteByValue(root, value, isDeleted);
}

void printDefaultTree(const Tnode* node) {
    if (node) {
        cout << "|" << string(node->lvl * 2, '-'); 
        cout << node->val;
        cout << endl;
        printDefaultTree(node->left);
        printDefaultTree(node->right);
    }
}

void printThreadedTree(const Tnode* node) {
    if (node) {
        cout << "|" << string(node->lvl * 2, '-'); 
        cout << node->val;
        if (node->rtag == true && node->next->val != 0) {
            cout << " Next: " << node->next->val;
        }
        cout << endl;
        printThreadedTree(node->left);
        printThreadedTree(node->right);
    }
}

int main() {
    char choice;

    cout << "Input file name: " << endl;
    string inputFile;
	cin >> inputFile;
    
    Tnode* tree = nullptr;

    
    while(true) {
        buildTree(inputFile, tree);
        cout << "Default tree:" << endl;
        printDefaultTree(tree);
        cout << endl; 

        deque<Tnode*> values;
        thravesalTree(tree, values);
        threadTree(values);

        cout << "Threaded tree:" << endl;
        printThreadedTree(tree);
        cout << endl; 

        int nodeToDelete;
        cout << "Input node to delete:" << endl;
        cin >> nodeToDelete;
        deleteSubtree(tree, nodeToDelete);
        deque<Tnode*> valuesWhileDelete;
        thravesalTree(tree, valuesWhileDelete);
        threadTree(valuesWhileDelete);
        cout << "Threaded tree after deleteing:" << endl;
        printThreadedTree(tree);
        cout << endl; 

        cout << "Continue (Y/N)?";
        cin >> choice;

        if (choice != 'Y' && choice != 'y') {
            break;
        }
    }

    return 0;
}