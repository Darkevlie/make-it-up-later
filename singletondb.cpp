/**
 * @file singletondb.cpp
 * @brief Содержит объявление и определения класса синглтона базы данных.
 */
#pragma once
#include "singletondb.h"
#include <QCoreApplication>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
/**
 * @brief Конструирует объект Database и инициализирует базу данных.
 */
Database::Database()
{
    initDB();
}

/**
 * @brief Инициализирует базу данных.
 */
void Database::initDB() {
    this->dbInstance = QSqlDatabase::addDatabase("QSQLITE");
    this->dbInstance.setDatabaseName("./SQLite.sql");
    if (!this->dbInstance.open()) {
        qDebug() << this->dbInstance.lastError().text();
    }
    QSqlQuery query(this->dbInstance);
    query.exec("CREATE TABLE user (log VARCHAR (255) UNIQUE PRIMARY KEY NOT NULL, password VARCHAR (255) NOT NULL, email VARCHAR (255) NOT NULL UNIQUE, stat VARCHAR (255) NOT NULL, status VARCHAR (255) NOT NULL, stat1 VARCHAR (255) NOT NULL, stat2 VARCHAR (255) NOT NULL, stat3 VARCHAR (255) NOT NULL)");
};

/**
 * @brief Деструктор для базы данных.
 */
Database::~Database() {
    this->dbInstance.close();
};

/**
 * @brief Указатель на экземпляр синглтон класса Database.
 */
Database* Database::pInstance = nullptr;

/**
 * @brief Экземпляр QSqlDatabase, связанный с классом базы данных.
 */
QSqlDatabase Database::dbInstance;

/**
 * @brief Дейстройер базы данных, связанный с классом базы данных.
 */
DatabaseDestroyer Database::destroyer;
