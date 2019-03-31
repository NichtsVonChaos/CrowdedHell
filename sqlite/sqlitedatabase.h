#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QtSql>
#include <QTime>
#include <QRandomGenerator>

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
	SQLiteDatabase(QString databasePath);
	~SQLiteDatabase();

signals:
	void sendMessage(MessageType type, QString module, QString message);

private:
	QString m_connectionName;

	QString m_currentTable;
};

#endif // SQLITEDATABASE_H
