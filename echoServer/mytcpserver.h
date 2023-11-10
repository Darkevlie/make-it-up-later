#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include "serverfunction.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:

    // Слот для обработки нового подключения клиента
    void slotNewConnection();

    // Слот для обработки отключения клиента
    void slotClientDisconnected();

    // Слот для обработки данных, полученных от клиента
    void slotServerRead();

private:
    QTcpServer * mTcpServer; // Объект сервера
    std::list<QTcpSocket*> TcpSocketList; // Список сокетов клиентов
    int server_status; // Статус сервера (0 - выключен, 1 - включен)
};
#endif // MYTCPSERVER_H
