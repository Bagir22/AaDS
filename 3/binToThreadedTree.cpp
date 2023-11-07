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

void buildTree(istream& inFile, Tnode*& tree) {
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
}

void travesalTree(Tnode* node, deque<Tnode*>& values) {
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
    if (root == nullptr) {
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

bool deleteSubtree(Tnode *root, int value)
{
    bool isDeleted = false;

    deleteByValue(root, value, isDeleted);

    return isDeleted;
}

void printDefaultTree(ostream& out, const Tnode* node) {
    if (node) {
        out << "|" << string(node->lvl * 2, '-'); 
        out << node->val;
        out << endl;
        printDefaultTree(out, node->left);
        printDefaultTree(out, node->right);
    }
}

void printThreadedTree(ostream& out, const Tnode* node) {
    if (node) {
        out << "|" << string(node->lvl * 2, '-'); 
        out << node->val;
        if (node->rtag && node->next->val != 0) {
            out << " Next: " << node->next->val;
        }
        out << endl;
        printThreadedTree(out, node->left);
        printThreadedTree(out, node->right);
    }
}

int main() {
    char choice;

    cout << "Input file name: " << endl;
    string inputFile;
	cin >> inputFile;
    
    Tnode* tree = nullptr;

    ofstream outFile("out.txt", ios_base::out);
    ifstream inFile(inputFile);
    outFile.clear();

    buildTree(inFile, tree);
    inFile.close();
    outFile << "Default tree:" << endl;
    printDefaultTree(outFile, tree);
    outFile << endl;

    deque<Tnode*> values;
    travesalTree(tree, values);
    threadTree(values);

    outFile << "Threaded tree:" << endl;
    printThreadedTree(outFile, tree);
    outFile << endl;

    while(true) {
        ofstream outFile("out.txt", ios_base::app);
        int nodeToDelete;
        cout << "Input node to delete:" << endl;
        cin >> nodeToDelete;
        outFile << "Node to delete " << nodeToDelete << endl;
        bool isDeleted = deleteSubtree(tree, nodeToDelete);

        if (isDeleted) {
            deque<Tnode*> valuesWhileDelete;
            travesalTree(tree, valuesWhileDelete);
            threadTree(valuesWhileDelete);
            outFile << "Threaded tree after deleting " << nodeToDelete << " node:" << endl;
            printThreadedTree(outFile, tree);
            outFile << endl; 
        } else {
            outFile << "No node found to delete" << endl;
        }
        
        outFile.close();

        cout << "Continue (Y/N)?";
        cin >> choice;

        if (choice != 'Y' && choice != 'y') {
            break;
        }
    }

    return 0;
}