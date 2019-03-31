#include "sqlitedatabase.h"

SQLiteDatabase::SQLiteDatabase(QString databasePath)
{
	QRandomGenerator randomGenerator(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));
	m_connectionName = "_C_H_D_B_" + QString::number((randomGenerator.generate() & std::numeric_limits<qint32>::max()));
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
	database.setDatabaseName(databasePath);
	database.setHostName("localhost");
	database.open();
};

SQLiteDatabase::~SQLiteDatabase()
{

};
