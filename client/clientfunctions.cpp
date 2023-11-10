#include "clientfunctions.h"

// Функция входа в систему
bool login(QString login, QString password) {
    // Формирование запроса на аутентификацию
    QVariantMap request;
    request["method"] = "authentication";
    request["login"] = login;
    request["password"] = password;

    // Выполнение запроса
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();

    // Обработка ответа
    if (status == "success") {
        QString token = response["token"].toString();
        QString isTeacher = response["isTeacher"].toBool() ? "true" : "false";
        save_setting("login", login);
        save_setting("token", token);
        save_setting("isTeacher", isTeacher);
        return true;
    } else {
        return false;
    }
}

// Функция регистрации аккаунта
bool register_account(QString login, QString password) {
    // Формирование запроса на регистрацию
    QVariantMap request;
    request["method"] = "register";
    request["login"] = login;
    request["password"] = password;

    // Выполнение запроса
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();

    // Обработка ответа
    if (status == "success") {
        QString token = response["token"].toString();
        QString isTeacher = response["isTeacher"].toBool() ? "true" : "false";
        save_setting("login", login);
        save_setting("token", token);
        save_setting("isTeacher", isTeacher);
        return true;
    } else {
        return false;
    }
}

// Функция получения статистики текущего пользователя
QString getStat() {
    // Формирование запроса на получение статистики
    QVariantMap request;
    QString result_str;
    request["method"] = "getUserStat";
    request["token"] = load_setting("token");

    // Выполнение запроса
    QJsonObject response = Network::getInstance()->doRequest(request);

    // Обработка ответа
    const auto stat = response["result"].toArray();
    for (const auto& item : stat) {
        const auto item_obj = item.toObject();
        const int type = item_obj["type"].toInt();
        const int solved_tasks = item_obj["solved_tasks"].toInt();
        result_str += QString("%1 - %2\n").arg(type).arg(solved_tasks);
    }
    return result_str;
}

// Функция получения общей статистики
QJsonArray getAllStat() {
    // Формирование запроса на получение общей статистики
    QVariantMap request;
    request["method"] = "getAllStat";
    request["token"] = load_setting("token");

    // Выполнение запроса
    QJsonObject response = Network::getInstance()->doRequest(request);
    QJsonArray res = response.value("result").toArray();
    return res;
}

// Функция получения задачи по типу
QVariantMap getTask(int taskType) {
    // Формирование запроса на получение задачи
    QVariantMap request;
    request["method"] = "getTask";
    request["token"] = load_setting("token");
    request["taskType"] = taskType;

    // Выполнение запроса
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();

    // Обработка ответа
    if (status == "success") {
        QVariantMap task;
        task["id"] = response["taskId"].toInt();
        task["text"] = response["taskText"].toString();
        return task;
    } else {
        return QVariantMap();
    }
}

// Функция проверки задачи
bool checkTask(int taskId, QString answer) {
    // Формирование запроса на проверку задачи
    QVariantMap request;
    request["method"] = "checkTask";
    request["token"] = load_setting("token");
    request["taskId"] = taskId;
    request["answer"] = answer;

    // Выполнение запроса
    QJsonObject response = Network::getInstance()->doRequest(request);
    QString status = response["status"].toString();

    // Обработка ответа
    if (status == "success") return true;
    else return false;
}

// Функция сохранения настроек
bool save_setting(QString param_name, QString param_value) {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.beginGroup("Settings");
    settings.setValue(param_name, param_value);
    return true;
}

// Функция загрузки настроек
QString load_setting(QString param_name) {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.beginGroup("Settings");
    QString param = settings.value(param_name, -1 ).toString();
    return param;
}

// Функция сброса настроек
bool reset_settings() {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.clear();
    return true;
}
