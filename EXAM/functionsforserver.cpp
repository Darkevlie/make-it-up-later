#include "functionsforserver.h"

QByteArray parsing (QString data_from_client, QString connectionDescriptor)
 {
     /* data_from_client = NameOfFunc&Param1&Param2&Param3
      * Цель парсинга:
      * return NameOfFunc(Param1,Param2,Param3)
      */
     QStringList data_from_client_list = data_from_client.split(QLatin1Char('&'));
     // data_from_client_list  = "NameOfFunc" "Param1" "Param2" Param3"
     QString nameOfFunc = data_from_client_list.front();
     data_from_client_list.pop_front();
     if (nameOfFunc == "auth")
         return auth(data_from_client_list.at(0),data_from_client_list.at(1));
     else if (nameOfFunc == "reg")
         return reg(data_from_client_list.at(0),data_from_client_list.at(1),data_from_client_list.at(2));
     else if (nameOfFunc == "start") {
         return start(data_from_client_list.at(0), connectionDescriptor); }
     else if (nameOfFunc == "finish") {
         return finish(data_from_client_list.at(0), connectionDescriptor); }
     else
         return "error\n";
 }


QByteArray auth(QString log, QString pass)
{
    return DataBase::auth(log, pass);
}

QByteArray start(QString time, QString connectionDescriptor)
{
    return DataBase::start(time, connectionDescriptor);
}

QByteArray finish(QString time, QString connectionDescriptor)
{
    return DataBase::finish(time, connectionDescriptor);
}

QByteArray reg(QString log, QString pass,QString mail)
{
    return DataBase::registerUser(log, pass, mail);
}
