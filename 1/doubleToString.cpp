#include <iostream>
#include <sstream> 

using namespace std;

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

int getWholePart(double *num, int *eCount, int *zeroes) {
    while(*num >= 10) {
        *num = *num / 10;
        *eCount = *eCount + 1;
    }

    int steps = 0;
    while(*num >= 1) {
        steps++;
        *num = *num - 1;
    }

    if (*eCount % 10 == 0) {
        *zeroes *= 10;
    } 

    return steps;
}

int getDigit(int *num, int *precision) {
    return *num / *precision;
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

    int eCount = 0;
    int zeroes = 1;

    int wholePart = getWholePart(&num, &eCount, &zeroes);
    result += intToString(wholePart);
    result += ".";

    double precision = 0.1;
    
    while (count != 0) {
        int digit = getNum(&num, &precision);
        result += intToString(digit);
        precision = precision * 0.1;
        count--;
        eCount++;
        if (eCount % 10 == 0) {
            zeroes *= 10;
        } 
    }

    while (num > 0 && num >= precision) {
        int digit = getNum(&num, &precision);

        precision = precision * 0.1;
        eCount++;

        if (eCount % 10 == 0) {
            zeroes *= 10;
        } 
    }

    result += "e+";

    if (eCount < 10) {
        result += intToString(eCount);
    } else {
        while (eCount > 0) {
            if (eCount < 10) {
                result += intToString(eCount);
                break;
            } else {
                int digit = getDigit(&eCount, &zeroes);
                eCount /= 10;
                zeroes /= 10;
                result += intToString(digit);
            }
        }
    }

    cout << "Результат: ";    
    cout << result;
    cout << "\n";
}
