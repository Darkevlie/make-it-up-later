#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H
#include <QString>
#include "dataBase.h"
#include <QStringList>
/**
 * \brief parsing - функция, которая по введенному названию
 * и параметрам функции возращает результат.
 * \param [in] data_from_client - строка, которая приходит от клиента
 * в формате....
 * \return Возвращает результат работы функции, которая была подана на вход.
 *
 * \example указать_файл.txt
 */
QByteArray parsing (QString data_from_client, QString connectionDescriptor);

QByteArray auth(QString log, QString pass);

QByteArray reg(QString log, QString pass,QString mail);

QByteArray start(QString time, QString connectionDescriptor);

QByteArray finish(QString time, QString connectionDescriptor);

#endif
// FUNCTIONSFORSERVER_H
