/**
 * @file mytcpserver.cpp
 * @brief Содержит объявление и определения класса MyTcpServer.
 */
#include "server.h"

using namespace std;

/**
 * @brief Деструктор для MyTcpServer
 *
 * Этот деструктор закрывает сервер, если он открыт, и устанавливает server_status в 0.
 */
MyTcpServer::~MyTcpServer()
{
    mTcpServer->close(); // закрывает сервер
    server_status=0; // сервер не запущен
}

/**
 * @brief Конструирует объект MyTcpServer.
 *
 * @param parent Родительский QObject.
 */
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this); // выделение памяти под объект сервера
    connect(mTcpServer, &QTcpServer::newConnection, // отвечает за сигналы и слоты; mTcpServer - источник сигнала,
            // &QTcpServer::newConnection - название сигнала,
            // this - наш сервер, slotnewconnection - название слота
            this, &MyTcpServer::slotNewConnection); // connect - ключевое слово, соединяющее сигналы и слоты

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){ // прослушивает любой ip адрес с портом 33333
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

/**
 * @brief Слот, который вызывается при установлении нового соединения.
 */
void MyTcpServer::slotNewConnection(){
    if(server_status==1){
        QTcpSocket *cur_mTcpSocket; // создаем переменную дял текущего сокета
        cur_mTcpSocket = mTcpServer->nextPendingConnection();// инициализация текущего сокета, который возвращается методом nextpendingconnection
        cur_mTcpSocket->write("Hello, World!!! I am echo server!\r\n");// сокет отправляет сообщение
        connect(cur_mTcpSocket, &QTcpSocket::readyRead,// connect на сообщение; если получение сообщение
                this,&MyTcpServer::slotServerRead); // запускаем slotserveread
        connect(cur_mTcpSocket,&QTcpSocket::disconnected,// connect на отключение
                this,&MyTcpServer::slotClientDisconnected); // если сокет отвалился то slotclientdisconnected
        mTcpSocket.insert(cur_mTcpSocket->socketDescriptor(),cur_mTcpSocket);// для хранения текущего сокета
    }
}

/**
 * @brief Слот, который вызывается, когда сервер готов к чтению данных.
 */
void MyTcpServer::slotServerRead(){
    QTcpSocket *cur_mTcpSocket = (QTcpSocket*)sender(); // возвращает указатель на объект пославший сигнал
    QString res = "";
    while(cur_mTcpSocket->bytesAvailable()>0){ // пока можно что то считать
        QByteArray array=cur_mTcpSocket->readAll(); // считываем строку
        res.append(array);
    }
    cur_mTcpSocket->write(parsing(res.toUtf8(), 6)); // обратно отправляем клиенту; toUtf8 возвращает строку
}

/**
 * @brief Слот, который вызывается при отключении клиента.
 */
void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *cur_mTcpSocket = (QTcpSocket*)sender(); // возвращает указатель на объект пославший сигнал
    cur_mTcpSocket->close();    // закрываем сокет
}
