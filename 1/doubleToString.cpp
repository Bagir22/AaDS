#include <iostream>
#include <sstream> 

using namespace std;

/*
Составить программу перевода вещественного числа в форму
константы с плавающей  точкой  в строковом формате. Целая часть
мантиссы должна состоять из одной цифры в диапазоне  от 1 до 9.
Исходное число имеет тип double, а преобразуется к типу string.
Число знаков после десятичной точки задается (8).
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

int getNum(double *num, double *precision) 
{
    if (*num == *precision) {
        return 1;
    }

    int steps = 0;
    while (*num >= *precision) {
        steps++;
        *num -= *precision;
    }

    return steps;
}

int getWholePart(double *num) {
    while(*num > 10) {
        *num = *num / 10;
    }

    int steps = 0;
    while(*num > 1) {
        steps++;
        *num = *num -1;
    }

    return steps;
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

    if (isNegative == true) {
        result += "-";
    }

    int wholePart = getWholePart(&num);
    result += intToString(wholePart);
    result += ".";

    cout << "Результат: ";    
    cout << result;
    cout << "\n";
}



