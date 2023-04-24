/**
 * @file server.h
 * @brief Содержит объявление класса MyTcpServer.
 */
#ifndef MYTCPSERVER_H // нужно для того чтобы сервер объявлялся один раз
#define MYTCPSERVER_H // нужно для того чтобы сервер объявлялся один раз
#include <QObject> // класс вокруг которого построена объектная модель Qt. Без него не сможем включать классические вещи из консольных приложений
#include <QTcpServer> // для сервера
#include <QTcpSocket> // для соединения используем сокет

#include <QtNetwork> // для работы по сети
#include <QByteArray> // для передачи сообщений по сети
#include <QDebug>  // для вывода в консоль сообщений
#include <QMap> // для подключения контейнера map

#include "functions.h" *

/**
 * @class MyTcpServer
 * @brief Класс MyTcpServer предоставляет TCP-сервер.
 */
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструирует объект MyTcpServer.
     *
     * @param parent Родительский QObject.
     */
    explicit MyTcpServer(QObject *parent = nullptr); // конструктор, где ключевое слово explicit не дает использовать конструктора неявное преобразование типов
    /**
     * @brief Уничтожает объект MyTcpServer.
     */
    ~MyTcpServer(); // деструктор
public slots: // когда приходит запрос на подключение запускает последовательность поераций которые необходимо сделать при подключении нового клиента
    /**
     * @brief Слот, который вызывается при установлении нового соединения.
     */
    void slotNewConnection();
    /**
     * @brief Слот, который вызывается при отключении клиента.
     */
    void slotClientDisconnected();
    /**
     * @brief Слот, который вызывается, когда сервер готов к чтению данных.
     */
    void slotServerRead();
private:
    /**
     * @brief Экземпляр QTcpServer, связанный с MyTcpServer.
     */
    QTcpServer * mTcpServer; // свойство объекта которое будет самим сервером
    /**
     * @brief Карта дескрипторов клиентских сокетов в указатели QTcpSocket.
     */
    QMap <int, QTcpSocket*> mTcpSocket; // отвечает за подключение; будем хранить сокеты с их id
    /**
     * @brief Текущее состояние сервера.
     */
    int server_status; // для отладки
};
#endif // MYTCPSERVER_H
