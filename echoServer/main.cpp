#include <QCoreApplication>
#include "mytcpserver.h"

// Основная функция программы
int main(int argc, char *argv[])
{
    // Создание объекта приложения Qt
    QCoreApplication a(argc, argv);

    // Создание объекта MyTcpServer
    MyTcpServer server;

    // Запуск цикла обработки событий приложения
    return a.exec();
}
