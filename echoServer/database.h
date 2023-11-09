#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

// Класс для уничтожения объекта Database при завершении программы
class Database;

class DatabaseDestroyer
{
private:
    Database * pInstance;
public:
    ~DatabaseDestroyer() {
        delete this->pInstance;
    }
    void initialize(Database *p) {
        this->pInstance = p;
    }
};


class Database {
private:
    static Database * pInstance; // Статический указатель на объект класса

    void initDB(); // Приватный метод для инициализации базы данных
    static QSqlDatabase dbInstance; // Статический экземпляр QSqlDatabase
    static DatabaseDestroyer destroyer; // Статический объект для уничтожения базы данных

protected:
    Database();
    Database(const Database&) = delete; // Запрет на копирование
    Database& operator = (Database &)=delete; // Запрет на присваивание

    ~Database();
    friend class DatabaseDestroyer; // Дружественный класс для уничтожения базы данных

public:
    static Database* getInstance() { // Статический метод для получения экземпляра класса
        if (!pInstance) {
            pInstance = new Database(); // Создание экземпляра, если он не существует
            destroyer.initialize(pInstance); // Инициализация уничтожителя
        }
        return pInstance; // Возврат указателя на объект
    }

    QSqlQuery doSQLQuery(QString stringQuery); // Выполнение SQL-запроса
};



#endif // DATABASE_H
