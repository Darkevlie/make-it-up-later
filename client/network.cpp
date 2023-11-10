#include "network.h"
#include "QCoreApplication"
#include "QMessageBox"

Network::Network()
{
    // Инициализация TCP соединения при создании объекта сети
    this->initTCP();
}

Network::~Network()
{
    // Закрытие TCP соединения при уничтожении объекта сети
    this->tcpInstance->close();
}

void Network::initTCP() {
    // Установка TCP соединения с сервером (127.0.0.1:3333)
    this->tcpInstance = new QTcpSocket(this);
    this->tcpInstance->connectToHost("127.0.0.1", 3333);

    // Проверка успешности соединения
    if (this->tcpInstance->waitForConnected(1000)) {
        qDebug() << "Connected!";
        tcpInstance->waitForReadyRead(1000);
        this->tcpInstance->readAll();
    }
    else {
        qDebug() << "Not Connected!";
    }
}

QJsonObject Network::doRequest(QVariantMap request) {
    // Отправка запроса на сервер и получение ответа
    this->tcpInstance->write(QJsonDocument::fromVariant(request).toJson());
    this->tcpInstance->waitForBytesWritten(1000);
    this->tcpInstance->waitForReadyRead(2000);
    QByteArray response = this->tcpInstance->readAll();
    qDebug() << response.toStdString();
    return QJsonDocument::fromJson(response).object();
}

Network *Network::pInstance = nullptr;
QTcpSocket *Network::tcpInstance;
NetworkDestroyer Network::destroyer;
