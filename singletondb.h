/**
 * @file singletondb.h
 * @brief Содержит объявление класса синглтона и связанные с ним функции.
 */
#ifndef SINGLETONDB_H
#define SINGLETONDB_H

#include <QString>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QByteArray>

class Database;

/**
 * @brief Класс для обеспечения корректного уничтожения экземпляра базы данных синглтон.
 */
class DatabaseDestroyer
{
private:
    Database * pInstance;
public:
    ~DatabaseDestroyer() {
        delete this->pInstance;
    }
    void initialize(Database * p) {
        this->pInstance = p;
    }
};

/**
 * @brief Класс синглтон для обработки взаимодействия с базой данных.
 */
class Database {
private:
    static Database * pInstance;
    void initDB();
    static QSqlDatabase dbInstance;
    static DatabaseDestroyer destroyer;
protected:
    Database();
    Database(const Database& ) = delete;
    Database& operator = (Database &)=delete;
    ~Database();
    friend class DatabaseDestroyer;
public:
    static Database* getInstance() {
        if (!pInstance)
        {
            pInstance = new Database();
            destroyer.initialize(pInstance);
        }
        return pInstance;
    }

    static QString count_stat(int sock_desc)
    {
        QByteArray result;
        QSqlQuery query;
        query.prepare("SELECT stat FROM user WHERE status=:sock_desc");
        query.bindValue(":sock_desc", sock_desc);
        query.exec();
        QSqlRecord rec = query.record();
        const int statIndex = rec.indexOf("stat");
        while(query.next()) result.append(query.value(statIndex).toInt());

        query.clear();
        qDebug()<<result;
        return result;
    }

    QString count_stat1(int sock_desc)
    {

        QByteArray result;
        QSqlQuery query;
        query.prepare("SELECT stat1 FROM user WHERE status=:sock_desc");
        query.bindValue(":sock_desc", sock_desc);
        query.exec();
        QSqlRecord rec = query.record();
        const int statIndex = rec.indexOf("stat1");
        while(query.next()) result.append(query.value(statIndex).toInt());

        query.clear();
        qDebug()<<result;
        return result;
    }

    QString count_stat2(int sock_desc)
    {
        QByteArray result;
        QSqlQuery query;
        query.prepare("SELECT stat2 FROM user WHERE status=:sock_desc");
        query.bindValue(":sock_desc", sock_desc);
        query.exec();
        QSqlRecord rec = query.record();
        const int statIndex = rec.indexOf("stat2");
        while(query.next()) result.append(query.value(statIndex).toInt());
        query.clear();
        qDebug()<<result;
        return result;
    }

    QString count_stat3(int sock_desc)
    {

        QByteArray result;

        QSqlQuery query;
        query.prepare("SELECT stat3 FROM user WHERE status=:sock_desc");
        query.bindValue(":sock_desc", sock_desc);
        query.exec();
        QSqlRecord rec = query.record();
        const int statIndex = rec.indexOf("stat3");
        while(query.next()) result.append(query.value(statIndex).toInt());

        query.clear();
        qDebug()<<result;
        return result;
    }

    QByteArray check_ans(QString numb, QString ans, int sock_desc) {
        if (ans=="+")
        {
            QSqlQuery query;
            if (numb=="1") query.prepare("UPDATE user SET stat1=stat1+1, stat=stat+1 WHERE status=:sock_desc;");
            else if (numb=="2") query.prepare("UPDATE user SET stat2=stat2+1, stat=stat+1 WHERE status=:sock_desc");
            else query.prepare("UPDATE user SET stat3=stat3+1, stat=stat+1 WHERE status=:sock_desc");
            query.bindValue(":sock_desc", sock_desc);
            query.exec();


            query.clear();
            return "true";
        }
        else
        {
            dbInstance.open();
            QSqlQuery query;
            if (numb=='1') query.prepare("UPDATE user SET stat1=stat1-1, stat=stat-1 WHERE status=:sock_desc;");
            else if (numb=='2') query.prepare("UPDATE user SET stat2=stat2-1, stat=stat-1 WHERE status=:sock_desc;");
            else query.prepare("UPDATE user SET stat3=stat3-1, stat=stat-1 WHERE status=:sock_desc;");
            query.bindValue(":sock_desc", sock_desc);
            query.exec();
            dbInstance.close();
            query.clear();
            return "false";
        }
    }
    QByteArray auth(QString log, QString pass) {
        QSqlQuery query;
        query.prepare("SELECT * FROM user where log = :login and password = :password" );
        query.bindValue(":login", log);
        query.bindValue(":password", pass);
        query.exec();
        if (query.next()){
            qDebug()<<log;

            return "true";
        }
        else{
            return "false";
        }
    }
    QByteArray reg(QString log, QString pass, QString mail, int sock_desc)
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM user where log = :login" );
        query.bindValue(":login", log);
        query.exec();
        if (query.next()){
            return "Данный пользователь зарегистрирован";
        }
        else{
            qDebug()<<log;
            query.prepare("INSERT INTO user (log, password, email, stat, status, stat1, stat2, stat3) VALUES (:login, :password, :email, 0, :sock_desc, 0, 0, 0)");
            query.bindValue(":login", log);
            query.bindValue(":password", pass);
            query.bindValue(":email", mail);
            query.bindValue(":sock_desc", sock_desc);
            query.exec();
            return "true";
        }
    }
    QByteArray change_status(QString log, int sock_desc)
    {
        QSqlQuery query(dbInstance);
        query.prepare("UPDATE user SET status=:sock_desc WHERE log=:login");
        query.bindValue(":sock_desc", sock_desc);
        query.bindValue(":login", log);
        query.exec();

        query.clear();
        return "true";
    }
    QByteArray del_status(int sock_desc)
    {
        QSqlQuery query;
        query.prepare("UPDATE user SET status=0 WHERE status=:sock_desc");
        query.bindValue(":sock_desc", sock_desc);
        //query.bindValue(":login", log);
        query.exec();

        query.clear();
        return "true";
    }
};

#endif // SINGLETONDB_H
