#include <iostream>
#include <sstream> 
#include <unistd.h>
#include <iomanip> 

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
 
    //cout << "Num: " <<setprecision(10) << num << endl;

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

    while (num < 1)
    {
        num *= 10;
        eCount--;
    }
    

    while (num > 10)
    {
        num /= 10;
        eCount++;
    }

    //cout << "Num: " <<setprecision(10) << num << endl;
    double precision = 1;

    bool isFirst = true;

    while (num > 0) {
        int digit = num / precision;
        if (isFirst == true) {
            result += intToString(digit) += ".";
            isFirst = false;
        } else if (count < 0) {
            eCount++;
        } else {
            result += intToString(digit);
        }

        num -= digit * precision;
       
        precision *= 0.1;
        count--;
        
        cout << "N : " <<setprecision(20) << num << endl;
        cout << "P : " << precision << endl;

        if (num <= precision || num < 1e-10) {
            break;
        }

        sleep(0.5);
    }
    

    while (count >= 0) {
        result += "0";
        count--;
    }

    if (eCount != 0) {
        result += "e";
        if (isNegative == true) {
            result += "-";
        } else {
            result += "+";
        }
        //cout << "E : " << eCount << endl;
        result += intToString(eCount);
    }

    cout << "Результат: " << result << endl;    
}
