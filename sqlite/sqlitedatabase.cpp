#include "sqlitedatabase.h"

SQLiteDatabase::SQLiteDatabase(const QString &databasePath) :
	m_valid(true)
{
	connect(this, &SQLiteDatabase::sendMessage, g_mainWindow, &CrowdedHellGUI::sendMessage);

	QRandomGenerator randomGenerator(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));

	do
		m_connectionName = "_C_H_D_B_" + QString::number((randomGenerator.generate() & std::numeric_limits<qint32>::max()));
	while(QSqlDatabase::database(m_connectionName).isValid());

	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
	database.setDatabaseName(databasePath);
	database.setHostName("localhost");
	if(!database.open())
	{
		sendMessage(MessageType::Error, "SQLite", tr("Open the database file failed, at : \"%1\".").arg(databasePath));
		m_valid = false;
	}
};

SQLiteDatabase::~SQLiteDatabase()
{
	close();
	QSqlDatabase::removeDatabase(m_connectionName);
};

bool SQLiteDatabase::isValid()
{
	return m_valid;
};

QStringList SQLiteDatabase::getAllTableNames()
{
	return QSqlDatabase::database(m_connectionName).tables();
};

bool SQLiteDatabase::createTable(const SQLiteDatabase::DataTable &table)
{
	if(!isValid())
	{
		sendMessage(MessageType::Error, "SQLite", tr("Database is not valid."));
		return false;
	}

	if(exists(table.tableName))
	{
		sendMessage(MessageType::Warning, "SQLite", tr("Table is already exists, so it won't be created again."));
		return false;
	}

	if(table.columns.size() == 0)
	{
		sendMessage(MessageType::Error, "SQLite", tr("Cannot create a table who has no column."));
		return false;
	}

	QString sqlSentence = "CREATE TABLE IF NOT EXISTS ";
	sqlSentence += table.tableName;
	sqlSentence += "(";
	if(table.hasAutoIncrementIndex)
	{
		sqlSentence += "_AutoIndex INTEGER PRIMARY KEY AUTO INCREMENT, ";
	}



	return true;
};

bool SQLiteDatabase::exists(const QString &tableName)
{
	return getAllTableNames().contains(tableName);
};

void SQLiteDatabase::close()
{
	QSqlDatabase::database(m_connectionName).close();
	m_valid = false;
};
