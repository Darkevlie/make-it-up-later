#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <network.h>
#include <QJsonObject>
#include <QSettings>
#include <QVariantMap>

// Функция входа в систему
bool login(QString login, QString password);

// Функция регистрации аккаунта
bool register_account(QString login, QString password);

// Функция получения статистики текущего пользователя
QString getStat();

// Функция получения общей статистики
QJsonArray getAllStat();

// Функция получения задачи по типу
QVariantMap getTask(int taskType);

// Функция проверки задачи
bool checkTask(int taskId, QString answer);

// Функция сохранения настроек
bool save_setting(QString param_name, QString param_value);

// Функция загрузки настроек
QString load_setting(QString param_name);

// Функция сброса настроек
bool reset_settings();

#endif // CLIENTFUNCTIONS_H
