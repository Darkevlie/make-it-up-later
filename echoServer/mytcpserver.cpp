#include "mytcpserver.h"

// Конструктор класса MyTcpServer
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent) {
    // Создание объекта QTcpServer
    mTcpServer = new QTcpServer(this);

    // Подключение сигнала newConnection к слоту slotNewConnection
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    // Попытка запустить сервер на всех доступных сетевых интерфейсах и порту 3333
    if (!mTcpServer->listen(QHostAddress::Any, 3333)) {
        qDebug() << "Сервер не запущен";
    } else {
        server_status = 1;
        qDebug() << "Сервер запущен";
    }
}

// Деструктор класса MyTcpServer
MyTcpServer::~MyTcpServer()
{
    // Закрытие сервера и изменение статуса сервера
    mTcpServer->close();
    server_status = 0;
}

// Слот для нового подключения к серверу
void MyTcpServer::slotNewConnection(){
    if (server_status == 1){
        // Получение временного сокета для нового подключения
        QTcpSocket *tempSocket;
        tempSocket = mTcpServer->nextPendingConnection();

        // Отправка приветственного сообщения клиенту
        tempSocket->write("Привет!!!\r\n");

        // Подключение сигнала readyRead сокета к слоту slotServerRead
        connect(tempSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);

        // Подключение сигнала disconnected сокета к слоту slotClientDisconnected
        connect(tempSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

        // Добавление сокета в список активных сокетов
        TcpSocketList.push_front(tempSocket);
    }
}

// Слот для чтения данных от клиента
void MyTcpServer::slotServerRead() {
    // Получение временного сокета, отправившего данные
    QTcpSocket* tempSocket = (QTcpSocket*)sender();

    // Чтение данных из сокета
    QByteArray request;
    while (tempSocket->bytesAvailable() > 0) {
        request.append(tempSocket->readAll());
    }

    // Обработка запроса и отправка ответа
    QByteArray response = parse(request) + "\r\n";
    tempSocket->write(response);
}

// Слот для отключения клиента
void MyTcpServer::slotClientDisconnected(){
    // Получение временного сокета, отключившегося
    QTcpSocket* tempSocket = (QTcpSocket*) sender();

    // Закрытие сокета
    tempSocket->close();
}
