#include "serverfunction.h"

// Функция разбора запроса и вызов соответствующей функции
QByteArray parse(QByteArray request) {
    // Преобразование JSON-строки запроса в объект QJsonObject
    QJsonObject json_request = QJsonDocument::fromJson(request).object();
    qDebug() << json_request;

    // Инициализация строки ответа
    QString response;

    // Получение метода из запроса
    QString method = json_request["method"].toString();

    // Выбор соответствующей функции в зависимости от метода
    if (method == QString("authentication")) {
        response = auth(json_request);
    }
    else if (method == QString("register")) {
        response = reg(json_request);
    }
    else if (method == QString("getTask")) {
        response = getTask(json_request);
    }
    else if (method == QString("checkTask")) {
        response = checkTask(json_request);
    }
    else if (method == QString("getUserStat")) {
        response = getUserStat(json_request);
    }
    else {
        // Формирование ответа об ошибке для неопределенного метода
        QVariantMap error_response;
        error_response["error"] = QString("Функция не определена");
        response = QJsonDocument::fromVariant(error_response).toJson(QJsonDocument::Compact);
    }

    // Преобразование ответа в QByteArray и добавление символа новой строки
    return QByteArray((response + "\r\n").toUtf8());
}

// Функция аутентификации
QByteArray auth(QJsonObject request) {
    // Инициализация ответа
    QVariantMap response;

    // Получение логина и пароля из запроса
    QString login = request["login"].toString();
    QString input_password = request["password"].toString();

    // Формирование SQL-запроса для получения данных пользователя
    QString query = QString("SELECT * FROM Users WHERE login = \"%1\"").arg(login);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.next();

    // Получение пароля из базы данных
    QString dbPassword = queryResult.value(record.indexOf("password")).toString();

    // Проверка совпадения паролей
    if (dbPassword == input_password) {
        QString token = queryResult.value(record.indexOf("token")).toString();
        bool isTeacher = queryResult.value(record.indexOf("isTeacher")).toBool();
        response["status"] = "success";
        response["token"] = token;
        response["isTeacher"] = isTeacher;
    }
    else {
        response["status"] = "fail";
    }

    // Логирование попытки аутентификации
    qDebug() << QString("Аутентификация с логином %1 и паролем %2").arg(login, input_password);

    // Преобразование ответа в JSON и компактный формат
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

// Функция регистрации
QByteArray reg(QJsonObject request) {
    // Инициализация ответа
    QVariantMap response;

    // Получение логина и пароля из запроса
    QString login = request["login"].toString();
    QString password = request["password"].toString();

    // Генерация токена с использованием хеша от логина и пароля
    QString token = QString(QCryptographicHash::hash(((login+password).toUtf8()), QCryptographicHash::Md5).toHex());

    // Формирование SQL-запроса для добавления нового пользователя
    QString query = QString("INSERT INTO Users(login, password, token, isTeacher) "
                            "VALUES (\"%1\", \"%2\", \"%3\", false)")
                        .arg(login, password, token);
    Database::getInstance()->doSQLQuery(query);

    // Заполнение ответа данными
    response["token"] = token;
    response["status"] = "success";
    response["isTeacher"] = false;

    // Преобразование ответа в JSON и компактный формат
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

// Функция получения статистики пользователя
QByteArray getUserStat(QJsonObject request) {
    // Инициализация ответа
    QVariantMap response;

    // Получение токена пользователя из запроса
    QString userToken = request["token"].toString();

    // Формирование SQL-запроса для получения статистики пользователя
    QString query = QString("SELECT Tasks.type, COUNT(*) AS solved_tasks "
                            "FROM UsersTasks "
                            "JOIN Tasks ON Tasks.taskID = UsersTasks.taskID "
                            "WHERE UsersTasks.userID = (SELECT userID FROM Users WHERE token = \"%1\") "
                            "GROUP BY Tasks.type")
                        .arg(userToken);

    // Выполнение запроса и обработка результата
    QSqlQuery stat = Database::getInstance()->doSQLQuery(query);
    QJsonArray recordsArray;
    while(stat.next()) {
        QJsonObject recordObject;
        for (int x = 0; x < stat.record().count(); x++) {
            recordObject.insert(stat.record().fieldName(x), QJsonValue::fromVariant(stat.value(x)));
        }
        recordsArray.push_back(recordObject);
    }

    // Заполнение ответа данными
    response["status"] = "success";
    response["result"] = recordsArray;

    // Преобразование ответа в JSON и компактный формат
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

// Функция получения задачи
QByteArray getTask(QJsonObject request) {
    // Инициализация ответа
    QVariantMap response;

    // Получение типа задачи из запроса
    int taskType = request["taskType"].toInt();

    // Выбор соответствующей задачи
    QVariantMap task;
    switch (taskType) {
    case 1: {task = TaskApokina(); break;}
    case 2: {task = TaskKutuzova(); break;}
    case 3: {task = Task3(); break;}
    default: {task = TaskApokina(); break;}
    }

    // Получение текста и ответа задачи
    QString taskText = task["text"].toString();
    QString taskAnswer = task["answer"].toString();

    // Формирование SQL-запроса для добавления задачи в базу данных
    QString query = QString("INSERT INTO Tasks(text, answer, type) "
                            "VALUES (\"%1\", \"%2\", %3)")
                        .arg(taskText).arg(taskAnswer).arg(taskType);

    // Выполнение запроса и получение ID добавленной задачи
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);

    // Заполнение ответа данными
    response["status"] = "success";
    response["taskId"] = queryResult.lastInsertId().toInt();
    response["taskText"] = taskText;

    // Преобразование ответа в JSON и компактный формат
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

// Функция проверки ответа на задачу
QByteArray checkTask(QJsonObject request) {
    // Инициализация ответа
    QVariantMap response;

    // Получение токена пользователя, ID задачи и ответа из запроса
    QString userToken = request["token"].toString();
    int taskId = request["taskId"].toInt();
    QString answer = request["answer"].toString();

    // Формирование SQL-запроса для получения правильного ответа на задачу
    QString query = QString("SELECT * FROM Tasks WHERE taskId = %1").arg(taskId);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.next();

    // Получение правильного ответа из базы данных
    QString correctAnswer = queryResult.value(record.indexOf("answer")).toString();

    // Проверка совпадения ответов
    if (answer == correctAnswer) {
        response["status"] = "success";

        // Формирование SQL-запроса для добавления задачи в список решенных пользователя
        QString query = QString("INSERT INTO UsersTasks(userID, taskid) "
                                "SELECT userID, %1 FROM Users WHERE token == \"%2\"")
                            .arg(taskId).arg(userToken);
        Database::getInstance()->doSQLQuery(query);
    }
    else {
        response["status"] = "fail";
    }
    qDebug() << correctAnswer;

    // Преобразование ответа в JSON и компактный формат
    return QJsonDocument::fromVariant(response).toJson(QJsonDocument::Compact);
}

// Функция определения является ли пользователь учителем
bool isTeacher(QString userToken) {
    // Формирование SQL-запроса для получения статуса учителя
    QString query = QString("SELECT isTeacher FROM Users WHERE token == \"%2\"")
                        .arg(userToken);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.next();

    // Получение статуса учителя из результата запроса
    bool isTeacher = queryResult.value(record.indexOf("isTeacher")).toBool();

    // Возвращение статуса учителя
    return isTeacher;
}
