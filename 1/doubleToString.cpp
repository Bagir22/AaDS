#include <iostream>

using namespace std;

/*
Составить программу перевода вещественного числа в форму
константы с плавающей  точкой  в строковом формате. Целая часть
мантиссы должна состоять из одной цифры в диапазоне  от 1 до 9.
Исходное число имеет тип double, а преобразуется к типу string.
Число знаков после десятичной точки задается (8).

Январев Данила ПС-22
VS Code
*/

string intToString(int i) {
    switch(i) {
        case 0:
            return "0";
        case 1:
            return "1";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        default:
            return "-1";                                        
    }
}

string eString(int eCount) {
    if (eCount <= 9) {
        return intToString(eCount);
    }

    string result = "";
    int len = 0;
    while (eCount > 0) {
        int digit = eCount % 10;
        string digitStr = intToString(digit);
        eCount /= 10;
        len++;
        result += digitStr;
    }

    if (len == 2) {
        char tmp = result[0];
        result[0] = result[1];
        result[1] = tmp;
        return result;
    }

    int n = len - 1;
    int i, tmp;
    for(i = 0; i <= (len / 2); i++) {
        tmp = result[i];
        result[i] = result[n];
        result[n] = tmp;
        n--;
    }

    return result;
}

int main()
{
    int count;
    cout << "Введите число символов после точки:\n";    
    cin >> count;

    if (count < 0) {
        cout << "Число слишком маленькое\n";
        return 1;
    }

    double num;   
    cout << "Введите число:\n";
    cin >> num;
 
    if (num == 0) {
        cout << "Результат: ";
        cout << 0;
        cout << "\n";
        return 0;
    }

    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num = num * -1;
    }    
    
    string result = "";

    int eCount = 0;

    bool isZero = false;

    while (num < 1)
    {
        num *= 10;
        eCount--;
        isZero = true;
    }
    
    while (num > 10)
    {
        num /= 10;
        eCount++;
    }
    
    string t = eString(21);
    double precision = 1;

    bool isFirst = true;

    while (num > 0) {
        int digit = num / precision;

        if (isFirst == true) {
            result += intToString(digit) += ".";
            isFirst = false;
        } else if (count >= 0) {
            result += intToString(digit);
        }

        num -= digit * precision;
       
        precision *= 0.1;
        count--;

        if (num <= precision || num < 1e-20) {
            break;
        }
    }
    

    while (count >= 0) {
        result += "0";
        count--;
    }

    if (eCount != 0) {
        result += "e";
        if (isZero == true) {
            result += "-";
            eCount *= -1;
        } else {
            result += "+";
        }

        result += eString(eCount);
    }
         
    cout << "Результат: " << result << endl;    
}
