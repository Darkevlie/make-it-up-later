#ifndef serverfunction_h
#define serverfunction_h

#include <QCoreApplication>
#include <QString>
#include <stdio.h>
#include "database.h"
#include "tasks.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QDebug>

/*
   Функция parse обрабатывает сообщения клиента: выделяет команду и аргументы, а затем вызывает соответствующую функцию.
   Аргумент data: сообщение, пришедшее от клиента.
   Возвращаемое значение: ответ соответствующей функции.
*/
QByteArray parse(QByteArray request);

// Функция для аутентификации
QByteArray auth(QJsonObject request);

// Функция для регистрации
QByteArray reg(QJsonObject request);

// Функция для получения общей статистики
QByteArray getAllStat(QJsonObject request);

// Функция для получения статистики пользователя
QByteArray getUserStat(QJsonObject request);

// Функция для получения задания
QByteArray getTask(QJsonObject request);

// Функция для проверки задания
QByteArray checkTask(QJsonObject request);

// Проверка, является ли пользователь учителем
bool isTeacher(QString userToken);

#endif
