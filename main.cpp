/**
 * @file main.cpp
 * @brief Содержит главную функцию, которая запускает приложение.
 */
#include <QCoreApplication>
//#include "functions.h"
#include "server.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * @brief Основная функция, запускающая приложение.
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); // создает консольное приложение с именем а с параметрами argc и argv

    MyTcpServer myserv;

    return a.exec();
}
