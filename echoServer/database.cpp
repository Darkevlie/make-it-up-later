#pragma once

#include "database.h"
#include <QCoreApplication>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

// Конструктор для класса Database
Database::Database()
{
    // Инициализация базы данных
    initDB();
}

// Инициализация базы данных и создание необходимых таблиц, если они не существуют
void Database::initDB() {
    // Установка базы данных SQLite
    this->dbInstance = QSqlDatabase::addDatabase("QSQLITE");
    this->dbInstance.setDatabaseName("db2.sql");

    // Попытка открыть базу данных и обработка ошибок, если они есть
    if (!this->dbInstance.open()) {
        qDebug() << this->dbInstance.lastError().text();
    }

    // Создание таблицы Users, если она не существует
    this->doSQLQuery("CREATE TABLE IF NOT EXISTS Users (userID INTEGER PRIMARY KEY, login VARCHAR (255) UNIQUE NOT NULL, password VARCHAR (255) NOT NULL, token VARCHAR (255) UNIQUE NOT NULL, isTeacher BOOL NOT NULL)");

    // Создание таблицы Tasks, если она не существует
    this->doSQLQuery("CREATE TABLE IF NOT EXISTS Tasks (taskID INTEGER PRIMARY KEY, text VARCHAR (1024) NOT NULL, answer VARCHAR (1024) NOT NULL, type INT NOT NULL)");

    // Создание таблицы UsersTasks, если она не существует
    this->doSQLQuery("CREATE TABLE IF NOT EXISTS UsersTasks (userID int references Users(userID), taskID int references Tasks(taskID), primary key (userID, taskID))");
};

// Деструктор для класса Database
Database::~Database() {
    // Закрытие базы данных при уничтожении экземпляра
    this->dbInstance.close();
};

// Выполнение SQL-запроса и обработка транзакции
QSqlQuery Database::doSQLQuery(QString stringQuery) {
    // Создание объекта QSqlQuery с текущим экземпляром базы данных
    QSqlQuery query(this->dbInstance);

    // Выполнение запроса и получение статуса
    bool status = query.exec(stringQuery);

    // Логирование запроса и его статуса
    qDebug().noquote() << QString("Query %1, status %2").arg(stringQuery).arg(status);

    // Откат транзакции, если запрос не удался, в противном случае фиксация
    if (!status) {
        this->dbInstance.rollback();
    }
    else {
        this->dbInstance.commit();
    }

    // Возврат объекта QSqlQuery
    return query;
}

// Инициализация статических членов
Database* Database::pInstance = nullptr;
QSqlDatabase Database::dbInstance;
DatabaseDestroyer Database::destroyer;
