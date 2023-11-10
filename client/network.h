#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

class Network;

// Класс для уничтожения экземпляра Network
class NetworkDestroyer
{
private:
    Network* pInstance;
public:
    ~NetworkDestroyer() {
        delete this->pInstance;
    }
    void initialize(Network * p) {
        this->pInstance = p;
    }
};

// Основной класс для работы с сетью
class Network : public QObject {
private:
    // Статический экземпляр Network
    static Network* pInstance;
    // Инициализация TCP-соединения
    void initTCP();
    // Статический экземпляр QTcpSocket
    static QTcpSocket* tcpInstance;
    // Экземпляр NetworkDestroyer для уничтожения
    static NetworkDestroyer destroyer;

protected:
    Network();
    // Запрет копирования и присваивания
    Network(const Network&) = delete;
    Network& operator=(Network&) = delete;

    ~Network();
    friend class NetworkDestroyer;

public:
    // Получение экземпляра Network
    static Network* getInstance() {
        if (!pInstance) {
            pInstance = new Network();
            destroyer.initialize(pInstance);
        }
        return pInstance;
    }

    // Выполнение запроса к серверу
    QJsonObject doRequest(QVariantMap request);
};

#endif // NETWORK_H
