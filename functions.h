/**
 * @file functions.h
 * @brief Содержит объявления функций для парсинга, аутентификации, регистрации,
 * проверки ответов, просмотра статистики, изменения дескриптора сокета и удаления статуса.
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include "singletondb.h"

/**
 * @brief Разбирает данные от клиента и выполняет соответствующее действие на основе разобранных данных.
 *
 * @param data_from_client Данные, полученные от клиента.
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Результат выполненного действия.
 */
QByteArray parsing(QString data_from_client, int sock_desc);
/**
 * @brief Аутентификация пользователя.
 *
 * @param log Имя пользователя.
 * @param pass Пароль пользователя.
 * @return Результат аутентификации.
 */
QByteArray auth(QString log, QString pass);
/**
 * @brief Регистрирует нового пользователя.
 *
 * @param login Желаемое имя пользователя нового пользователя.
 * @param pass Желаемый пароль нового пользователя.
 * @param mail Электронная почта нового пользователя.
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Результат регистрации.
 */
QByteArray reg(QString login, QString pass, QString mail, int sock_desc);
/**
 * @brief Проверяет ответ, предоставленный пользователем.
 *
 * @param numb Номер, связанный с ответом.
 * @param ans Ответ, предоставленный пользователем.
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Результат проверки.
 */
QByteArray check_ans(QString numb, QString ans, int sock_desc);
/**
 * @brief Получает статистику для первого пользователя.
 *
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Запрошенная статистика.
 */
QByteArray look_stat(int sock_desc);
/**
 * @brief Получает статистику для второго пользователя.
 *
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Запрошенная статистика.
 */
QByteArray look_stat1(int sock_desc);
/**
 * @brief Получает статистику для третьего пользователя.
 *
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Запрошенная статистика.
 */
QByteArray look_stat2(int sock_desc);
/**
 * @brief Получает статистику для четвертого пользователя.
 *
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Запрошенная статистика.
 */
QByteArray look_stat3(int sock_desc);
/**
 * @brief Изменяет дескриптор сокета для пользователя.
 *
 * @param login Имя пользователя.
 * @param sock_desc Новый дескриптор сокета для пользователя.
 * @return Результат операции.
 */
QByteArray change_sock_desc(QString login, int sock_desc);
/**
 * @brief Удаляет статистику пользователя.
 *
 * @param sock_desc Дескриптор сокета для клиента.
 * @return Результат операции.
 */
QByteArray del_status(int sock_desc);
#endif // FUNCTIONS_H
