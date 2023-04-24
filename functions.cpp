/**
 * @file functions.cpp
 * @brief Описание функций сервера
 */
#include "functions.h"
#include "server.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>


/**
 * @brief Получает данные от клиента и выполняет соответствующее действие.
 *
 * @param data_from_client QString, представляющий данные, полученные от клиента.
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий результат выполненного действия или сообщение об ошибке.
 */
QByteArray parsing(QString data_from_client, int sock_desc){
    QString login;
    data_from_client = data_from_client.left(data_from_client.length()-2);
    qDebug() << data_from_client;

    QStringList data_from_client_list = data_from_client.split(QLatin1Char('&'));
    qDebug()<<data_from_client_list.front();

    QString nameOfFunc = data_from_client_list.front();
    data_from_client_list.pop_front();

    if (nameOfFunc == "auth"){
        change_sock_desc(data_from_client_list.at(0), sock_desc);
        return auth(data_from_client_list.at(0), data_from_client_list.at(1));
    }
    else if (nameOfFunc == "reg"){
        QString log =data_from_client_list.at(0);
        return reg(data_from_client_list.at(0), data_from_client_list.at(1), data_from_client_list.at(2), sock_desc);
        change_sock_desc(data_from_client_list.at(0), sock_desc);
    }else if (nameOfFunc == "updstat") {
        qDebug()<<login;
        return check_ans(data_from_client_list.at(0), data_from_client_list.at(1), sock_desc);}
    else if (nameOfFunc == "stat")
    {
        qDebug()<<"stat" + data_from_client_list.at(0);
        return look_stat(sock_desc);
    }
    else if (nameOfFunc == "stat1")
    {
        qDebug()<<"stat" + data_from_client_list.at(0);
        return look_stat1(sock_desc);
    }
    else if (nameOfFunc == "stat2")
    {
        qDebug()<<"stat" + data_from_client_list.at(0);
        return look_stat2(sock_desc);
    }
    else if (nameOfFunc == "stat3")
    {
        qDebug()<<"stat" + data_from_client_list.at(0);
        return look_stat3(sock_desc);
    }
    else if (nameOfFunc == "del")
    {
        qDebug()<<"stat" + data_from_client_list.at(0);
        return del_status(sock_desc);
    }
    else return "error\r\n";
}

/**
 * @brief Аутентифицирует пользователя с заданными учетными данными.
 *
 * @param log Имя пользователя.
 * @param pass Пароль пользователя.
 *
 * @return QByteArray, содержащий результат аутентификации.
 */
QByteArray auth(QString log, QString pass)
{

    QByteArray result = "";
    result.append(Database::getInstance()->auth(log, pass));
    qDebug() << result;
    return result;
}

/**
 * @brief Регистрирует нового пользователя в базе данных.
 *
 * @param log QString, обозначающий логин пользователя.
 * @param pass QString, обозначающий пароль пользователя.
 * @param mail QString, обозначающий электронную почту пользователя.
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий результат операции регистрации.
 */
QByteArray reg(QString log, QString pass, QString mail, int sock_desc)
{
    QByteArray result = "";
    result.append(Database::getInstance()->reg(log, pass, mail, sock_desc));
    qDebug() << result;
    change_sock_desc(log, sock_desc);
    return result;
}

/**
 * @brief Проверяет ответ пользователя в базе данных.
 *
 * @param numb QString, представляющий номер связанный с пользователем.
 * @param ans QString, представляющий проверяемый ответ.
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий результат операции проверки ответа.
 */
QByteArray check_ans(QString numb, QString ans, int sock_desc)
{
    QByteArray res;

    bool res = false;
    if(task_number=="1")
    {
        res = (ans==forward(variant));
    }
    else if(task_number=="2")
    {
        res = (ans == backward(variant));
    }
    else if(tase_number =="3")
    {
        res = (ans == viterbi(variant))
    }
    if(res)
        ans = "true";

    res.append(Database::getInstance()->check_ans(numb, ans, sock_desc));
    qDebug()<<res;
    return res;
}

/**
 * @brief Получает статистику первого пользователя из базы данных.
 *
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий статистику.
 */
QByteArray look_stat(int sock_desc)
{
    QByteArray result;
    result.append(Database::getInstance()->count_stat(sock_desc).toInt());
    qDebug()<<result;
    return result;
}
/**
 * @brief Получает статистику второго пользователя из базы данных.
 *
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий статистику.
 */
QByteArray look_stat1(int sock_desc)
{
    QByteArray result;
    result.append(Database::getInstance()->count_stat1(sock_desc).toInt());
    qDebug()<<result;
    return result;
}
/**
 * @brief Получает статистику третьего пользователя из базы данных.
 *
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий статистику.
 */
QByteArray look_stat2(int sock_desc)
{
    QByteArray result;
    result.append(Database::getInstance()->count_stat2(sock_desc).toInt());
    qDebug()<<result;
    return result;
}
/**
 * @brief Получает статистику четвертого пользователя из базы данных.
 *
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий статистику.
 */
QByteArray look_stat3(int sock_desc)
{
    QByteArray result;
    result.append(Database::getInstance()->count_stat3(sock_desc).toInt());
    qDebug()<<result;
    return result;
}
/**
 * @brief Изменение статуса пользователя в базе данных.
 *
 * @param login QString, представляющий логин пользователя.
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий результат операции изменения статуса.
 */
QByteArray change_sock_desc(QString login, int sock_desc)
{
    QByteArray result;
    result.append(Database::getInstance()->change_status(login, sock_desc));
    qDebug()<<result;
    return result;
}

/**
 * @brief Удаляет статус пользователя в базе данных.
 *
 * @param sock_desc Целое число, представляющее дескриптор сокета.
 *
 * @return QByteArray, содержащий результат операции удаления статуса.
 */
QByteArray del_status(int sock_desc)
{
    QByteArray result;
    result.append(Database::getInstance()->del_status(sock_desc));
    return result;
}



double defSumRows(double** arr, int max, int colum) {
    double result = 0;
    for (int row = 0; row < max; row++) {
        result = result + arr[colum][row];
    }
    return result;
}

double defMultiplyCols(double** arr, double** arr_2, int hmax, int row) {
    double result = 0;
    for (int column = 0; column < hmax; column++) {
        result = result + arr[column][row] * arr_2[column][row];
        // std::cout << arr[column][row] << '*' << arr_2[column][row] << '\n';
        // std::cout << result << ' ';
    }
    return result;
}

void defSecondPos(double** arr, int hmax, int wmax) {
    int sumRows = 0;
    for (int column = 0; column < hmax; column++) {
        sumRows = defSumRows(arr, wmax, column);
        for (int row = 0; row < wmax; row++) {
            arr[column][row] = arr[column][row] / sumRows;
        }
    }
}




void defForword(double** speech, double** words, int hmax, int wmax) {
    for (int i = 1; i < hmax; i++) {
        double temp = 0;
        for (int j = 0; j < wmax; j++) {
            temp = words[i][j];
            words[i][j] = 0;
            for (int k = 0; k < wmax; k++) {
                words[i][j] += words[i - 1][k] * speech[k][j];
            }
            words[i][j] *= temp;
        }
    }

}

void defBackward(double** speech, double** words, int hmax, int wmax) {
    double** temp;
    temp = new double* [hmax];
    for (int i = 0; i < wmax; i++) {
        temp[i] = new double[wmax];
    }
    for (int i = 0; i < hmax; i++) {
        for (int j = 0; j < wmax; j++) {
            if (i == hmax - 1) {
                temp[i][j] = words[i][j];
            }
            else {
                temp[i][j] = 0;
            }
        }
    }

    for (int i = hmax - 2; i >= 0; i--) {
        for (int j = 0; j < wmax; j++) {
            for (int k = 0; k < wmax; k++) {
                temp[i][j] += temp[i + 1][k] * words[i + 1][k] * speech[j][k];
            }
        }
    }

    for (int i = 0; i < hmax; i++) {
        for (int j = 0; j < wmax; j++) {
            words[i][j] = temp[i][j];
        }
    }
}

double Max(std::vector<double> row) {
    double max = 0;
    for (int i = 0; i < row.size(); i++) {
        if (row[i] > max) {
            max = row[i];
        }
    }
    return max;
}

void defViterbi(double** speech, double** words, int hmax, int wmax) {
    double** temp;
    temp = new double* [hmax];
    for (int i = 0; i < wmax; i++) {
        temp[i] = new double[wmax];
    }
    for (int i = 0; i < hmax; i++) {
        for (int j = 0; j < wmax; j++) {
            if (i == 0) {
                temp[i][j] = words[i][j];
            }
            else {
                temp[i][j] = 0;
            }
        }
    }
    std::vector<double> row;
    int counter = 0;
    for (int i = 1; i < hmax; i++) {
        for (int j = 0; j < wmax; j++) {
            if (i != 1) {
                counter = i;
            }
            if (i == 4) {
                counter = 2;
            }
            double max = 0;
            for (int k = 0; k < wmax; k++) {
                row.push_back(temp[i - 1][k] * words[i][j] * speech[k][counter]);
            }
            temp[i][j] = Max(row);
            row.clear();
        }
    }

    for (int i = 0; i < hmax; i++) {
        for (int j = 0; j < wmax; j++) {
            words[i][j] = temp[i][j];
        }
    }
}

void forward(int countWords, double** speech, double** words){
    int const speechmax = 10;
    defSecondPos(speech, speechmax, speechmax);
    defArrShowing(speech, speechmax, speechmax);
    defSecondPos(words, countWords, speechmax);
    defArrShowing(words, countWords, speechmax);
    defForword(speech, words, countWords, speechmax);

}

void backward(int countWords, double** speech, double** words){
    int const speechmax = 10;
    defSecondPos(speech, speechmax, speechmax);
    defArrShowing(speech, speechmax, speechmax);
    defSecondPos(words, countWords, speechmax);
    defArrShowing(words, countWords, speechmax);
    defBackward(speech, words, countWords, speechmax);
}
void viterbi(int countWords, double** speech, double** words){
    int const speechmax = 10;
    defSecondPos(speech, speechmax, speechmax);
    defArrShowing(speech, speechmax, speechmax);
    defSecondPos(words, countWords, speechmax);
    defArrShowing(words, countWords, speechmax);
    defViterbi(speech, words, countWords, speechmax);
}
