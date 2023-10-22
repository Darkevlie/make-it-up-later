#ifndef DATABASE_H
#define DATABASE_H

//#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTime>
//#include <QSqlRecord>


class DataBase;

class DataBaseDestroyer
{
    private:
        DataBase * dataBasePointer;
    public:
        ~DataBaseDestroyer() {
            delete dataBasePointer;
        }
        void initialize(DataBase * p){dataBasePointer = p;}
};


class DataBase
{
    private:
        static DataBase * p_instance;
        static DataBaseDestroyer destroyer;
        static QSqlDatabase db;
    protected:
        DataBase(){
            p_instance->db = QSqlDatabase::addDatabase("QSQLITE");
            p_instance->db.setDatabaseName("Test.db");

            if(!p_instance->db.open())
                qDebug()<<p_instance->db.lastError().text();


        }
        DataBase(const DataBase& ) = delete;
        DataBase& operator = (DataBase &) = delete;
        ~DataBase() {db.close();}

        friend class DataBaseDestroyer;
    public:
        static DataBase* getInstance(){
            if (!p_instance)
            {
                p_instance = new DataBase();

                destroyer.initialize(p_instance);
            }
            return p_instance;
        }

        ///здесь пишем функцию сделать запрос к БД
        static QByteArray DataBase::auth(const QString &login, const QString &password) {
            QSqlQuery query;
            query.prepare("SELECT * FROM users WHERE login = :login AND password = :password");
            query.bindValue(":login", login);
            query.bindValue(":password", password);

            if (query.exec()) {
                if (query.next()) {
                    // Пользователь найден
                    QString status = query.value("status").toString();
                    if (status.isEmpty()) {
                        query.prepare("UPDATE users SET status = :status WHERE login = :login");
                        query.bindValue(":status", "your_connection_descriptor");
                        query.bindValue(":login", login);
                        if (query.exec()) {
                            return "auth_ok";
                        } else {
                            return "auth_err\n"; // Ошибка при установке статуса
                        }
                    } else {
                        return "auth_err\n"; // Пользователь уже зареган
                    }
                } else {
                    return "auth_false\n"; // Пользователь не найден
                }
            } else {
                return "auth_err\n"; // Ошибка при выполнении запроса
            }
        }

        static QByteArray DataBase::start(const QString &time, const QString &connectionDescriptor) {
            // Создаем объект запроса
            QSqlQuery query;

            // Проверяем, существует ли пользователь с заданным дескриптором соединения
            query.prepare("SELECT * FROM users WHERE status = :status");
            query.bindValue(":status", connectionDescriptor);

            if (query.exec()) {
                if (query.next()) {
                    QString startTime = query.value("start_time").toString();

                    if (startTime.isEmpty()) {
                        // Поле начато пусто, устанавливаем значение
                        query.prepare("UPDATE users SET start_time = :start_time WHERE status = :status");
                        query.bindValue(":start_time", time);
                        query.bindValue(":status", connectionDescriptor);

                        if (query.exec()) {
                            return "start_ok\n";
                        } else {
                            return "start_err\n"; // Ошибка при установке времени начала
                        }
                    } else {
                        // Поле начато не пусто, обновляем статистику и "начато"
                        QTime timeReceived = QTime::fromString(time, "hh:mm:ss");
                        QTime startTimeValue = QTime::fromString(startTime, "hh:mm:ss");

                        int hoursDifference = startTimeValue.secsTo(timeReceived) / 3600;

                        // Обновляем статистику
                        query.prepare("UPDATE users SET statistics = statistics + :hours WHERE status = :status");
                        query.bindValue(":hours", hoursDifference);
                        query.bindValue(":status", connectionDescriptor);

                        // Уменьшаем "начато" на 1 час
                        startTimeValue = startTimeValue.addSecs(-3600);

                        // Обновляем значение начато
                        query.prepare("UPDATE users SET start_time = :start_time WHERE status = :status");
                        query.bindValue(":start_time", startTimeValue.toString("hh:mm:ss"));
                        query.bindValue(":status", connectionDescriptor);

                        if (query.exec()) {
                            return "start_reok\n";
                        } else {
                            return "start_err\n"; // Ошибка при обновлении времени начала
                        }
                    }
                } else {
                    return "start_err\n"; // Пользователь с заданным дескриптором соединения не найден
                }
            } else {
                return "start_err\n"; // Ошибка при выполнении запроса
            }
        }


        static QByteArray DataBase::finish(const QString &time, const QString &connectionDescriptor) {
            // Создаем объект запроса
            QSqlQuery query;

            // Проверяем, существует ли пользователь с заданным дескриптором соединения
            query.prepare("SELECT * FROM users WHERE status = :status");
            query.bindValue(":status", connectionDescriptor);

            if (query.exec()) {
                if (query.next()) {
                    QString startTime = query.value("start_time").toString();

                    if (startTime.isEmpty()) {
                        return "finish_reerr\n"; // Поле начато пусто
                    } else {
                        // Поле начато не пусто, обновляем статистику
                        QTime timeReceived = QTime::fromString(time, "hh:mm:ss");
                        QTime startTimeValue = QTime::fromString(startTime, "hh:mm:ss");

                        int hoursDifference = startTimeValue.secsTo(timeReceived) / 3600;

                        // Обновляем статистику
                        query.prepare("UPDATE users SET statistics = statistics + :hours WHERE status = :status");
                        query.bindValue(":hours", hoursDifference);
                        query.bindValue(":status", connectionDescriptor);

                        if (query.exec()) {
                            return "finish_ok\n";
                        } else {
                            return "finish_err\n"; // Ошибка при обновлении статистики
                        }
                    }
                } else {
                    return "finish_err\n"; // Пользователь с заданным дескриптором соединения не найден
                }
            } else {
                return "finish_err\n"; // Ошибка при выполнении запроса
            }
        }

        static QByteArray DataBase::registerUser(const QString &login, const QString &password, const QString &email) {
            QSqlQuery query;

            // Проверяем, существует ли пользователь с таким логином
            query.prepare("SELECT * FROM users WHERE login = :login");
            query.bindValue(":login", login);

            if (query.exec()) {
                if (query.next()) {
                    return "reg_err\n"; // Пользователь с таким логином уже существует
                } else {
                    // Пользователь с таким логином не найден, регистрируем нового пользователя
                    query.prepare("INSERT INTO users (login, password, email) VALUES (:login, :password, :email)");
                    query.bindValue(":login", login);
                    query.bindValue(":password", password);
                    query.bindValue(":email", email);

                    if (query.exec()) {
                        return "reg_ok\n"; // Пользователь успешно зарегистрирован
                    } else {
                        return "reg_err\n"; // Ошибка при выполнении запроса
                    }
                }
            } else {
                return "reg_err\n"; // Ошибка при выполнении запроса
            }
        }


        static QString sendQuerry(QString str){return "dfgh";}
};
/*
DataBase * DataBase::p_instance;
DataBaseDestroyer DataBase::destroyer;
QSqlDatabase DataBase::db;
*/

#endif // DATABASE_H
