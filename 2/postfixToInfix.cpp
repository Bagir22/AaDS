#include <iostream>
#include <fstream>
#include <string>   // для std::getline

using namespace std;

/*
  Общими требованиями к лабораторной работе являются:
   1) организовать  ввод  данных  из  файла  в   понятной  для 
пользователя форме;
   2) обеспечить   возможность   многократных   запросов   без
повторного запуска программы;
   3) при реализации в С++ не использовать контейнерные классы
для работы с линейными списками типа stack, queue и т. п.

9. В   символьной  строке  записано  выражение  из  букв  и
операций в постфиксной форме (знак операции  после операндов).
Проверить   правильность   записи   и  перевести  выражение  в
инфиксную (обычную)  форму  со  скобками. Например,  выражение
(a+b)*c-d*e  записывается  в постфиксной форме как  ab+c*de*-.
Требуется выдать его в виде (((a+b)*c)-(d*e)) (7).
*/

struct Stack {
    string key;
    Stack *next;
};

void push(Stack *&s, string elem) {
    Stack* newNode = new Stack;
    newNode->key = elem;
    newNode->next = s;
    s = newNode;
}

string pop(Stack *&s) {
    if (s == nullptr) {
        return "";
    }
    
    string elem = s->key;     
    Stack* temp = s; 
    s = s->next;
    delete temp;

    return elem;
}

int stackLen(Stack *&s) {
    int len = 0;
    const Stack* current = s;

    while (current != nullptr) {
        len++;
        current = current->next;
    }

    return len;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool  isAlphabet(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
        return true;
    }

    return false;
}

string toInfix(string in) {
    if (in.length() == 0) {
        return "Ошибка: Пустая строка";
    }

    Stack* operandStack = nullptr;

    for (int i = 0; i < in.length(); i++) {
        char currentChar = in[i];

        if (isAlphabet(currentChar) == true || isOperator(currentChar) == true) {
            if (isOperator(currentChar)) {
                if (operandStack == nullptr || operandStack->next == nullptr) {
                    return "Ошибка: лишний операнд " + string(1, currentChar) + " на позиции " + to_string(i+1);
                }

                string operand2 = pop(operandStack);
                string operand1 = pop(operandStack);

                string expression = "(" + operand1 + currentChar + operand2 + ")";
                push(operandStack, expression);
            } else {
                push(operandStack, string(1, currentChar));
            }
        } else if (isAlphabet(currentChar) == false) {
            return "Ошибка: некорректный операнд " + string(1, currentChar) + " на позиции " + to_string(i+1);
        } else {
            return "Ошибка: некорректный оператор " + string(1, currentChar) + " на позиции " + to_string(i+1);
        }
    }

 
    if (operandStack == nullptr || operandStack->next != nullptr) {
        return "Ошибка: недостаточно операторов";
    }

    return operandStack->key;
}

int main() {
    char choice;
    
    cout << "Введите имя файла: ";
    string inputFile;
	cin >> inputFile;
	cout << "\n";
    char outputFile[] = "out.txt";

    int readedLines = 0;
    while (true) {
        ifstream inFile(inputFile, ios_base::in);
        ofstream outFile("out.txt", ios_base::app);
        string line;
        if (!inFile) {
            cout << "Файл не открыт\n\n";
            return -1;
        }

        int currLine = 0;
        if (currLine < readedLines) {
            while (getline(inFile, line)) {
                currLine++;
                if (currLine == readedLines) {
                    break;
                }
            }
            
        }
        
        while (getline(inFile, line)) {
            string result = toInfix(line);
            outFile << result << endl;
            readedLines++;
        }

        inFile.close();
        outFile.close();

        cout << "Продолжить (Y/N)?";
        cin >> choice;

        if (choice != 'Y' && choice != 'y') {
            break;
        }
    }

    return 0;
}