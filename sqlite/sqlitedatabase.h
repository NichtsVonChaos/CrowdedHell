#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QtSql>

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

class SQLiteDatabase : QObject
{
	Q_OBJECT

public:
	SQLiteDatabase();

signals:
	void sendMessage(MessageType type, QString module, QString message);
};

#endif // SQLITEDATABASE_H
