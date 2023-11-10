#include "tasks.h"
#include <vector>

// Функция преобразования числа в двоичную строку заданной длины
QString to_binary(int n, int bits) {
    QString res;
    for (int i = bits - 1; i >= 0; i--) {
        res += (n & (1 << i)) ? '1' : '0';
    }
    return res;
}

// Генерация случайной степени двойки от 2^2 до 2^4
int twoToRandomPower() {
    int value = QRandomGenerator::global()->bounded(2, 5);
    return qPow(2, value);
}

// Генерация случайных значений функции
std::vector<bool> randomFunctionValues(int length) {
    std::vector<bool> function_values;
    for (int i = 0; i < length; i++) {
        function_values.push_back(QRandomGenerator::global()->generateDouble() > 0.5);
    }
    return function_values;
}

// Преобразование вектора булевых значений в строку
QString vectorToString(std::vector<bool> functionValues) {
    QString result;
    for (auto value: functionValues) {
        result += value ? '1' : '0';
    }
    return result;
}

// Вычисление СДНФ по вектору значений
QString сalc_sdnf(std::vector<bool> f) {
    QString result;
int n = log2(f.size());
bool first_term = true;
for (int i = 0; i < int(f.size()); ++i) {
    if (f[i]) {
        if (!first_term) {
            result += "+";
        } else {
            first_term = false;
        }
        QString binary = to_binary(i, n);
        for (int j = 0; j < n; ++j) {
            if (binary[j] == '0') {
                result += QString("!x%1").arg(j + 1);
            } else {
                result += QString("x%1").arg(j + 1);
            }
            if (j < n - 1) {
                result += ""; //&
            }
        }
    }
}
if (first_term) {
    result += "0";
}
return result;
}

// Вычисление СКНФ по вектору значений
QString calc_sknf(std::vector<bool> f) {
    QString result;
    int n = log2(f.size());
    bool first_term = true;
    for (int i = 0; i < int(f.size()); ++i) {
        if (!f[i]) {
            if (!first_term) {
                result += ""; //&
            } else {
                first_term = false;
            }
            QString binary = to_binary(i, n);
            result += "(";
            for (int j = 0; j < n; ++j) {
                if (binary[j] == '0') {
                    result +=  QString("x%1").arg(j + 1);
                } else {
                    result += QString("!x%1").arg(j + 1);
                }
                if (j < n - 1) {
                    result += "+";
                }
            }
            result += ")";
        }
    }
    if (first_term) {
        result += "1";
    }
    return result;
}


// Генерация задачи по методу Апокина
QVariantMap TaskApokina() {
    QVariantMap task;
    int length = twoToRandomPower();
    std::vector<bool> functionValues = randomFunctionValues(length);
    QString answer = сalc_sdnf(functionValues);
        task["text"] = QString("Найдите СДНФ по данному вектору значений: %1")
              .arg(vectorToString(functionValues));
    task["answer"] = answer;
    return task;
}

// Генерация задачи по методу Кутузова
QVariantMap TaskKutuzova() {
    QVariantMap task;
    int n = twoToRandomPower();
    std::vector<bool> functionValues = randomFunctionValues(n);
    QString answer = calc_sknf(functionValues);
    task["text"] = QString("Найдите СКНФ по данному вектору значений: %1")
                       .arg(vectorToString(functionValues));
    task["answer"] = answer;
    return task;
}

// Генерация третьей задачи
QVariantMap Task3() {
}
